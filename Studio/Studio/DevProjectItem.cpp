#include <QSettings>
#include <QMessageBox>
#include <QFileInfo>

#include "DevProjectItem.h"
#include "DevProjectManager.h"
#include "MDProjectConfiger.h"
#include "FormScrollArea.h"
#include "BaseForm.h"
#include "PropertyBrowser.h"
#include "MDHMICreator.h"
#include "MDReportDesigner.h"
#include "MDDataArchivesUI.h"
#include "MDRTDBManagerUIFactory.h"
#include "../include/MDScriptUI/MDScriptUI.h"
#include "MDEventActionUI.h"
#include "MDSecurityPolicyUI.h"
#include "MDProjectConfiger.h"
#include "MDAlarmUI.h"
#include "MDRuntimeUI.h"
#include "MDRecipeUI.h"
#include "IHMIStudio.h"
#include "MainWindow.h"
#include "../include/MDDBQueryUI/MDDBQueryUI.h"
//////////////////////////////////////////////////////////////////////////
CDevProjectItem::CDevProjectItem()
{
    m_configer = new CMDProjectConfiger();
    m_manager = NULL;
    m_projectRoot = NULL;

    m_dictManager = NULL;
}
CDevProjectItem::~CDevProjectItem()
{
    clear();
}
//////////////////////////////////////////////////////////////////////////
QString CDevProjectItem::fullName()
{
    return m_configer->fullFileName();
}
QTreeWidget *CDevProjectItem::treeWidget()
{
    return m_manager->treeWidget();
}
QMdiArea* CDevProjectItem::mdiArea()
{
    return m_manager->mdiArea();
}
MainWindow *CDevProjectItem::mainWindow()
{
    return m_manager->mainWindow();
}
IMDRTDBManagerUI *CDevProjectItem::rtdbManagerUI()
{
    if(m_modules.count() < 1)
        return NULL;
    return dynamic_cast<IMDRTDBManagerUI*>(m_modules.at(0));
}
IHMIFramework *CDevProjectItem::hmiFramework()
{
    if(m_modules.count() < 2)
        return NULL;
    return dynamic_cast<IHMIFramework*>(m_modules.at(1));
}
CPropertyBrowser *CDevProjectItem::propertyBrowser()
{
    return mainWindow()->propertyBrowser();
}
//////////////////////////////////////////////////////////////////////////
bool CDevProjectItem::setActiveForm(CBaseForm *form)
{
    QObject* p = form->parent();
    while(p)
    {
        QMdiSubWindow* sub = qobject_cast<QMdiSubWindow*>(p);
        if(sub)
        {
            mdiArea()->setActiveSubWindow(sub);
            break;
        }

        p = p->parent();
    }

    return true;
}
void CDevProjectItem::openMDIWindow(CBaseForm *form)
{
    Q_ASSERT(form);

    QMdiSubWindow *sub = NULL;
    if(form->needScroll())
    {
        CFormScrollArea* scroll = new CFormScrollArea(mdiArea());
        scroll->addWidget(form);
        sub = mdiArea()->addSubWindow(scroll);
    }
    else
        sub = mdiArea()->addSubWindow(form);

    sub->show();
    sub->installEventFilter(this);
    sub->setProperty("projectID", fullName());

    QTabBar *tabBar = mdiArea()->findChildren<QTabBar*>().at(0);
    if(tabBar && tabBar->count() > 0)
    {
        QString path = QFileInfo(fullName()).absolutePath();
        tabBar->setTabToolTip(tabBar->count() - 1, path);
    }
}
//////////////////////////////////////////////////////////////////////////
bool CDevProjectItem::load(const QString &name)
{
    m_configer->load(name);
    m_configer->loadRTDB();
    initLanguage();
    initModules();
    changeLanguage();

    return true;
}
bool CDevProjectItem::unload()
{
    foreach(IStudioModule* m, m_modules)
        m->unload();

    m_configer->unloadRTDB();
    m_configer->unLoad();

    return true;
}
CBaseForm *CDevProjectItem::getBaseForm(QMdiSubWindow *sub)
{
    if(sub == NULL)
        return NULL;

    CBaseForm* f = dynamic_cast<CBaseForm*>(sub->widget());
    if(f != NULL)
        return f;

    QScrollArea* area = dynamic_cast<QScrollArea*>(sub->widget());
    if(area != NULL)
    {
        CBaseForm* form = dynamic_cast<CBaseForm*>(area->widget());
        if(form != NULL)
            return form;
    }

    return NULL;
}
//////////////////////////////////////////////////////////////////////////
void CDevProjectItem::initModules()
{
    Q_ASSERT(m_projectRoot == NULL);

    m_modules.append(CMDRTDBManagerUIFactory::createMDRTDBManagerUI());
    m_modules.append(m_manager->hmiStudio()->open());
    m_modules.append(new CMDEventActionUI);
    m_modules.append(new CMDAlarmUI);
    m_modules.append(new CMDDataArchivesUI);
    //m_modules.append(new MDDBQueryUI);
    m_modules.append(new CMDReportDesigner);
    m_modules.append(new CMDScriptUI);
    m_modules.append(new CMDSecurityPolicyUI);
    m_modules.append(new CMDRuntimeUI);
    m_modules.append(new CMDRecipeUI);

    QIcon icon(":/images/folder.png");
    QString name = projectConfiger()->name();
    m_projectRoot = new QTreeWidgetItem(QStringList(name));
    m_projectRoot->setIcon(0,icon);
    m_projectRoot->setToolTip(0, QFileInfo(fullName()).absolutePath());
    treeWidget()->addTopLevelItem(m_projectRoot);

    foreach (IStudioModule* m, m_modules)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(QStringList(m->rootName()));
        if(m->rootIcon() == NULL)
            item->setIcon(0, icon);
        else
            item->setIcon(0, *m->rootIcon());
        m_projectRoot->addChild(item);
        m->setRootItem(item);
        m->setStudioInterface(this);
    }
    foreach(IStudioModule* m, m_modules)
        m->load();

    treeWidget()->expandItem(m_projectRoot);
}
void CDevProjectItem::clear()
{
    releaseLanguage();
    qDeleteAll(m_modules);
    m_modules.clear();

    delete m_configer;
    m_configer = NULL;
    delete m_projectRoot;
    m_projectRoot = NULL;
}
//////////////////////////////////////////////////////////////////////////
bool CDevProjectItem::eventFilter(QObject *watched, QEvent *event)
{
    //close event
    if(event->type() == QEvent::Close &&watched->inherits("QMdiSubWindow"))
    {
        QMdiSubWindow* sub = dynamic_cast<QMdiSubWindow*>(watched);
        CBaseForm* base = getBaseForm(sub);

        if(base != NULL && base->isModify())
        {
            //不显示修改标记的窗体自动保存，不会有提示保存信息
            if(!base->modifiedVisible())
                base->saveEvent();
            else
            {
                QString str = QString(tr("%1 是否保存？")).arg(base->factWindowTitle());
                QMessageBox::StandardButton result = QMessageBox::question(base, tr("确认"), str,
                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);

                if(result == QMessageBox::Cancel)
                {
                    event->ignore();
                    return true;
                }
                else if(result == QMessageBox::Yes)
                    base->saveEvent();
            }
        }
    }

    return QObject::eventFilter(watched, event);
}
//////////////////////////////////////////////////////////////////////////
void CDevProjectItem::initLanguage()
{
    ILanguageObject* l = dynamic_cast<ILanguageObject*>(mainWindow());
    m_dictManager = CMDMultiLanguage::createDictionaryManager(l, m_configer->path());
    CMDMultiLanguage::instance()->setCurrentProject(m_dictManager);
}
void CDevProjectItem::releaseLanguage()
{
    CMDMultiLanguage::instance()->setCurrentProject(NULL);
    delete m_dictManager;
    m_dictManager = NULL;
}
void CDevProjectItem::changeLanguage()
{
    foreach(IStudioModule* m, m_modules)
    {
        //tree root
        QString translated = CMDMultiLanguage::instance()->value(m->rootName());
        m->rootItem()->setText(0, translated);
        m->changeLanguage();
    }
}
void CDevProjectItem::collectWords()
{
    foreach(IStudioModule* m, m_modules)
        m->collectWords();
}


