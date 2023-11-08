#include "CCurvePropertySettingDialog.h"
#include "ui_CCurvePropertySettingDialog.h"

#include "CVariantDialog.h"

#include "CCurveObject.h"
#include "CAbstractAxis.h"
#include "CXAxisManager.h"
#include "CYAxisManager.h"
#include "CPoint.h"

#include "CXAxis.h"
#include "CYAxis.h"

#include "IMDRTDBManagerUI.h"
#include "CTranslator.h"
#include "CFormToolFunc.h"
#include "../../MDDBQueryUI/MDDBQueryUI.h"

#include <QColorDialog>
#include <QRegExp>
#include <QRegExpValidator>

#include <QDebug>

using namespace Translator;
using namespace FormToolFunc;

CCurvePropertySettingDialog::CCurvePropertySettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CCurvePropertySettingDialog),
    m_curve(0),
    m_isRealTime(0),
    m_upWarningLine(0),
    m_belowWarningLine(0),
    m_normalPoint(0),
    m_warningPoint(0),
    m_variantForm(0),
    m_curveLineStyle(0),
    m_upWarningLineStyle(0),
    m_belowWarningLineStyle(0),
    m_translateTool(0)
{
    ui->setupUi(this);
    installDialogTranslator(m_translateTool,this);
}

CCurvePropertySettingDialog::~CCurvePropertySettingDialog()
{
    delete ui;
}

CCurveObject *CCurvePropertySettingDialog::getRealCurve(bool *ok, CCurveObject *initial,
                                                        const QList<CXAxis *> &xList, const QList<CYAxis *> &yList,
                                                        IMDRTDBManagerUI *variantForm, QWidget *parent)
{
    if(initial)
    {
        CCurvePropertySettingDialog dlg(parent);
        dlg.setIsRealTime(true);
        dlg.setData(initial,xList,yList);
        dlg.setVariantForm(variantForm);
        dlg.setWindowTitle(QString("%1-%2").arg(initial->name(),initial->comment()));
        int ret = dlg.exec();
        if(ok) {
            *ok = !!ret;
            return initial;
        }
    }
    return NULL;
}

CCurveObject *CCurvePropertySettingDialog::getHistoryCurve(bool *ok, CCurveObject *initial,
                                                           const QList<CXAxis *> &xList, const QList<CYAxis *> &yList,
                                                           const QStringList &fields, QWidget *parent)
{
    if(initial)
    {
        CCurvePropertySettingDialog dlg(parent);
        dlg.setIsRealTime(false);
        dlg.setData(initial,xList,yList);
        dlg.setFields(fields);
        dlg.setWindowTitle(QString("%1-%2").arg(initial->name(),initial->comment()));
        int ret = dlg.exec();
        if(ok)
        {
            *ok = !!ret;
            return initial;
        }
    }
    return NULL;
}

void CCurvePropertySettingDialog::setData(CCurveObject *curve, const QList<CXAxis *> &xList, const QList<CYAxis *> &yList)
{
    if(curve && !xList.isEmpty() && !yList.isEmpty())
    {
        m_curve = curve;
        for(int i =0; i< xList.count(); i++)
        {
            if(xList.at(i)->valueType() == CXAxis::ValueType::Time)
            {
                m_xList.append(xList.at(i));
            }
        }
//        m_xList = xList;
        m_yList = yList;
        initialize();
    }
}


void CCurvePropertySettingDialog::setVariantForm(IMDRTDBManagerUI *variantForm)
{
    m_variantForm = variantForm;
}

void CCurvePropertySettingDialog::setFields(const QStringList &fields)
{
    m_fields = fields;
}

void CCurvePropertySettingDialog::setIsRealTime(bool isRealTime)
{
    m_isRealTime = isRealTime;
}

void CCurvePropertySettingDialog::applyChanges()
{
    applyCurveChanges();
    applyUpWarningLineChanges();
    applyBelowWarningLineChanges();
    applyNormalPointChanges();
    applyWarningPointChanges();
}

