#include "MDActionGeneralDialog.h"
#include "ui_MDActionGeneralDialog.h"
#include "MDActionProperty_Config.h"
#include <QMessageBox>
#include "MDActionTreeWindow.h"
#include "MDStringResource.h"

CMDActionGeneralDialog::CMDActionGeneralDialog(QWidget *parent) :
    CMDObjectPropertyDialog(parent),
    ui(new Ui::CMDActionGeneralDialog)
{
    ui->setupUi(this);
    m_type = MDActionType::null_action;

    QStringList exs;
    exs << "QLineEdit";
    setLanguageExceptions(&exs);
}

CMDActionGeneralDialog::~CMDActionGeneralDialog()
{
    delete ui;
}

void CMDActionGeneralDialog::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);

    CMDActionProperty_Config* src = (CMDActionProperty_Config*)sourceProperty;

    ui->nameEdit->setText(src->name());
    ui->descriptionEdit->setText(src->comment());
    ui->objectEdit->setText(src->actionID().objName());
    QString typeString = STRANS(CMDActionID::typeToString(src->actionID().type()));
    ui->typeEdit->setText(typeString);
    ui->delayCheckBox->setChecked(src->isEnableDelay());
    ui->delayLengthEdit->setText(QVariant(src->delayLength()).toString());
    onDelayChanged(src->isEnableDelay() ? Qt::Checked : Qt::Unchecked);
    ui->execExpCheckBox->setChecked(src->isEnableExecExp());
    ui->execExpEdit->setText(QVariant(src->execExp()).toString());
    onExecExpChanged(src->isEnableExecExp() ? Qt::Checked : Qt::Unchecked);
    ui->enableLogCheckBox->setChecked(src->isEnableLog());

    connect(ui->delayCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onDelayChanged(int)));

    m_type = src->actionID().type();
}

void CMDActionGeneralDialog::onDelayChanged(int state)
{
    ui->delayLengthEdit->setVisible(state == Qt::Checked);
    ui->delayUnitLabel->setVisible(state == Qt::Checked);
}

void CMDActionGeneralDialog::onExecExpChanged(int state)
{
    ui->execExpEdit->setVisible(state == Qt::Checked);
}

bool CMDActionGeneralDialog::UI2Property(void *destProperty)
{
    if(ui->nameEdit->text().isEmpty())
    {
        QMessageBox::warning(NULL, STRANS(STRING_ERROR), STRANS(STRING_NAME_IS_EMPTY), QMessageBox::Ok);
        return false;
    }

    CMDActionProperty_Config* dest = (CMDActionProperty_Config*)destProperty;

    dest->setName(ui->nameEdit->text());
    dest->setComment(ui->descriptionEdit->text());
    dest->actionID().setObjName(ui->objectEdit->text());
    dest->setEnableDelay(Qt::Checked == ui->delayCheckBox->checkState());
    dest->setDelayLength((qint32)(QVariant(ui->delayLengthEdit->text()).toInt()));
    dest->setEnableExecExp(Qt::Checked == ui->execExpCheckBox->checkState());
    dest->setExecExp(ui->execExpEdit->text());
    dest->setEnableLog(Qt::Checked == ui->enableLogCheckBox->checkState());

    CMDMultiLanguage::instance()->addWord(dest->comment(), false);

    return true;
}

void CMDActionGeneralDialog::on_selectObjectButton_clicked()
{
    if(!objectUI())
        return;
    CMDActionTreeWindow* treeWindow = (CMDActionTreeWindow*)objectUI();

    QString dest;
    if(treeWindow->selectSingleObject(m_type, ui->objectEdit->text(), dest))
    {
        ui->objectEdit->setText(dest);
    }
}
