#include "PrintForm.h"
#include "ui_PrintForm.h"
#include "MDMultiLanguage.h"

CPrintForm::CPrintForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CPrintForm)
{
    ui->setupUi(this);

    sDate = "&Date";
    sTime = "&Time";
    sPage = "&Page";
    sPageCount = "&Count";
    m_drawWidget = NULL;
    m_lastFocusInEdit = NULL;

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

CPrintForm::~CPrintForm()
{
    delete ui;
}

void CPrintForm::on_pushButtonDate_clicked()
{
    addCustomString(sDate);
}
void CPrintForm::on_pushButtonTime_clicked()
{
    addCustomString(sTime);
}
void CPrintForm::on_pushButtonPage_clicked()
{
    addCustomString(sPage);
}
void CPrintForm::on_pushButtonPageCount_clicked()
{
    addCustomString(sPageCount);
}
void CPrintForm::on_pushButtonOk_clicked()
{
    saveData();
    accept();
}
void CPrintForm::on_pushButtonCancel_clicked()
{
    reject();
}
//////////////////////////////////////////////////////////////
bool CPrintForm::eventFilter(QObject *o, QEvent *e)
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
void CPrintForm::init(CDrawWidget* drawWidget)
{
    m_drawWidget = drawWidget;

    ui->spinBoxTop->setValue(m_drawWidget->m_printTop);
    ui->spinBoxBottom->setValue(m_drawWidget->m_printBottom);
    ui->spinBoxLeft->setValue(m_drawWidget->m_printLeft);
    ui->spinBoxRight->setValue(m_drawWidget->m_printRight);
    ui->spinBoxHeader->setValue(m_drawWidget->m_printHeader);
    ui->spinBoxFooter->setValue(m_drawWidget->m_printFooter);

    ui->textEditLeftHeader->setText(m_drawWidget->m_printLeftHeader);
    ui->textEditMiddleHeader->setText(m_drawWidget->m_printMiddleHeader);
    ui->textEditRightHeader->setText(m_drawWidget->m_printRightHeader);
    ui->textEditLeftFooter->setText(m_drawWidget->m_printLeftFooter);
    ui->textEditMiddleFooter->setText(m_drawWidget->m_printMiddleFooter);
    ui->textEditRightFooter->setText(m_drawWidget->m_printRightFooter);

    ui->lineEditDateFormat->setText(m_drawWidget->m_printDateFormat);
    ui->lineEditTimeFormat->setText(m_drawWidget->m_printTimeFormat);

}
///////////////////////////////////////////////////////////////////////////////////////////
void CPrintForm::saveData()
{
    m_drawWidget->m_printTop = ui->spinBoxTop->value();
    m_drawWidget->m_printBottom = ui->spinBoxBottom->value();
    m_drawWidget->m_printLeft = ui->spinBoxLeft->value();
    m_drawWidget->m_printRight = ui->spinBoxRight->value();
    m_drawWidget->m_printHeader = ui->spinBoxHeader->value();
    m_drawWidget->m_printFooter = ui->spinBoxFooter->value();

    m_drawWidget->m_printLeftHeader = ui->textEditLeftHeader->toPlainText();
    m_drawWidget->m_printMiddleHeader = ui->textEditMiddleHeader->toPlainText();
    m_drawWidget->m_printRightHeader = ui->textEditRightHeader->toPlainText();
    m_drawWidget->m_printLeftFooter = ui->textEditLeftFooter->toPlainText();
    m_drawWidget->m_printMiddleFooter = ui->textEditMiddleFooter->toPlainText();
    m_drawWidget->m_printRightFooter = ui->textEditRightFooter->toPlainText();

    m_drawWidget->m_printDateFormat = ui->lineEditDateFormat->text();
    m_drawWidget->m_printTimeFormat = ui->lineEditTimeFormat->text();
}
void CPrintForm::addCustomString(const QString& custom)
{
    if (m_lastFocusInEdit == NULL)
        return;

    m_lastFocusInEdit->setText(m_lastFocusInEdit->toPlainText() + custom);
}

