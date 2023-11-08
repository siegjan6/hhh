#include "CReferenceCurvePropertyDialog.h"
#include "ui_CReferenceCurvePropertyDialog.h"
#include "CReferenceCurve.h"
#include "CTranslator.h"
#include "CFormToolFunc.h"
#include "CPoint.h"

#include "CXAxis.h"
#include "CYAxis.h"
#include "CVariantDialog.h"
#include "IMDRTDBManagerUI.h"
#include <QPainter>
#include <QColorDialog>
#include <QDebug>

using namespace Translator;
using namespace FormToolFunc;
CReferenceCurvePropertyDialog::CReferenceCurvePropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CReferenceCurvePropertyDialog),
    m_reCurve(0),
    m_normalPoint(0),
    m_reCurveLineStyle(0),
    m_translateTool(0),
    m_variantForm(0),
    m_reCurveColor(Qt::black)
{
    ui->setupUi(this);
    installDialogTranslator(m_translateTool,this);
}

CReferenceCurvePropertyDialog::~CReferenceCurvePropertyDialog()
{
    delete ui;
}

CReferenceCurve *CReferenceCurvePropertyDialog::getReferenceCurve(bool *ok, CReferenceCurve *initial,IMDRTDBManagerUI *variantForm,const QList<CXAxis *> &xList,const QList<CYAxis *> &yList, QWidget *parent)
{
    if(initial)
    {
        CReferenceCurvePropertyDialog dlg(parent);
        dlg.setVariantForm(variantForm);
        if(variantForm)
            dlg.setIsRealTime(true);
        else
            dlg.setIsRealTime(false);
        dlg.setReCureColor(initial->reCurveColor());
        dlg.setData(initial,xList,yList);
        dlg.setWindowTitle(QString("%1-%2").arg(initial->name(),initial->comment()));

        int ret = dlg.exec();
        if(ok) {
            *ok = !!ret;
            return initial;
        }
    }
    return NULL;
}

void CReferenceCurvePropertyDialog::setData(CReferenceCurve *reCurve, const QList<CXAxis *> &xList, const QList<CYAxis *> &yList)
{
    if(reCurve && !xList.isEmpty() && !yList.isEmpty()){
        m_reCurve = reCurve;
        m_xList = xList;
        m_yList = yList;
        initialize();
    }
}

void CReferenceCurvePropertyDialog::setReCureColor(QColor c)
{
    m_reCurveColor = c;
}

void CReferenceCurvePropertyDialog::setVariantForm(IMDRTDBManagerUI *variantForm)
{
    m_variantForm = variantForm;
}

void CReferenceCurvePropertyDialog::setIsRealTime(bool isRealTime)
{
    m_isRealTime = isRealTime;
}

void CReferenceCurvePropertyDialog::applyChanges()
{
    applyReCurveChanges();
    applyNormalPointChanges();
}

void CReferenceCurvePropertyDialog::applyReCurveChanges()
{
    if(m_reCurve && !m_xList.isEmpty() && !m_yList.isEmpty())  {
        m_reCurve->setCurveVisible(ui->reCurveVisibleCheckBox->isChecked());
        m_reCurve->setReCurveVernierVisible(ui->showVernierCheckBox->isChecked());
        QString comment = ui->commentLineEdit->text().trimmed();
        m_reCurve->setComment(comment);
        m_reCurve->setCurveWidth(ui->reCurveWidthSpinBox->value());
        m_reCurve->setReCurveColor(m_reCurveColor);
        m_reCurve->setCurveStyle(m_reCurveLineStyle);
        m_reCurve->setXName(m_xList.at(ui->xAxisComboBox->currentIndex())->name());
        m_reCurve->setYName(m_yList.at(ui->yAxisComboBox->currentIndex())->name());
    }
}

