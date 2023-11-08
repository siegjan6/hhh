#include "PrintConfigDialog.h"
#include "ui_PrintConfigDialog.h"
#include "ShareTool.h"

CPrintConfigDialog::CPrintConfigDialog(const PrintPara &printPara, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CPrintConfigDialog)
{
    sDate = "&Date";
    sTime = "&Time";
    sPage = "&Page";
    sPageCount = "&Count";
    m_lastFocusInEdit = NULL;
    m_printPara = printPara;

    ui->setupUi(this);

    initWidgets();

    ui->textEditLeftHeader->installEventFilter(this);
    ui->textEditMiddleHeader->installEventFilter(this);
    ui->textEditRightHeader->installEventFilter(this);
    ui->textEditLeftFooter->installEventFilter(this);
    ui->textEditMiddleFooter->installEventFilter(this);
    ui->textEditRightFooter->installEventFilter(this);

    CShareTool::installDialogTranslator(this);
}
CPrintConfigDialog::~CPrintConfigDialog()
{
    delete ui;
}

void CPrintConfigDialog::setDefault(PrintPara &printPara)
{
    printPara.printHorizontalView = true;
    printPara.printVerticalView = true;
    printPara.printScale = false;
    printPara.printTop = 5;
    printPara.printBottom = 5;
    printPara.printLeft = 5;
    printPara.printRight = 5;
    printPara.printHeader = 20;
    printPara.printFooter = 20;
    printPara.printDateFormat = "yyyy-MM-dd";
    printPara.printTimeFormat = "hh:mm:ss";
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
void CPrintConfigDialog::initWidgets()
{
    ui->checkboxHorizontal->setChecked(m_printPara.printHorizontalView);
    ui->checkboxVertical->setChecked(m_printPara.printVerticalView);
    ui->checkboxScale->setChecked(m_printPara.printScale);
    ui->spinBoxTop->setValue(m_printPara.printTop);
    ui->spinBoxBottom->setValue(m_printPara.printBottom);
    ui->spinBoxLeft->setValue(m_printPara.printLeft);
    ui->spinBoxRight->setValue(m_printPara.printRight);
    ui->spinBoxHeader->setValue(m_printPara.printHeader);
    ui->spinBoxFooter->setValue(m_printPara.printFooter);

    ui->textEditLeftHeader->setText(m_printPara.printLeftHeader);
    ui->textEditMiddleHeader->setText(m_printPara.printMiddleHeader);
    ui->textEditRightHeader->setText(m_printPara.printRightHeader);
    ui->textEditLeftFooter->setText(m_printPara.printLeftFooter);
    ui->textEditMiddleFooter->setText(m_printPara.printMiddleFooter);
    ui->textEditRightFooter->setText(m_printPara.printRightFooter);

    ui->lineEditDateFormat->setText(m_printPara.printDateFormat);
    ui->lineEditTimeFormat->setText(m_printPara.printTimeFormat);
}
//////////////////////////////////////////////////////////////
void CPrintConfigDialog::saveData()
{
    m_printPara.printHorizontalView = ui->checkboxHorizontal->isChecked();
    m_printPara.printVerticalView = ui->checkboxVertical->isChecked();
    m_printPara.printScale = ui->checkboxScale->isChecked();
    m_printPara.printTop = ui->spinBoxTop->value();
    m_printPara.printBottom = ui->spinBoxBottom->value();
    m_printPara.printLeft = ui->spinBoxLeft->value();
    m_printPara.printRight = ui->spinBoxRight->value();
    m_printPara.printHeader = ui->spinBoxHeader->value();
    m_printPara.printFooter = ui->spinBoxFooter->value();

    m_printPara.printLeftHeader = ui->textEditLeftHeader->toPlainText();
    m_printPara.printMiddleHeader = ui->textEditMiddleHeader->toPlainText();
    m_printPara.printRightHeader = ui->textEditRightHeader->toPlainText();
    m_printPara.printLeftFooter = ui->textEditLeftFooter->toPlainText();
    m_printPara.printMiddleFooter = ui->textEditMiddleFooter->toPlainText();
    m_printPara.printRightFooter = ui->textEditRightFooter->toPlainText();

    m_printPara.printDateFormat = ui->lineEditDateFormat->text();
    m_printPara.printTimeFormat = ui->lineEditTimeFormat->text();
}
void CPrintConfigDialog::addCustomString(const QString& custom)
{
    if (m_lastFocusInEdit == NULL)
        return;

    m_lastFocusInEdit->setText(m_lastFocusInEdit->toPlainText() + custom);
}
