#include "CXAxisSettingForm.h"
#include "ui_CXAxisSettingForm.h"
#include "CXPropertySettingDialog.h"
#include "CBisectrixLineSettingDialog.h"
#include "CCurveSettingForm.h"
#include "CReferenceCurveSettingForm.h"
#include "CCurveObjectManager.h"
#include "CCurveObject.h"
#include "CReferenceCurve.h"
#include "CReferenceCurveManager.h"
#include "CFeatureTagSettingForm.h"

#include "CXAxisManager.h"
#include "CXAxis.h"
#include "CGridRegion.h"
#include "CFormToolFunc.h"
#include "CTranslator.h"
using namespace Translator;
using namespace FormToolFunc;

CXAxisSettingForm::CXAxisSettingForm(QWidget *parent) :
    QWidget(parent),
    m_xAxisManager(0),
    m_gridRegion(0),
    m_curveForm(0),
    m_reCurveForm(0),
    m_isModified(false),
    ui(new Ui::CXAxisSettingForm),
    m_preMainlBisectrixLine(0),
    m_preSecontraryBisectrixLine(0),
    m_autoHideAxis(false),
    m_mainBisectrixLine(new CBisectrixLine),
    m_secontraryBisectrixLine(new CBisectrixLine)
{
    ui->setupUi(this);
}

CXAxisSettingForm::~CXAxisSettingForm()
{
    if(m_mainBisectrixLine)
    {
        delete m_mainBisectrixLine;
        m_mainBisectrixLine = NULL;
    }

    if(m_secontraryBisectrixLine)
    {
        delete m_secontraryBisectrixLine;
        m_secontraryBisectrixLine = NULL;
    }
    delete ui;
}

/*!
\section
*/

void CXAxisSettingForm::setData(CXAxisManager *xAxisManager, CGridRegion *gridRegion)
{
    if(xAxisManager && gridRegion)
    {
        m_xAxisManager = xAxisManager;
        m_gridRegion = gridRegion;
        initialize();
    }
}
CCurveSettingForm *CXAxisSettingForm::curveForm() const
{
    return m_curveForm;
}

void CXAxisSettingForm::setCurveForm(CCurveSettingForm *curveForm)
{
    m_curveForm = curveForm;
}

CReferenceCurveSettingForm *CXAxisSettingForm::referenceForm() const
{
    return m_reCurveForm;
}

void CXAxisSettingForm::setFeatureTagForm(CFeatureTagSettingForm *featureTagForm)
{
    m_featureTagForm = featureTagForm;
}

CFeatureTagSettingForm *CXAxisSettingForm::featureTagForm() const
{
    return m_featureTagForm;
}

void CXAxisSettingForm::setReferenceForm(CReferenceCurveSettingForm *reCurveForm)
{
    m_reCurveForm = reCurveForm;
}

QList<CXAxis *> CXAxisSettingForm::list() const
{
    return m_list;
}

void CXAxisSettingForm::setList(const QList<CXAxis *> &list)
{
    m_list = list;
}

/*!
\section
*/
void CXAxisSettingForm::applyChanges()
{
    if(ui->mainBisectrixLineEdit->text().isEmpty() || ui->mainBisectrixLineEdit->text().toInt() ==0)
        return;
    applyXAxisChanges();
    applyGridRegionChanges();
}

