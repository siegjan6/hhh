#include "CReferenceCurveSettingForm.h"
#include "ui_CReferenceCurveSettingForm.h"
#include "CReferenceCurveManager.h"
#include "CReferenceCurve.h"
#include "CReferenceCurvePropertyDialog.h"
#include "CXAxisManager.h"
#include "CYAxisManager.h"
#include "CRealTimeTagManager.h"
#include "IMDRTDBManagerUI.h"
#include <QHeaderView>
#include "CFormToolFunc.h"
#include <QDebug>

using namespace FormToolFunc;

CReferenceCurveSettingForm::CReferenceCurveSettingForm(QWidget *parent) :
    QWidget(parent),
    m_reCurves(NULL),
    m_isModified(false),
    m_variantForm(NULL),
    ui(new Ui::CReferenceCurveSettingForm)
{
    ui->setupUi(this);
}

CReferenceCurveSettingForm::~CReferenceCurveSettingForm()
{
    delete ui;
}

void CReferenceCurveSettingForm::setData(CReferenceCurveManager *reCurves, const QList<CXAxis *> &xList, const QList<CYAxis *> &yList)
{
    if(reCurves && !xList.isEmpty() && !yList.isEmpty()){
        m_reCurves = reCurves;
        m_xList = xList;
        m_yList = yList;
        initialize();
    }
}

void CReferenceCurveSettingForm::setXList(const QList<CXAxis *> &xList)
{
   m_xList = xList;
}

void CReferenceCurveSettingForm::setYList(const QList<CYAxis *> &yList)
{
    m_yList = yList;
}

void CReferenceCurveSettingForm::setVariantSettingUI(IMDRTDBManagerUI *variantForm)
{
    m_variantForm = variantForm;
}

void CReferenceCurveSettingForm::setReCurveProperty(CReferenceCurve *curveInitial)
{
    bool ok = false;
    CReferenceCurve *curve = NULL;
    curve = CReferenceCurvePropertyDialog::getReferenceCurve(&ok,curveInitial,m_variantForm,
                                                             m_xList,m_yList,this);
LOOP:    if(curve && ok)
    {
        for(int i = 0; i < m_reList.count(); i++)
        {
            for(int j = i+1;j<m_reList.count();j++)
            {
                QString curveName_i = m_reList.at(i)->name();
                QString curveName_j = m_reList.at(j)->name();
                if(curveName_i.compare(curveName_j,Qt::CaseInsensitive) == 0)
                {
                    showMessage(tr("基准曲线名称不能重复,请重新设置"),this);
                    curve = CReferenceCurvePropertyDialog::getReferenceCurve(&ok,curveInitial,m_variantForm,
                                                                             m_xList,m_yList,this);
                    goto LOOP;
                }
            }
        }
        parseReCurvesInformation();
        slotHandleModified();
    }
}

void CReferenceCurveSettingForm::resetModified()
{
    m_isModified = false;
}

void CReferenceCurveSettingForm::slotHandleModified()
{
    if(!m_isModified)
    {
        m_isModified = true;
        emit itemModified();
    }
}

void CReferenceCurveSettingForm::applyChanges()
{
    if(m_reCurves){
        m_reCurves->setList(m_reList);

        for(int i = 0; i < m_reList.count(); i++)
        {
            CReferenceCurve *curve = new CReferenceCurve;
            *curve = *m_reList.at(i);
            m_rePreList.append(curve);
        }
    }

}

void CReferenceCurveSettingForm::initialize()
{
    breakConnections();
    initializeReCuves();
    initializeUI();
    createConnections();
}

void CReferenceCurveSettingForm::initializeUI()
{
//    ui->referenceCurveTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->referenceCurveTableWidget->setColumnWidth(0,150);
    parseReCurvesInformation();
    if(!m_reList.isEmpty())
    {
        ui->referenceCurveTableWidget->selectRow(0);
    }
    createMenu();
}

void CReferenceCurveSettingForm::initializeReCuves()
{
    if(m_reCurves)
    {
        m_rePreList = m_reCurves->list();
        for(int i = 0; i < m_rePreList.count(); i++)
        {
            CReferenceCurve *curve = new CReferenceCurve;
            *curve = *m_rePreList.at(i);
            m_reList.append(curve);
        }
    }
}

void CReferenceCurveSettingForm::breakConnections()
{
    disconnect(ui->referenceCurveTableWidget,&QTableWidget::itemDoubleClicked,this,&CReferenceCurveSettingForm::slotSetProperty);

    disconnect(ui->addButton,&QPushButton::clicked,this,&CReferenceCurveSettingForm::slotAddReCurve);
    disconnect(ui->deleteButton,&QPushButton::clicked,this,&CReferenceCurveSettingForm::slotDeleteReCurve);
    disconnect(ui->propertyButton,&QPushButton::clicked,this,&CReferenceCurveSettingForm::slotPropertyButtonClicked);

    disconnect(ui->moveUpButton,&QPushButton::clicked,this,&CReferenceCurveSettingForm::slotMoveUp);
    disconnect(ui->moveDownButton,&QPushButton::clicked,this,&CReferenceCurveSettingForm::slotMoveDown);

}

