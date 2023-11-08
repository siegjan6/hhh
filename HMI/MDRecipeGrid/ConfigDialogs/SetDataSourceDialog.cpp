#include "SetDataSourceDialog.h"
#include "ui_SetDataSourceDialog.h"
#include "ShareTool.h"

CSetDataSourceDialog::CSetDataSourceDialog(dataSourceType dataSource, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSetDataSourceDialog)
{
    ui->setupUi(this);

    m_dataSource = dataSource;

    CShareTool::initDataSourceTypeComboBox(ui->comboboxDataSource, m_dataSource);

    CShareTool::installDialogTranslator(this);
}

CSetDataSourceDialog::~CSetDataSourceDialog()
{
    delete ui;
}

void CSetDataSourceDialog::on_pushbuttonOk_clicked()
{
    m_dataSource = (dataSourceType)ui->comboboxDataSource->currentData().toInt();

    accept();
}

void CSetDataSourceDialog::on_pushbuttonCancel_clicked()
{
    reject();
}
