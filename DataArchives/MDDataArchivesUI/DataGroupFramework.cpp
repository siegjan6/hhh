#include "DataGroupFramework.h"
#include "ui_mainwindow.h"
#include "MDDataGroupTableWidget.h"
#include "MDDataArchivesInterface/IMDDataGroupManager.h"
#include "MDDataArchivesInterface/IMDDataArchives.h"
#include "IMDDataGroup.h"
#include "DataArchivesUI.h"
#include <QCloseEvent>
#include <QMessageBox>

CDataGroupFramework::CDataGroupFramework(QWidget *parent) :
    CBaseForm(parent),
    ui(new Ui::DBstudioFramework)
{
    ui->setupUi(this);

    m_pDataArchivesUI           = NULL;
    m_pDataGrpDlg = new CMDDataGroupTableWidget(this);
    ui->stackedWidget->addWidget(m_pDataGrpDlg);
    ui->stackedWidget->setCurrentWidget(m_pDataGrpDlg);
//    connect(m_pDataGrpDlg, SIGNAL(accepted()), this, SLOT(close()));
}

CDataGroupFramework::~CDataGroupFramework()
{
    if (ui)
    {
        delete ui;
        ui = NULL;
    }

    if (m_pDataGrpDlg)
    {
        delete m_pDataGrpDlg;
        m_pDataGrpDlg = NULL;
    }
}

bool CDataGroupFramework::save()
{
    if (m_pDataGrpDlg->dataArchives()->dataGroupManager())
    {
        m_pDataGrpDlg->save();
    }

    return true;
}

void CDataGroupFramework::setDataGrpProperty(IMDRTDBManagerUI *pRTDBManagerUI, IMDDataArchives *pDataArchives, IMDDataGroup *pDataGrp)
{
    if (m_pDataGrpDlg)
    {
        m_pDataGrpDlg->setDataGrpProperty(pRTDBManagerUI, pDataArchives, pDataGrp);
        setWindowTitle(pDataGrp->name());
    }
}

void CDataGroupFramework::setDataArchivesUI(CDataArchivesUI *pDataArchivesUI)
{
    if (m_pDataGrpDlg)
    {
        m_pDataArchivesUI = pDataArchivesUI;
        m_pDataGrpDlg->setDataArchivesUI(pDataArchivesUI);
    }
}

void CDataGroupFramework::changeLanguage()
{
    if (m_pDataGrpDlg)
    {
        m_pDataGrpDlg->changeLanguage();
    }
}

void CDataGroupFramework::closeEvent(QCloseEvent *e)
{
    CBaseForm::closeEvent(e); 
    m_pDataGrpDlg->save();
    m_pDataGrpDlg->close();    
    if(m_pDataArchivesUI)
    {
        m_pDataArchivesUI->closeEvent(this);
    }

    e->accept();
}
