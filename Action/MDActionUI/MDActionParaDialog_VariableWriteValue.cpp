#include "MDActionParaDialog_VariableWriteValue.h"
#include "ui_MDActionParaDialog_VariableWriteValue.h"
#include "MDStringResource.h"

CMDActionParaDialog_VariableWriteValue::CMDActionParaDialog_VariableWriteValue(QWidget *parent) :
    CMDObjectPropertyDialog(parent),
    ui(new Ui::CMDActionParaDialog_VariableWriteValue)
{
    ui->setupUi(this);

    QStringList exs;
    exs << "QLineEdit"<<"QComboBox";
    setLanguageExceptions(&exs);
}

CMDActionParaDialog_VariableWriteValue::~CMDActionParaDialog_VariableWriteValue()
{
    delete ui;
}

void CMDActionParaDialog_VariableWriteValue::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);

    CAPC_VariableWriteValue* src = (CAPC_VariableWriteValue*)sourceProperty;

    QStringList modeList;
    CAPC_VariableWriteValue::enumMode(modeList);

    ui->modeComboBox->addItems(modeList);
    ui->modeComboBox->setCurrentText(CAPC_VariableWriteValue::modeToString(src->mode()));
    ui->valueEdit->setText(src->value().toString());
}

bool CMDActionParaDialog_VariableWriteValue::UI2Property(void *destProperty)
{
    CAPC_VariableWriteValue* dest = (CAPC_VariableWriteValue*)destProperty;

    dest->setMode(CAPC_VariableWriteValue::stringToMode(ui->modeComboBox->currentText()));
    dest->setValue(QVariant(ui->valueEdit->text()));

    return true;
}
