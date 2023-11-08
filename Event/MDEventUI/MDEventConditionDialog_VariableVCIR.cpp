#include "MDEventConditionDialog_VariableVCIR.h"
#include "ui_MDEventConditionDialog_VariableVCIR.h"
#include "EPC_VariableValueChangedInRange.h"
#include "MDStringResource.h"

CMDEventConditionDialog_VariableVCIR::CMDEventConditionDialog_VariableVCIR(QWidget *parent) :
    CMDObjectPropertyDialog(parent),
    ui(new Ui::CMDEventConditionDialog_VariableVCIR)
{
    ui->setupUi(this);

    QStringList exs;
    exs << "QLineEdit"<<"QComboBox";
    setLanguageExceptions(&exs);
}

CMDEventConditionDialog_VariableVCIR::~CMDEventConditionDialog_VariableVCIR()
{
    delete ui;
}

void CMDEventConditionDialog_VariableVCIR::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);

    CEPC_VariableValueChangedInRange* src = (CEPC_VariableValueChangedInRange*)sourceProperty;

    QStringList operatorList;
    CValueRangeCondition::enumOperators(operatorList);

    ui->operator1ComboBox->addItems(operatorList);
    MDOperator operator1;
    QVariant operand1;
    src->rangeCondition().condition1(operator1, operand1);
    ui->operator1ComboBox->setCurrentText(CValueRangeCondition::operatorToString(operator1));
    ui->value1Edit->setText(operand1.toString());

    ui->operator2ComboBox->addItems(operatorList);
    MDOperator operator2;
    QVariant operand2;
    src->rangeCondition().condition2(operator2, operand2);
    ui->operator2ComboBox->setCurrentText(CValueRangeCondition::operatorToString(operator2));
    ui->value2Edit->setText(operand2.toString());

    QStringList relationshipList;
    CValueRangeCondition::enumRelationships(relationshipList);
    ui->relationshipComboBox->addItems(relationshipList);
    ui->relationshipComboBox->setCurrentText(CValueRangeCondition::relationshipToString(src->rangeCondition().relationship()));
}

bool CMDEventConditionDialog_VariableVCIR::UI2Property(void *destProperty)
{
    CEPC_VariableValueChangedInRange* dest = (CEPC_VariableValueChangedInRange*)destProperty;

    dest->rangeCondition().setCondition1(CValueRangeCondition::stringToOperator(ui->operator1ComboBox->currentText()), QVariant(ui->value1Edit->text()));
    dest->rangeCondition().setCondition2(CValueRangeCondition::stringToOperator(ui->operator2ComboBox->currentText()), QVariant(ui->value2Edit->text()));
    dest->rangeCondition().setRelationship(CValueRangeCondition::stringToRelationship(ui->relationshipComboBox->currentText()));

    return true;
}