void CCurvePropertySettingDialog::applyCurveChanges()
{
    if(m_curve && !m_xList.isEmpty() && !m_yList.isEmpty())
    {
        m_curve->setCurveVisible(ui->curveVisibleCheckBox->isChecked());
        m_curve->setCurveVernierVisible(ui->verinerShowInforCheckBox->isChecked());
        QString comment = ui->commentLineEdit->text().trimmed();
        m_curve->setComment(comment);
        m_curve->setVariantDepict(ui->variantButton->text());
        m_curve->setTimeVariantDepict(ui->timeVariantButton->text());
        m_curve->setViewName(ui->viewButton->text());
        m_curve->setCurveWidth(ui->curveWidthSpinBox->value());
        m_curve->setCurveColor(m_curveColor);
        m_curve->setCurveStyle((Qt::PenStyle)m_curveLineStyle);
        m_curve->setXName(m_xList.at(ui->xAxisComboBox->currentIndex())->name());
        m_curve->setYName(m_yList.at(ui->yAxisComboBox->currentIndex())->name());
    }
}

void CCurvePropertySettingDialog::applyUpWarningLineChanges()
{
    if(m_upWarningLine)
    {
        m_upWarningLine->setVisible(ui->upWarningLineVisibleCheckBox->isChecked());
        m_upWarningLine->setValue(ui->upValueLineEdit->text().toDouble());
        m_upWarningLine->setLineWidth(ui->upWarningLineWidthSpinBox->value());
        m_upWarningLine->setLineColor(m_upWarningLineColor);
        m_upWarningLine->setLineStyle((Qt::PenStyle)m_upWarningLineStyle);
    }
}

void CCurvePropertySettingDialog::applyBelowWarningLineChanges()
{
    if(m_belowWarningLine)
    {
        m_belowWarningLine->setVisible(ui->belowWarningLineVisibleCheckBox->isChecked());
        m_belowWarningLine->setValue(ui->belowWarningValueLineEdit->text().toDouble());
        m_belowWarningLine->setLineWidth(ui->belowWarningLineWidthSpinBox->value());
        m_belowWarningLine->setLineColor(m_belowWarningLineColor);
        m_belowWarningLine->setLineStyle((Qt::PenStyle)m_belowWarningLineStyle);
    }
}

void CCurvePropertySettingDialog::applyNormalPointChanges()
{
    if(m_normalPoint)
    {
        m_normalPoint->setVisible(ui->normalPointVisibleCheckBox->isChecked());
        m_normalPoint->setColor(m_normalPointColor);
        m_normalPoint->setPointStyle((CPoint::PointStyle)ui->normalPointStyleComboBox->currentIndex());
        m_normalPoint->setSize(ui->normalPointSizeSpinBox->value());
    }
}

void CCurvePropertySettingDialog::applyWarningPointChanges()
{
    if(m_warningPoint)
    {
        m_warningPoint->setVisible(ui->wariningPointVisibleCheckBox->isChecked());
        m_warningPoint->setColor(m_warningPointColor);
        m_warningPoint->setPointStyle((CPoint::PointStyle)ui->warningPointStyleComboBox->currentIndex());
        m_warningPoint->setSize(ui->normalPointSizeSpinBox->value());
    }
}

void CCurvePropertySettingDialog::initialize()
{
    ui->listWidget->setCurrentRow(0);
    ui->viewLabel->setVisible(!m_isRealTime);
    ui->viewButton->setVisible(!m_isRealTime);
    ui->timeLabel->setVisible(!m_isRealTime);
    ui->timeVariantButton->setVisible(!m_isRealTime);
    if(m_isRealTime)
        ui->valueLabel->setText("变量");
    else
        ui->valueLabel->setText("数值变量");
    createUIConnections();
    initializeData();
    initializeUI();
    createConnections();
}

