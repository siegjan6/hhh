#include "PrintConfigDialog.h"
#include "ui_PrintConfigDialog.h"
#include "MDMultiLanguage.h"

CPrintConfigDialog::CPrintConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CPrintConfigDialog)
{
    sDate = "&Date";
    sTime = "&Time";
    sPage = "&Page";
    sPageCount = "&Count";
    m_environment = NULL;
    m_lastFocusInEdit = NULL;

    ui->setupUi(this);

    ui->textEditLeftHeader->installEventFilter(this);
    ui->textEditMiddleHeader->installEventFilter(this);
    ui->textEditRightHeader->installEventFilter(this);
    ui->textEditLeftFooter->installEventFilter(this);
    ui->textEditMiddleFooter->installEventFilter(this);
    ui->textEditRightFooter->installEventFilter(this);

    QStringList types;
    types << "QTextEdit" << "QLineEdit";
    CMDMultiLanguage::instance()->translateWidget(this, &types);
}
CPrintConfigDialog::~CPrintConfigDialog()
{
    delete ui;
}
//////////////////////////////////////////////////////////////
void CPrintConfigDialog::on_pushButtonDate_clicked()
{
    addCustomString(sDate);
}
void CPrintConfigDialog::on_pushButtonTime_clicked()
{
    addCustomString(sTime);
}
void CPrintConfigDialog::on_pushButtonPage_clicked()
{
    addCustomString(sPage);
}
void CPrintConfigDialog::on_pushButtonPageCount_clicked()
{
    addCustomString(sPageCount);
}
void CPrintConfigDialog::on_pushButtonOk_clicked()
{
    saveData();
    accept();
}
void CPrintConfigDialog::on_pushButtonCancel_clicked()
{
    reject();
}
//////////////////////////////////////////////////////////////
bool CPrintConfigDialog::eventFilter(QObject *o, QEvent *e)
{
    if (e->type() == QEvent::FocusIn)
    {
        QTextEdit* edit = qobject_cast<QTextEdit*>(o);
        if (edit != NULL)
            m_lastFocusInEdit = edit;
    }

    return QDialog::eventFilter(o, e);
}
//////////////////////////////////////////////////////////////
void CPrintConfigDialog::init(CEnvironment *environment)
{
    m_environment = environment;

    ui->spinBoxTop->setValue(m_environment->m_printTop);
    ui->spinBoxBottom->setValue(m_environment->m_printBottom);
    ui->spinBoxLeft->setValue(m_environment->m_printLeft);
    ui->spinBoxRight->setValue(m_environment->m_printRight);
    ui->spinBoxHeader->setValue(m_environment->m_printHeader);
    ui->spinBoxFooter->setValue(m_environment->m_printFooter);

    ui->textEditLeftHeader->setText(m_environment->m_printLeftHeader);
    ui->textEditMiddleHeader->setText(m_environment->m_printMiddleHeader);
    ui->textEditRightHeader->setText(m_environment->m_printRightHeader);
    ui->textEditLeftFooter->setText(m_environment->m_printLeftFooter);
    ui->textEditMiddleFooter->setText(m_environment->m_printMiddleFooter);
    ui->textEditRightFooter->setText(m_environment->m_printRightFooter);

    ui->lineEditDateFormat->setText(m_environment->m_printDateFormat);
    ui->lineEditTimeFormat->setText(m_environment->m_printTimeFormat);
}
//////////////////////////////////////////////////////////////
void CPrintConfigDialog::saveData()
{
    m_environment->m_printTop = ui->spinBoxTop->value();
    m_environment->m_printBottom = ui->spinBoxBottom->value();
    m_environment->m_printLeft = ui->spinBoxLeft->value();
    m_environment->m_printRight = ui->spinBoxRight->value();
    m_environment->m_printHeader = ui->spinBoxHeader->value();
    m_environment->m_printFooter = ui->spinBoxFooter->value();

    m_environment->m_printLeftHeader = ui->textEditLeftHeader->toPlainText();
    m_environment->m_printMiddleHeader = ui->textEditMiddleHeader->toPlainText();
    m_environment->m_printRightHeader = ui->textEditRightHeader->toPlainText();
    m_environment->m_printLeftFooter = ui->textEditLeftFooter->toPlainText();
    m_environment->m_printMiddleFooter = ui->textEditMiddleFooter->toPlainText();
    m_environment->m_printRightFooter = ui->textEditRightFooter->toPlainText();

    m_environment->m_printDateFormat = ui->lineEditDateFormat->text();
    m_environment->m_printTimeFormat = ui->lineEditTimeFormat->text();
}
void CPrintConfigDialog::addCustomString(const QString& custom)
{
    if (m_lastFocusInEdit == NULL)
        return;

    m_lastFocusInEdit->setText(m_lastFocusInEdit->toPlainText() + custom);
}
