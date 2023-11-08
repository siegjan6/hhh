#include "CVernierSettingForm.h"
#include "ui_CVernierSettingForm.h"

#include "CVernier.h"
#include "MDMultiLanguage.h"
#include <QFontDialog>
#include "CFormToolFunc.h"
#include <QDebug>
#include <QRegExp>
#include <QRegExpValidator>

CVernierSettingForm::CVernierSettingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CVernierSettingForm),
    m_isModified(false),
    m_vernier(0),
    m_visible(false),
    m_isVernierFixed(false),
    m_isChoseNearer(false),
    m_isVertical(false),
    m_itemInternal(0),
    m_overTimeLength(0)
{
    ui->setupUi(this);
}

CVernierSettingForm::~CVernierSettingForm()
{
    delete ui;
}

void CVernierSettingForm::setData(CVernier *vernier)
{
    if(vernier) {
        m_vernier = vernier;
        initialize();
    }
}


void CVernierSettingForm::applyChanges()
{
    if(m_vernier)  {
        m_vernier->setDisplay(ui->vernierVisibleCheckBox->isChecked()); // display
        m_vernier->setInterval(ui->itemInternalLineEdit->text().toInt()); // Internal

        m_vernier->setIsFixed(ui->fixedRadioButton->isChecked());   // is fixed
        QPoint fixedPoint(ui->xLineEdit->text().toInt(),
                          ui->yLineEdit->text().toInt());

        m_vernier->setFixedPoint(fixedPoint);

        m_vernier->setFont(m_vernierFont);  // font
        m_vernier->setIsVertical(ui->verticalLayoutRadioButton->isChecked()); // vertical
        m_vernier->setTimeLength(ui->overTimeLengthLineEdit->text().toInt()); // Length
        m_vernier->setIsChoseNearer(ui->choseNearerRadioButton->isChecked()); // chose the nearer
        m_vernier->setCustomString(ui->customStringLineEdit->text());       // custom string
        m_vernier->setTimeFormat(ui->timeFormatLineEdie->text());            // time format
    }
}
/*!
\section
*/
void CVernierSettingForm::resetModified()
{
    m_isModified = false;
}

void CVernierSettingForm::slotHandleModified()
{
    if(!m_isModified) {
        m_isModified = true;
        emit itemModified();
    }
}

/*!
\section
*/
void CVernierSettingForm::initialize()
{
    initializeData();
    initializeUI();
}

void CVernierSettingForm::initializeData()
{
    if(m_vernier)  {
        m_visible = m_vernier->display(); // display

        m_isVernierFixed = m_vernier->isFixed();        // is fixed
        m_fixedPoint = m_vernier->fixedPoint();

        m_isVertical = m_vernier->isVertical();        // is vertical

        m_isChoseNearer = m_vernier->isChoseNearer(); // is chose the nearer
        m_customString = m_vernier->customString();  // custom string

        m_itemInternal = m_vernier->interval();     // internal
        m_timeFormat = m_vernier->timeFormat();     // timeFormat
        m_vernierFont = m_vernier->font();          // font
        m_overTimeLength = m_vernier->timeLength(); // length
    }
}

void CVernierSettingForm::initializeUI()
{
    breakConnections();

    QRegExp rx("^\\d+$");// 只允许输入整数
    QRegExpValidator *pReg = new QRegExpValidator(rx, this);
    ui->xLineEdit->setValidator(pReg);
    ui->yLineEdit->setValidator(pReg);
    ui->overTimeLengthLineEdit->setValidator(pReg);
    ui->itemInternalLineEdit->setValidator(pReg);

    ui->verticalLayoutRadioButton->setChecked(m_isVertical); // is vertical
    ui->horizontalLayoutRadioButton->setChecked(!m_isVertical);

    ui->vernierVisibleCheckBox->setChecked(m_visible);         // visible
    ui->choseNearerRadioButton->setChecked(m_isChoseNearer);    // is ChoseNearer
    ui->customStringRadioButton->setChecked(!m_isChoseNearer);

    ui->timeFormatLineEdie->setText(m_timeFormat);              // format string

    ui->fixedRadioButton->setChecked(m_isVernierFixed);     // is fixed
    ui->xLineEdit->setText(QString::number(m_fixedPoint.x()));
    ui->yLineEdit->setText(QString::number(m_fixedPoint.y()));

    slotSetFloatPoint(m_isVernierFixed);
    slotSetCustomString(!m_isChoseNearer);

    ui->customStringLineEdit->setText(m_customString);
    ui->itemInternalLineEdit->setText(QString::number(m_itemInternal));
    ui->overTimeLengthLineEdit->setText(QString::number(m_overTimeLength));

    createConnections();
}