void CCurvePropertySettingDialog::initializeData()
{
    if(m_curve)
    {
        m_curveColor = m_curve->curveColor();
        m_curveLineStyle = m_curve->curveStyle();

        m_upWarningLine = m_curve->upWarningLine();
        if(m_upWarningLine)
        {
            m_upWarningLineColor = m_upWarningLine->lineColor();
            m_upWarningLineStyle = m_upWarningLine->lineStyle();
        }
        m_belowWarningLine = m_curve->belowWarningLine();
        if(m_belowWarningLine)
        {
            m_belowWarningLineColor = m_belowWarningLine->lineColor();
            m_belowWarningLineStyle = m_belowWarningLine->lineStyle();
        }
        m_normalPoint = m_curve->normalPoint();
        if(m_normalPoint)
        {
            m_normalPointColor = m_normalPoint->color();
        }
        m_warningPoint = m_curve->warningPoint();
        if(m_warningPoint)
        {
            m_warningPointColor = m_warningPoint->color();
        }
    }
}

void CCurvePropertySettingDialog::initializeUI()
{
    initializeCurve();
    initializeUpWarningLine();
    initializeBelowWarningLine();
    initializeNormalPoint();
    initializeWarningPoint();
}

void CCurvePropertySettingDialog::initializeCurve()
{
    setLineStyle(ui->curveStyleComboBox);
    initializeAxisComboBox(m_xList,ui->xAxisComboBox);
    initializeAxisComboBox(m_yList,ui->yAxisComboBox);
    ui->commentLineEdit->setMaxLength(100);
    ui->nameLineEdit->setMaxLength(100);
    if(m_curve)
    {
        setButtonColor(m_curveColor,*ui->curveColorButton);
        ui->nameLineEdit->setText(m_curve->name());
        ui->commentLineEdit->setText(m_curve->comment());
        ui->variantButton->setText(m_curve->variantDepict());
        ui->viewButton->setText(m_curve->viewName());
        ui->timeVariantButton->setText(m_curve->timeVariantDepict());
        ui->curveWidthSpinBox->setValue(m_curve->curveWidth());
        ui->curveVisibleCheckBox->setChecked(m_curve->curveVisible());
        ui->verinerShowInforCheckBox->setChecked(m_curve->curveVernierVisible());
        ui->curveStyleComboBox->setCurrentIndex(m_curveLineStyle -1);

        if(!m_curve->xName().isNull() && !m_curve->yName().isNull())
        {
            for(int i = 0;i<m_xList.count();i++)
            {
                if(m_curve->xName() == m_xList.at(i)->name())
                {
                    ui->xAxisComboBox->setCurrentIndex(i);
                }
            }
            for(int i = 0;i<m_yList.count();i++)
            {
                if(m_curve->yName() == m_yList.at(i)->name())
                {
                    ui->yAxisComboBox->setCurrentIndex(i);
                }
            }
//            ui->xAxisComboBox->setCurrentText(m_curve->xName());
//            ui->yAxisComboBox->setCurrentText(m_curve->yName());
        }
    }
}

void CCurvePropertySettingDialog::initializeUpWarningLine()
{
    setLineStyle(ui->upWarningLineStyleComboBox);

    if(m_curve)
    {
        CWarningLine *upWarningLine = m_curve->upWarningLine();
        if(upWarningLine)
        {
            setButtonColor(m_upWarningLineColor,*ui->upWarningLineColorButton);
            ui->upWarningLineVisibleCheckBox->setChecked(upWarningLine->visible());
            ui->upValueLineEdit->setText(QString::number(upWarningLine->value()));
            ui->upWarningLineStyleComboBox->setCurrentIndex(m_upWarningLineStyle -1);
            ui->upWarningLineWidthSpinBox->setValue(upWarningLine->lineWidth());
        }
        slotSetUpWarningLineEnable(ui->upWarningLineVisibleCheckBox->isChecked());

        QRegExp rx("^[0-9]*.?[0-9]*$");
        QRegExpValidator *pReg = new QRegExpValidator(rx, this);
        ui->upValueLineEdit->setValidator(pReg);
    }
}

