#include "MDCurveSettingDialog.h"
#include "ui_MDCurveSettingDialog.h"
#include "CCurveProperty.h"
#include "CCurveWidget.h"

#include "IMDDataArchives.h"
#include "CFind.h"
#include "CAppearanceSettingForm.h"
#include "CCurveSettingForm.h"
#include "CFeatureTagSettingForm.h"
#include "CVernierSettingForm.h"
#include "CXAxisSettingForm.h"
#include "CYAxisSettingForm.h"
#include "CDrawModeSettingForm.h"
#include "CReferenceCurveSettingForm.h"
#include "CPreviewCurveSettingForm.h"
#include "CPageSettingForm.h"
#include "CToolsSettingForm.h"
#include "CCurveStisticalForm.h"
#include "CCurveProperty.h"
#include "IDrawEnvironment.h"

#include "CTagManager.h"
#include "CTranslator.h"
#include "IStudioInterface.h"

#include <QVBoxLayout>
#include <QGridLayout>

using namespace Translator;
MDCurveSettingDialog::MDCurveSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MDCurveSettingDialog),
    m_isModified(false),
    m_isRealTime(false),
    m_widget(0),
    m_realCurve(0),
    m_environment(0),
    m_apperanceForm(new CAppearanceSettingForm(this)),
    m_curvesForm(new CCurveSettingForm(this)),
    m_drawModeForm(new CDrawModeSettingForm(this)),
    m_vernierForm(new CVernierSettingForm(this)),
    m_xAxisForm(new CXAxisSettingForm(this)),
    m_yAxisForm(new CYAxisSettingForm(this)),
    m_featureTagForm(new CFeatureTagSettingForm(this)),
    m_referenceCurveForm(new CReferenceCurveSettingForm(this)),
    m_previewCurveForm(new CPreviewCurveSettingForm(this)),
    m_toolsSettingForm(new CToolsSettingForm(this)),
    m_pageSettingForm(new CPageSettingForm(this)),
    m_curveSitsticalForm(new CCurveStisticalForm(this)),
    m_translateTool(0)
{
    ui->setupUi(this);

    installDialogTranslator(m_translateTool,this);
}

MDCurveSettingDialog::~MDCurveSettingDialog()
{
    delete ui;
    delete m_translateTool;
}

/*!
\section
*/
void MDCurveSettingDialog::setData(CCurveProperty *curve, IDrawEnvrionment *environment, bool isRealTime)
{
    if(curve && environment) {
        setIsRealTime(isRealTime);
        m_realCurve = curve;
        m_environment = environment;
        setVariantSettingUI(m_environment->studioProject()->rtdbManagerUI());
        setDataArchives(m_environment->dataArchives());
        initialize();
    }
}


void MDCurveSettingDialog::setWidget(CCurveWidget *widget)
{
    m_widget = widget;
}

void MDCurveSettingDialog::setVariantSettingUI(IMDRTDBManagerUI *variantUI)
{
    m_curvesForm->setVariantSettingUI(variantUI);
    m_featureTagForm->setVariantForm(variantUI);
    if(m_isRealTime)
    {
        m_referenceCurveForm->setVariantSettingUI(variantUI);
    }
}
void MDCurveSettingDialog::setDataArchives(IMDDataArchives *dataArchives)
{

    if(m_curvesForm){
        m_curvesForm->setDataArchives(dataArchives);
    }
    if(m_featureTagForm){
        m_featureTagForm->setDataArchives(dataArchives);
    }
}
/*!
\section
*/
void MDCurveSettingDialog::resetModified()
{
    m_apperanceForm->resetModified();
    m_vernierForm->resetModified();
    m_yAxisForm->resetModified();
    m_xAxisForm->resetModified();
    m_curvesForm->resetModified();
    m_drawModeForm->resetModified();
    m_featureTagForm->resetModified();
    m_toolsSettingForm->resetModified();
    m_referenceCurveForm->resetModified();
    m_previewCurveForm->resetModified();
    ui->applyButton->setEnabled(false);
    m_isModified = false;
}

void MDCurveSettingDialog::slotHandleModified()
{
    if(!m_isModified)
    {
        m_isModified = true;
        ui->applyButton->setEnabled(true);
    }
}

void MDCurveSettingDialog::closeEvent(QCloseEvent *e)
{
    Q_UNUSED(e);
    m_realCurve->refresh();
}

/*!
\section
*/

