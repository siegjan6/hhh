#include <QDir>
#include <QCoreApplication>
#include "HMIFramework.h"
#include "HMIStudio.h"
#include "FormOperation.h"
#include "FileNode.h"
#include "MDHMIForm.h"
#include "LaunchConfigWidget.h"
#include "PreviewDialog.h"
#include "CreateTool.h"
#include "math.h"
//////////////////////////////////////////////////////////////////////////
CHMIFramework::CHMIFramework(CHMIStudio *manager)
{
    m_forms = NULL;
    m_projectExpert = NULL;
    m_projectConfiger = NULL;
    m_mdiArea = NULL;
    m_isFullScreen = false;

    m_manager = manager;
    m_isStudioMode = (m_manager != NULL);

    m_formScale = QPointF(1, 1);
    m_rootItem = NULL;
    m_studioProject = NULL;

    init();
}
CHMIFramework::~CHMIFramework()
{
    clear();
}
IFormOperation *CHMIFramework::forms()
{
    return m_forms;
}
//////////////////////////////////////////////////////////////////////////
void CHMIFramework::setProjectConfiger(CMDProjectConfiger *value)
{
    m_projectConfiger = value;
}
void CHMIFramework::setParent(QMainWindow *parent)
{
    if (m_isStudioMode)
        return;

    m_mdiArea = new QMdiArea;
    m_mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    parent->setCentralWidget(m_mdiArea);
}
//////////////////////////////////////////////////////////////////////////
void CHMIFramework::load()
{
    if(m_projectConfiger == NULL)
        m_projectConfiger = studioProject()->projectConfiger();

    m_hmiPath = m_projectConfiger->path() + "/HMI";
    initProjectImagePath();

    if(m_isStudioMode)
    {
        dynamic_cast<CHMIStudio*>(m_manager)->fileNode()->initNodes(rootItem(), this);
    }
    else
    {
        //重新加载项目时CHMIFramework不会被删除，所以只能在这里初始化下
        CMDExpConfig::clear();
    }

    m_launchConfig.init(this);
    m_formManager.init(this);

    log("load");
}
void CHMIFramework::unload()
{
    log("unload");

    clear();
}
//////////////////////////////////////////////////////////////////////////
void CHMIFramework::init()
{
    m_rootIcon = new QIcon(":/rs/image/root.png");
    m_forms = new CFormOperation(this);

    connect(&m_timerRefresh, &QTimer::timeout, this, &CHMIFramework::timeRefresh);
    m_timerRefresh.start(100);

    if(!m_isStudioMode)
        CMDDrawTool::init(new CCreateTool);

    initLog();
}
void CHMIFramework::clear()
{
    if(m_rootIcon == NULL)
        return;

    if(m_isStudioMode)
        dynamic_cast<CHMIStudio*>(m_manager)->fileNode()->close(rootItem());

    delete m_rootIcon;
    m_rootIcon = NULL;
    m_timerRefresh.stop();
    m_forms->closeAll();
    delete m_forms;

    if (m_mdiArea != NULL)
        delete m_mdiArea;

    if(!m_isStudioMode)
    {
        CMDExpConfig::clear();
        CMDDrawTool::release();
    }

    m_log = NULL;
}
IHMIStudio *CHMIFramework::manager()
{
    return m_manager;
}
bool CHMIFramework::openSelectHIMFormDialog(QStringList &forms)
{
    QWidget* w = dynamic_cast<CHMIStudio*>(m_manager)->fileNode()->view();
    CLaunchConfigWidget launch(w);
    launch.initSelect(this, forms);
    if (launch.exec() == QDialog::Accepted)
    {
        forms = launch.selectForms();
        return true;
    }

    return false;
}
//////////////////////////////////////////////////////////////////////////
void CHMIFramework::onDataChanged(IMDVariable *var)
{
    for(int i = 0; i < m_forms->openedList()->count(); i++)
    {
        CMDHMIForm* form= m_forms->openedList()->at(i);
        form->run()->onDataChanged(var->name().toLower(), var->currentValue());
    }
}
void CHMIFramework::timeRefresh()
{
    for(int i = 0; i < m_forms->openedList()->count(); i++)
    {
        CMDHMIForm* form= m_forms->openedList()->at(i);
        if(form->common())
            form->common()->timeInvalidate();
    }

    if (!isStudioMode())
    {
        //notify
        static int notifyCount = 0;
        notifyCount++;
        if (notifyCount >= 10)
        {
            checkNotify();
            notifyCount = 0;
        }
    }
}
void CHMIFramework::setVariableValue(const QString &name, const QVariant &value)
{
    if (m_projectExpert == NULL)
        return;

    m_projectExpert->writeVariableValue(name, value);
}
QVariant CHMIFramework::variableValue(const QString& name)
{
    if (m_projectExpert == NULL)
        return QVariant();

    return m_projectExpert->readVariableValue(name);
}
//////////////////////////////////////////////////////////////////////////
void CHMIFramework::enterFullScreen()
{
    m_isFullScreen = true;

    for(int i = 0; i < m_forms->openedList()->count(); i++)
    {
        CMDHMIForm* form= m_forms->openedList()->at(i);
        form->run()->enterFullScreen();
    }
}
void CHMIFramework::exitFullScreen()
{
    m_isFullScreen = false;

    for(int i = 0; i < m_forms->openedList()->count(); i++)
    {
        CMDHMIForm* form= m_forms->openedList()->at(i);
        form->run()->exitFullScreen();
    }
}
//////////////////////////////////////////////////////////////////////////
void CHMIFramework::checkNotify()
{
    if (m_projectExpert == NULL)
        return;

    QDateTime log = m_projectExpert->logRecentUpdateTime();
    QDateTime alarm = m_projectExpert->alarmRecentUpdateTime();

    bool logChanged = false;
    bool alarmChanged = false;

    if (log > m_lastLogTime)
    {
        m_lastLogTime = log;
        logChanged = true;
    }
    if (alarm > m_lastAlarmTime)
    {
        m_lastAlarmTime = alarm;
        alarmChanged = true;
    }

    for(int i = 0; i < m_forms->openedList()->count(); i++)
    {
        CMDHMIForm* form= m_forms->openedList()->at(i);

        if (logChanged)
            form->run()->notify(NotifyType::Log);
        if (alarmChanged)
            form->run()->notify(NotifyType::Alarm);
    }
}