void CCurvePropertySettingDialog::initializeBelowWarningLine()
{
    setLineStyle(ui->belowWarningLineStyleComboBox);

    QRegExp rx("^[0-9]*.?[0-9]*$");
    QRegExpValidator *pReg = new QRegExpValidator(rx, this);
    ui->belowWarningValueLineEdit->setValidator(pReg);

    if(m_curve)
    {
        CWarningLine *belowWarningLine = m_curve->belowWarningLine();
        if(belowWarningLine)
        {
            setButtonColor(m_belowWarningLineColor,*ui->belowWarningLineColorButton);

            ui->belowWarningLineVisibleCheckBox->setChecked(belowWarningLine->visible());
            ui->belowWarningValueLineEdit->setText(QString::number(belowWarningLine->value()));
            ui->belowWarningLineStyleComboBox->setCurrentIndex(m_belowWarningLineStyle -1);
            ui->belowWarningLineWidthSpinBox->setValue(belowWarningLine->lineWidth());
        }

        slotSetBelowWarningLineEnable(ui->belowWarningLineVisibleCheckBox->isChecked());
    }
}

void CCurvePropertySettingDialog::initializeNormalPoint()
{
    setPointStyle(ui->normalPointStyleComboBox);

    if(m_curve)
    {
        CPoint *point = m_curve->normalPoint();
        if(point)
        {
            setButtonColor(m_normalPointColor,*ui->normalPointColorButton);
            ui->normalPointVisibleCheckBox->setChecked(point->visible());
            ui->normalPointSizeSpinBox->setValue(point->size());
            ui->normalPointStyleComboBox->setCurrentIndex((int)point->pointStyle());
        }
        slotSetNormalPointEnable(ui->normalPointVisibleCheckBox->isChecked());
    }
}

void CCurvePropertySettingDialog::initializeWarningPoint()
{
    setPointStyle(ui->warningPointStyleComboBox);

    if(m_curve)
    {
        CPoint *point = m_curve->warningPoint();
        if(point)
        {
            setButtonColor(m_warningPointColor,*ui->warningPointColorButton);
            ui->wariningPointVisibleCheckBox->setChecked(point->visible());
            ui->warningPointSizeSpinBox->setValue(point->size());
            ui->warningPointStyleComboBox->setCurrentIndex((int)point->pointStyle());
        }
        slotSetWarningPointEnable(ui->wariningPointVisibleCheckBox->isChecked());
    }
}

void CCurvePropertySettingDialog::setLineStyle(QComboBox *comboBox)
{
    comboBox->clear();
    comboBox->setToolTip(tr("Line style"));
    comboBox->setEditable(false);
    comboBox->setIconSize(QSize(92,14));
    comboBox->setMinimumWidth(92);

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

void CCurvePropertySettingDialog::setPointStyle(QComboBox *comboBox)
{
    comboBox->clear();
    comboBox->setToolTip(tr("Point Style"));
    comboBox->setEditable(false);
    comboBox->setIconSize(QSize(20,14));
    comboBox->setMinimumWidth(20);

    comboBox->addItem(QIcon(":/icon/icon/MDCurveCircle.png"),"");
    comboBox->addItem(QIcon(":/icon/icon/MDCurveSquare.png"),"");
    comboBox->addItem(QIcon(":/icon/icon/MDCurveTriangle.png"),"");
    comboBox->addItem(QIcon(":/icon/icon/MDCurveCross.png"),"");
}

void CCurvePropertySettingDialog::initializeAxisComboBox(QList<CXAxis *> axisList, QComboBox *comboBox)
{
    if(!axisList.isEmpty() && comboBox)
    {
        comboBox->clear();
        for(int i  = 0; i < axisList.count();i++)
        {
            CXAxis *axis = axisList.at(i);
            comboBox->addItem(/*axis->name()*/QString("%1--%2").arg(axis->name())
                              .arg(axis->comment()));
        }
    }
}
void CCurvePropertySettingDialog::initializeAxisComboBox(QList<CYAxis *> axisList, QComboBox *comboBox)
{
    if(!axisList.isEmpty() && comboBox)
    {
        comboBox->clear();
        for(int i  = 0; i < axisList.count();i++)
        {
            CYAxis *axis = axisList.at(i);
            comboBox->addItem(/*axis->name()*/QString("%1--%2").arg(axis->name())
                              .arg(axis->comment()));
        }
    }
}

void CCurvePropertySettingDialog::createConnections()
{
    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(slotSetCurveAndWarning(int)));
    connect(ui->okButton,&QPushButton::clicked,this,&CCurvePropertySettingDialog::slotOkButtonClicked);
    connect(ui->cancelButton,&QPushButton::clicked,this,&CCurvePropertySettingDialog::slotCancelButtonClicked);
    connect(ui->curveColorButton,&QPushButton::clicked,this,&CCurvePropertySettingDialog::slotSetCurveColor);
    connect(ui->upWarningLineColorButton,&QPushButton::clicked,this,&CCurvePropertySettingDialog::slotSetUpWarningLineColor);
    connect(ui->belowWarningLineColorButton,&QPushButton::clicked,this,&CCurvePropertySettingDialog::slotSetBelowWarningLineColor);
    connect(ui->normalPointColorButton,&QPushButton::clicked,this,&CCurvePropertySettingDialog::slotSetNormalPointColor);
    connect(ui->warningPointColorButton,&QPushButton::clicked,this,&CCurvePropertySettingDialog::slotSetWarningPointColor);
    connect(ui->variantButton,&QPushButton::clicked,this,&CCurvePropertySettingDialog::slotSetVarinatName);

    connect(ui->curveStyleComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetCurveLineStyle(int)));
    connect(ui->upWarningLineStyleComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetUpWarningLineStyle(int)));
    connect(ui->belowWarningLineStyleComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetBelowWarningLineStyle(int)));
    connect(ui->viewButton,&QPushButton::clicked,this,&CCurvePropertySettingDialog::slotSetViewName);
    connect(ui->timeVariantButton,&QPushButton::clicked,this,&CCurvePropertySettingDialog::slotSetTimeVariantName);

}

