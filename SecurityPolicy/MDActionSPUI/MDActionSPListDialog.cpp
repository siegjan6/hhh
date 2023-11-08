#include "MDActionSPListDialog.h"
#include "ui_MDActionSPListDialog.h"
#include "MDActionSP.h"
#include "MDActionID.h"
#include "MDActionSPDialog.h"
#include "MDActionSPListDialogParaImp.h"
#include "MDStringResource.h"

CMDActionSPListDialog::CMDActionSPListDialog(QWidget *parent) :
    CMDObjectPropertyDialog(parent),
    ui(new Ui::CMDActionSPListDialog)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnWidth(0, 100);
    ui->tableWidget->setColumnWidth(1, 300);

    m_para = new CMDActionSPListDialogParaImp;
}

CMDActionSPListDialog::~CMDActionSPListDialog()
{
    delete ui;

    if(m_para)
        delete m_para;
}

void CMDActionSPListDialog::setPowerGroupConfiger(CMDPowerGroupConfiger* configer)
{
    m_para->m_pgConfiger = configer;
}

CMDPowerGroupConfiger* CMDActionSPListDialog::powerGroupConfiger()
{
    return m_para->m_pgConfiger;
}

void CMDActionSPListDialog::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);

    initDestList(sourceProperty, &m_para->m_destList);

    connect(ui->tableWidget, &QTableWidget::itemDoubleClicked, this, &CMDActionSPListDialog::tableWidgetDoubleClicked);

    updateTableWidget();
}

bool CMDActionSPListDialog::UI2Property(void *destProperty)
{
    return true;
    //return CMDObjectPropertyDialog::UI2Property(destProperty);
}

void CMDActionSPListDialog::initDestList(void *sourceProperty, CMDActionSPList* destList)
{
}

CMDActionSPList* CMDActionSPListDialog::destList()
{
    return &m_para->m_destList;
}

void CMDActionSPListDialog::setDestList(const CMDActionSPList* srcList)
{
    m_para->m_destList = *srcList;
}

void CMDActionSPListDialog::tableWidgetDoubleClicked(QTableWidgetItem* item)
{
    CMDActionSP* sp = m_para->m_destList.at(item->row());

    CMDActionSPDialog dlg;
    dlg.initial(m_para->m_pgConfiger, sp);
    dlg.exec();
    if(QDialog::Accepted == dlg.result())
        updateTableWidget();
}

void CMDActionSPListDialog::updateTableWidget()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->clearContents();

    int count = m_para->m_destList.count();
    for(int i = 0; i < count; i++)
    {
        ui->tableWidget->insertRow(i);

        CMDActionSP* sp = m_para->m_destList.at(i);

        QTableWidgetItem* ti0 = new QTableWidgetItem();
        ti0->setData(Qt::DisplayRole, QVariant(CMDActionID::typeToString(sp->actionID().type())));
        ui->tableWidget->setItem(i, 0, ti0);

        QString spText = STRANS(STRING_NOT_ENABLED);
        if(sp->isEnable())
        {
            spText = STRANS(CMDActionSP::modeToString(sp->protectMode()));
            spText += "::";
            spText += STRANS(CMDActionSP::sigTypeToString(sp->sigType()));
        }
        QTableWidgetItem* ti1 = new QTableWidgetItem;
        ti1->setData(Qt::DisplayRole, QVariant(spText));
        ui->tableWidget->setItem(i, 1, ti1);

        QTableWidgetItem* ti2 = new QTableWidgetItem(QTableWidgetItem::ItemType::UserType);
        ti2->setData(Qt::UserRole, QVariant(-1, sp));
        ui->tableWidget->setItem(i, 2, ti2);
    }
}
