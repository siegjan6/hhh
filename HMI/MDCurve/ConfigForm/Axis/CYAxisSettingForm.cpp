#include "CYAxisSettingForm.h"
#include "ui_CYAxisSettingForm.h"
#include "CBisectrixLineSettingDialog.h"
#include "CCurveSettingForm.h"
#include "CReferenceCurveSettingForm.h"
#include "CCurveObject.h"
#include "CCurveObjectManager.h"
#include "CReferenceCurve.h"
#include "CReferenceCurveManager.h"

#include "CYAxisManager.h"
#include "CYAxis.h"
#include "CGridRegion.h"

#include "CYPropertySettingDialog.h"
#include <QDebug>

#include "CFormToolFunc.h"
using namespace FormToolFunc;

CYAxisSettingForm::CYAxisSettingForm(QWidget *parent) :
    QWidget(parent),
    m_yAxisManager(0),
    m_gridRegion(0),
    m_curveForm(0),
    m_reCurveForm(0),
    m_isModified(false),
    ui(new Ui::CYAxisSettingForm),
    m_autoHideAxis(false),
    m_preMainlBisectrixLine(0),
    m_preSecontraryBisectrixLine(0),
    m_mainBisectrixLine(new CBisectrixLine),
    m_secontraryBisectrixLine(new CBisectrixLine)
{
    ui->setupUi(this);
}

CYAxisSettingForm::~CYAxisSettingForm()
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

void CYAxisSettingForm::setData(CYAxisManager *yAxisManager, CGridRegion *gridRegion)
{
    if(yAxisManager && gridRegion)
    {
        m_yAxisManager = yAxisManager;
        m_gridRegion = gridRegion;
        initialize();
    }
}
CCurveSettingForm *CYAxisSettingForm::curveForm() const
{
    return m_curveForm;
}

void CYAxisSettingForm::setCurveForm(CCurveSettingForm *curveForm)
{
    m_curveForm = curveForm;
}

CReferenceCurveSettingForm *CYAxisSettingForm::referenceForm() const
{
    return m_reCurveForm;
}

void CYAxisSettingForm::setReferenceForm(CReferenceCurveSettingForm *reCurveForm)
{
    m_reCurveForm = reCurveForm;
}

QList<CYAxis *> CYAxisSettingForm::list() const
{
    return m_list;
}

void CYAxisSettingForm::setList(const QList<CYAxis *> &list)
{
    m_list = list;
}
/*!
\section
*/


void CYAxisSettingForm::applyChanges()
{
    if(ui->mainBisectrixLineEdit->text().isEmpty() || ui->mainBisectrixLineEdit->text().toInt() ==0)
        return;
    applyYAxisChanges();
    applyGridRegionChanges();
}

void CYAxisSettingForm::applyYAxisChanges()
{
    m_yAxisManager->setDivNum(ui->mainBisectrixLineEdit->text().toInt());
    m_yAxisManager->setList(m_list);
    m_yAxisManager->setScale(ui->scaleLineEdit->text().toInt());
    m_yAxisManager->setAutoHideCurve(m_autoHideAxis);

    for(int i = 0; i < m_list.count(); i++)
    {
        CYAxis *yAxis = new CYAxis;
        *yAxis = *m_list.at(i);
        m_preList.append(yAxis);
    }

    m_yBoundVisibleList.clear();

    for(int i = 0;i<m_list.count();i++)
    {
        int bound = 0;
        int curveBound = 0;
        int reCurveBound = 0;
        int curveVisible = 0;
        int reCurveVisible = 0;
        for(int j = 0; j<m_curveForm->m_list.count();j++)
        {
            if(m_curveForm->m_list.at(j)->yName() == m_list.at(i)->name())
            {
                bound++;
                curveBound++;
                if(m_curveForm->m_list.at(j)->curveVisible())
                    curveVisible++;
                for(int k =0; k< m_reCurveForm->m_reList.count(); k++)
                {
                    if(m_reCurveForm->m_reList.at(k)->yName() == m_list.at(i)->name())
                    {
                        bound++;
                        curveBound++;
                        if(m_reCurveForm->m_reList.at(k)->curveVisible())
                        {
                            curveVisible++;
                        }
                    }
                }
            }
        }
        for(int j = 0; j<m_reCurveForm->m_reList.count();j++)
        {
            if(m_reCurveForm->m_reList.at(j)->yName() == m_list.at(i)->name())
            {
                bound++;
                reCurveBound++;
                if(m_reCurveForm->m_reList.at(j)->curveVisible())
                {
                    reCurveVisible++;
                }

                for(int k = 0; k<m_curveForm->m_list.count(); k++)
                {
                    if(m_curveForm->m_list.at(k)->yName() == m_list.at(i)->name())
                    {
                        bound++;
                        reCurveBound++;
                        if(m_curveForm->m_list.at(k)->curveVisible())
                            reCurveVisible++;
                    }
                }
            }
        }
        if(!bound || (curveBound && !curveVisible) ||(reCurveBound && !reCurveVisible)) //曲线无绑定或绑定曲线不可见(true)
        {
           m_yBoundVisibleList.append(true);
        }
        else
            m_yBoundVisibleList.append(false);
    }

    m_yAxisManager->setYBoundVisible(m_yBoundVisibleList);
}