/*!
\section
*/
void CVernierSettingForm::createConnections()
{
    connect(ui->vernierFontButton,&QPushButton::clicked,this,&CVernierSettingForm::slotSetVernierFont);

    connect(ui->fixedRadioButton,&QRadioButton::toggled,this,&CVernierSettingForm::slotSetFloatPoint);
    connect(ui->customStringRadioButton,&QRadioButton::toggled,this,&CVernierSettingForm::slotSetCustomString);

    // handle the item changed signal
    connect(ui->vernierVisibleCheckBox,&QCheckBox::toggled,this,&CVernierSettingForm::slotHandleModified);
    connect(ui->itemInternalLineEdit,&QLineEdit::textEdited,this,&CVernierSettingForm::slotHandleModified);

    connect(ui->verticalLayoutRadioButton,&QRadioButton::toggled,this,&CVernierSettingForm::slotHandleModified);
    connect(ui->horizontalLayoutRadioButton,&QRadioButton::toggled,this,&CVernierSettingForm::slotHandleModified);

    connect(ui->floatRadioButton,&QRadioButton::toggled,this,&CVernierSettingForm::slotHandleModified);
    connect(ui->fixedRadioButton,&QRadioButton::toggled,this,&CVernierSettingForm::slotHandleModified);
    connect(ui->xLineEdit,&QLineEdit::textEdited,this,&CVernierSettingForm::slotHandleModified);
    connect(ui->yLineEdit,&QLineEdit::textEdited,this,&CVernierSettingForm::slotHandleModified);

    connect(ui->overTimeLengthLineEdit,&QLineEdit::textEdited,this,&CVernierSettingForm::slotHandleModified);
    connect(ui->choseNearerRadioButton,&QRadioButton::toggled,this,&CVernierSettingForm::slotHandleModified);
    connect(ui->customStringRadioButton,&QRadioButton::toggled,this,&CVernierSettingForm::slotHandleModified);
    connect(ui->customStringLineEdit,&QLineEdit::textEdited,this,&CVernierSettingForm::slotHandleModified);

    connect(ui->timeFormatLineEdie,&QLineEdit::textEdited,this,&CVernierSettingForm::slotHandleModified);
}
void CVernierSettingForm::breakConnections()
{
    disconnect(ui->vernierFontButton,&QPushButton::clicked,this,&CVernierSettingForm::slotSetVernierFont);

    disconnect(ui->fixedRadioButton,&QRadioButton::toggled,this,&CVernierSettingForm::slotSetFloatPoint);
    disconnect(ui->customStringRadioButton,&QRadioButton::toggled,this,&CVernierSettingForm::slotSetCustomString);

    // handle the item changed signal
    disconnect(ui->vernierVisibleCheckBox,&QCheckBox::toggled,this,&CVernierSettingForm::slotHandleModified);
    disconnect(ui->itemInternalLineEdit,&QLineEdit::textEdited,this,&CVernierSettingForm::slotHandleModified);

    disconnect(ui->verticalLayoutRadioButton,&QRadioButton::toggled,this,&CVernierSettingForm::slotHandleModified);
    disconnect(ui->horizontalLayoutRadioButton,&QRadioButton::toggled,this,&CVernierSettingForm::slotHandleModified);

    disconnect(ui->floatRadioButton,&QRadioButton::toggled,this,&CVernierSettingForm::slotHandleModified);
    disconnect(ui->fixedRadioButton,&QRadioButton::toggled,this,&CVernierSettingForm::slotHandleModified);
    disconnect(ui->xLineEdit,&QLineEdit::textEdited,this,&CVernierSettingForm::slotHandleModified);
    disconnect(ui->yLineEdit,&QLineEdit::textEdited,this,&CVernierSettingForm::slotHandleModified);

    disconnect(ui->overTimeLengthLineEdit,&QLineEdit::textEdited,this,&CVernierSettingForm::slotHandleModified);
    disconnect(ui->choseNearerRadioButton,&QRadioButton::toggled,this,&CVernierSettingForm::slotHandleModified);
    disconnect(ui->customStringRadioButton,&QRadioButton::toggled,this,&CVernierSettingForm::slotHandleModified);
    disconnect(ui->customStringLineEdit,&QLineEdit::textEdited,this,&CVernierSettingForm::slotHandleModified);

    disconnect(ui->timeFormatLineEdie,&QLineEdit::textEdited,this,&CVernierSettingForm::slotHandleModified);
}


void CVernierSettingForm::slotSetVernierFont()
{
//    bool ok;
//    QFont font = QFontDialog::getFont(&ok,m_vernierFont,this);
//    if (ok) {
//        // font is set to the font the user selected
//        m_vernierFont = font;
//        slotHandleModified();
//    } else {
//        // the user canceled the dialog; font is set to the initial
//        // value,
//    }

//    QFontDialog dialog(m_vernierFont,this);

//    CMDMultiLanguage::instance()->translateWidget(&dialog);

//    int ret = dialog.exec();
//    if(ret == QDialog::Accepted){
//        m_vernierFont = dialog.selectedFont();
//    }else{

//    }
        FormToolFunc::selectFont(m_vernierFont,this);
}

void CVernierSettingForm::slotSetFloatPoint(bool enable)
{
    ui->xLineEdit->setEnabled(enable);
    ui->yLineEdit->setEnabled(enable);
}

void CVernierSettingForm::slotSetCustomString(bool enable)
{
    ui->customStringLineEdit->setEnabled(enable);
}
