#include "CXPropertySettingDialog.h"
#include "ui_CXPropertySettingDialog.h"

#include "CXAxis.h"
#include "CBisectrixLineSettingDialog.h"
#include <QFontDialog>
#include <QRegExp>
#include <QRegExpValidator>
#include <QColorDialog>
#include <math.h>

#include "CTranslator.h"
#include "CFormToolFunc.h"
#include <QDebug>
#include <QString>

#define TIMEINDEX 0
#define VALUEINDEX 1

using namespace Translator;
using namespace FormToolFunc;

CXPropertySettingDialog::CXPropertySettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CXPropertySettingDialog),
    m_axis(0),
    m_translateTool(0)
{
    ui->setupUi(this);

    m_time.clear();
    m_time.resize(4);
    m_time.fill(0,4);
    installDialogTranslator(m_translateTool,this);
}

CXPropertySettingDialog::~CXPropertySettingDialog()
{
    delete ui;
}

/*!
\section
*/

CXAxis *CXPropertySettingDialog::getXAxis(bool *ok, CXAxis *initial, QWidget *parent)
{
    if(initial) {
        CXPropertySettingDialog dlg(parent);
        dlg.setData(initial);
        dlg.setWindowTitle(QString("%1-%2").arg(initial->name(),initial->comment()));
        int ret = dlg.exec();
        if(ok) {
            *ok = !!ret;
            return initial;
        }
    }
    return NULL;
}

void CXPropertySettingDialog::setData(CXAxis *x)
{
    if(x){
        m_axis = x;
        initialize();
    }
}
/*!
\section
*/
void CXPropertySettingDialog::initialize()
{
    initializeData();
    initializeUI();
    createConnections();
}

void CXPropertySettingDialog::initializeData()
{
    if(m_axis)
    {
        m_font = m_axis->font();
        m_fontColor = m_axis->fontColor();
    }
}

void CXPropertySettingDialog::initializeUI()
{
    if(m_axis)
    {
        initializeCommon();
        initializeTimeType();
        initializeValueType();
        switch (m_axis->valueType())
        {
        case CXAxis::ValueType::Time:
            ui->axisTypeComboBox->setCurrentIndex(TIMEINDEX);
            ui->stackedWidget->setCurrentIndex(TIMEINDEX);
            break;
        case CXAxis::ValueType::Value:
            ui->axisTypeComboBox->setCurrentIndex(VALUEINDEX);
            ui->stackedWidget->setCurrentIndex(VALUEINDEX);
            break;
        default:
            break;
        }
    }
}

void CXPropertySettingDialog::initializeCommon()
{
    if(m_axis)
    {
        ui->nameLineEdit->setEnabled(false);
        ui->nameLineEdit->setText(m_axis->name());

        // time
        ui->timeAxisVisibleCheckBox->setChecked(m_axis->axisVisible());
        ui->timeAxisLineVisibleCheckBox->setChecked(m_axis->axisLineVisible());
        ui->timeCommentTextEdit->setText(m_axis->comment());
        ui->timeUnitLineEdit->setText(m_axis->unit());
        ui->timeHeightLineEdit->setText(QString::number(m_axis->xHeight()));
        ui->timeHeightLineEdit->setValidator(new QIntValidator(0,1000,this));
        setButtonColor(m_fontColor,*ui->timeFontColorButton);
        // value
        ui->valueAxisVisibleCheckBox->setChecked(m_axis->axisVisible());
        ui->valueAxisLineVisibleCheckBox->setChecked(m_axis->axisLineVisible());
        ui->valueCommentTextEdit->setText(m_axis->comment());
        ui->valueUnitLineEdit->setText(m_axis->unit());
        ui->valueHeightLineEdit->setText(QString::number(m_axis->xHeight()));
        ui->valueHeightLineEdit->setValidator(new QIntValidator(0,1000,this));
        setButtonColor(m_fontColor,*ui->valueFontColorButton);
    }
}