void CYAxisSettingForm::applyGridRegionChanges()
{
    m_gridRegion->setYMainBisectrixNum(ui->mainBisectrixLineEdit->text().toInt());
    m_gridRegion->setYSecondaryBisectrixNum(ui->secondaryBisectrixLineEdit->text().toInt());

    *m_preMainlBisectrixLine = *m_mainBisectrixLine;
    *m_preSecontraryBisectrixLine = *m_secontraryBisectrixLine;
}

void CYAxisSettingForm::keyPressEvent(QKeyEvent *event)
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

/*!
\section
*/
void CYAxisSettingForm::resetModified()
{
    m_isModified = false;
}
void CYAxisSettingForm::slotHandleModified()
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
void CYAxisSettingForm::initialize()
{
    initializeYAxis();
    initializeGridRegion();
    initializeUI();

    createConnections();
}

void CYAxisSettingForm::initializeUI()
{
    if(m_yAxisManager && m_gridRegion)
    {
        ui->scaleLabel->setVisible(false);
        ui->scaleLineEdit->setVisible(false);
        ui->scaleLineEdit->setText(QString::number(m_yAxisManager->scale()));
        ui->autoHideCheckBox->setChecked(m_autoHideAxis);

        ui->mainBisectrixLineEdit->setText(QString::number(m_gridRegion->yMainBisectrixNum()));
        ui->secondaryBisectrixLineEdit->setText(QString::number(m_gridRegion->ySecondaryBisectrixNum()));

        QIntValidator *bisectrixValidator = new QIntValidator(1,50,this);
        ui->mainBisectrixLineEdit->setValidator(bisectrixValidator);
        ui->secondaryBisectrixLineEdit->setValidator(bisectrixValidator);

        ui->scaleLineEdit->setValidator(new QDoubleValidator(0,1,2,this));
        createMenu();
    }
}

void CYAxisSettingForm::initializeYAxis()
{
    if(m_yAxisManager)
    {
//        m_list = m_yAxisManager->list();

        m_preList = m_yAxisManager->list();
        for(int i = 0; i < m_preList.count(); i++)
        {
            CYAxis *yAxis = new CYAxis;
            *yAxis = *m_preList.at(i);
            m_list.append(yAxis);
        }
        m_autoHideAxis = m_yAxisManager->autoHideCurve();

        ui->axisTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        parseAxisInformation();
        if(!m_list.isEmpty())
            ui->axisTableWidget->selectRow(0);
    }
}