void CXAxisSettingForm::applyXAxisChanges()
{
    if(m_xAxisManager)
    {
        m_xAxisManager->setDivNum(ui->mainBisectrixLineEdit->text().toInt());
        m_xAxisManager->setList(m_list);
        m_xAxisManager->setAutoHideCurve(m_autoHideAxis);

        for(int i = 0;i<m_list.count();i++)
        {
            CXAxis *xAxis = new CXAxis;
            *xAxis = *m_list.at(i);
            m_xAxisPrelist.append(xAxis);
        }
    }

    m_xBoundVisibleList.clear();
    for(int i = 0;i<m_list.count();i++)
    {
        int bound = 0;
        int curveBound = 0;
        int curveVisible = 0;
        int reCurveBound = 0;
        int reCurveVisible = 0;
        for(int j = 0; j<m_curveForm->m_list.count();j++)
        {
            if(m_curveForm->m_list.at(j)->xName() == m_list.at(i)->name())
            {
                bound++;
                curveBound++;
                if(m_curveForm->m_list.at(j)->curveVisible())
                    curveVisible++;
                for(int k = 0; k< m_reCurveForm->m_reList.count();k++)
                {
                    if(m_reCurveForm->m_reList.at(k)->xName() == m_list.at(i)->name())
                    {
                        bound++;
                        curveBound++;
                        if(m_reCurveForm->m_reList.at(k)->curveVisible())
                            curveVisible++;
                    }

                }
            }
        }
        for(int j = 0; j<m_reCurveForm->m_reList.count();j++)
        {
            if(m_reCurveForm->m_reList.at(j)->xName() == m_list.at(i)->name())
            {
                bound++;
                reCurveBound++;
                if(m_reCurveForm->m_reList.at(j)->curveVisible())
                    reCurveVisible++;
                for(int k = 0; k <m_curveForm->m_list.count();k++)
                {
                    if(m_curveForm->m_list.at(k)->xName() == m_list.at(i)->name())
                    {
                        bound++;
                        reCurveBound++;
                        if(m_curveForm->m_list.at(k)->curveVisible())
                            reCurveVisible++;
                    }
                }
            }
        }
        if(!bound || (curveBound && !curveVisible) || (reCurveBound&& !reCurveVisible)) //曲线无绑定或绑定曲线不可见(true)
        {
           m_xBoundVisibleList.append(true);
        }
        else
            m_xBoundVisibleList.append(false);
    }

    m_xAxisManager->setXBoundVisible(m_xBoundVisibleList);
}

void CXAxisSettingForm::applyGridRegionChanges()
{
    if(m_gridRegion)
    {
        m_gridRegion->setXMainBisectrixNum(ui->mainBisectrixLineEdit->text().toInt());
        m_gridRegion->setXSecondaryBisectrixNum(ui->secondaryBisectrixLineEdit->text().toInt());

        *m_preMainlBisectrixLine = *m_mainBisectrixLine;
        *m_preSecontraryBisectrixLine = *m_secontraryBisectrixLine;
    }
}
/*!
\section
*/
void CXAxisSettingForm::resetModified()
{
    m_isModified = false;
}

void CXAxisSettingForm::slotHandleModified()
{
    if(!m_isModified)
    {
        m_isModified = true;
        emit itemModified();
    }
}

/*!
\section
*/
void CXAxisSettingForm::createConnections()
{
    connect(ui->mainBisectrixButton,&QPushButton::clicked,this,&CXAxisSettingForm::slotSetMainBisectrix);
    connect(ui->secondaryBisectrixButton, &QPushButton::clicked,this,&CXAxisSettingForm::slotSetSecondaryBisectrix);
    connect(ui->addAxisButton,&QPushButton::clicked,this,&CXAxisSettingForm::slotAddAxis);
    connect(ui->deleteButton,&QPushButton::clicked,this,&CXAxisSettingForm::slotDeleteAxis);
    connect(ui->propertyButton,&QPushButton::clicked,this,&CXAxisSettingForm::slotPropertyButtonClicked);
    connect(ui->axisTableWidget,&QTableWidget::itemDoubleClicked,this,&CXAxisSettingForm::slotSetProperty);
    connect(ui->autoHideCheckBox,&QCheckBox::clicked,this,&CXAxisSettingForm::slotSetNoneCurvesBoundVisible);
    connect(ui->moveUpButton,&QPushButton::clicked,this,&CXAxisSettingForm::slotMoveUpRow);
    connect(ui->moveDownButton,&QPushButton::clicked,this,&CXAxisSettingForm::slotMoveDownRow);

    connect(ui->mainBisectrixLineEdit,&QLineEdit::textEdited,this,&CXAxisSettingForm::slotHandleModified);
    connect(ui->secondaryBisectrixLineEdit,&QLineEdit::textEdited,this,&CXAxisSettingForm::slotHandleModified);
    connect(ui->autoHideCheckBox,&QCheckBox::clicked,this,&CXAxisSettingForm::slotHandleModified);
}

