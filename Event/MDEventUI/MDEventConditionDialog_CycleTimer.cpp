#include "MDEventConditionDialog_CycleTimer.h"
#include "ui_MDEventConditionDialog_CycleTimer.h"
#include <QMessageBox>
#include "EPC_TimeCycle.h"
#include "MDStringResource.h"

CMDEventConditionDialog_CycleTimer::CMDEventConditionDialog_CycleTimer(QWidget *parent) :
    CMDObjectPropertyDialog(parent),
    ui(new Ui::CMDEventConditionDialog_CycleTimer)
{
    ui->setupUi(this);

    QStringList exs;
    exs << "QLineEdit";
    setLanguageExceptions(&exs);
}

CMDEventConditionDialog_CycleTimer::~CMDEventConditionDialog_CycleTimer()
{
    delete ui;
}

void CMDEventConditionDialog_CycleTimer::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);

    CEPC_TimeCycle* src = (CEPC_TimeCycle*)sourceProperty;

    quint32 minutes;
    quint32 seconds;
    quint32 millSeconds;
    src->to_M_S_Mill(minutes, seconds, millSeconds);

    ui->cycleEdit->setText(QVariant(minutes).toString());
    ui->cycleEditSecond->setText(QVariant(seconds).toString());
    ui->cycleEditMill->setText(QVariant(millSeconds).toString());
}

bool CMDEventConditionDialog_CycleTimer::UI2Property(void *destProperty)
{
    CEPC_TimeCycle* dest = (CEPC_TimeCycle*)destProperty;

    dest->setTimeCycle(QVariant(ui->cycleEdit->text()).toInt(),
                       QVariant(ui->cycleEditSecond->text()).toInt(),
                       QVariant(ui->cycleEditMill->text()).toInt());

    return true;
}
