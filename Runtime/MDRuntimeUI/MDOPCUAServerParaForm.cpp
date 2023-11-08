#include "MDOPCUAServerParaForm.h"
#include "ui_MDOPCUAServerParaForm.h"
#include "MDProjectConfiger.h"
#include "MDProjectPara.h"
#include "MDSecurityPolicyConfiger.h"
#include "MDSecurityPolicyPara.h"
#include "MDActionSPUI.h"
#include <QSettings>
#include "MDStringResource.h"

CMDOPCUAServerParaForm::CMDOPCUAServerParaForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CMDOPCUAServerParaForm)
{
    ui->setupUi(this);
    ui->rb_authFile->setVisible(false);

    ui->spTableWidget->setColumnWidth(0, 100);
    ui->spTableWidget->setColumnWidth(1, 300);
    connect(ui->spTableWidget, &QTableWidget::itemDoubleClicked, this, &CMDOPCUAServerParaForm::tableWidgetDoubleClicked);

    m_configer = NULL;
}

CMDOPCUAServerParaForm::~CMDOPCUAServerParaForm()
{
    delete ui;
}

bool CMDOPCUAServerParaForm::initial(CMDProjectConfiger* configer)
{
    m_configer = configer;

    CMDProjectPara* pPara = configer->projectPara();
    ui->spinBox_port->setValue(pPara->port());
    ui->le_cert->setText(pPara->certificatePath());
    ui->le_pkey->setText(pPara->privateKeyPath());
    if((pPara->caMode() & 1) == 1)
    {
        ui->rb_anonymous->setChecked(true);
    }
    else if((pPara->caMode() & 2) == 2)
    {
        ui->rb_userPassword->setChecked(true);
    }
    else if((pPara->caMode() & 4) == 4)
    {
        ui->rb_authFile->setChecked(true);
    }
    else if((pPara->caMode() & 8) == 8)
    {
        ui->rb_authFile->setChecked(true);
    }

//    m_destList.append(MDActionType::project_Load);
//    m_destList.append(MDActionType::project_Unload);
//    m_destList.append(MDActionType::project_ExitFS);
//    m_destList.append(MDActionType::account_powergroup_Manage);
//    m_destList.update(*configer->projectPara()->spList(), configer->path());
//    updateTableWidget();
    return true;
}

bool CMDOPCUAServerParaForm::accept()
{
    if(!m_configer)
        return false;

    CMDProjectPara* pPara = m_configer->projectPara();
    pPara->setPort(ui->spinBox_port->value());
    pPara->setCertificatePath(ui->le_cert->text());
    pPara->setPrivateKeyPath(ui->le_pkey->text());
    quint16 caMode = 0;
    if(ui->rb_anonymous->isChecked())
    {
        caMode = 1;
    }
    else if(ui->rb_userPassword->isChecked())
    {
        caMode = 2;
        if(!pPara->certificatePath().isEmpty())
        {
            caMode = caMode | 4;
        }
        if(!pPara->privateKeyPath().isEmpty())
        {
            caMode = caMode | 8;
        }
    }
    else if(ui->rb_authFile->isChecked())
    {
        if(!pPara->certificatePath().isEmpty())
        {
            caMode = caMode | 4;
        }
        if(!pPara->privateKeyPath().isEmpty())
        {
            caMode = caMode | 8;
        }
    }
    pPara->setCAMode(caMode);

    return m_configer->saveToParaFile();
}

void CMDOPCUAServerParaForm::tableWidgetDoubleClicked(QTableWidgetItem* item)
{
    CMDActionSP* sp = m_destList.at(item->row());

    CMDActionSPUI spUI;
    spUI.init(m_configer->powerGroupConfiger());
    if(spUI.doModalActionSPDialog(sp))
        updateTableWidget();
}

void CMDOPCUAServerParaForm::updateTableWidget()
{
    ui->spTableWidget->setRowCount(0);
    ui->spTableWidget->clearContents();

    int count = m_destList.count();
    for(int i = 0; i < count; i++)
    {
        ui->spTableWidget->insertRow(i);

        CMDActionSP* sp = m_destList.at(i);

        QTableWidgetItem* ti0 = new QTableWidgetItem();
        QString actionType = STRANS(CMDActionID::typeToString(sp->actionID().type()));
        ti0->setData(Qt::DisplayRole, QVariant(actionType));
        ui->spTableWidget->setItem(i, 0, ti0);

        QString spText = STRANS(STRING_NOT_ENABLED);
        if(sp->isEnable())
        {
            spText = STRANS(CMDActionSP::modeToString(sp->protectMode()));
            spText += "::";
            spText += STRANS(CMDActionSP::sigTypeToString(sp->sigType()));
        }
        QTableWidgetItem* ti1 = new QTableWidgetItem;
        ti1->setData(Qt::DisplayRole, QVariant(spText));
        ui->spTableWidget->setItem(i, 1, ti1);

        QTableWidgetItem* ti2 = new QTableWidgetItem(QTableWidgetItem::ItemType::UserType);
        ti2->setData(Qt::UserRole, QVariant(-1, sp));
        ui->spTableWidget->setItem(i, 2, ti2);
    }
}