void CXAxisSettingForm::slotSetMainBisectrix()
{
    bool ok = false;

    CBisectrixLine *mainBisectrix = CBisectrixLineSettingDialog::getLine(&ok,m_mainBisectrixLine,this);
    if(mainBisectrix) {
        if(ok){
            slotHandleModified();
        } else {
            // To do
        }
    }
}

void CXAxisSettingForm::slotSetSecondaryBisectrix()
{
    bool ok = false;
    CBisectrixLine *mainBisectrix = CBisectrixLineSettingDialog::getLine(&ok,m_secontraryBisectrixLine,this);
    if(mainBisectrix){
        if(ok){
            slotHandleModified();
        } else {
            // To do
        }
    }
}
void CXAxisSettingForm::slotAddAxis()
{
    CXAxis *newAxis = new CXAxis;
    QStringList nameList;
    for(int i = 0; i < m_list.count();i++)
        nameList.append(m_list.at(i)->name());
    newAxis->setName(createName(nameList));

    bool ok = false;

    if(CXPropertySettingDialog::getXAxis(&ok,newAxis,this) && ok){
        m_list.append(newAxis);
        parseAxisInformation();
        updateCurveAxis();
        slotHandleModified();
    } else {
        delete newAxis;
        newAxis = NULL;
    }
}
void CXAxisSettingForm::slotDeleteAxis()
{
    if(!ui->axisTableWidget->selectedItems().isEmpty())
    {
        int rowCount = ui->axisTableWidget->currentRow();
        CXAxis *xAxisInitial = m_list.at(rowCount);
        if(m_list.count() == 1)
        {
            showMessage(tr("至少保留一条X轴!"),this);
        }

        else
        {
            int num = 0;
            for(int i = 0; i<m_curveForm->m_list.count();i++)
            {
                if(m_curveForm->m_list.at(i)->xName() == m_list.at(rowCount)->name())
                    num++;
            }
            for(int i = 0; i<m_reCurveForm->m_reList.count();i++)
            {
                if(m_reCurveForm->m_reList.at(i)->xName() == m_list.at(rowCount)->name())
                    num++;
            }
            if(num)
            {
                showMessage(tr("至少有一条曲线绑定了X轴!"),this);
            }
            else
            {
                int ret = showMessage(translatorString(tr("确定删除 "))
                                      +xAxisInitial->name(),this,
                                      QMessageBox::Ok|QMessageBox::Cancel);
                if(ret == QMessageBox::Ok)
                {
                    delete m_list.takeAt(rowCount);
                    updateCurveAxis();
                    parseAxisInformation();
                }
                else
                {
                    // To do
                }
            }
        }
        slotHandleModified();
    }
    else
    {
        showMessage(tr("请选择一条轴!"),this);
    }
}
void CXAxisSettingForm::slotPropertyButtonClicked()
{

    QList<CXAxis*> list = m_list;
    if(!ui->axisTableWidget->selectedItems().isEmpty())
    {
        int rowCount = ui->axisTableWidget->currentRow();

        CXAxis *xAxisInitial = list.at(rowCount);
        setAxisProperty(xAxisInitial);
    } else {
        showMessage(tr("请选择一条轴!"),this);
    }
}

void CXAxisSettingForm::slotSetProperty(QTableWidgetItem *item)
{
    int rowCount = item->row();

    CXAxis *xAxisInitial = m_list.at(rowCount);
    setAxisProperty(xAxisInitial);
}

void CXAxisSettingForm::slotSetNoneCurvesBoundVisible(bool boundVisible)
{
    m_autoHideAxis = boundVisible;
}

