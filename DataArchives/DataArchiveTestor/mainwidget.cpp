#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <MDDataArchivesEngine.h>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QFileDialog>
#include <QPluginLoader>
#include <ITestorInterface.h>
#include <JSScriptExcutor.h>
#include "newprojectdialog.h"
#include "TestorGlobalInst.h"
#include "testdatagroupmgr.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    setWindowState(Qt::WindowMaximized);//窗体最大化
    ui->setupUi(this);
    CreateMenus();
    m_pContentWidget = new ContentQSplitter(this);
    InitLayout();
    loadPlugins();
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::DisplayReSize()
{
    this->m_pContentWidget->DisplayReSize();
}

void MainWidget::CreateMenus()
{
    m_pMenuBar = new QMenuBar(this);
    m_pFileMenu = m_pMenuBar->addMenu(tr("文件"));

    m_pNewProjectAction = new QAction(tr("新建项目"), m_pFileMenu);
    connect(m_pNewProjectAction, SIGNAL(triggered()), this, SLOT(slotNewProject()));
    m_pFileMenu->addAction(m_pNewProjectAction);

    m_pOpenProjectAction = new QAction(tr("打开项目"), m_pFileMenu);
    connect(m_pOpenProjectAction, SIGNAL(triggered()), this, SLOT(slotOpenProject()));
    m_pFileMenu->addAction(m_pOpenProjectAction);
}

void MainWidget::InitLayout()
{
    QVBoxLayout * vLayout = new QVBoxLayout(this);
    //void QLayout::setContentsMargins(int left, int top, int right, int bottom)
    //margin 版心外的空白
     vLayout->setContentsMargins(0, 0, 0, 0);//使窗体仅仅贴着窗体边框
    //水平方向自动最小取值为sizeHint中的宽度 故会拉伸宽度
    //垂直方向限制最大尺寸为sizeHint中的高度 故高度为菜单的高度
    m_pMenuBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    //m_pMenuBar->setMaximumHeight(100);
    vLayout->addWidget(m_pMenuBar);
    vLayout->addWidget(m_pContentWidget);
}

void MainWidget::slotNewProject()
{
    //m_pContentWidget->ResetUI();
    NewProjectDialog dlg;
    if(dlg.exec() != QDialog::Accepted)
        return ;

    QString projPath = dlg.GetProjectPath();
    if(projPath.isEmpty())
        return ;

    TestorGlobalInst::Reload(projPath);
    m_pContentWidget->ReloadUI();

    int pluginNum = m_Plugins.size();
    for(int i=0; i<pluginNum;++i)
    {
        m_Plugins[i]->SetParams(JSScriptExcutor::GetInst()->GetScriptEngine(), TestorGlobalInst::GetDataArchives(), TestDataGroupMgr::GetInst());
    }
}

void MainWidget::slotOpenProject()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("打开工程"),
                                                    "",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(dir.isEmpty())
        return ;

    TestorGlobalInst::Reload(dir);

    m_pContentWidget->ReloadUI();

    int pluginNum = m_Plugins.size();
    for(int i=0; i<pluginNum;++i)
    {
        m_Plugins[i]->SetParams(JSScriptExcutor::GetInst()->GetScriptEngine(), TestorGlobalInst::GetDataArchives(), TestDataGroupMgr::GetInst());
    }
}

void MainWidget::loadPlugins()
{

    QDir pluginsDir = QDir(qApp->applicationDirPath());

    /*
#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    */

    pluginsDir.cd("testorplugins");

    JSScriptExcutor * pJSScriptExcutor = JSScriptExcutor::GetInst();
    IMDDataArchives * pDataArchives = TestorGlobalInst::GetDataArchives();
    TestDataGroupMgr * pTestDataGroupMgr = TestDataGroupMgr::GetInst();

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin) {
            ITestorInterface * pTestorInterface = qobject_cast<ITestorInterface *>(plugin);
            if(pTestorInterface)
            {
                pTestorInterface->SetParams(pJSScriptExcutor->GetScriptEngine(), pDataArchives, pTestDataGroupMgr);
                m_Plugins.append(pTestorInterface);
            }
        }
    }
}