void MDCurveSettingDialog::createConnections()
{
    connect(ui->applyButton,&QPushButton::clicked,this,&MDCurveSettingDialog::slotApplyChanges);
    connect(ui->okButton,&QPushButton::clicked,this,&MDCurveSettingDialog::slotConfirmButtonClicked);
    connect(ui->cancelButton,&QPushButton::clicked,this,&MDCurveSettingDialog::slotCancelButtonClicked);

    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slotShowWidget(QListWidgetItem*)));

    connect(m_apperanceForm,SIGNAL(itemModified()),this,SLOT(slotHandleModified()));

    connect(m_vernierForm,&CVernierSettingForm::itemModified,this,&MDCurveSettingDialog::slotHandleModified);
    connect(m_yAxisForm,&CYAxisSettingForm::itemModified,this,&MDCurveSettingDialog::slotHandleModified);
    connect(m_xAxisForm,&CXAxisSettingForm::itemModified,this,&MDCurveSettingDialog::slotHandleModified);
    connect(m_curvesForm,&CCurveSettingForm::itemModified,this,&MDCurveSettingDialog::slotHandleModified);
    connect(m_drawModeForm,&CDrawModeSettingForm::itemModified,this,&MDCurveSettingDialog::slotHandleModified);
    connect(m_featureTagForm,&CFeatureTagSettingForm::itemModified,this,&MDCurveSettingDialog::slotHandleModified);
    connect(m_toolsSettingForm,&CToolsSettingForm::itemModified,this,&MDCurveSettingDialog::slotHandleModified);
    connect(m_referenceCurveForm,&CReferenceCurveSettingForm::itemModified,this,&MDCurveSettingDialog::slotHandleModified);
    connect(m_previewCurveForm,&CPreviewCurveSettingForm::itemModified,this,&MDCurveSettingDialog::slotHandleModified);

}
void MDCurveSettingDialog::breakConnections()
{
    disconnect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slotShowWidget(QListWidgetItem*)));
    disconnect(m_apperanceForm,SIGNAL(itemModified()),this,SLOT(slotHandleModified()));
    disconnect(m_vernierForm,&CVernierSettingForm::itemModified,this,&MDCurveSettingDialog::slotHandleModified);
    disconnect(m_yAxisForm,&CYAxisSettingForm::itemModified,this,&MDCurveSettingDialog::slotHandleModified);
    disconnect(m_xAxisForm,&CXAxisSettingForm::itemModified,this,&MDCurveSettingDialog::slotHandleModified);
    disconnect(m_curvesForm,&CCurveSettingForm::itemModified,this,&MDCurveSettingDialog::slotHandleModified);
    disconnect(m_drawModeForm,&CDrawModeSettingForm::itemModified,this,&MDCurveSettingDialog::slotHandleModified);
    disconnect(m_featureTagForm,&CFeatureTagSettingForm::itemModified,this,&MDCurveSettingDialog::slotHandleModified);
    disconnect(m_toolsSettingForm,&CToolsSettingForm::itemModified,this,&MDCurveSettingDialog::slotHandleModified);
    disconnect(m_referenceCurveForm,&CReferenceCurveSettingForm::itemModified,this,&MDCurveSettingDialog::slotHandleModified);
    disconnect(m_previewCurveForm,&CPreviewCurveSettingForm::itemModified,this,&MDCurveSettingDialog::slotHandleModified);
}

void MDCurveSettingDialog::slotApplyChanges()
{
    applyChanges();
    if(m_widget)
        m_widget->update();
    resetModified();
}

void MDCurveSettingDialog::slotConfirmButtonClicked()
{
    if(m_isModified)
        slotApplyChanges();
    accept();
}

void MDCurveSettingDialog::slotCancelButtonClicked()
{
    m_realCurve->refresh();
    reject();
}

/*!
 \brief addWidgets
     \a   Appearance,
        Xaxis,
        Yaxis,
        Curve,
        DrawMode,
        Vernier,
        FeatureTag,
        ReferanceCurve,
        CurveStistical,
        ToolsSetting,
        PageSetting
*/

void MDCurveSettingDialog::slotShowWidget(QListWidgetItem *item)
{

    switch (item->data(Qt::UserRole).toInt()) {
    case (int)WidgetName::Appearance:
        ui->stackedWidget->setCurrentIndex( (int)WidgetName::Appearance);
        break;
    case  (int)WidgetName::Xaxis:
        ui->stackedWidget->setCurrentIndex( (int)WidgetName::Xaxis);
        break;
    case  (int)WidgetName::Yaxis:
        ui->stackedWidget->setCurrentIndex( (int)WidgetName::Yaxis);
        break;
    case  (int)WidgetName::Curve:
        ui->stackedWidget->setCurrentIndex( (int)WidgetName::Curve);
        break;
    case (int)WidgetName::ReferenceCurve:
        ui->stackedWidget->setCurrentIndex((int)WidgetName::ReferenceCurve);
        break;
    case (int)WidgetName::PreviewCurve:
        ui->stackedWidget->setCurrentIndex((int)WidgetName::PreviewCurve);
        break;
    case  (int)WidgetName::DrawMode:
        ui->stackedWidget->setCurrentIndex( (int)WidgetName::DrawMode);
        break;
    case  (int)WidgetName::Vernier:
        ui->stackedWidget->setCurrentIndex( (int)WidgetName::Vernier);
        break;
    case (int) WidgetName::FeatureTag:
        ui->stackedWidget->setCurrentIndex( (int)WidgetName::FeatureTag);
        break;
    case (int) WidgetName::ToolsSetting:
        ui->stackedWidget->setCurrentIndex( (int)WidgetName::ToolsSetting);
        break;

    default:
        break;
    }
}