/*!
\section
*/
void CYAxisSettingForm::createConnections()
{
    connect(ui->mainBisectrixButton,&QPushButton::clicked,this,&CYAxisSettingForm::slotSetMainBisectrix);
    connect(ui->secondaryBisectrixButton, &QPushButton::clicked,this,&CYAxisSettingForm::slotSetSecondaryBisectrix);
    connect(ui->addAxisButton,&QPushButton::clicked,this,&CYAxisSettingForm::slotAddAxis);
    connect(ui->deleteButton,&QPushButton::clicked,this,&CYAxisSettingForm::slotDeleteAxis);
    connect(ui->moveUpButton,&QPushButton::clicked,this,&CYAxisSettingForm::slotMoveUp);
    connect(ui->moveDownButton,&QPushButton::clicked,this,&CYAxisSettingForm::slotMoveDown);

    connect(ui->propertyButton,&QPushButton::clicked,this,&CYAxisSettingForm::slotPropertyButtonClicked);
    connect(ui->axisTableWidget,&QTableWidget::itemDoubleClicked,this,&CYAxisSettingForm::slotSetProperty);
    connect(ui->autoHideCheckBox,&QCheckBox::clicked,this,&CYAxisSettingForm::slotSetNoneCurvesBoundVisible);

    connect(ui->scaleLineEdit,&QLineEdit::textEdited,this,&CYAxisSettingForm::slotHandleModified);
    connect(ui->mainBisectrixLineEdit,&QLineEdit::textEdited,this,&CYAxisSettingForm::slotHandleModified);
    connect(ui->secondaryBisectrixLineEdit,&QLineEdit::textEdited,this,&CYAxisSettingForm::slotHandleModified);
    connect(ui->autoHideCheckBox,&QCheckBox::clicked,this,&CYAxisSettingForm::slotHandleModified);
}
void CYAxisSettingForm::initializeGridRegion()
{
    if(m_gridRegion)
    {
        m_preMainlBisectrixLine = m_gridRegion->yMainBisectrixLine();
        m_preSecontraryBisectrixLine = m_gridRegion->ySecondaryBisectrixLine();

        *m_mainBisectrixLine = *m_preMainlBisectrixLine;
        *m_secontraryBisectrixLine = *m_preSecontraryBisectrixLine;
    }
}


void CYAxisSettingForm::slotSetMainBisectrix()
{
    bool ok = false;

    CBisectrixLine *mainBisectrix = CBisectrixLineSettingDialog::getLine(&ok,m_mainBisectrixLine,this);
    if(mainBisectrix)
    {
        if(ok)
        {
            slotHandleModified();
        } else {
            // To do
        }
    }
}

void CYAxisSettingForm::slotSetSecondaryBisectrix()
{
    bool ok = false;
    CBisectrixLine *mainBisectrix = CBisectrixLineSettingDialog::getLine(&ok,m_secontraryBisectrixLine,this);
    if(mainBisectrix)
    {
        if(ok)
        {
            slotHandleModified();
        } else {
            // To do
        }
    }
}

void CYAxisSettingForm::slotAddAxis()
{
    CYAxis *newAxis = new CYAxis;
    QStringList nameList;
    for(int i = 0; i < m_list.count();i++)
        nameList.append(m_list.at(i)->name());
    newAxis->setName(createName(nameList));

    bool ok = false;

    if(CYPropertySettingDialog::getYAxis(&ok,newAxis,this) && ok) {
        m_list.append(newAxis);
        parseAxisInformation();
        updateCurveAxis();
        slotHandleModified();
    } else {
        delete newAxis;
        newAxis = NULL;
    }
}