void CReferenceCurveSettingForm::createConnections()
{
    connect(ui->referenceCurveTableWidget,&QTableWidget::itemDoubleClicked,this,&CReferenceCurveSettingForm::slotSetProperty);

    connect(ui->addButton,&QPushButton::clicked,this,&CReferenceCurveSettingForm::slotAddReCurve);
    connect(ui->deleteButton,&QPushButton::clicked,this,&CReferenceCurveSettingForm::slotDeleteReCurve);
    connect(ui->propertyButton,&QPushButton::clicked,this,&CReferenceCurveSettingForm::slotPropertyButtonClicked);

    connect(ui->moveUpButton,&QPushButton::clicked,this,&CReferenceCurveSettingForm::slotMoveUp);
    connect(ui->moveDownButton,&QPushButton::clicked,this,&CReferenceCurveSettingForm::slotMoveDown);
}

void CReferenceCurveSettingForm::parseReCurvesInformation()
{
    int count = m_reList.count();
    ui->referenceCurveTableWidget->setRowCount(count);
    for(int i = 0; i < count;i++) {
        CReferenceCurve *curve = m_reList.at(i);

        QTableWidgetItem *colorItem = new QTableWidgetItem;
        colorItem->setData(Qt::UserRole,(int)TableRole::Color);
        colorItem->setBackgroundColor(curve->reCurveColor());
        ui->referenceCurveTableWidget->setItem(i,0,colorItem);

        QTableWidgetItem *nameItem = new QTableWidgetItem;
        nameItem->setTextAlignment(Qt::AlignCenter);
        nameItem->setData(Qt::UserRole,(int)TableRole::Name);
        nameItem->setText(curve->name());
        ui->referenceCurveTableWidget->setItem(i,1,nameItem);

    }

}

void CReferenceCurveSettingForm::slotAddReCurve()
{
    CReferenceCurve *obj = new CReferenceCurve;
    QStringList nameList;
    for(int i = 0; i < m_reList.count();i++)
        nameList.append(m_reList.at(i)->name());
    obj->setName(createName(nameList));

    QColor c;
    c.setRgb(qrand()%255,qrand()%255,qrand()%255,255);
    obj->setReCurveColor(c);

    bool ok = false;
    CReferenceCurve *curve = NULL;

    curve = CReferenceCurvePropertyDialog::getReferenceCurve(&ok,obj,m_variantForm,m_xList,m_yList,this);

LOOP: if(curve && ok)
        {
            m_reList.append(obj);

            for(int i = 0; i < m_reList.count(); i++)
            {
                for(int j = i+1;j<m_reList.count();j++)
                {
                    QString curveName_i = m_reList.at(i)->name();
                    QString curveName_j = m_reList.at(j)->name();
                    if(curveName_i.compare(curveName_j,Qt::CaseInsensitive) == 0)
                    {
                        showMessage(tr("基准曲线名称不能重复,请重新设置"),this);

                        m_reList.removeOne(obj);
                        curve = CReferenceCurvePropertyDialog::getReferenceCurve(&ok,obj,m_variantForm,m_xList,m_yList,this);
                        goto LOOP;
                    }
                }
            }

            parseReCurvesInformation();
            slotHandleModified();
        }
        else
        {
            delete obj;
            obj = NULL;
        }
}

void CReferenceCurveSettingForm::slotDeleteReCurve()
{
    if(!ui->referenceCurveTableWidget->rowCount()) {
        showMessage(tr("没有基准曲线!"),this);
    }else{
        QList<QTableWidgetItem*> selectedTableItems = ui->referenceCurveTableWidget->selectedItems();
        if(!selectedTableItems.isEmpty()) {
            QStringList itemNames;
            QString names;
            for(int i = 0; i < selectedTableItems.count();i++){
                if(selectedTableItems.at(i)->data(Qt::UserRole).toInt() == (int)TableRole::Name){
                    itemNames.append(selectedTableItems.at(i)->text());
                    names += selectedTableItems.at(i)->text() + QString(",");
                }
            }
            names = names.left(names.length()-1); // remove the last spilter ,

            int ret = showMessage(tr("确定删除 ")+names + "?",this,
                                  QMessageBox::Ok|QMessageBox::Cancel);

            if(ret == QMessageBox::Ok) {
                removeCurves(itemNames);
                parseReCurvesInformation();
                slotHandleModified();
            } else {
                // To do
            }
        } else {
            showMessage(tr("请选择一条基准曲线"),this);
        }
    }
}

void CReferenceCurveSettingForm::slotSetProperty(QTableWidgetItem *item)
{
    if(item) {
        int rowCount = item->row();
        CReferenceCurve *curve = m_reList.at(rowCount);
        setReCurveProperty(curve);
    }
}