void CXPropertySettingDialog::initializeValueType()
{
    QRegExp rx("^-?\\d+$");
    QRegExpValidator *pReg = new QRegExpValidator(rx, this);
    ui->maxValueLineEdit->setValidator(pReg);
    ui->minValueLineEdit->setValidator(pReg);
    ui->valueUnitLineEdit->setMaxLength(100);

    ui->valueHeightLineEdit->setValidator(new QIntValidator(0,1000,this));
    ui->valuePrecisionLineEdit->setValidator(new QIntValidator(0,5,this));
    ui->valueHeightLineEdit->setValidator(new QIntValidator(0,1000,this));
    if(m_axis)
    {
        ui->maxValueLineEdit->setText(QString::number(m_axis->maxValue()));
        ui->minValueLineEdit->setText(QString::number(m_axis->minValue()));

        ui->valueHeightLineEdit->setText(QString::number(m_axis->xHeight()));
        ui->valuePrecisionLineEdit->setText(QString::number(m_axis->valuePrecision()));
        ui->valueHeightLineEdit->setText(QString::number(m_axis->xHeight()));
    }
}

void CXPropertySettingDialog::initializeTimeType()
{
    void transSecondsToDayTime(qint64,QVector<int> &);
    ui->valueUnitLineEdit->setMaxLength(100);

    QRegExp rx("^\\d+$");
    QRegExpValidator *pReg = new QRegExpValidator(rx, this);
    ui->timeDayLineEdit->setValidator(pReg);
    ui->timeHourLineEdit->setValidator(pReg);
    ui->timeMinuteLineEdit->setValidator(pReg);
    ui->timeSecondLineEdit->setValidator(pReg);

    ui->timeHeightLineEdit->setValidator(new QIntValidator(0,1000,this));

    if(m_axis){
        ui->timeHeightLineEdit->setText(QString::number(m_axis->xHeight()));
        ui->timeFormatLineEdit->setText(m_axis->timeFormat());
        transSecondsToDayTime(m_axis->totalLength(),m_time);

        ui->timeDayLineEdit->setText(QString::number(m_time[0]));
        ui->timeHourLineEdit->setText(QString::number(m_time[1]));
        ui->timeMinuteLineEdit->setText(QString::number(m_time[2]));
        ui->timeSecondLineEdit->setText(QString::number(m_time[3]));
    }
}
/*!
\section
*/
void CXPropertySettingDialog::applyChanges()
{
    if(m_axis)
    {
        m_axis->setFont(m_font);
        m_axis->setFontColor(m_fontColor);

        if(TIMEINDEX == ui->axisTypeComboBox->currentIndex())
            applyTimeChanges();
        else if( VALUEINDEX == ui->axisTypeComboBox->currentIndex())
            applyValueChanges();
    }
}

void CXPropertySettingDialog::applyValueChanges()
{
    if(m_axis)
    {
        m_axis->setValueType(CXAxis::ValueType::Value);
        m_axis->setAxisVisible(ui->valueAxisVisibleCheckBox->isChecked());
        m_axis->setAxisLineVisible(ui->valueAxisLineVisibleCheckBox->isChecked());
        QString comment = ui->valueCommentTextEdit->toPlainText().trimmed();
        m_axis->setComment(comment);
        m_axis->setUnit(ui->valueUnitLineEdit->text());
        m_axis->setValuePrecision(ui->valuePrecisionLineEdit->text().toInt());
        m_axis->setXHeight(ui->valueHeightLineEdit->text().toDouble());
        double maxValue = ui->maxValueLineEdit->text().toDouble();
        double minValue = ui->minValueLineEdit->text().toDouble();
        if(maxValue > minValue)
        {
            m_axis->setMaxValue(maxValue);
            m_axis->setMinValue(minValue);
            m_axis->setOriginMax(maxValue);
            m_axis->setOriginMin(minValue);
        }
        else
            return;

//        m_axis->setTotalLength(ui->maxValueLineEdit->text().toDouble() - ui->minValueLineEdit->text().toDouble());
    }
}

