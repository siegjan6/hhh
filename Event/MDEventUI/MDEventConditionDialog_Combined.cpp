#include "MDEventConditionDialog_Combined.h"
#include "ui_MDEventConditionDialog_Combined.h"
#include <QMessageBox>
#include "MDEventProperty_Config.h"
#include "MDStringResource.h"

CMDEventConditionDialog_Combined::CMDEventConditionDialog_Combined(QWidget *parent) :
    CMDObjectPropertyDialog(parent),
    ui(new Ui::CMDEventConditionDialog_Combined)
{
    ui->setupUi(this);

    QStringList exs;
    exs << "QLineEdit";
    setLanguageExceptions(&exs);
}

CMDEventConditionDialog_Combined::~CMDEventConditionDialog_Combined()
{
    delete ui;
}

void CMDEventConditionDialog_Combined::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);

    CMDEventProperty_Config* src = (CMDEventProperty_Config*)sourceProperty;
    ui->expressionEdit->setText(src->expressionString());
}

bool CMDEventConditionDialog_Combined::UI2Property(void *destProperty)
{
    if(ui->expressionEdit->text().isEmpty())
    {
        QMessageBox::warning(NULL, STRANS(STRING_ERROR), STRANS(STRING_EVENT_EXPRESSION_IS_EMPTY), QMessageBox::Ok);
        return false;
    }

    CMDEventProperty_Config* dest = (CMDEventProperty_Config*)destProperty;
    dest->setExpressionString(ui->expressionEdit->text());

    return true;
}