void CXAxisSettingForm::slotMoveUpRow()
{
    int currentIndex = ui->axisTableWidget->currentRow();
    if(currentIndex != -1) {
        if(currentIndex > 0){
            int preIndex = currentIndex -1;

            QString currentName = ui->axisTableWidget->item(currentIndex,0)->text();
            QString currentComment = ui->axisTableWidget->item(currentIndex,1)->text();
            QString currentType = ui->axisTableWidget->item(currentIndex,2)->text();

            QString preName = ui->axisTableWidget->item(preIndex,0)->text();
            QString preComment = ui->axisTableWidget->item(preIndex,1)->text();
            QString preType = ui->axisTableWidget->item(preIndex,2)->text();

            ui->axisTableWidget->item(currentIndex,0)->setText(preName);
            ui->axisTableWidget->item(currentIndex,1)->setText(preComment);
            ui->axisTableWidget->item(currentIndex,2)->setText(preType);

            ui->axisTableWidget->item(preIndex,0)->setText(currentName);
            ui->axisTableWidget->item(preIndex,1)->setText(currentComment);
            ui->axisTableWidget->item(preIndex,2)->setText(currentType);

            ui->axisTableWidget->selectRow(preIndex);

            m_list.swap(preIndex,currentIndex);
            slotHandleModified();
        }
    }
    else
    {
        showMessage(tr("请选择一条轴线"),this);
    }
}

void CXAxisSettingForm::slotMoveDownRow()
{
    int maxIndex = ui->axisTableWidget->rowCount()-1;
    int currentIndex = ui->axisTableWidget->currentRow();

    if( currentIndex != -1) {
        if(currentIndex < maxIndex){
            int preIndex = currentIndex + 1;

            QString currentName = ui->axisTableWidget->item(currentIndex,0)->text();
            QString currentComment = ui->axisTableWidget->item(currentIndex,1)->text();
            QString currentType = ui->axisTableWidget->item(currentIndex,2)->text();

            QString preName = ui->axisTableWidget->item(preIndex,0)->text();
            QString preComment = ui->axisTableWidget->item(preIndex,1)->text();
            QString preType = ui->axisTableWidget->item(preIndex,2)->text();

            ui->axisTableWidget->item(currentIndex,0)->setText(preName);
            ui->axisTableWidget->item(currentIndex,1)->setText(preComment);
            ui->axisTableWidget->item(currentIndex,2)->setText(preType);

            ui->axisTableWidget->item(preIndex,0)->setText(currentName);
            ui->axisTableWidget->item(preIndex,1)->setText(currentComment);
            ui->axisTableWidget->item(preIndex,2)->setText(currentType);

            ui->axisTableWidget->selectRow(preIndex);

            m_list.swap(preIndex,currentIndex);
            slotHandleModified();
        }
    }
    else
    {
        showMessage(tr("请选择一条轴线"),this);
    }
}

void CXAxisSettingForm::slotCustomMenu(const QPoint &p)
{
    QPoint point =  mapToGlobal(p);
    m_menu->exec(point);
}

/*!
\section
*/
void CXAxisSettingForm::initialize()
{
    initializeXAxis();
    initializeGridRegion();
    initializeUI();

    createConnections();
}

void CXAxisSettingForm::initializeUI()
{
    if(m_gridRegion)
    {
        ui->mainBisectrixLineEdit->setText(QString::number(m_gridRegion->xMainBisectrixNum()));
        ui->secondaryBisectrixLineEdit->setText(QString::number(m_gridRegion->xSecondaryBisectrixNum()));

        QIntValidator *bisectrixValidator = new QIntValidator(1,50,this);
        ui->mainBisectrixLineEdit->setValidator(bisectrixValidator);
        ui->secondaryBisectrixLineEdit->setValidator(bisectrixValidator);

        ui->autoHideCheckBox->setChecked(m_autoHideAxis);
        createMenu();
    }
}

void CXAxisSettingForm::initializeXAxis()
{
    if(m_xAxisManager)
    {
//        m_list = m_xAxisManager->list();
        m_xAxisPrelist = m_xAxisManager->list();
        for(int i = 0;i<m_xAxisPrelist.count();i++)
        {
            CXAxis *xAxis = new CXAxis;
            xAxis->setTotalLength(m_xAxisPrelist.at(i)->totalLength());
            *xAxis =  *m_xAxisPrelist.at(i);
            m_list.append(xAxis);
        }
        m_autoHideAxis = m_xAxisManager->autoHideCurve();

        ui->axisTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        parseAxisInformation();
        if(!m_list.isEmpty())
            ui->axisTableWidget->selectRow(0);
    }
}

