#include "InputVariableDialog.h"
#include "ui_InputVariableDialog.h"
#include "MDMultiLanguage.h"

CInputVariableDialog::CInputVariableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CInputVariableDialog)
{
    ui->setupUi(this);//t

    m_framework = NULL;

    QStringList types;
    types << "QDialog" << "QLineEdit";
    CMDMultiLanguage::instance()->translateWidget(this, &types);
}
CInputVariableDialog::~CInputVariableDialog()
{
    delete ui;
}
//////////////////////////////////////////////////////////////////////////
void CInputVariableDialog::on_pushButtonOk_clicked()
{
    QString str = ui->lineEditInput->text().simplified();
    double d = 0;
    QVariant v;

    switch (v.type())
    {
    //num
    case QMetaType::Double:
    case QMetaType::Int:
    case QMetaType::UInt:
    case QMetaType::LongLong:
    case QMetaType::ULongLong:
    case QMetaType::Bool:
    {
        bool ok = false;
        d = str.toDouble(&ok);
        v = QVariant(d);
    }
        break;
    //other
    default:
        v = QVariant(str);
        break;
    }

    m_framework->setVariableValue(m_name, v);
    accept();
}
void CInputVariableDialog::on_pushButtonCancel_clicked()
{
    reject();
}
//////////////////////////////////////////////////////////////////////////
void CInputVariableDialog::init(const QString &name, IHMIFramework* framework)
{
    m_name = name;
    m_framework = framework;

    setWindowTitle(name);
    ui->lineEditCurrent->setText(framework->variableValue(name).toString());
    ui->lineEditInput->setFocus();
}
//////////////////////////////////////////////////////////////////////////