void CCurvePropertySettingDialog::createUIConnections()
{
    connect(ui->upWarningLineVisibleCheckBox,&QCheckBox::clicked,this,&CCurvePropertySettingDialog::slotSetUpWarningLineEnable);
    connect(ui->belowWarningLineVisibleCheckBox,&QCheckBox::clicked,this,&CCurvePropertySettingDialog::slotSetBelowWarningLineEnable);
    connect(ui->normalPointVisibleCheckBox,&QCheckBox::clicked,this,&CCurvePropertySettingDialog::slotSetNormalPointEnable);
    connect(ui->wariningPointVisibleCheckBox,&QCheckBox::clicked,this,&CCurvePropertySettingDialog::slotSetWarningPointEnable);
}

void CCurvePropertySettingDialog::slotSetCurveAndWarning(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}

void CCurvePropertySettingDialog::slotSetVarinatName()
{
    QString variantName = m_curve->variantDepict();
    if(m_isRealTime)
    {
        if(m_variantForm)
        {
            QStringList nameList;
            m_variantForm->selectVariables(nameList);
            if(!nameList.isEmpty())
                variantName = nameList.first();
        }
    }
    else
    {
//        bool ok = false;
//        QStringList name = CVariantDialog::getField(&ok,variantName,m_fields,this);
//        if(ok)
//        {
//            variantName = name.count() > 0 ? name.first() : variantName;
//        }
        if(m_curve->viewName().isEmpty())
        {
            showMessage(tr("请先选择视图！"),this);
            return;
        }
        QList<DBQ::ViewField *>viewFiled;
        viewFiled = DBQUI::GetFieldsFromViewSelected(m_curve->projectPath()+"/DBQuery",m_curve->viewName(),DBQUI::VIEW_FIELD_RETURN_TYPE_VARIABLE);
        variantName = viewFiled.count() > 0 ? viewFiled.first()->fieldName : variantName.remove(m_curve->viewName()+".");
    }
    ui->variantButton->setText(variantName);
}

