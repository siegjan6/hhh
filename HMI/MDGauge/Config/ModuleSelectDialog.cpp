#include "ModuleSelectDialog.h"
#include "ui_ModuleSelectDialog.h"
#include "MDMultiLanguage.h"

CModuleSelectDialog::CModuleSelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CModuleSelectDialog)
{
    ui->setupUi(this);
    m_index = 0;
    this->setWindowTitle(tr("组件选择"));
    QObject::connect(ui->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem *)),
                     this,SLOT(moduleSelect(QListWidgetItem *)));
     CMDMultiLanguage::instance()->translateWidget(this);
}

CModuleSelectDialog::~CModuleSelectDialog()
{
    delete ui;
}

void CModuleSelectDialog::moduleSelect(QListWidgetItem *item)
{
    if(item != NULL)
    {
        m_index = ui->listWidget->currentRow();
        accept();
    }
    else
        close();
}

void CModuleSelectDialog::on_CancelBtn_clicked()
{
    this->close();
}
/*!
\section 获取在listWidget上选择的索引从而确定选择添加的哪个仪表组件。
*/
void CModuleSelectDialog::on_OKBtn_clicked()
{
    QListWidgetItem* listCurrentItem = ui->listWidget->currentItem();
    if(listCurrentItem != NULL)
    {
        m_index = ui->listWidget->currentRow();
        accept();
    }
    else
        close();
}
