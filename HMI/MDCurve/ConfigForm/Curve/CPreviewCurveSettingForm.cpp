#include "CPreviewCurveSettingForm.h"
#include "ui_CPreviewCurveSettingForm.h"
#include "CPreviewCurve.h"
#include "CPreviewCurveData.h"
#include "CCurveObject.h"
#include "CCurveObjectManager.h"
#include "CFormToolFunc.h"
#include "CTranslator.h"
#include <QDebug>

using namespace FormToolFunc;
using namespace Translator;

CPreviewCurveSettingForm::CPreviewCurveSettingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CPreviewCurveSettingForm)
{
    ui->setupUi(this);
    m_isModified = false;
    m_previewCurve =NULL;

}

CPreviewCurveSettingForm::~CPreviewCurveSettingForm()
{
    delete ui;
}

void CPreviewCurveSettingForm::setData(CPreviewCurve *previewCurve, CCurveObjectManager *curves)
{
    if(!previewCurve)
        return;
    m_previewCurve = previewCurve;
    m_curves = curves;
    initialize();
}

void CPreviewCurveSettingForm::setProjectPath(const QString &path)
{
    m_projectPath = path;
}

void CPreviewCurveSettingForm::setCurveObjectManager(CCurveObjectManager *curves)
{
    m_curves = curves;
    initializeComboBox(m_curves,ui->curveBoundedComboBox);
}

void CPreviewCurveSettingForm::resetModified()
{
    m_isModified = false;
}

void CPreviewCurveSettingForm::slotHandleModified()
{
    if(!m_isModified)
    {
        m_isModified = true;
        emit itemModified();
    }
}

void CPreviewCurveSettingForm::initialize()
{
    if(!m_previewCurve)
        return;
    breakConnections();
    initializeUI();
    initializePreviewArea();
    initializeScrollBar();
    initializeCurveAndLabel();
    createConnections();
}

void CPreviewCurveSettingForm::initializeUI()
{
    QValidator *validator1 = new QIntValidator(1,20,this);
    ui->mainBisectrixLineEdit->setValidator(validator1);
    QValidator *validator2 = new QIntValidator(1,10,this);
    ui->subBisectrixLineEdit->setValidator(validator2);
    QValidator *validator3 = new QIntValidator(3,8,this);
    ui->subBisectrixHightLineEdit->setValidator(validator3);
    m_isPreviewVisible = m_previewCurve->previewData()->isPreviewVisible();
    ui->isVisibleCheckBox->setChecked(m_isPreviewVisible);
}

void CPreviewCurveSettingForm::initializePreviewArea()
{
    m_previewBackBrush = m_previewCurve->previewData()->previewBackBrush();
    setButtonBackGroundColor(m_previewBackBrush.brush(ui->backBrushButton->rect()),*ui->backBrushButton);
    m_previewHightLightBrush = m_previewCurve->previewData()->previewHightLightBrush();
    setButtonBackGroundColor(m_previewHightLightBrush.brush(ui->hightLightButton->rect()),*ui->hightLightButton);

    QString string;
    m_mainBisectrix = m_previewCurve->previewData()->mainBisectrix();
    string.setNum(m_mainBisectrix);
    ui->mainBisectrixLineEdit->setText(string);
    m_subBisectrix = m_previewCurve->previewData()->subBisectrix();
    string.setNum(m_subBisectrix);
    ui->subBisectrixLineEdit->setText(string);
    m_subBisectrixheight = m_previewCurve->previewData()->subBisectrixHeight();
    string.setNum(m_subBisectrixheight);
    ui->subBisectrixHightLineEdit->setText(string);

    m_mainBisectrixColor = m_previewCurve->previewData()->mainBisectrixColor();
    setButtonColor(m_mainBisectrixColor,*ui->mainBisectrixColorBtn);
    m_subBisectrixColor = m_previewCurve->previewData()->subBisectrixColor();
    setButtonColor(m_subBisectrixColor,*ui->subBisectrixColorBtn);

    m_startBackColor = m_previewCurve->previewData()->startBackColor();
    setButtonColor(m_startBackColor,*ui->startBackColorBtn);
    m_startScaleColor = m_previewCurve->previewData()->startScaleColor();
    setButtonColor(m_startScaleColor,*ui->startScaleColorBtn);
    m_startLineMarkColor = m_previewCurve->previewData()->startLineMarkColor();
    setButtonColor(m_startLineMarkColor,*ui->startLineMarkColorBtn);
    m_endBackColor = m_previewCurve->previewData()->endBackColor();
    setButtonColor(m_endBackColor,*ui->endBackColorBtn);
    m_endScaleColor = m_previewCurve->previewData()->endScaleColor();
    setButtonColor(m_endScaleColor,*ui->endScaleColorBtn);
    m_endLineMarkColor = m_previewCurve->previewData()->endLineMarkColor();
    setButtonColor(m_endLineMarkColor,*ui->endLineMarkColorBtn);
}

