#include "CFeatureTagPropertySettingDialog.h"
#include "ui_CFeatureTagPropertySettingDialog.h"

#include "CTagPosition.h"
#include "CFeatureTagLine.h"

#include "CFeatureTag.h"
#include "CHistoryTagManager.h"
#include "CRealTimeTagManager.h"

#include "CPoint.h"

#include <QColorDialog>
#include <QGridLayout>
#include <QDebug>

#include "CTranslator.h"
#include "CFormToolFunc.h"

using namespace Translator;
using namespace FormToolFunc;

CFeatureTagPropertySettingDialog::CFeatureTagPropertySettingDialog(QWidget *parent) :
    QDialog(parent),
    m_historyTag(0),
    m_realTag(0),
    m_isReal(false),
    ui(new Ui::CFeatureTagPropertySettingDialog),
    m_lineStyle(0),
    m_topPointStyle(0),
    m_bottomPointStyle(0)
{
    ui->setupUi(this);
    installDialogTranslator(m_transTool,this);

    this->move(135,0);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
}

CFeatureTagPropertySettingDialog::~CFeatureTagPropertySettingDialog()
{
    delete ui;
}


void CFeatureTagPropertySettingDialog::setData(CFeatureTag *featureTag,const CTagPos &tagPos)
{
    if(featureTag){
        m_historyTag  = featureTag;
        m_tagPos = tagPos;
        m_isReal = false;
        initialize();
    }
}

void CFeatureTagPropertySettingDialog::setData(CRealFTag *realTag,const CTagPos &tagPos)
{
    if(realTag){
        m_realTag = realTag;
        m_tagPos = tagPos;
        m_isReal = true;
        initialize();
    }
}

CTagPos CFeatureTagPropertySettingDialog::tagPos() const
{
    return m_tagPos;
}

/*!
\section
*/
void CFeatureTagPropertySettingDialog::initialize()
{
    initializeData();
    initializeUI();
}
void CFeatureTagPropertySettingDialog::initializeData()
{
    if(m_isReal)
    {
        if(m_realTag)
        {
            m_lineColor = m_realTag->tagLine()->lineColor();
            m_topPointColor = m_realTag->topPoint()->color();
            m_bottomPointColor = m_realTag->bottomPoint()->color();
        }
    }
    else
    {
        if(m_historyTag)
        {
            m_lineColor = m_historyTag->tagLine()->lineColor();
            m_topPointColor = m_historyTag->topPoint()->color();
            m_bottomPointColor = m_historyTag->bottomPoint()->color();
        }
    }
}
void CFeatureTagPropertySettingDialog::initializeUI()
{
    breakConnections();
    setLineStyle(ui->lineStyleComboBox);
    setPointStyle(ui->topPointStyleComboBox);
    setPointStyle(ui->bottomPointStyleComboBox);
    CTagHPos hPos;
    CTagVPos vPos;
    CFeatureTagLine *line = NULL;
    CPoint *top = NULL;
    CPoint *bottom = NULL;
    if(m_isReal)
    {
        hPos = m_tagPos.hPos();
        vPos = m_tagPos.vPos();
        line = m_realTag->tagLine();
        top = m_realTag->topPoint();
        bottom = m_realTag->bottomPoint();
    }
    else
    {
        hPos = m_tagPos.hPos();
        vPos = m_tagPos.vPos();
        line = m_historyTag->tagLine();
        top  = m_historyTag->topPoint();
        bottom = m_historyTag->bottomPoint();
    }
    initializeHPos(hPos);
    initializeVPos(vPos);
    initializeLine(line);
    initializeTopPoint(top);
    initializeBottomPoint(bottom);

    createConnections();
}


