#include "MDRecipeDownloadParaDialog.h"
#include "ui_MDRecipeDownloadParaDialog.h"
#include "MDRecipeProperty_Config.h"
#include <QMessageBox>
#include "MDStringResource.h"

CMDRecipeDownloadParaDialog::CMDRecipeDownloadParaDialog(QWidget *parent) :
    CMDObjectPropertyDialog(parent),
    ui(new Ui::CMDRecipeDownloadParaDialog)
{
    ui->setupUi(this);
}

CMDRecipeDownloadParaDialog::~CMDRecipeDownloadParaDialog()
{
    delete ui;
}

void CMDRecipeDownloadParaDialog::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);

    CMDRecipeProperty_Config* src = (CMDRecipeProperty_Config*)sourceProperty;

    // 下载模式
    QStringList modeList;
    CMDRecipe::enumDownloadModes(modeList);
    foreach (const QString& modeID, modeList)
    {
        DownloadMode dMode = CMDRecipe::stringToMode(modeID);
        ui->downloadModeComboBox->addItem(modeID, QVariant((int)dMode));
    }
    ui->downloadModeComboBox->setCurrentText(CMDRecipe::modeToString(src->head()->downloadMode()));
    ui->downloadRetryCountEdit->setText(QVariant(src->head()->retryCount()).toString());
    ui->downloadTimeOutEdit->setText(QVariant(src->head()->timeOut()).toString());
    ui->isDynamicCheckBox->setChecked(src->head()->isDynamic());
    ui->isSameValueIgnoredCheckBox->setChecked(src->head()->isSameValueIgnored());
    ui->itemsCountOnGeneralEdit->setText(QVariant(src->head()->itemsCountOnGeneral()).toString());

    enableUI(src->head()->downloadMode());

//    if(m_twTool)
//    {
//        m_twTool->init(this);
//        m_twTool->changeLanguage();
//    }
}

bool CMDRecipeDownloadParaDialog::UI2Property(void *destProperty)
{
    CMDRecipeProperty_Config* dest = (CMDRecipeProperty_Config*)destProperty;

    DownloadMode mode = (DownloadMode)ui->downloadModeComboBox->currentData().toInt();
    dest->head()->setDownloadMode(mode);
    dest->head()->setDynamic(Qt::Checked == ui->isDynamicCheckBox->checkState());
    dest->head()->setItemsCountOnGeneral(QVariant(ui->itemsCountOnGeneralEdit->toPlainText()).toInt());
    dest->head()->setRetryCount(QVariant(ui->downloadRetryCountEdit->toPlainText()).toInt());
    dest->head()->setSameValueIgnored(Qt::Checked == ui->isSameValueIgnoredCheckBox->checkState());
    dest->head()->setTimeOut(QVariant(ui->downloadTimeOutEdit->toPlainText()).toInt());

    return true;
}

void CMDRecipeDownloadParaDialog::enableUI(DownloadMode mode)
{
    switch (mode)
    {
    case DownloadMode::general:
    {
        ui->label_2->setVisible(true);
        ui->itemsCountOnGeneralEdit->setVisible(true);
    }
    break;
    case DownloadMode::quick:
    {
        ui->label_2->setVisible(false);
        ui->itemsCountOnGeneralEdit->setVisible(false);
    }
    break;
    default:
        break;
    }
}

void CMDRecipeDownloadParaDialog::on_downloadModeComboBox_currentIndexChanged(const QString &arg1)
{
    DownloadMode mode = (DownloadMode)ui->downloadModeComboBox->currentData().toInt();
    enableUI(mode);
}