void CPreviewCurveSettingForm::initializeScrollBar()
{
    m_rollBackBrush = m_previewCurve->previewData()->rollBackBrush();
    setButtonBackGroundColor(m_rollBackBrush.brush(ui->rollBackBrushBtn->rect()),*ui->rollBackBrushBtn);
    m_rollMoveBackBrush = m_previewCurve->previewData()->rollMoveBackBrush();
    setButtonBackGroundColor(m_rollMoveBackBrush.brush(ui->rollMoveBackBrushBtn->rect()),*ui->rollMoveBackBrushBtn);
    m_rollStartBackBrush = m_previewCurve->previewData()->rollStartBackBrush();
    setButtonBackGroundColor(m_rollStartBackBrush.brush(ui->rollStartBackBrushBtn->rect()),*ui->rollStartBackBrushBtn);
    m_rollEndBackBrush = m_previewCurve->previewData()->rollEndBackBrush();
    setButtonBackGroundColor(m_rollEndBackBrush.brush(ui->rollEndBackBrushBtn->rect()),*ui->rollEndBackBrushBtn);

    m_rollScaleColor = m_previewCurve->previewData()->rollScaleColor();
    setButtonColor(m_rollScaleColor,*ui->rollScaleColorBtn);
    m_rollStartArrowsColor = m_previewCurve->previewData()->rollStartArrowsColor();
    setButtonColor(m_rollStartArrowsColor,*ui->rollStartArrowsColorBtn);
    m_rollEndArrowsColor = m_previewCurve->previewData()->rollEndArrowsColor();
    setButtonColor(m_rollEndArrowsColor,*ui->rollEndArrowsColorBtn);
}

void CPreviewCurveSettingForm::initializeCurveAndLabel()
{
    initializeComboBox(m_curves,ui->curveBoundedComboBox);
    m_isCurveVisible = m_previewCurve->previewData()->isCurveVisible();
    ui->isCurveVisibleCheckBox->setChecked(m_isCurveVisible);
}

void CPreviewCurveSettingForm::initializeComboBox(CCurveObjectManager *curves, QComboBox *comboBox)
{
    ui->curveBoundedComboBox->clear();
    if(curves->list().count() != 0)
    {
        for(int i  = 0; i < curves->list().count();i++)
        {
            CCurveObject *curve = m_curves->list().at(i);
            comboBox->addItem(curve->name());
        }
        m_boundedCurve = m_previewCurve->previewData()->boundedCurve();
        for(int i = 0;i<curves->list().count();i++)
        {
            if(curves->list().at(i)->name() == m_boundedCurve)
            {
                curves->list().at(i)->setBoundedPreview(true);
                comboBox->setCurrentIndex(i);
            }
        }
    }
}

void CPreviewCurveSettingForm::slotSetPreviewVisible(bool checked)
{
    m_isPreviewVisible = checked;
}

void CPreviewCurveSettingForm::slotSetCurveVisible(bool checked)
{
    m_isCurveVisible = checked;
}

void CPreviewCurveSettingForm::slotSetCurveComboBox(int index)
{
    ui->curveBoundedComboBox->setCurrentIndex(index);
    emit itemModified();
}

