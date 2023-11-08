#include "DBstudioFramework.h"
#include "ui_mainwindow.h"
#include "DataArchivesUI.h"
#include "DBSourceParamWidget.h"
#include "MDDataSrcTabWidget.h"
#include "IMDDataSourceManager.h"
#include "IMDDataSource.h"
#include "IMDDataTable.h"
#include "IMDDataArchives.h"
#include "MDMultiLanguage.h"

#include <QtWidgets>
#include <QMessageBox>
#include<QLayout>

CDBstudioFramework::CDBstudioFramework(QWidget *parent) :
    CBaseForm(parent),
    ui(new Ui::DBstudioFramework)
{
    ui->setupUi(this);

    m_pDataArchivesUI          = NULL; //! 数据归档窗体
    m_pDataSourceManager       = NULL; //! 数据源管理器
    m_iParamType               = 0;    //! 负责参数窗体与数据源窗体切换标识

    m_pDataSourceParamWidget = new CDBSourceParamWidget;
    m_pDataSourceTableWidget = new CMDDataSrcTabWidget;

    ui->stackedWidget->addWidget(m_pDataSourceParamWidget);
    ui->stackedWidget->addWidget(m_pDataSourceTableWidget);
    ui->stackedWidget->setCurrentWidget(m_pDataSourceParamWidget);
    ui->stackedWidget->resize(DATASRC_WIDGET_INIT_W, DATASRC_WIDGET_INIT_H);
}

CDBstudioFramework::~CDBstudioFramework()
{
    if (ui)
    {
        delete ui;
        ui = NULL;
    }

    if(m_pDataSourceParamWidget)
    {
        delete m_pDataSourceParamWidget;
        m_pDataSourceParamWidget = NULL;
    }

    if(m_pDataSourceTableWidget)
    {
        delete m_pDataSourceTableWidget;
        m_pDataSourceTableWidget = NULL;
    }

    delete ui;
}

void CDBstudioFramework::closeEvent(QCloseEvent *e)
{
    CBaseForm::closeEvent(e);
    save();
    m_pDataSourceParamWidget->close();
    m_pDataSourceTableWidget->close();    
    if(m_pDataArchivesUI)
    {
        m_pDataArchivesUI->closeEvent(this);
    }

    e->accept();

}
bool CDBstudioFramework::save()
{
    QString strError = "";
    return CheckError(strError);
}

void CDBstudioFramework::initUI(IMDDataArchives *pDataArchives, IMDDataSource *pDataSource)
{
    m_iParamType = 0;
    if(NULL == m_pDataSourceParamWidget || NULL == pDataArchives)
    {
        return;
    }

    if(pDataSource)
    {
        setWindowTitle(pDataSource->name());
    }

    m_pDataSourceManager = pDataArchives->dataSourceManager();
    ui->stackedWidget->setCurrentWidget(m_pDataSourceParamWidget);
//    m_pDataSourceParamWidget->setDataSource(pDataSource);
//    m_pDataSourceParamWidget->setDataArchivesUI(m_pDataArchivesUI);
    m_pDataSourceParamWidget->initUI(pDataSource, m_pDataArchivesUI);
//    m_pDataSourceParamWidget->changeLanguage();
}

void CDBstudioFramework::initUI(IMDDataArchives *pDataArchives, IMDDataTable *pDataTable, IMDDataSource *pDataSource)
{
    m_iParamType = 1;
    if(NULL == m_pDataSourceTableWidget)
    {
        return;
    }

    if(pDataTable)
    {
        setWindowTitle(pDataTable->name());
    }

    ui->stackedWidget->setCurrentWidget(m_pDataSourceTableWidget);
    m_pDataSourceTableWidget->setDataArchives(pDataArchives);
    m_pDataSourceTableWidget->initData(pDataTable, pDataTable->dataGroupName(), pDataSource->name());
    m_pDataSourceTableWidget->setDataArchivesUI(m_pDataArchivesUI);
//    m_pDataSourceTableWidget->changeLanguage();
}

void CDBstudioFramework::changeLanguage()
{
    int index = ui->stackedWidget->currentIndex();
    if (0 == index)
    {
        m_pDataSourceParamWidget->changeLanguage();
    }
    else if (1 == index)
    {
        m_pDataSourceTableWidget->changeLanguage();
    }
}

void CDBstudioFramework::GotoError()
{
    QString strError = "";
    CheckError(strError);
}

bool CDBstudioFramework::CheckError(QString &/*strError*/)
{
    bool bSuccess = true;
    if(m_iParamType == 0)
    {
        if(m_pDataSourceParamWidget)
        {
            m_pDataSourceParamWidget->save();
            m_pDataSourceManager->save();
        }
    }
    else if(m_iParamType == 1)
    {       
        if(m_pDataSourceTableWidget)
        {
            m_pDataSourceTableWidget->save();
        }
    }
    return bSuccess;
}