void CReferenceCurveSettingForm::slotPropertyButtonClicked()
{
    if(!ui->referenceCurveTableWidget->rowCount()) {
        showMessage(tr("没有基准曲线!"),this);
    }else{
        if(!ui->referenceCurveTableWidget->selectedItems().isEmpty()) {
            int rowCount = ui->referenceCurveTableWidget->currentRow();

            CReferenceCurve *curve = m_reList.at(rowCount);
            setReCurveProperty(curve);
        }else{
            showMessage(tr("请选择一条基准曲线"),this);
        }
    }

}

void CReferenceCurveSettingForm::slotMoveUp()
{
    int currentIndex = ui->referenceCurveTableWidget->currentRow();
    if(currentIndex != -1) {
        if(currentIndex > 0){
            int preIndex = currentIndex -1;
            if(preIndex != -1) //! not the first one
                swapTableItem(preIndex,currentIndex);
            m_reList.swap(preIndex,currentIndex);
            slotHandleModified();
        }
    }
    else {
        showMessage(tr("请选中一条基准曲线"));
    }
}

void CReferenceCurveSettingForm::slotMoveDown()
{
    int currentIndex = ui->referenceCurveTableWidget->currentRow();
    int maxIndex = ui->referenceCurveTableWidget->rowCount()-1;
    if(currentIndex != -1) {
        if(currentIndex < maxIndex){
            int nextIndex = currentIndex +1;
            if(nextIndex <= maxIndex) //!
                swapTableItem(nextIndex,currentIndex);
            m_reList.swap(nextIndex,currentIndex);
            slotHandleModified();
        }
    } else {
        showMessage(tr("请选中一条基准曲线"));
    }
}

void CReferenceCurveSettingForm::slotCustomMenu(const QPoint &p)
{
    QPoint point = mapToGlobal(p);
    m_menu->exec(point);
}

void CReferenceCurveSettingForm::removeCurves(const QStringList &itemNames)
{
    foreach (const QString &name, itemNames)
    {
        int index = findItemByName(name);
        if(index != -1)
        {
            delete m_reList.takeAt(index);
        }   
    }
}

int CReferenceCurveSettingForm::findItemByName(const QString &name)
{
    for(int i = 0; i< m_reList.count();i++){
        if(m_reList.at(i)->name() == name)
            return i;
    }
    return -1;
}

void CReferenceCurveSettingForm::swapTableItem(int preIndex, int currentIndex)
{
    QColor currentColor = ui->referenceCurveTableWidget->item(currentIndex,0)->backgroundColor();
    QString currentName = ui->referenceCurveTableWidget->item(currentIndex,1)->text();

    QColor preColor = ui->referenceCurveTableWidget->item(preIndex,0)->backgroundColor();
    QString preName = ui->referenceCurveTableWidget->item(preIndex,1)->text();

    ui->referenceCurveTableWidget->item(currentIndex,0)->setBackgroundColor(preColor);
    ui->referenceCurveTableWidget->item(currentIndex,1)->setText(preName);

    ui->referenceCurveTableWidget->item(preIndex,0)->setBackgroundColor(currentColor);
    ui->referenceCurveTableWidget->item(preIndex,1)->setText(currentName);

    ui->referenceCurveTableWidget->selectRow(preIndex);

}

QString CReferenceCurveSettingForm::createName(const QStringList &nameList)
{
    int count = nameList.count();
    for(int i= 0; i < count;i++)
    {
        QString name = QString("基准曲线%1").arg(QString::number(i+1));
        if(!nameList.contains(name))
            return name;
    }
    return QString("基准曲线%1").arg(QString::number(count+1));
}

void CReferenceCurveSettingForm::createMenu()
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slotCustomMenu(QPoint)));// 响应右键菜单

    m_menu = new QMenu(ui->referenceCurveTableWidget);
    QAction *addAction = new QAction(tr("增加"), ui->referenceCurveTableWidget);
    m_menu->addAction(addAction);
    QAction* deleteAction = new QAction(tr("删除"), ui->referenceCurveTableWidget);
    m_menu->addAction(deleteAction);
    QAction *upAction = new QAction(tr("上移"),ui->referenceCurveTableWidget);
    m_menu->addAction(upAction);
    QAction *downAction = new QAction(tr("下移"),ui->referenceCurveTableWidget);
    m_menu->addAction(downAction);
    QAction *propertyAction = new QAction(tr("属性"),ui->referenceCurveTableWidget);
    m_menu->addAction(propertyAction);

    connect (addAction,&QAction::triggered,this,&CReferenceCurveSettingForm::slotAddReCurve);
    connect (deleteAction,&QAction::triggered,this,&CReferenceCurveSettingForm::slotDeleteReCurve);
    connect (upAction,&QAction::triggered,this,&CReferenceCurveSettingForm::slotMoveUp);
    connect (downAction,&QAction::triggered,this,&CReferenceCurveSettingForm::slotMoveDown);
    connect (propertyAction,&QAction::triggered,this,&CReferenceCurveSettingForm::slotPropertyButtonClicked);
}

void CReferenceCurveSettingForm::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
    {
        slotDeleteReCurve();
    }
    else if(event->key() == Qt::Key_Enter)
    {
        slotPropertyButtonClicked();
    }
}