void CHMIFramework::moveForm(const QString &fullName, const QRect &rect)
{
    for(int i = 0; i < m_forms->openedList()->count(); i++)
    {
        CMDHMIForm* form= m_forms->openedList()->at(i);

        if (form->common()->fullName().compare(fullName, Qt::CaseInsensitive) == 0)
        {
            form->setRect(rect);
            break;
        }
    }
}
void CHMIFramework::moveForm(const QString &fullName, const QPoint &point)
{
    for(int i = 0; i < m_forms->openedList()->count(); i++)
    {
        CMDHMIForm* form= m_forms->openedList()->at(i);

        if (form->common()->fullName().compare(fullName, Qt::CaseInsensitive) == 0)
        {
            QRect r = form->rect();
            r.moveTopLeft(point);
            form->setRect(r);
            break;
        }
    }
}
//////////////////////////////////////////////////////////////////////////
void CHMIFramework::changeLanguage()
{
    for(int i = 0; i < m_forms->openedList()->count(); i++)
        m_forms->openedList()->at(i)->changeLanguage();

    if(m_manager != NULL)
    {
        m_manager->fileNode()->changeLanguage(NULL);
        m_manager->changeLanguage();
    }
}
void CHMIFramework::collectWords()
{
    QStringList names;
    m_manager->fileNode()->getFileNames(m_hmiPath, names);
    for(int i = 0; i < names.count(); i++)
    {
        bool hasOpened = false;
        CMDHMIForm* f = m_forms->openBackgroundForm(names.at(i), hasOpened);
        if(f == NULL)
            continue;
        f->collectWords();
        //!如果是后台打开
        if(!hasOpened)
        {
            f->close();
            delete f;
        }
    }
}
//////////////////////////////////////////////////////////////////////////
QString CHMIFramework::storageImagePath()
{
    if(isStudioMode())
        return m_manager->storageImagePath();

    return "";
}
void CHMIFramework::initProjectImagePath()
{
    m_projectImagePath = m_projectConfiger->path() + "/image";

    if(!m_isStudioMode)
        return;

    QDir dir(m_projectImagePath);
    if (!dir.exists())
        dir.mkdir(m_projectImagePath);
}
//////////////////////////////////////////////////////////////////////////
QString CHMIFramework::rootName()
{
    return QObject::tr("图形");
}
QIcon *CHMIFramework::rootIcon()
{
    return m_rootIcon;
}
//////////////////////////////////////////////////////////////////////////
void CHMIFramework::log(const QString &message)
{
    m_log->LOG_INFO(message);
}
void CHMIFramework::initLog()
{
    QString path = QCoreApplication::applicationDirPath();
    m_log = CMDLogSwitchFactory::createSwitchWithLogger(path, "HMI", "");
    m_log->turnOn();
}
//////////////////////////////////////////////////////////////////////////