void CXAxisSettingForm::initializeGridRegion()
{
    if(m_gridRegion)
    {
        m_preMainlBisectrixLine = m_gridRegion->xMainBisectrixLine();
        m_preSecontraryBisectrixLine = m_gridRegion->xSecondaryBisectrixLine();

        *m_mainBisectrixLine = *m_preMainlBisectrixLine;
        *m_secontraryBisectrixLine = *m_preSecontraryBisectrixLine;
    }
}

/*!
\section
*/

void CXAxisSettingForm::setAxisProperty(CXAxis *x)
{
    bool ok = false;

    CXAxis *xAxis = CXPropertySettingDialog::getXAxis(&ok,x,this);
    if(xAxis)
    {
        if(ok)
        {
            parseAxisInformation();
            slotHandleModified();
        }
        else
        {
            // To do
        }
    }
}

void CXAxisSettingForm::updateCurveAxis()
{
    if(m_curveForm)
        m_curveForm->setXList(m_list);
    if(m_reCurveForm)
        m_reCurveForm->setXList(m_list);
    if(m_featureTagForm)
        m_featureTagForm->setXList(m_list);
}
void CXAxisSettingForm::parseAxisInformation()
{
    int count  = m_list.count();
    ui->axisTableWidget->setRowCount(count);
    for(int i = 0; i < count;i++)
    {
        CXAxis *axis = m_list.at(i);

        QTableWidgetItem *nameItem = new QTableWidgetItem;
        nameItem->setTextAlignment(Qt::AlignCenter);
        nameItem->setText(axis->name());
        ui->axisTableWidget->setItem(i,0,nameItem);

        QTableWidgetItem *commentItem = new QTableWidgetItem;
        commentItem->setTextAlignment(Qt::AlignCenter);
        commentItem->setText(axis->comment());
        ui->axisTableWidget->setItem(i,1,commentItem);

        QTableWidgetItem *typeItem = new QTableWidgetItem;
        typeItem->setTextAlignment(Qt::AlignCenter);
        typeItem->setText(transEnumToString(axis->valueType()));
        ui->axisTableWidget->setItem(i,2,typeItem);

    }
}

void CXAxisSettingForm::createMenu()
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slotCustomMenu(QPoint)));// 响应右键菜单

    m_menu = new QMenu(ui->axisTableWidget);
    QAction* addAction = new QAction(tr("增加"), ui->axisTableWidget);
    m_menu->addAction(addAction);
    QAction* deleteAction = new QAction(tr("删除"), ui->axisTableWidget);
    m_menu->addAction(deleteAction);
    QAction *upAction = new QAction(tr("上移"),ui->axisTableWidget);
    m_menu->addAction(upAction);
    QAction *downAction = new QAction(tr("下移"),ui->axisTableWidget);
    m_menu->addAction(downAction);
    QAction *propertyAction = new QAction(tr("属性"),ui->axisTableWidget);
    m_menu->addAction(propertyAction);

    connect (addAction,&QAction::triggered,this,&CXAxisSettingForm::slotAddAxis);
    connect (deleteAction,&QAction::triggered,this,&CXAxisSettingForm::slotDeleteAxis);
    connect (upAction,&QAction::triggered,this,&CXAxisSettingForm::slotMoveUpRow);
    connect (downAction,&QAction::triggered,this,&CXAxisSettingForm::slotMoveDownRow);
    connect (propertyAction,&QAction::triggered,this,&CXAxisSettingForm::slotPropertyButtonClicked);
}

QString CXAxisSettingForm::createName(const QStringList &nameList)
{
    int count = nameList.count();
    for(int i= 0; i < count;i++)
    {
        QString name = QString("X%1").arg(QString::number(i+1));
        if(!nameList.contains(name))
            return name;
    }
    return QString("X%1").arg(QString::number(count+1));
}

/*!
\section
*/


QString CXAxisSettingForm::transEnumToString(CXAxis::ValueType valueType)
{
    switch (valueType) {
    case CXAxis::ValueType::Time:
        return QObject::tr("时间");
    case CXAxis::ValueType::Value:
        return QObject::tr("数值");
    default:
        return QString();
    }
}

void CXAxisSettingForm::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
    {
        slotDeleteAxis();
    }
    else if(event->key() == Qt::Key_Enter)
    {
        slotPropertyButtonClicked();
    }
}
