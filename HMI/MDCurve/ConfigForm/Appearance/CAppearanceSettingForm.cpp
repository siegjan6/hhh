#include "CAppearanceSettingForm.h"
#include "ui_CAppearanceSettingForm.h"
#include "CAppearance.h"
#include "CGridRegion.h"
#include "CLabelManager.h"

#include "MDBrushManager.h"
#include "MDBrushSelectDialog.h"
#include "MDMultiLanguage.h"

#include <QDebug>

#include <QValidator>
#include <QIntValidator>
#include <QFontDialog>
#include <QColorDialog>
#include <QPalette>
#include "CFormToolFunc.h"
using namespace FormToolFunc;

CAppearanceSettingForm::CAppearanceSettingForm(QWidget *parent) :
    QWidget(parent),
    m_isModified(false),
    ui(new Ui::CAppearanceSettingForm),
    m_widgetPanelVisible(false),
    m_customMenuEnable(false),
    m_labelVisible(false),
    m_currentCurveValueVisible(false),
    m_appearance(0),
    m_gridRegion(0),
    m_labels(0),
    m_curveDialog(new CMDBrushSelectDialog(this)),
    m_widgetDialog(new CMDBrushSelectDialog(this))
{
    ui->setupUi(this);
}

CAppearanceSettingForm::~CAppearanceSettingForm()
{
    if(m_curveDialog)
    {
        delete m_curveDialog;
        m_curveDialog = NULL;
    }

    if(m_widgetDialog)
    {
        delete m_widgetDialog;
        m_widgetDialog = NULL;
    }
    delete ui;
}

void CAppearanceSettingForm::setData(CAppearance *apperance, CGridRegion *gridRegion, CLabelManager *labels)
{
    if(!apperance || !gridRegion || !labels)
        return;
    m_appearance = apperance;
    m_gridRegion = gridRegion;
    m_labels = labels;

    initialize();
}

void CAppearanceSettingForm::setProjectPath(const QString &path)
{
    m_projectPath = path;
}
/*!
\section
*/
void CAppearanceSettingForm::resetModified()
{
    m_isModified = false;
}

void CAppearanceSettingForm::slotHandleModified()
{
    if(!m_isModified){
        m_isModified = true;
        emit itemModified();
    }
}

/*!
\section
*/

void CAppearanceSettingForm::applyChanges()
{
    applyApperanceChanges();
    applyGridRegionChanges();
    applyLabelChanges();
}

void CAppearanceSettingForm::applyApperanceChanges()
{
    if(m_appearance){
        CHeader *header = m_appearance->header();
        if(header)  {
            QString title = ui->titleLineEdit->text().trimmed();
            header->setText(title);
            header->setFont(m_titleFont);
            header->setFontColor(m_titleFontColor);
            header->setStart(QPoint(ui->titleXLineEdit->text().toInt(),ui->titleYLineEdit->text().toInt()));
        }
        m_appearance->setIsShowBorder(m_widgetPanelVisible);
        m_appearance->setWidgetBrush(m_widgetBrush);
        m_appearance->setCurveBrush(m_curveBrush);
    }
}

void CAppearanceSettingForm::applyGridRegionChanges()
{
    if(m_gridRegion) {
        m_gridRegion->setBorderColor(m_borderColor);
    }
}

void CAppearanceSettingForm::applyLabelChanges()
{
    if(m_labels){
        m_labels->setLableVisible(m_labelVisible);
        m_labels->setFont(m_labelFont);
        m_labels->setMovingValueVisible(m_currentCurveValueVisible);

        m_labels->calculate();// to reset the height of the label
    }
}


/*!
\section slots
*/
void CAppearanceSettingForm::slotSetTitleFont()
{
    setFont(m_titleFont);
}

void CAppearanceSettingForm::slotSetBorderColor()
{
   setColor(m_borderColor,*ui->borderColorButton);
}

void CAppearanceSettingForm::slotSetLabelFont()
{
    setFont(m_labelFont);
}

void CAppearanceSettingForm::slotSetWidgetPannelVisible(bool checked)
{
    m_widgetPanelVisible = checked;
}

void CAppearanceSettingForm::slotSetCustomMenuEnable(bool checked)
{
    m_customMenuEnable = checked;
}

void CAppearanceSettingForm::slotSetLabelVisible(bool checked)
{
    m_labelVisible = checked;
}

void CAppearanceSettingForm::slotSetCurrentCurveValueVisible(bool checked)
{
    m_currentCurveValueVisible = checked;
}