void CPreviewCurveSettingForm::slotSetPreviewBack()
{
    m_previewBackBrush.setProjectPath(m_projectPath + "/image");
    if(m_previewBackBrush.showDialog())
    {
         setButtonBackGroundColor(m_previewBackBrush.brush(ui->backBrushButton->rect()),*ui->backBrushButton);
         slotHandleModified();
         update();
    }
}

void CPreviewCurveSettingForm::slotSetHighLight()
{
    m_previewHightLightBrush.setProjectPath(m_projectPath + "/image");
    if(m_previewHightLightBrush.showDialog())
    {
         setButtonBackGroundColor(m_previewHightLightBrush.brush(ui->hightLightButton->rect()),*ui->hightLightButton);
         slotHandleModified();
         update();
    }
}

void CPreviewCurveSettingForm::slotSetMainColor()
{
    setColor(m_mainBisectrixColor,*ui->mainBisectrixColorBtn);
}

void CPreviewCurveSettingForm::slotSetSubColor()
{
    setColor(m_subBisectrixColor,*ui->subBisectrixColorBtn);
}

void CPreviewCurveSettingForm::slotSetStartBackColor()
{
    setColor(m_startBackColor,*ui->startBackColorBtn);
}

void CPreviewCurveSettingForm::slotSetStartScaleColor()
{
    setColor(m_startScaleColor,*ui->startScaleColorBtn);
}

void CPreviewCurveSettingForm::slotStartLineMarkColor()
{
    setColor(m_startLineMarkColor,*ui->startLineMarkColorBtn);
}

void CPreviewCurveSettingForm::slotSetEndBackColor()
{
    setColor(m_endBackColor,*ui->endBackColorBtn);
}

void CPreviewCurveSettingForm::slotSetEndScaleColor()
{
    setColor(m_endScaleColor,*ui->endScaleColorBtn);
}

void CPreviewCurveSettingForm::slotEndLineMarkColor()
{
    setColor(m_endLineMarkColor,*ui->endLineMarkColorBtn);
}

void CPreviewCurveSettingForm::slotSetRollBack()
{
    m_rollBackBrush.setProjectPath(m_projectPath + "/image");
    if(m_rollBackBrush.showDialog())
    {
         setButtonBackGroundColor(m_rollBackBrush.brush(ui->rollBackBrushBtn->rect()),*ui->rollBackBrushBtn);
         slotHandleModified();
         update();
    }
}

void CPreviewCurveSettingForm::slotSetRollMoveBack()
{
    m_rollMoveBackBrush.setProjectPath(m_projectPath + "/image");
    if(m_rollMoveBackBrush.showDialog())
    {
         setButtonBackGroundColor(m_rollMoveBackBrush.brush(ui->rollMoveBackBrushBtn->rect()),*ui->rollMoveBackBrushBtn);
         slotHandleModified();
         update();
    }
}

void CPreviewCurveSettingForm::slotSetRollStartBack()
{
    m_rollStartBackBrush.setProjectPath(m_projectPath + "/image");
    if(m_rollStartBackBrush.showDialog())
    {
         setButtonBackGroundColor(m_rollStartBackBrush.brush(ui->rollStartBackBrushBtn->rect()),*ui->rollStartBackBrushBtn);
         slotHandleModified();
         update();
    }
}

void CPreviewCurveSettingForm::slotSetRollEndBack()
{
    m_rollEndBackBrush.setProjectPath(m_projectPath + "/image");
    if(m_rollEndBackBrush.showDialog())
    {
         setButtonBackGroundColor(m_rollEndBackBrush.brush(ui->rollEndBackBrushBtn->rect()),*ui->rollEndBackBrushBtn);
         slotHandleModified();
         update();
    }
}

void CPreviewCurveSettingForm::slotSetRollScaleColor()
{
    setColor(m_rollScaleColor,*ui->rollScaleColorBtn);
}

void CPreviewCurveSettingForm::slotSetStartArrowColor()
{
    setColor(m_rollStartArrowsColor,*ui->rollStartArrowsColorBtn);
}

void CPreviewCurveSettingForm::slotSetEndArrrowColor()
{
    setColor(m_rollEndArrowsColor,*ui->rollEndArrowsColorBtn);
}

