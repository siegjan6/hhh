#include <QVector>

#include "LayerConfigDialog.h"
#include "ui_LayerConfigDialog.h"
#include "MDMultiLanguage.h"
//////////////////////////////////////////////////////////////////////////
CLayerConfigDialog::CLayerConfigDialog(CMDHMIForm *data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLayerConfigDialog)
{
    ui->setupUi(this);

    Q_ASSERT(data != NULL);
    m_data = data;
    init();

    CMDMultiLanguage::instance()->translateWidget(this);
}
CLayerConfigDialog::~CLayerConfigDialog()
{
    delete ui;
}
//////////////////////////////////////////////////////////////////////////
void CLayerConfigDialog::on_pushButtonOk_clicked()
{
    accept();
}
void CLayerConfigDialog::on_pushButtonCancel_clicked()
{
    reject();
}
//////////////////////////////////////////////////////////////////////////
void CLayerConfigDialog::init()
{
    connect(this, &QDialog::accepted, this, &CLayerConfigDialog::close);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    QVector<bool>* visibles = m_data->common()->visibleLayers();
    QVector<bool>* lockeds = m_data->common()->lockedLayers();

    for (int i = 0; i < visibles->count(); i++)
    {
        Qt::CheckState state = visibles->at(i) ? Qt::Checked : Qt::Unchecked;
        ui->tableWidget->item(i, 0)->setCheckState(state);
    }
    for (int i = 0; i < lockeds->count(); i++)
    {
        Qt::CheckState state = lockeds->at(i) ? Qt::Checked : Qt::Unchecked;
        ui->tableWidget->item(i, 1)->setCheckState(state);
    }
}
void CLayerConfigDialog::close()
{
    QVector<bool>* visibles = m_data->common()->visibleLayers();
    QVector<bool>* lockeds = m_data->common()->lockedLayers();

    for (int i = 0; i < visibles->count(); i++)
        (*visibles)[i] = (ui->tableWidget->item(i, 0)->checkState() == Qt::Checked);
    for (int i = 0; i < lockeds->count(); i++)
        (*lockeds)[i] = (ui->tableWidget->item(i, 1)->checkState() == Qt::Checked);
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////