void CXPropertySettingDialog::applyTimeChanges()
{
    if(m_axis)
    {
        m_axis->setValueType(CXAxis::ValueType::Time);
        m_axis->setAxisVisible(ui->timeAxisVisibleCheckBox->isChecked());
        m_axis->setAxisLineVisible(ui->timeAxisLineVisibleCheckBox->isChecked());
        QString comment = ui->timeCommentTextEdit->toPlainText().trimmed();
        m_axis->setComment(comment);
        m_axis->setUnit(ui->timeUnitLineEdit->text());

        m_axis->setXHeight(ui->timeHeightLineEdit->text().toDouble());
        m_axis->setTimeFormat(ui->timeFormatLineEdit->text());
        m_axis->setXHeight(ui->timeHeightLineEdit->text().toDouble());
        m_axis->setTotalLength(ui->timeDayLineEdit->text().toInt(),
                               ui->timeHourLineEdit->text().toInt(),
                               ui->timeMinuteLineEdit->text().toInt(),
                               ui->timeSecondLineEdit->text().toInt());
        m_axis->setOriginalLength(m_axis->totalLength());
    }
}
/*!
\section
*/
void CXPropertySettingDialog::createConnections()
{
    connect(ui->axisTypeComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetAxisType(int)));
    connect(ui->okButton,SIGNAL(clicked()),this,SLOT(slotOkButtonClicked()));
    connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(slotCancelButtonClicked()));
    connect(ui->timeFontColorButton,SIGNAL(clicked()),this,SLOT(slotSetFontColor()));
    connect(ui->valueFontColorButton,SIGNAL(clicked()),this,SLOT(slotSetFontColor()));

    connect(ui->valueFontButton,&QPushButton::clicked,this,&CXPropertySettingDialog::slotSetFont);
    connect(ui->timeFontButton,&QPushButton::clicked,this,&CXPropertySettingDialog::slotSetFont);
    connect(ui->axisLineSettingButton,&QPushButton::clicked,this,&CXPropertySettingDialog::slotSetAxisLineProperty);
    connect(ui->valueAxisLineSettingButton,&QPushButton::clicked,this,&CXPropertySettingDialog::slotSetAxisLineProperty);
}

void CXPropertySettingDialog::slotSetAxisType(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}

void CXPropertySettingDialog::slotOkButtonClicked()
{
    applyChanges();
    accept();
}

void CXPropertySettingDialog::slotCancelButtonClicked()
{
    reject();
}

void CXPropertySettingDialog::slotSetFont()
{

//    bool ok = false;
//    QFont font = QFontDialog::getFont(&ok,m_font,this,tr("选择字体"));

//    if(ok) {
//        m_font = font;
//    }else{

//    }

//    QFontDialog dialog(m_font,this);

//    installTranslatorWidget(&dialog);

//    int ret = dialog.exec();
//    if(ret == QDialog::Accepted){
//        m_font = dialog.selectedFont();
//    }else{

//    }
    selectFont(m_font,this);
}

void CXPropertySettingDialog::slotSetFontColor()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    setColor(m_fontColor,*button);
}

void CXPropertySettingDialog::slotSetAxisLineProperty()
{
    CBisectrixLine *initialLine = m_axis->axisLine();
    bool ok = false;

    CBisectrixLineSettingDialog::getLine(&ok,initialLine,this);
}
/*!
\section
*/
void transSecondsToDayTime(qint64 seconds,QVector<int> &time)
{
//    int count = 4;
//    while(count--)
//    {
//        time[count] = seconds % 60;
//        seconds /= 60;
//    }
    int second = seconds % 60;
    int minute = (seconds - second)/60 % 60;
    int hour = (seconds - second - minute*60) /60 /60 % 24;
    int day = (seconds - second - minute*60 - hour*60*60) / (60*60*24);
    time[0] = day;
    time[1] = hour;
    time[2] = minute;
    time[3] = second;
}

void transDateTimeToSeconds(const QVector<int> &time,int &seconds)
{
    int count = 4;
    while(count--) {
         seconds += time[count]*pow(60,3-count);
    }
}
void CXPropertySettingDialog::setColor( QColor &initialColor, QPushButton &colorButton)
{
    selectColor(initialColor,colorButton,this);
}