void CPreviewCurveSettingForm::applyChanges()
{
    m_previewCurve->previewData()->setPreviewVisible(m_isPreviewVisible);

    applyPreviewAreaChanges();
    applyScrollBarChanges();
    applyCurveAndLabelChanges();
}

void CPreviewCurveSettingForm::applyPreviewAreaChanges()
{
    m_previewCurve->previewData()->setPreviewBackBrush(m_previewBackBrush);
    m_previewCurve->previewData()->setPreviewHightLightBrush(m_previewHightLightBrush);

    m_mainBisectrix = ui->mainBisectrixLineEdit->text().toInt();
    m_previewCurve->previewData()->setMainBisectrix(m_mainBisectrix);
    m_subBisectrix = ui->subBisectrixLineEdit->text().toInt();
    m_previewCurve->previewData()->setSubBisectrix(m_subBisectrix);
    m_subBisectrixheight = ui->subBisectrixHightLineEdit->text().toInt();
    m_previewCurve->previewData()->setSubBisectrixHeight(m_subBisectrixheight);
    m_previewCurve->previewData()->setMainBisectrixColor(m_mainBisectrixColor);
    m_previewCurve->previewData()->setSubBisectrixColor(m_subBisectrixColor);

    m_previewCurve->previewData()->setStartBackColor(m_startBackColor);
    m_previewCurve->previewData()->setStartScaleColor(m_startScaleColor);
    m_previewCurve->previewData()->setStartLineMarkColor(m_startLineMarkColor);
    m_previewCurve->previewData()->setEndBackColor(m_endBackColor);
    m_previewCurve->previewData()->setEndScaleColor(m_endScaleColor);
    m_previewCurve->previewData()->setEndLineMarkColor(m_endLineMarkColor);
}

void CPreviewCurveSettingForm::applyScrollBarChanges()
{
    m_previewCurve->previewData()->setRollBackBrush(m_rollBackBrush);
    m_previewCurve->previewData()->setRollMoveBackBrush(m_rollMoveBackBrush);
    m_previewCurve->previewData()->setRollStartBackBrush(m_rollStartBackBrush);
    m_previewCurve->previewData()->setRollEndBackBrush(m_rollEndBackBrush);
    m_previewCurve->previewData()->setRollScaleColor(m_rollScaleColor);
    m_previewCurve->previewData()->setRollStartArrowsColor(m_rollStartArrowsColor);
    m_previewCurve->previewData()->setRollEndArrowsColor(m_rollEndArrowsColor);

}

void CPreviewCurveSettingForm::applyCurveAndLabelChanges()
{
    m_previewCurve->previewData()->setCurveVisible(m_isCurveVisible);
    if(m_curves->list().count()!= 0)
    {
        m_previewCurve->previewData()->setBoundedCurve(m_curves->list().at(ui->curveBoundedComboBox->currentIndex())->name());
        m_curves->list().at(ui->curveBoundedComboBox->currentIndex())->setBoundedPreview(true);
    }
}