void CCurvePropertySettingDialog::slotSetTimeVariantName()
{
    QString timeVariantName = m_curve->timeVariantDepict();
    if(!m_isRealTime)
    {
        QList<DBQ::ViewField *>viewFiled;
        if(m_curve->viewName().isEmpty())
        {
            showMessage(tr("请先选择视图！"),this);
            return;
        }
        viewFiled = DBQUI::GetFieldsFromViewSelected(m_curve->projectPath()+"/DBQuery",m_curve->viewName(),DBQUI::VIEW_FIELD_RETURN_TYPE_DATETIME);
        if(viewFiled.count() > 0)
        {
            timeVariantName = viewFiled.first()->fieldName;
        }
    }
    ui->timeVariantButton->setText(timeVariantName);
}

void CCurvePropertySettingDialog::slotOkButtonClicked()
{
    applyChanges();
    QString str = ui->nameLineEdit->text();
    if(str.trimmed().isEmpty())
    {
        showMessage(tr("曲线名称不能为空,请重新设置"),this);
    }
    else
    {
        m_curve->setName(ui->nameLineEdit->text());
        accept();
    }
}

void CCurvePropertySettingDialog::slotCancelButtonClicked()
{
    reject();
}

void CCurvePropertySettingDialog::slotSetCurveColor()
{
    setColor(m_curveColor,*ui->curveColorButton);
}

void CCurvePropertySettingDialog::slotSetUpWarningLineColor()
{
    setColor(m_upWarningLineColor,*ui->upWarningLineColorButton);
}

void CCurvePropertySettingDialog::slotSetBelowWarningLineColor()
{
    setColor(m_belowWarningLineColor,*ui->belowWarningLineColorButton);
}

void CCurvePropertySettingDialog::slotSetNormalPointColor()
{
    setColor(m_normalPointColor,*ui->normalPointColorButton);
}

void CCurvePropertySettingDialog::slotSetWarningPointColor()
{
    setColor(m_warningPointColor,*ui->warningPointColorButton);
}

void CCurvePropertySettingDialog::slotSetUpWarningLineEnable(bool enable)
{
    ui->upValueLineEdit->setEnabled(enable);
    ui->upWarningLineColorButton->setEnabled(enable);
    ui->upWarningLineStyleComboBox->setEnabled(enable);
    ui->upWarningLineWidthSpinBox->setEnabled(enable);
}

void CCurvePropertySettingDialog::slotSetBelowWarningLineEnable(bool enable)
{
    ui->belowWarningValueLineEdit->setEnabled(enable);
    ui->belowWarningLineColorButton->setEnabled(enable);
    ui->belowWarningLineStyleComboBox->setEnabled(enable);
    ui->belowWarningLineWidthSpinBox->setEnabled(enable);
}

void CCurvePropertySettingDialog::slotSetNormalPointEnable(bool enable)
{
    ui->normalPointStyleComboBox->setEnabled(enable);
    ui->normalPointSizeSpinBox->setEnabled(enable);
    ui->normalPointColorButton->setEnabled(enable);
}

void CCurvePropertySettingDialog::slotSetWarningPointEnable(bool enable)
{
    ui->warningPointStyleComboBox->setEnabled(enable);
    ui->warningPointSizeSpinBox->setEnabled(enable);
    ui->warningPointColorButton->setEnabled(enable);
}

void CCurvePropertySettingDialog::slotSetCurveLineStyle(int index)
{
    m_curveLineStyle = index +1;
}

void CCurvePropertySettingDialog::slotSetUpWarningLineStyle(int index)
{
    m_upWarningLineStyle = index +1;
}

void CCurvePropertySettingDialog::slotSetBelowWarningLineStyle(int index)
{
    m_belowWarningLineStyle = index +1;
}

void CCurvePropertySettingDialog::slotSetViewName()
{
    QString viewName = m_curve->viewName();
    QStringList viewNameList;
    DBQUI::VIEW_SELECTION_MODE mode = DBQUI::VIEW_SINGLE_SELECTION;
    viewNameList = DBQUI::GetViewNameSelected(m_curve->projectPath()+"/DBQuery",mode);
    viewName = viewNameList.count() > 0 ? viewNameList.first() : viewName;
    m_curve->setViewName(viewName);
    ui->viewButton->setText(viewName);
}

void CCurvePropertySettingDialog::setColor(QColor &initialColor, QPushButton &colorButton)
{
    selectColor(initialColor,colorButton,this);
}