void CReferenceCurvePropertyDialog::applyNormalPointChanges()
{
    if(m_normalPoint) {
        m_normalPoint->setVisible(ui->normalPointVisibleCheckBox->isChecked());
        m_normalPoint->setColor(m_normalPointColor);
        m_normalPoint->setPointStyle((CPoint::PointStyle)ui->normalPointStyleComboBox->currentIndex());
        m_normalPoint->setSize(ui->normalPointSizeSpinBox->value());
    }
}

void CReferenceCurvePropertyDialog::initialize()
{
    createUIConnections();
    initializeData();
    initializeUI();
    createConnections();
}

void CReferenceCurvePropertyDialog::initializeData()
{
    if(m_reCurve) {
        m_reCurveColor = m_reCurve->reCurveColor();

        m_reCurveLineStyle = m_reCurve->curveStyle();
        m_normalPoint = m_reCurve->normalPoint();
        if(m_normalPoint) {
            m_normalPointColor = m_normalPoint->color();
        }
    }
}

void CReferenceCurvePropertyDialog::initializeUI()
{
    initializeReCurve();
    initializeNormalPoint();
}

void CReferenceCurvePropertyDialog::initializeReCurve()
{
    if(!m_isRealTime)
    {
        ui->startValueLable->setVisible(false);
        ui->startLineEdit->setVisible(false);
        ui->startPushButton->setVisible(false);
    }
    setLineStyle(ui->reCurveStyleComboBox);
    ui->nameLineEdit->setMaxLength(100);
    ui->commentLineEdit->setMaxLength(100);
    initializeAxisComboBox(m_xList,ui->xAxisComboBox);
    initializeAxisComboBox(m_yList,ui->yAxisComboBox);
    if(m_reCurve)
    {
        setButtonColor(m_reCurveColor,*ui->reCurveColorButton);
        ui->nameLineEdit->setText(m_reCurve->name());
        ui->commentLineEdit->setText(m_reCurve->comment());
        ui->reCurveWidthSpinBox->setValue(m_reCurve->curveWidth());
        ui->reCurveVisibleCheckBox->setChecked(m_reCurve->curveVisible());
        ui->showVernierCheckBox->setChecked(m_reCurve->reCurveVernierVisible());
        ui->reCurveStyleComboBox->setCurrentIndex(m_reCurve->curveStyle() -1);
        ui->startLineEdit->setText(m_reCurve->variantDepict());

        if(!m_reCurve->xName().isNull() && !m_reCurve->yName().isNull())
        {
            for(int i = 0; i<m_xList.count();i++)
            {
                if(m_reCurve->xName() == m_xList.at(i)->name())
                {
                   ui->xAxisComboBox->setCurrentIndex(i);
                }
            }
            for(int i = 0; i<m_yList.count();i++)
            {
                if(m_reCurve->xName() == m_yList.at(i)->name())
                {
                   ui->yAxisComboBox->setCurrentIndex(i);
                }
            }
//            ui->xAxisComboBox->setCurrentText(m_reCurve->xName() +"--" + m_reCurve->xName());
//            ui->yAxisComboBox->setCurrentText(m_reCurve->yName() +"--" + m_reCurve->yName());
        }
    }
}

void CReferenceCurvePropertyDialog::initializeNormalPoint()
{
    setPointStyle(ui->normalPointStyleComboBox);

    if(m_reCurve)   {
        CPoint *point = m_reCurve->normalPoint();
        if(point) {
            setButtonColor(m_normalPointColor,*ui->normalPointColorButton);
            ui->normalPointVisibleCheckBox->setChecked(point->visible());
            ui->normalPointSizeSpinBox->setValue(point->size());
            ui->normalPointStyleComboBox->setCurrentIndex((int)point->pointStyle());
        }
        slotSetNormalPointEnable(ui->normalPointVisibleCheckBox->isChecked());
    }
}

void CReferenceCurvePropertyDialog::initializeAxisComboBox(QList<CXAxis *> axisList, QComboBox *comboBox)
{
    if(!axisList.isEmpty() && comboBox) {
        comboBox->clear();
        for(int i  = 0; i < axisList.count();i++){
            CXAxis *axis = axisList.at(i);
            comboBox->addItem(QString("%1--%2").arg(axis->name())
                              .arg(axis->comment()));
        }
    }
}