void CYAxisSettingForm::slotDeleteAxis()
{
    if(!ui->axisTableWidget->selectedItems().isEmpty())
    {
        int rowCount = ui->axisTableWidget->currentRow();

        CYAxis *yAxisInitial = m_list.at(rowCount);
        if(m_list.count() == 1)
        {
            showMessage(tr("至少保留一条Y轴!"),this);
        }
        else
        {
            int num = 0;
            for(int i = 0; i<m_curveForm->m_list.count();i++)
            {
                if(m_curveForm->m_list.at(i)->yName() == m_list.at(rowCount)->name())
                    num++;
            }
            for(int i = 0; i<m_reCurveForm->m_reList.count();i++)
            {
                if(m_reCurveForm->m_reList.at(i)->yName() == m_list.at(rowCount)->name())
                    num++;
            }
            if(num)
            {
                showMessage(tr("至少有一条曲线绑定了Y轴!"),this);
            }
            else
            {
                int ret = showMessage(Translator::translatorString(tr("确定删除 "))
                                      +yAxisInitial->name(),this,
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
    } else {
        showMessage(tr("请选择一条轴!"),this);
    }
}

void CYAxisSettingForm::slotMoveUp()
{
    int currentIndex = ui->axisTableWidget->currentRow();
    if(currentIndex != -1)
    {
        if(currentIndex > 0)
        {
            int preIndex = currentIndex -1;

            QString currentName = ui->axisTableWidget->item(currentIndex,0)->text();
            QString currentComment = ui->axisTableWidget->item(currentIndex,1)->text();
            QString currentMin = ui->axisTableWidget->item(currentIndex,2)->text();
            QString currentMax = ui->axisTableWidget->item(currentIndex,3)->text();

            QString preName = ui->axisTableWidget->item(preIndex,0)->text();
            QString preComment = ui->axisTableWidget->item(preIndex,1)->text();
            QString preMin = ui->axisTableWidget->item(preIndex,2)->text();
            QString preMax = ui->axisTableWidget->item(preIndex,3)->text();

            ui->axisTableWidget->item(currentIndex,0)->setText(preName);
            ui->axisTableWidget->item(currentIndex,1)->setText(preComment);
            ui->axisTableWidget->item(currentIndex,2)->setText(preMin);
            ui->axisTableWidget->item(currentIndex,3)->setText(preMax);

            ui->axisTableWidget->item(preIndex,0)->setText(currentName);
            ui->axisTableWidget->item(preIndex,1)->setText(currentComment);
            ui->axisTableWidget->item(preIndex,2)->setText(currentMin);
            ui->axisTableWidget->item(preIndex,3)->setText(currentMax);

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

void CYAxisSettingForm::slotMoveDown()
{
    int maxIndex = ui->axisTableWidget->rowCount()-1;
    int currentIndex = ui->axisTableWidget->currentRow();

    if( currentIndex != -1)
    {
        if(currentIndex < maxIndex)
        {
            int preIndex = currentIndex + 1;

            QString currentName = ui->axisTableWidget->item(currentIndex,0)->text();
            QString currentComment = ui->axisTableWidget->item(currentIndex,1)->text();
            QString currentMin = ui->axisTableWidget->item(currentIndex,2)->text();
            QString currentMax = ui->axisTableWidget->item(currentIndex,3)->text();

            QString preName = ui->axisTableWidget->item(preIndex,0)->text();
            QString preComment = ui->axisTableWidget->item(preIndex,1)->text();
            QString preMin = ui->axisTableWidget->item(preIndex,2)->text();
            QString preMax = ui->axisTableWidget->item(preIndex,3)->text();

            ui->axisTableWidget->item(currentIndex,0)->setText(preName);
            ui->axisTableWidget->item(currentIndex,1)->setText(preComment);
            ui->axisTableWidget->item(currentIndex,2)->setText(preMin);
            ui->axisTableWidget->item(currentIndex,3)->setText(preMax);

            ui->axisTableWidget->item(preIndex,0)->setText(currentName);
            ui->axisTableWidget->item(preIndex,1)->setText(currentComment);
            ui->axisTableWidget->item(preIndex,2)->setText(currentMin);
            ui->axisTableWidget->item(preIndex,3)->setText(currentMax);

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

void CYAxisSettingForm::slotPropertyButtonClicked()
{

    QList<CYAxis*> list = m_list;
    if(!ui->axisTableWidget->selectedItems().isEmpty())
    {
        int rowCount = ui->axisTableWidget->currentRow();

        CYAxis *yAxisInitial = list.at(rowCount);
        setAxisProperty(yAxisInitial);
    } else {
        showMessage(tr("请选择一条轴!"),this);
    }
}

void CYAxisSettingForm::slotSetProperty(QTableWidgetItem *item)
{
    if(item)
    {
        int rowCount = ui->axisTableWidget->currentRow();

        CYAxis *yAxisInitial = m_list.at(rowCount);
        setAxisProperty(yAxisInitial);
    }
}

void CYAxisSettingForm::slotSetNoneCurvesBoundVisible(bool visible)
{
    m_autoHideAxis = visible;
}

void CYAxisSettingForm::slotCustomMenu(const QPoint &p)
{
    QPoint point =  mapToGlobal(p);
    m_menu->exec(point);
}

/*!
\section
*/
void CYAxisSettingForm::updateCurveAxis()
{
    if(m_curveForm)
        m_curveForm->setYList(m_list);
    if(m_reCurveForm)
        m_reCurveForm->setYList(m_list);
}

void CYAxisSettingForm::parseAxisInformation()
{
    if(!m_list.isEmpty())
    {
        int count  = m_list.count();
        ui->axisTableWidget->setRowCount(count);
        for(int i = 0; i < count;i++)
        {
            CYAxis *axis = m_list.at(i);

            QTableWidgetItem *nameItem = new QTableWidgetItem;
            nameItem->setTextAlignment(Qt::AlignCenter);
            nameItem->setText(axis->name());
            ui->axisTableWidget->setItem(i,0,nameItem);

            QTableWidgetItem *commentItem = new QTableWidgetItem;
            commentItem->setTextAlignment(Qt::AlignCenter);
            commentItem->setText(axis->comment());
            ui->axisTableWidget->setItem(i,1,commentItem);

            if(axis->isNumeric() == true)
            {
                QTableWidgetItem *minValueItem = new QTableWidgetItem;
                minValueItem->setTextAlignment(Qt::AlignCenter);
                minValueItem->setText(QString::number(axis->minValue()));
                ui->axisTableWidget->setItem(i,2,minValueItem);

                QTableWidgetItem *maxValueItem = new QTableWidgetItem;
                maxValueItem->setTextAlignment(Qt::AlignCenter);
                maxValueItem->setText(QString::number(axis->maxValue()));
                ui->axisTableWidget->setItem(i,3,maxValueItem);
            }
            else
            {
                QTableWidgetItem *minValueItem = new QTableWidgetItem;
                minValueItem->setTextAlignment(Qt::AlignCenter);
                minValueItem->setText(QString::number(axis->minPercent()));
                ui->axisTableWidget->setItem(i,2,minValueItem);

                QTableWidgetItem *maxValueItem = new QTableWidgetItem;
                maxValueItem->setTextAlignment(Qt::AlignCenter);
                maxValueItem->setText(QString::number(axis->maxPercent()));
                ui->axisTableWidget->setItem(i,3,maxValueItem);
            }

        }
    }
}

void CYAxisSettingForm::setAxisProperty(CYAxis *y)
{
    bool ok = false;

    CYAxis *yAxis = CYPropertySettingDialog::getYAxis(&ok,y,this);
    if(yAxis)
    {
        if(ok)
        {
            parseAxisInformation();
            slotHandleModified();
        } else {
            // To do
        }
    }
}

QString CYAxisSettingForm::createName(const QStringList &nameList)
{
    int count = nameList.count();
    for(int i= 0; i < count;i++)
    {
        QString name = QString("Y%1").arg(QString::number(i+1));
        if(!nameList.contains(name))
            return name;
    }
    return QString("Y%1").arg(QString::number(count+1));
}

void CYAxisSettingForm::createMenu()
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

    connect (addAction,&QAction::triggered,this,&CYAxisSettingForm::slotAddAxis);
    connect (deleteAction,&QAction::triggered,this,&CYAxisSettingForm::slotDeleteAxis);
    connect (upAction,&QAction::triggered,this,&CYAxisSettingForm::slotMoveUp);
    connect (downAction,&QAction::triggered,this,&CYAxisSettingForm::slotMoveDown);
    connect (propertyAction,&QAction::triggered,this,&CYAxisSettingForm::slotPropertyButtonClicked);
}