void CAppearanceSettingForm::slotSetCurveGradientColor()
{
//    m_curveBrush.setProjectPath(m_projectPath+"/image");
//    m_curveDialog->setBrushManager(m_curveBrush);
//    if(QDialog::Accepted == m_curveDialog->exec()){
//        m_curveBrush = m_curveDialog->brushManager();
//        setButtonBackGroundColor(m_curveBrush.brush(ui->curveStyleButton->rect()),*ui->curveStyleButton);
//        slotHandleModified();
//    }

    m_curveBrush.setProjectPath(m_projectPath+"/image");
    if(m_curveBrush.showDialog())
    {
        setButtonBackGroundColor(m_curveBrush.brush(ui->curveStyleButton->rect()),*ui->curveStyleButton);
        slotHandleModified();
        update();
    }
}

void CAppearanceSettingForm::slotSetWidgetGradientColor()
{
//    m_widgetBrush.setProjectPath(m_projectPath + "/image");
//    m_widgetDialog->setBrushManager(m_widgetBrush);
//    if(QDialog::Accepted == m_widgetDialog->exec()){
//        m_widgetBrush = m_widgetDialog->brushManager();
//        setButtonBackGroundColor(m_widgetBrush.brush(ui->widgetStyleButton->rect()),*ui->widgetStyleButton);
//        slotHandleModified();
//    }
    m_widgetBrush.setProjectPath(m_projectPath + "/image");
    if(m_widgetBrush.showDialog())
    {
         setButtonBackGroundColor(m_widgetBrush.brush(ui->widgetStyleButton->rect()),*ui->widgetStyleButton);
         slotHandleModified();
         update();
    }
}

void CAppearanceSettingForm::slotSetTitleFontColor()
{
    setColor(m_titleFontColor,*ui->fontColorButton);
}
/*!
\section
*/
void CAppearanceSettingForm::createConnections()
{
    connect(ui->titleFontButton,&QPushButton::clicked,this,&CAppearanceSettingForm::slotSetTitleFont);
    connect(ui->fontColorButton,&QPushButton::clicked,this,&CAppearanceSettingForm::slotSetTitleFontColor);
    connect(ui->borderColorButton,&QPushButton::clicked,this,&CAppearanceSettingForm::slotSetBorderColor);
    connect(ui->labelFontButton,&QPushButton::clicked,this,&CAppearanceSettingForm::slotSetLabelFont);
    connect(ui->disableCustomMenuCheckBox,&QCheckBox::toggled,this,&CAppearanceSettingForm::slotSetCustomMenuEnable);
    connect(ui->widgetPanelVisibleCheckBox,&QCheckBox::toggled,this,&CAppearanceSettingForm::slotSetWidgetPannelVisible);
    connect(ui->labelVisibleCheckBox,&QCheckBox::toggled,this,&CAppearanceSettingForm::slotSetLabelVisible);
    connect(ui->currentCurveValueVisibleCheckBox,&QCheckBox::toggled,this,&CAppearanceSettingForm::slotSetCurrentCurveValueVisible);
    connect(ui->curveStyleButton,&QPushButton::clicked,this,&CAppearanceSettingForm::slotSetCurveGradientColor);
    connect(ui->widgetStyleButton,&QPushButton::clicked,this,&CAppearanceSettingForm::slotSetWidgetGradientColor);

    connect(ui->titleLineEdit,&QLineEdit::textChanged,this,&CAppearanceSettingForm::slotHandleModified);
    connect(ui->titleXLineEdit,&QLineEdit::textChanged,this,&CAppearanceSettingForm::slotHandleModified);
    connect(ui->titleYLineEdit,&QLineEdit::textChanged,this,&CAppearanceSettingForm::slotHandleModified);

    connect(ui->currentCurveValueVisibleCheckBox,&QCheckBox::stateChanged,this,&CAppearanceSettingForm::slotHandleModified);
    connect(ui->disableCustomMenuCheckBox,&QCheckBox::stateChanged,this,&CAppearanceSettingForm::slotHandleModified);
    connect(ui->labelVisibleCheckBox,&QCheckBox::stateChanged,this,&CAppearanceSettingForm::slotHandleModified);
    connect(ui->widgetPanelVisibleCheckBox,&QCheckBox::stateChanged,this,&CAppearanceSettingForm::slotHandleModified);
}

