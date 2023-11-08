#include "MDProjectParaForm.h"
#include "ui_MDProjectParaForm.h"
#include "MDProjectConfiger.h"
#include "MDProjectPara.h"
#include "MDSecurityPolicyConfiger.h"
#include "MDSecurityPolicyPara.h"
#include "MDActionSPUI.h"
#include <QSettings>
#include "MDStringResource.h"

CMDProjectParaForm::CMDProjectParaForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CMDProjectParaForm)
{
    ui->setupUi(this);

    ui->spTableWidget->setColumnWidth(0, 100);
    ui->spTableWidget->setColumnWidth(1, 300);
    connect(ui->spTableWidget, &QTableWidget::itemDoubleClicked, this, &CMDProjectParaForm::tableWidgetDoubleClicked);

    m_configer = NULL;
}

CMDProjectParaForm::~CMDProjectParaForm()
{
    delete ui;
}

bool CMDProjectParaForm::initial(CMDProjectConfiger* configer)
{
    m_configer = configer;

    CMDSecurityPolicyPara* spPara = configer->securityConfiger()->para();
    ui->enableSPCheckBox->setChecked(spPara->isEnableSP());
    ui->enableLoginBeforeRunCheckBox->setChecked(spPara->isEnableLoginBeforeRun());

    m_destList.append(MDActionType::project_Load);
    m_destList.append(MDActionType::project_Unload);
    m_destList.append(MDActionType::project_ExitFS);
    m_destList.append(MDActionType::account_powergroup_Manage);
    m_destList.update(*configer->projectPara()->spList(), configer->path());
    updateTableWidget();
    return true;
}

bool CMDProjectParaForm::accept()
{
    if(!m_configer)
        return false;

    CMDSecurityPolicyPara* spPara = m_configer->securityConfiger()->para();
    spPara->setEnableSP(ui->enableSPCheckBox->isChecked());
    spPara->setEnableLoginBeforeRun(ui->enableLoginBeforeRunCheckBox->isChecked());
    m_configer->securityConfiger()->saveToFile();

    m_configer->projectPara()->setSPList(&m_destList);

    //return m_configer->saveToParaFile();
    return true;
}

void CMDProjectParaForm::tableWidgetDoubleClicked(QTableWidgetItem* item)
{
    CMDActionSP* sp = m_destList.at(item->row());

    CMDActionSPUI spUI;
    spUI.init(m_configer->powerGroupConfiger());
    if(spUI.doModalActionSPDialog(sp))
        updateTableWidget();
}

void CMDProjectParaForm::updateTableWidget()
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
