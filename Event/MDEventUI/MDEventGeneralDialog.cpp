#include "MDEventGeneralDialog.h"
#include "ui_MDEventGeneralDialog.h"
#include "MDEventProperty_Config.h"
#include <QMessageBox>
#include "MDEventTreeWindow.h"
#include "MDStringResource.h"

CMDEventGeneralDialog::CMDEventGeneralDialog(QWidget *parent) :
    CMDObjectPropertyDialog(parent),
    ui(new Ui::CMDEventGeneralDialog)
{
    ui->setupUi(this);

    m_type = MDEventType::null_event;

    QStringList exs;
    exs << "QLineEdit";
    setLanguageExceptions(&exs);
}

CMDEventGeneralDialog::~CMDEventGeneralDialog()
{
    delete ui;
}

void CMDEventGeneralDialog::initial(void* eventUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(eventUI, sourceProperty);

    CMDEventProperty_Config* src = (CMDEventProperty_Config*)sourceProperty;
    ui->nameEdit->setText(src->name());
    ui->descriptionEdit->setText(src->comment());
    ui->objectEdit->setText(src->objName());
    QString typeString = STRANS(CMDEventProperty_Config::typeToString(src->type()));
    ui->typeEdit->setText(typeString);
    ui->enableLogCheckBox->setChecked(src->isEnableLog());

    m_type = src->type();

    if(MDEventType::timer_Cycle == m_type || MDEventType::combined == m_type)
    {
        ui->objectLabel->setVisible(false);
        ui->objectEdit->setVisible(false);
        ui->selectObjectButton->setVisible(false);
    }
}

bool CMDEventGeneralDialog::UI2Property(void *destProperty)
{
    if(ui->nameEdit->text().isEmpty())
    {
        QMessageBox::warning(NULL, STRANS(STRING_ERROR), STRANS(STRING_NAME_IS_EMPTY), QMessageBox::Ok);
        return false;
    }

    CMDEventProperty_Config* dest = (CMDEventProperty_Config*)destProperty;
    dest->setName(ui->nameEdit->text());
    dest->setComment(ui->descriptionEdit->text());
    dest->setObjName(ui->objectEdit->text());
    dest->setEnableLog(Qt::Checked == ui->enableLogCheckBox->checkState());

    CMDMultiLanguage::instance()->addWord(dest->comment(), false);

    return true;
}

void CMDEventGeneralDialog::on_selectObjectButton_clicked()
{
    if(!objectUI())
        return;
    CMDEventTreeWindow* treeWindow = (CMDEventTreeWindow*)objectUI();

    QString dest;
    if(treeWindow->selectSingleObject(m_type, ui->objectEdit->text(), dest))
    {
        ui->objectEdit->setText(dest);
    }
}