void CAppearanceSettingForm::breakConnections()
{
    disconnect(ui->titleFontButton,&QPushButton::clicked,this,&CAppearanceSettingForm::slotSetTitleFont);
    disconnect(ui->fontColorButton,&QPushButton::clicked,this,&CAppearanceSettingForm::slotSetTitleFontColor);
    disconnect(ui->borderColorButton,&QPushButton::clicked,this,&CAppearanceSettingForm::slotSetBorderColor);
    disconnect(ui->labelFontButton,&QPushButton::clicked,this,&CAppearanceSettingForm::slotSetLabelFont);
    disconnect(ui->disableCustomMenuCheckBox,&QCheckBox::toggled,this,&CAppearanceSettingForm::slotSetCustomMenuEnable);
    disconnect(ui->widgetPanelVisibleCheckBox,&QCheckBox::toggled,this,&CAppearanceSettingForm::slotSetWidgetPannelVisible);
    disconnect(ui->labelVisibleCheckBox,&QCheckBox::toggled,this,&CAppearanceSettingForm::slotSetLabelVisible);
    disconnect(ui->currentCurveValueVisibleCheckBox,&QCheckBox::toggled,this,&CAppearanceSettingForm::slotSetCurrentCurveValueVisible);
    disconnect(ui->curveStyleButton,&QPushButton::clicked,this,&CAppearanceSettingForm::slotSetCurveGradientColor);
    disconnect(ui->widgetStyleButton,&QPushButton::clicked,this,&CAppearanceSettingForm::slotSetWidgetGradientColor);

    disconnect(ui->titleLineEdit,&QLineEdit::textChanged,this,&CAppearanceSettingForm::slotHandleModified);
    disconnect(ui->titleXLineEdit,&QLineEdit::textChanged,this,&CAppearanceSettingForm::slotHandleModified);
    disconnect(ui->titleYLineEdit,&QLineEdit::textChanged,this,&CAppearanceSettingForm::slotHandleModified);

    disconnect(ui->currentCurveValueVisibleCheckBox,&QCheckBox::stateChanged,this,&CAppearanceSettingForm::slotHandleModified);
    disconnect(ui->disableCustomMenuCheckBox,&QCheckBox::stateChanged,this,&CAppearanceSettingForm::slotHandleModified);
    disconnect(ui->labelVisibleCheckBox,&QCheckBox::stateChanged,this,&CAppearanceSettingForm::slotHandleModified);
    disconnect(ui->widgetPanelVisibleCheckBox,&QCheckBox::stateChanged,this,&CAppearanceSettingForm::slotHandleModified);

}

/*!
\section
*/
void CAppearanceSettingForm::initialize()
{
    breakConnections();
    initializeAppearance();
    initializeGridRegion();
    initializeLabels();
    initializeUI();
    createConnections();
}

void CAppearanceSettingForm::initializeUI()
{
    QValidator *validator = new QIntValidator(0,1000,this);
    ui->titleXLineEdit->setValidator(validator);
    ui->titleYLineEdit->setValidator(validator);
    ui->titleLineEdit->setMaxLength(100);
}

void CAppearanceSettingForm::initializeAppearance()
{
    CAppearance *appearance = m_appearance;
    if(appearance) {
        CHeader *header = appearance->header();
        if(header){
            ui->titleLineEdit->setText(header->text());
            QFontMetrics fm(header->font());

            int height = fm.height();
            ui->titleXLineEdit->setText(QString::number(header->start().x()));
            ui->titleYLineEdit->setText(QString::number(header->start().y()-height));

            m_titleFont = header->font();
            m_titleFontColor = header->fontColor();
            setButtonColor(m_titleFontColor,*ui->fontColorButton);
        }

        m_widgetPanelVisible = appearance->isShowBorder();
        ui->widgetPanelVisibleCheckBox->setChecked(m_widgetPanelVisible);

        ui->disableCustomMenuCheckBox->setVisible(false);// 后期添加

        m_curveBrush = appearance->curveBrush();
        m_widgetBrush = appearance->widgetBrush();

        setButtonBackGroundColor(m_curveBrush.brush(ui->curveStyleButton->rect()),*ui->curveStyleButton);
        setButtonBackGroundColor(m_widgetBrush.brush(ui->widgetStyleButton->rect()),*ui->widgetStyleButton);
    }
}

void CAppearanceSettingForm::initializeGridRegion()
{
    CGridRegion *gridRegion = m_gridRegion;
    if(gridRegion)
    {
        m_borderColor = gridRegion->borderColor();
        setButtonColor(m_borderColor,*ui->borderColorButton);
    }
}

void CAppearanceSettingForm::initializeLabels()
{
    CLabelManager *labels = m_labels;
    if(labels)
    {
        m_labelVisible = labels->lableVisible();
        m_currentCurveValueVisible = labels->movingValueVisible();
        m_labelFont = labels->font();

        ui->labelVisibleCheckBox->setChecked(m_labelVisible);
        ui->currentCurveValueVisibleCheckBox->setChecked(m_currentCurveValueVisible);
    }
}


/*!
\section
*/

void CAppearanceSettingForm::setFont(QFont & initialFont)
{
    if(selectFont(initialFont,this) == QDialog::Accepted)
        slotHandleModified();
}

void CAppearanceSettingForm::setColor( QColor &initialColor, QPushButton &colorButton)
{
    int ret =  selectColor(initialColor,colorButton,this);
    if(ret == QDialog::Accepted)
        slotHandleModified();
}
void CAppearanceSettingForm::setButtonBackGroundColor(const QBrush &brush, QPushButton &button)
{
    QPalette buttonPalette;
    buttonPalette.setBrush(button.backgroundRole(),brush);
    button.setFlat(true);
    button.setAutoFillBackground(true);
    button.setPalette(buttonPalette);
}