void CFeatureTagPropertySettingDialog::initializeHPos(const CTagHPos &hPos)
{
    switch (hPos.position())
    {
    case CTagHPos::Position::Left:
        ui->HLeftRadioButton->setChecked(true);
        ui->HLeftOffsetLineEdit->setText(QString::number(hPos.offset()));
        ui->HRightOffsetLineEdit->setEnabled(false);
        break;
    case CTagHPos::Position::Right:
        ui->HRightRadioButton->setChecked(true);
        ui->HRightOffsetLineEdit->setText(QString::number(-hPos.offset()));
        ui->HLeftOffsetLineEdit->setEnabled(false);
        break;
    case CTagHPos::Position::Middle:
        ui->HMiddleRadioButton->setChecked(true);
        ui->HLeftOffsetLineEdit->setEnabled(false);
        ui->HRightOffsetLineEdit->setEnabled(false);
        break;
    default:
        break;
    }
}
void CFeatureTagPropertySettingDialog::initializeVPos(const CTagVPos &vPos)
{
    switch (vPos.position())
    {
    case CTagVPos::Position::Top:
        ui->vTopRadioButton->setChecked(true);
        ui->vTopOffsetLineEdit->setText(QString::number(vPos.offset()));
        ui->vBottomOffsetLineEdit->setEnabled(false);
        break;
    case CTagVPos::Position::Bottom:
        ui->vBottomRadioButton->setChecked(true);
        ui->vBottomOffsetLineEdit->setText(QString::number(vPos.offset()));
        ui->vTopOffsetLineEdit->setEnabled(false);
        break;
    case CTagVPos::Position::Middle:
        ui->vMiddleRadioButton->setChecked(true);
        ui->vBottomOffsetLineEdit->setEnabled(false);
        ui->vTopOffsetLineEdit->setEnabled(false);
        break;
    default:
        break;
    }
}
void CFeatureTagPropertySettingDialog::initializeLine(const CFeatureTagLine *tagLine)
{    
    ui->lineVisibleCheckBox->setChecked(tagLine->visible());
    setButtonColor(m_lineColor,*ui->lineColorButton);
    ui->lineWidthSpinBox->setValue(tagLine->lineWidth());
    ui->lineStyleComboBox->setCurrentIndex((int)(tagLine->lineStyle()) -1);
}

void CFeatureTagPropertySettingDialog::initializeTopPoint(const CPoint *topPoint)
{    
    ui->topPointVisibleCheckBox->setChecked(topPoint->visible());
    setButtonColor(m_topPointColor,*ui->topPointColorButton);
    ui->topPointSizeSpinBox->setValue(topPoint->size());
    ui->topPointStyleComboBox->setCurrentIndex((int)(topPoint->pointStyle()));
}

void CFeatureTagPropertySettingDialog::initializeBottomPoint(const CPoint *bottomPoint)
{    
    ui->bottomPointVisibleCheckBox->setChecked(bottomPoint->visible());
    setButtonColor(m_bottomPointColor,*ui->bottomPointColorButton);
    ui->bottomPointSizeSpinBox->setValue(bottomPoint->size());
    ui->bottomPointStyleComboBox->setCurrentIndex((int)(bottomPoint->pointStyle()));
}
/*!
\section
*/
bool CFeatureTagPropertySettingDialog::applyChanges()
{
    CTagHPos hPos;
    CTagVPos vPos;
    applyHPosChanges(hPos);
    applyVPosChanges(vPos);
    if(m_isReal)
    {
        m_tagPos.setHPos(hPos);
        m_tagPos.setVPos(vPos);
        if(applyLineChanges(m_realTag->tagLine())
                && applyBottomPointChanges(m_realTag->bottomPoint())
                && applyTopPointChanges(m_realTag->topPoint()))
            return true;
    }
    else
    {
        m_tagPos.setHPos(hPos);
        m_tagPos.setVPos(vPos);
        if(applyLineChanges(m_historyTag->tagLine())
                && applyBottomPointChanges(m_historyTag->bottomPoint())
                && applyTopPointChanges(m_historyTag->topPoint()))
            return true;
    }
    return false;
}

void CFeatureTagPropertySettingDialog::applyHPosChanges(CTagHPos &pos)
{
    if(ui->HLeftRadioButton->isChecked())
    {
        pos.setPosition(CTagHPos::Position::Left);
        pos.setOffset(ui->HLeftOffsetLineEdit->text().toInt());
    }
    else if(ui->HRightRadioButton->isChecked())
    {
        pos.setPosition(CTagHPos::Position::Right);
        pos.setOffset(ui->HRightOffsetLineEdit->text().toInt());
    }
    else if(ui->HMiddleRadioButton->isChecked())
    {
        pos.setPosition(CTagHPos::Position::Middle);
    }

}
void CFeatureTagPropertySettingDialog::applyVPosChanges(CTagVPos &pos)
{
    if(ui->vTopRadioButton->isChecked())
    {
        pos.setPosition(CTagVPos::Position::Top);
        pos.setOffset(ui->vTopOffsetLineEdit->text().toInt());
    }
    else if(ui->vBottomRadioButton->isChecked())
    {
        pos.setPosition(CTagVPos::Position::Bottom);
        pos.setOffset(ui->vBottomOffsetLineEdit->text().toInt());
    }
    else if(ui->vMiddleRadioButton->isChecked())
    {
        pos.setPosition(CTagVPos::Position::Middle);
    }
}