void CPreviewCurveSettingForm::createConnections()
{
    connect(ui->isVisibleCheckBox,&QCheckBox::toggled,this,&CPreviewCurveSettingForm::slotSetPreviewVisible);
    connect(ui->isVisibleCheckBox,&QCheckBox::stateChanged,this,&CPreviewCurveSettingForm::slotHandleModified);
    connect(ui->isCurveVisibleCheckBox,&QCheckBox::toggled,this,&CPreviewCurveSettingForm::slotSetCurveVisible);
    connect(ui->isCurveVisibleCheckBox,&QCheckBox::stateChanged,this,&CPreviewCurveSettingForm::slotHandleModified);
    connect(ui->curveBoundedComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetCurveComboBox(int)));

    connect(ui->backBrushButton,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotSetPreviewBack);
    connect(ui->hightLightButton,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotSetHighLight);
    connect(ui->mainBisectrixLineEdit,&QLineEdit::textEdited,this,&CPreviewCurveSettingForm::slotHandleModified);
    connect(ui->subBisectrixLineEdit,&QLineEdit::textEdited,this,&CPreviewCurveSettingForm::slotHandleModified);
    connect(ui->subBisectrixHightLineEdit,&QLineEdit::textEdited,this,&CPreviewCurveSettingForm::slotHandleModified);

    connect(ui->mainBisectrixColorBtn,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotSetMainColor);
    connect(ui->subBisectrixColorBtn,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotSetSubColor);

    connect(ui->startBackColorBtn,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotSetStartBackColor);
    connect(ui->startScaleColorBtn,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotSetStartScaleColor);
    connect(ui->startLineMarkColorBtn,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotStartLineMarkColor);
    connect(ui->endBackColorBtn,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotSetEndBackColor);
    connect(ui->endScaleColorBtn,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotSetEndScaleColor);
    connect(ui->endLineMarkColorBtn,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotEndLineMarkColor);


    connect(ui->rollBackBrushBtn,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotSetRollBack);
    connect(ui->rollMoveBackBrushBtn,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotSetRollMoveBack);
    connect(ui->rollStartBackBrushBtn,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotSetRollStartBack);
    connect(ui->rollEndBackBrushBtn,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotSetRollEndBack);
    connect(ui->rollScaleColorBtn,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotSetRollScaleColor);
    connect(ui->rollStartArrowsColorBtn,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotSetStartArrowColor);
    connect(ui->rollEndArrowsColorBtn,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotSetEndArrrowColor);
}

void CPreviewCurveSettingForm::breakConnections()
{
    disconnect(ui->isVisibleCheckBox,&QCheckBox::toggled,this,&CPreviewCurveSettingForm::slotSetPreviewVisible);
    disconnect(ui->isVisibleCheckBox,&QCheckBox::stateChanged,this,&CPreviewCurveSettingForm::slotHandleModified);
    disconnect(ui->isCurveVisibleCheckBox,&QCheckBox::toggled,this,&CPreviewCurveSettingForm::slotSetCurveVisible);
    disconnect(ui->isCurveVisibleCheckBox,&QCheckBox::stateChanged,this,&CPreviewCurveSettingForm::slotHandleModified);
    disconnect(ui->curveBoundedComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetCurveComboBox(int)));

    disconnect(ui->backBrushButton,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotSetPreviewBack);
    disconnect(ui->hightLightButton,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotSetHighLight);
    disconnect(ui->mainBisectrixLineEdit,&QLineEdit::textEdited,this,&CPreviewCurveSettingForm::slotHandleModified);
    disconnect(ui->subBisectrixLineEdit,&QLineEdit::textEdited,this,&CPreviewCurveSettingForm::slotHandleModified);
    disconnect(ui->subBisectrixHightLineEdit,&QLineEdit::textEdited,this,&CPreviewCurveSettingForm::slotHandleModified);

    disconnect(ui->mainBisectrixColorBtn,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotSetMainColor);
    disconnect(ui->subBisectrixColorBtn,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotSetSubColor);

    disconnect(ui->startBackColorBtn,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotSetStartBackColor);
    disconnect(ui->startScaleColorBtn,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotSetStartScaleColor);
    disconnect(ui->startLineMarkColorBtn,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotStartLineMarkColor);
    disconnect(ui->endBackColorBtn,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotSetEndBackColor);
    disconnect(ui->endScaleColorBtn,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotSetStartScaleColor);
    disconnect(ui->endLineMarkColorBtn,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotEndLineMarkColor);

    disconnect(ui->rollBackBrushBtn,&QPushButton::clicked,this,&CPreviewCurveSettingForm::slotSetRollBack);
}

void CPreviewCurveSettingForm::setColor( QColor &initialColor, QPushButton &colorButton)
{
    int ret =  selectColor(initialColor,colorButton,this);
    if(ret == QDialog::Accepted)
        slotHandleModified();
}
void CPreviewCurveSettingForm::setButtonBackGroundColor(const QBrush &brush, QPushButton &button)
{
    QPalette buttonPalette;
    buttonPalette.setBrush(button.backgroundRole(),brush);
    button.setFlat(true);
    button.setAutoFillBackground(true);
    button.setPalette(buttonPalette);
}