/*!
\section
*/
void MDCurveSettingDialog::initialize()
{
    breakConnections();
    addItems();
    addWidgets();
    setItemsData();
    createConnections();
}
void MDCurveSettingDialog::addItems()
{

    QStringList itemList;
    itemList <<tr("基准曲线") <<tr("页面设置");

    insertItem(tr("外观"),(int)WidgetName::Appearance);
    insertItem(tr("X轴"),(int)WidgetName::Xaxis);
    insertItem(tr("Y轴"),(int)WidgetName::Yaxis);
    insertItem(tr("曲线"),(int)WidgetName::Curve);
    insertItem(tr("基准曲线"),(int)WidgetName::ReferenceCurve);
    insertItem(tr("曲线预览"),(int)WidgetName::PreviewCurve);
    insertItem(tr("绘制模式"),(int)WidgetName::DrawMode,m_isRealTime);
    insertItem(tr("游标"),(int)WidgetName::Vernier);

    insertItem(tr("特征标签"),(int)WidgetName::FeatureTag);
    insertItem(tr("工具栏"),(int)WidgetName::ToolsSetting);

    ui->listWidget->setCurrentRow((int)WidgetName::Appearance);
}

void MDCurveSettingDialog::addWidgets()
{
    ui->stackedWidget->addWidget(m_apperanceForm);
    ui->stackedWidget->addWidget(m_xAxisForm);
    ui->stackedWidget->addWidget(m_yAxisForm);
    ui->stackedWidget->addWidget(m_curvesForm);
    ui->stackedWidget->addWidget(m_referenceCurveForm);
    ui->stackedWidget->addWidget(m_previewCurveForm);
    ui->stackedWidget->addWidget(m_drawModeForm);
    ui->stackedWidget->addWidget(m_vernierForm);
    ui->stackedWidget->addWidget(m_featureTagForm);
    ui->stackedWidget->addWidget(m_toolsSettingForm);
    ui->stackedWidget->addWidget(m_curveSitsticalForm);
    ui->stackedWidget->addWidget(m_pageSettingForm);
}
void MDCurveSettingDialog::setItemsData()
{
    m_apperanceForm->setProjectPath(m_environment->projectPath());
    m_apperanceForm->setData(m_realCurve->appearance(),m_realCurve->gridRegion(), m_realCurve->label());

    m_vernierForm->setData(m_realCurve->vernier());
    m_yAxisForm->setCurveForm(m_curvesForm);
    m_yAxisForm->setReferenceForm(m_referenceCurveForm);
    m_yAxisForm->setData(m_realCurve->ys(),m_realCurve->gridRegion());
    m_xAxisForm->setCurveForm(m_curvesForm);
    m_xAxisForm->setReferenceForm(m_referenceCurveForm);
    m_xAxisForm->setFeatureTagForm(m_featureTagForm);
    m_xAxisForm->setData(m_realCurve->xs(),m_realCurve->gridRegion());
    m_curvesForm->setDataManager(m_realCurve->data());// datasource filter
    m_curvesForm->setData(m_realCurve->curves(),m_xAxisForm->list(),m_yAxisForm->list());
    m_curvesForm->setPreviewCurveForm(m_previewCurveForm);
    m_referenceCurveForm->setData(m_realCurve->referenceCurves(),m_xAxisForm->list(),m_yAxisForm->list());
    m_previewCurveForm->setData(m_realCurve->previewCurve(),m_realCurve->curves());
    m_previewCurveForm->setProjectPath(m_environment->projectPath());

    m_drawModeForm->setData(m_realCurve->drawMode());
    // Mark need to change

    m_featureTagForm->setDataManager(m_realCurve->data());
    if(!m_isRealTime)
        m_featureTagForm->setData(m_realCurve->historyTag(),m_realCurve->xs());
    else
        m_featureTagForm->setData(m_realCurve->realTag(),m_realCurve->xs());

    m_toolsSettingForm->setData(m_realCurve->tools());
}
void MDCurveSettingDialog::setIsRealTime(bool isRealTime)
{
    m_isRealTime = isRealTime;

    m_curvesForm->setIsRealTime(isRealTime);
    m_featureTagForm->setIsRealTime(isRealTime);
}

void MDCurveSettingDialog::insertItem(const QString &name, int userRole, bool visible)
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setData(Qt::UserRole,userRole);
    item->setText(CMDMultiLanguage::instance()->value(name));
    ui->listWidget->addItem(item);

    item->setHidden(!visible); // should be called after the item added into the widget;
}

void MDCurveSettingDialog::applyChanges()
{
    m_vernierForm->applyChanges();
    m_yAxisForm->applyChanges();
    m_xAxisForm->applyChanges();
    m_curvesForm->applyChanges();
    m_referenceCurveForm->applyChanges();
    m_drawModeForm->applyChanges();
    m_featureTagForm->applyChanges();
    m_toolsSettingForm->applyChanges();
    m_apperanceForm->applyChanges();
    m_previewCurveForm->applyChanges();
    m_realCurve->applyChanges();
}