bool CFeatureTagPropertySettingDialog::applyLineChanges(CFeatureTagLine *line)
{
    if(line)
    {
        if(ui->lineVisibleCheckBox->isChecked())
            line->setVisible(true);
        else
            line->setVisible(false);
        line->setLineColor(m_lineColor);
        line->setLineStyle((Qt::PenStyle)(ui->lineStyleComboBox->currentIndex()+1));
        int lineValue = ui->lineWidthSpinBox->value();
        if(lineValue > 10 || lineValue < 1)
        {
            showMessage("请输入1到10之间的整数！",this);
            return false;
        }
        else
            line->setLineWidth(lineValue);
        return true;
    }
    return false;
}

bool CFeatureTagPropertySettingDialog::applyTopPointChanges(CPoint *point)
{
    if(point)
    {
        if(ui->topPointVisibleCheckBox->isChecked())
            point->setVisible(true);
        else
            point->setVisible(false);
        point->setColor(m_topPointColor);
        point->setPointStyle((CPoint::PointStyle)(ui->topPointStyleComboBox->currentIndex()));
        int topValue = ui->topPointSizeSpinBox->value();
        if(topValue > 10 || topValue < 1)
        {
            showMessage("请输入1到10之间的整数！",this);
            return false;
        }
        else
            point->setSize(topValue);
        return true;
    }
    return false;
}

bool CFeatureTagPropertySettingDialog::applyBottomPointChanges(CPoint *point)
{
    if(point)
    {
        if(ui->bottomPointVisibleCheckBox->isChecked())
            point->setVisible(true);
        else
            point->setVisible(false);
        point->setColor(m_bottomPointColor);
        point->setPointStyle((CPoint::PointStyle)(ui->bottomPointStyleComboBox->currentIndex()));
        int bottomValue = ui->bottomPointSizeSpinBox->value();
        if(bottomValue > 10 || bottomValue < 1)
        {
            showMessage("请输入1到10之间的整数！",this);
            return false;
        }
        else
            point->setSize(bottomValue);
        return true;
    }
    return false;
}
/*!
\section
*/
void CFeatureTagPropertySettingDialog::createConnections()
{
    connect(ui->lineColorButton,SIGNAL(clicked()),this,SLOT(slotSetLineColor()));
    connect(ui->topPointColorButton,SIGNAL(clicked()),this,SLOT(slotSetTopPointColor()));
    connect(ui->bottomPointColorButton,SIGNAL(clicked()),this,SLOT(slotSetBottomPointColor()));
}

void CFeatureTagPropertySettingDialog::breakConnections()
{
    disconnect(ui->lineColorButton,SIGNAL(clicked()),this,SLOT(slotSetLineColor()));
    disconnect(ui->topPointColorButton,SIGNAL(clicked()),this,SLOT(slotSetTopPointColor()));
    disconnect(ui->bottomPointColorButton,SIGNAL(clicked()),this,SLOT(slotSetBottomPointColor()));
}

void CFeatureTagPropertySettingDialog::slotSetLineColor()
{
    setColor(m_lineColor,*ui->lineColorButton);
}

void CFeatureTagPropertySettingDialog::slotSetTopPointColor()
{
    setColor(m_topPointColor,*ui->topPointColorButton);
}

void CFeatureTagPropertySettingDialog::slotSetBottomPointColor()
{
    setColor(m_bottomPointColor,*ui->bottomPointColorButton);
}

/*!
\section
*/
void CFeatureTagPropertySettingDialog::setColor(QColor &initialColor, QPushButton &colorButton)
{
    selectColor(initialColor,colorButton,this);
}

void CFeatureTagPropertySettingDialog::setLineStyle(QComboBox *comboBox)
{
    comboBox->setToolTip(tr("Line style"));
    comboBox->setEditable(false);
    comboBox->setIconSize(QSize(92,14));
    comboBox->setMinimumWidth(92);

    comboBox->clear();
    for (int i = (int)Qt::SolidLine; i < (int)Qt::CustomDashLine; i++)
    {
        QPixmap pix(92,14);
        pix.fill(Qt::white);

        QBrush brush(Qt::black);
        QPen pen(brush,2.5,(Qt::PenStyle)i);

        QPainter painter(&pix);
        painter.setPen(pen);
        painter.drawLine(2,7,90,7);

        comboBox->addItem(QIcon(pix),"");
    }
}

void CFeatureTagPropertySettingDialog::setPointStyle(QComboBox *comboBox)
{
    comboBox->setToolTip(tr("Point Style"));
    comboBox->setEditable(false);
    comboBox->setIconSize(QSize(92,14));
    comboBox->setMinimumWidth(92);
    comboBox->clear();

    comboBox->addItem(QIcon(":/icon/icon/MDCurveCircle.png"),"");
    comboBox->addItem(QIcon(":/icon/icon/MDCurveSquare.png"),"");
    comboBox->addItem(QIcon(":/icon/icon/MDCurveTriangle.png"),"");
    comboBox->addItem(QIcon(":/icon/icon/MDCurveCross.png"),"");
}