void CReferenceCurvePropertyDialog::initializeAxisComboBox(QList<CYAxis *> axisList, QComboBox *comboBox)
{
    if(!axisList.isEmpty() && comboBox) {
        comboBox->clear();
        for(int i  = 0; i < axisList.count();i++){
            CYAxis *axis = axisList.at(i);
            comboBox->addItem(QString("%1--%2").arg(axis->name())
                              .arg(axis->comment()));
        }
    }
}

void CReferenceCurvePropertyDialog::setLineStyle(QComboBox *comboBox)
{
    comboBox->clear();
    comboBox->setToolTip(tr("Line style"));
    comboBox->setEditable(false);
    comboBox->setIconSize(QSize(92,14));
    comboBox->setMinimumWidth(92);

    for (int i = (int)Qt::SolidLine; i < (int)Qt::CustomDashLine; i++){
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

void CReferenceCurvePropertyDialog::setPointStyle(QComboBox *comboBox)
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

void CReferenceCurvePropertyDialog::createConnections()
{

    connect(ui->okButton,&QPushButton::clicked,this,&CReferenceCurvePropertyDialog::slotOkButtonClicked);
    connect(ui->cancelButton,&QPushButton::clicked,this,&CReferenceCurvePropertyDialog::slotCancelButtonClicked);
    connect(ui->reCurveColorButton,&QPushButton::clicked,this,&CReferenceCurvePropertyDialog::slotSetReCurveColor);
    connect(ui->normalPointColorButton,&QPushButton::clicked,this,&CReferenceCurvePropertyDialog::slotSetNormalPointColor);
    connect(ui->reCurveStyleComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetReCurveLineStyle(int)));
    connect(ui->startPushButton,&QPushButton::clicked,this,&CReferenceCurvePropertyDialog::slotSetVarinatName);
}

void CReferenceCurvePropertyDialog::createUIConnections()
{
    connect(ui->normalPointVisibleCheckBox,&QCheckBox::clicked,this,&CReferenceCurvePropertyDialog::slotSetNormalPointEnable);

}

void CReferenceCurvePropertyDialog::slotOkButtonClicked()
{
    applyChanges();
    QString str = ui->nameLineEdit->text();
    if(str.trimmed().isEmpty())
    {
        showMessage(tr("曲线名称不能为空,请重新设置"),this);
    }
    else
    {
        m_reCurve->setName(ui->nameLineEdit->text());
        accept();
    }
}

void CReferenceCurvePropertyDialog::slotCancelButtonClicked()
{
    reject();
}

void CReferenceCurvePropertyDialog::slotSetReCurveColor()
{
    setColor(m_reCurveColor,*ui->reCurveColorButton);

}

void CReferenceCurvePropertyDialog::slotSetNormalPointColor()
{
    setColor(m_normalPointColor,*ui->normalPointColorButton);

}

void CReferenceCurvePropertyDialog::slotSetNormalPointEnable(bool enable)
{
    ui->normalPointStyleComboBox->setEnabled(enable);
    ui->normalPointSizeSpinBox->setEnabled(enable);
    ui->normalPointColorButton->setEnabled(enable);

}

void CReferenceCurvePropertyDialog::slotSetReCurveLineStyle(int index)
{
    m_reCurveLineStyle = index + 1;
}

void CReferenceCurvePropertyDialog::slotSetVarinatName()
{
    QString variantName = m_reCurve->variantDepict();
    if(m_variantForm){
        QStringList nameList;
        m_variantForm->selectVariables(nameList);
        if(!nameList.isEmpty())
            variantName = nameList.first();
    }
    m_reCurve->setVariantDepict(variantName);
    ui->startLineEdit->setText(variantName);

}

void CReferenceCurvePropertyDialog::setColor(QColor &initialColor, QPushButton &colorButton)
{
    selectColor(initialColor,colorButton,this);
}


