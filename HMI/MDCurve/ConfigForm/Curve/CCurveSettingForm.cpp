#include "CCurveSettingForm.h"
#include "ui_CCurveSettingForm.h"
#include "CCurvePropertySettingDialog.h"

#include "CXAxisManager.h"
#include "CYAxisManager.h"
#include "CCurveObjectManager.h"
#include "CCurveObject.h"
#include "CDataProperty.h"

#include "IMDDataArchives.h"
#include "IMDDataSourceManager.h"
#include "IMDDataSource.h"
#include "IMDDataTable.h"
#include "IMDRTDBManagerUI.h"
#include "CVariantDialog.h"
#include "CFormToolFunc.h"
#include "CPreviewCurveSettingForm.h"
#include "../MDDBQueryUI/MDDBQueryUI.h"
#include "CViewFieldDialog.h"
#include <QDebug>
#include <QMessageBox>
#include <QHeaderView>

using namespace FormToolFunc;

CCurveSettingForm::CCurveSettingForm(QWidget *parent) :
    QWidget(parent),
    m_isRealTime(false),
    m_curves(NULL),
    m_variantForm(NULL),
    m_dataArchives(NULL),
    m_dataManager(NULL),
    m_isModified(false),
    ui(new Ui::CCurveSettingForm)
{
    ui->setupUi(this);
}

CCurveSettingForm::~CCurveSettingForm()
{
    delete ui;
}
void CCurveSettingForm::setData(CCurveObjectManager *curves, const QList<CXAxis *> &xList, const QList<CYAxis *> &yList)
{
    if(curves && !xList.isEmpty() && !yList.isEmpty()){
        m_curves = curves;
        m_xList = xList;
        m_yList = yList;
        m_dataSource = curves->dataSource();
        initialize();
    }
}

void CCurveSettingForm::setDataManager(CDataProperty *data)
{
    m_dataManager = data;
}

void CCurveSettingForm::resetModified()
{
    m_isModified = false;
}

void CCurveSettingForm::applyChanges()
{
    if(m_curves && m_dataArchives)
    {
        m_curves->setList(m_list);

        QStringList fields;
        for(int i = 0; i < m_list.count();i++)
        {
            CCurveObject *curve = m_list.at(i);
            fields.append(curve->variantDepict());
        }
        m_curves->setFields(fields);

        for(int i = 0; i < m_list.count(); i++)
        {
            CCurveObject *curve = new CCurveObject;
            *curve = *m_list.at(i);
            m_preList.append(curve);
        }
    }
}

void CCurveSettingForm::setVariantSettingUI(IMDRTDBManagerUI *variantForm)
{
    m_variantForm = variantForm;
}

void CCurveSettingForm::setDataArchives(IMDDataArchives *dataArchives)
{
    m_dataArchives = dataArchives;
}

void CCurveSettingForm::initialize()
{
    breakConnections();
    initializeCuves();
    initializeUI();
    createConnections();
}

void CCurveSettingForm::initializeUI()
{
    ui->curveTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    parseCurvesInformation();
    if(!m_list.isEmpty())
        ui->curveTableWidget->selectRow(0);
    createMenu();
}

void CCurveSettingForm::initializeCuves()
{
    if(m_curves)
    {
//        m_list = m_curves->list();
        m_preList = m_curves->list();
        for(int i = 0; i < m_preList.count(); i++)
        {
            CCurveObject *curve = new CCurveObject;
            *curve = *m_preList.at(i);
            m_list.append(curve);
        }
    }
}  

void CCurveSettingForm::breakConnections()
{
    disconnect(ui->curveTableWidget,&QTableWidget::itemDoubleClicked,this,&CCurveSettingForm::slotSetProperty);
    disconnect(ui->curveTableWidget,&QTableWidget::itemClicked,this,&CCurveSettingForm::slotSetCurveVisible);
    disconnect(ui->addDozenButton,&QPushButton::clicked,this,&CCurveSettingForm::slotAddDozenCurves);
    disconnect(ui->addButton,&QPushButton::clicked,this,&CCurveSettingForm::slotAddOneCurve);
    disconnect(ui->deleteButton,&QPushButton::clicked,this,&CCurveSettingForm::slotDeleteOneCurve);
    disconnect(ui->propertyButton,&QPushButton::clicked,this,&CCurveSettingForm::slotPropertyButtonClicked);
    disconnect(ui->moveUpButton,&QPushButton::clicked,this,&CCurveSettingForm::slotMoveUp);
    disconnect(ui->moveDownButton,&QPushButton::clicked,this,&CCurveSettingForm::slotMoveDown);
}

void CCurveSettingForm::slotHandleModified()
{
    if(!m_isModified)
    {
        m_isModified = true;
        emit itemModified();
    }
}

void CCurveSettingForm::createConnections()
{
    connect(ui->curveTableWidget,&QTableWidget::itemDoubleClicked,this,&CCurveSettingForm::slotSetProperty);
    connect(ui->curveTableWidget,&QTableWidget::itemClicked,this,&CCurveSettingForm::slotSetCurveVisible);
    connect(ui->addDozenButton,&QPushButton::clicked,this,&CCurveSettingForm::slotAddDozenCurves);
    connect(ui->addButton,&QPushButton::clicked,this,&CCurveSettingForm::slotAddOneCurve);
    connect(ui->deleteButton,&QPushButton::clicked,this,&CCurveSettingForm::slotDeleteOneCurve);
    connect(ui->propertyButton,&QPushButton::clicked,this,&CCurveSettingForm::slotPropertyButtonClicked);

    connect(ui->moveUpButton,&QPushButton::clicked,this,&CCurveSettingForm::slotMoveUp);
    connect(ui->moveDownButton,&QPushButton::clicked,this,&CCurveSettingForm::slotMoveDown);
}

void CCurveSettingForm::parseCurvesInformation()
{
    int count = m_list.count();
    ui->curveTableWidget->setRowCount(count);
    for(int i = 0; i < count;i++)
    {
        CCurveObject *curve = m_list.at(i);

        QTableWidgetItem *nameItem = new QTableWidgetItem;
        nameItem->setTextAlignment(Qt::AlignCenter);
        nameItem->setData(Qt::UserRole,(int)TableRole::Name);
        nameItem->setText(curve->name());
        ui->curveTableWidget->setItem(i,0,nameItem);

        QTableWidgetItem *variantItem = new QTableWidgetItem;
        variantItem->setTextAlignment(Qt::AlignCenter);
        variantItem->setData(Qt::UserRole,(int)TableRole::Variant);
        if(curve->viewName().isEmpty())
            variantItem->setText(curve->variantDepict());
        else
            variantItem->setText(curve->viewName()+"."+curve->variantDepict());
        ui->curveTableWidget->setItem(i,1,variantItem);

        QTableWidgetItem *commentItem = new QTableWidgetItem;
        commentItem->setTextAlignment(Qt::AlignCenter);
        commentItem->setData(Qt::UserRole,(int)TableRole::Comment);
        commentItem->setText(curve->comment());
        ui->curveTableWidget->setItem(i,2,commentItem);

        QTableWidgetItem *visibleItem = new QTableWidgetItem;
        visibleItem->setFlags(Qt::ItemIsUserCheckable);
        visibleItem->setData(Qt::UserRole,(int)TableRole::Visible);
        if(curve->curveVisible())
            visibleItem->setCheckState(Qt::Checked);
        else
            visibleItem->setCheckState(Qt::Unchecked);
        ui->curveTableWidget->setItem(i,3,visibleItem);

        QTableWidgetItem *colorItem = new QTableWidgetItem;
        colorItem->setData(Qt::UserRole,(int)TableRole::Color);
        colorItem->setBackgroundColor(curve->curveColor());
        ui->curveTableWidget->setItem(i,4,colorItem);
    }
}

void CCurveSettingForm::slotAddDozenCurves()
{
    QString createName(const QStringList &);
    QMap<QString,QList<DBQ::ViewField *>>map;
    QStringList oldNameList;
    for(int i = 0; i < m_list.count();i++)
        oldNameList.append(m_list.at(i)->name());
    bool ok = false;
    QStringList nameList;
    if(m_isRealTime && m_variantForm)
    {
        m_variantForm->selectVariables(nameList);
        if(!nameList.isEmpty())
        {
            ok = true;
        }
    }
    else
    {
        QStringList list;

        DBQ::View_GetAllViewNameFromDir(m_curves->projectPath()+"/DBQuery",list);
        for(int i = 0; i<list.count();i++)
        {
            QString str;
            QString view = list.at(i);
            QList<DBQ::ViewField *>viewField;
            viewField = DBQ::View_GetAllViewFieldsInViewFromDir(m_curves->projectPath()+"/DBQuery"
                                                      ,view,str);
            map.insert(view,viewField);
        }
        nameList = CViewFieldDialog::getField(&ok,list,map,this);
    }

    if(ok)
    {
        CCurveObject * curve = NULL;
        for(int i = 0; i < nameList.count();i++)
        {
            bool curveHasRepeat = false;
            for(int j = 0; j< m_list.count(); j++)
            {
                if(m_isRealTime)
                {
                    if(m_list.at(j)->variantDepict() == nameList.at(i))
                        curveHasRepeat = true;
                    else
                        continue;
                }
                else
                {
                    if(m_list.at(j)->viewName() +"."+m_list.at(j)->variantDepict() == nameList.at(i))
                        curveHasRepeat = true;
                    else
                        continue;
                }
            }
            if(curveHasRepeat)
                continue;
            curve = new CCurveObject;
            curve->setProjectPath(m_curves->projectPath());
            QColor c;
            c.setRgb(qrand()%255,qrand()%255,qrand()%255,255);
            curve->setCurveColor(c);

            QString name  = createName(oldNameList);
            curve->setName(name);
            oldNameList.append(name);
            if(m_isRealTime)
                curve->setVariantDepict(nameList.at(i));
            else
            {
                int index = nameList.at(i).indexOf(".");
                int count = nameList.at(i).count();
                curve->setViewName(nameList.at(i).left(index));
                curve->setVariantDepict(nameList.at(i).right(count - index-1));

                QList<DBQ::ViewField *>viewField = map.value(nameList.at(i).left(index));
                for(int j = 0; j<viewField.count();j++)
                {
                    DBQ::ViewField *view = viewField.at(j);
                    if(view->type == DBQ::VIEW_FIELD_TYPE::VIEW_FIELD_TYPE_DATETIME)
                    {
                        curve->setTimeVariantDepict(viewField.at(j)->fieldName);
                        break;
                    }
                }
            }
            if(!m_xList.isEmpty())
            {
                for(int i =0 ; i<m_xList.count();i++)
                {
                    if(m_xList.at(i)->valueType() == CXAxis::ValueType::Time)
                    {
                       curve->setXName(m_xList.at(i)->name());
                    }
                }
            }

            if(!m_yList.isEmpty())
                curve->setYName(m_yList.first()->name());

            if(!curve->xName().isEmpty())
                m_list.append(curve);
        }
        parseCurvesInformation();
    }
    if(ok)
         slotHandleModified();
    updatePreviewCurve();
}
void CCurveSettingForm::setCurveProperty(CCurveObject *curveInitial , int index)
{
    bool ok = false;
    CCurveObject *curve = NULL;
    QString variant = curveInitial->variantDepict();
    if(m_isRealTime)
    {
        curve = CCurvePropertySettingDialog::getRealCurve(&ok,curveInitial,
                                                          m_xList,m_yList,m_variantForm,this);
        bool curveHasRepeat = isCurveRepeat(curve->variantDepict(),index);
        if(curveHasRepeat)
        {
           curve->setVariantDepict(variant);
           showMessage(tr("该曲线已存在，请重新设置"),this);
           return;
        }
    }

    else
    {
        curve = CCurvePropertySettingDialog::getHistoryCurve(&ok,curveInitial,m_xList,m_yList,
                                                             m_fields,this);
        bool curveHasRepeat = isCurveRepeat(curve->viewName()+"."+ curve->variantDepict(),index);
        if(curveHasRepeat)
        {
           curve->setVariantDepict(curve->viewName()+"."+variant);
           showMessage(tr("该曲线已存在，请重新设置"),this);
           return;
        }
    }

    if(!m_isRealTime && ok)
    {
        bool isContained  = isVariantContained(curve->projectPath(),curve->viewName(),curve->variantDepict());
        if(!isContained)
        {
            showMessage(tr("所选视图中无该变量，请重新设置"),this);
            curve = CCurvePropertySettingDialog::getHistoryCurve(&ok,curveInitial,m_xList,m_yList,
                                                                 m_fields,this);
        }
    }

   LOOP: if(curve && ok)
    {
        for(int i = 0; i < m_list.count(); i++)
        {
            for(int j = i+1;j<m_list.count();j++)
            {
                QString curveName_i = m_list.at(i)->name();
                QString curveName_j = m_list.at(j)->name();
                if(curveName_i.compare(curveName_j,Qt::CaseInsensitive) == 0)
                {
                    showMessage(tr("曲线名称不能重复,请重新设置"),this);
                    if(m_isRealTime)
                        curve = CCurvePropertySettingDialog::getRealCurve(&ok,curveInitial,
                                                                          m_xList,m_yList,m_variantForm,this);
                    else
                        curve = CCurvePropertySettingDialog::getHistoryCurve(&ok,curveInitial,m_xList,m_yList,
                                                                             m_fields,this);
                    goto LOOP;
                }
            }
        }

        parseCurvesInformation();
    }
    if(ok)
         slotHandleModified();
}

void CCurveSettingForm::slotAddOneCurve()
{
    QString createName(const QStringList &);

    CCurveObject *obj = new CCurveObject;
    obj->setProjectPath(m_curves->projectPath());

    QStringList nameList;
    for(int i = 0; i < m_list.count();i++)
        nameList.append(m_list.at(i)->name());
    obj->setName(createName(nameList));

    QColor color;
    color.setRgb(qrand()%255,qrand()%255,qrand()%255,255);
    obj->setCurveColor(color);

    bool ok = false;
    CCurveObject *curve = NULL;
    if(m_isRealTime)
    {
        curve = CCurvePropertySettingDialog::getRealCurve(&ok,obj,m_xList,m_yList,m_variantForm,this);
        bool isRepeat = isCurveRepeat(curve->variantDepict());
        if(isRepeat)
        {
            showMessage(tr("该曲线已存在，请重新设置"),this);
            return;
        }
    }
    else
    {
        curve = CCurvePropertySettingDialog::getHistoryCurve(&ok,obj,m_xList,m_yList,
                                                             m_fields,this);
        bool isRepeat = isCurveRepeat(curve->viewName()+"."+ curve->variantDepict());
        if(isRepeat)
        {
            showMessage(tr("该曲线已存在，请重新设置"),this);
            return;
        }
    }

LOOP:   if(curve && ok)
    {
        if(curve->xName().isEmpty())
            return;
        m_list.append(obj);
        for(int i = 0; i < m_list.count(); i++)
        {
            for(int j = i+1;j<m_list.count();j++)
            {
                QString curveName_i = m_list.at(i)->name();
                QString curveName_j = m_list.at(j)->name();
                if(curveName_i.compare(curveName_j,Qt::CaseInsensitive) == 0)
                {
                    showMessage(tr("曲线名称不能重复,请重新设置"),this);
                    m_list.removeOne(obj);
                    if(m_isRealTime)
                        curve = CCurvePropertySettingDialog::getRealCurve(&ok,obj,m_xList,m_yList,m_variantForm,this);
                    else
                        curve = CCurvePropertySettingDialog::getHistoryCurve(&ok,obj,m_xList,m_yList,
                                                                             m_fields,this);
                    goto LOOP;
                }
            }
        }
        slotHandleModified();
        parseCurvesInformation();
    }

    updatePreviewCurve();
}

void CCurveSettingForm::slotDeleteOneCurve()
{

    if(!ui->curveTableWidget->rowCount())
    {
        showMessage(tr("没有曲线!"),this);
    }
    else
    {
        QList<QTableWidgetItem*> selectedTableItems = ui->curveTableWidget->selectedItems();
        if(!selectedTableItems.isEmpty())
        {
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

            if(ret == QMessageBox::Ok)
            {
                removeCurves(itemNames);
                parseCurvesInformation();
                slotHandleModified();
            }
            else
            {
                // To do
            }
        }
        else
        {
            showMessage(tr("请选择一条曲线"),this);
        }
    }
    updatePreviewCurve();
}   

void CCurveSettingForm::slotSetProperty(QTableWidgetItem *item)
{
    if(item)
    {
        int rowCount = item->row();
        CCurveObject *curve = m_list.at(rowCount);
        setCurveProperty(curve,rowCount);
    }
}

void CCurveSettingForm::slotSetCurveVisible(QTableWidgetItem *item)
{
    if(Qt::ItemIsUserCheckable == item->flags())
    {
        if(Qt::Checked == item->checkState())
        {
            item->setCheckState(Qt::Unchecked);
            m_list.at(item->row())->setCurveVisible(false);
            slotHandleModified();
        }
        else if(Qt::Unchecked == item->checkState())
        {
            item->setCheckState(Qt::Checked);
            m_list.at(item->row())->setCurveVisible(true);
            slotHandleModified();
        }
    }
}

void CCurveSettingForm::slotPropertyButtonClicked()
{
    if(!ui->curveTableWidget->rowCount())
    {
        showMessage(tr("没有曲线!"),this);
    }
    else
    {
        if(!ui->curveTableWidget->selectedItems().isEmpty())
        {
            int rowCount = ui->curveTableWidget->currentRow();

            CCurveObject *curve = m_list.at(rowCount);
            setCurveProperty(curve,rowCount);
        }
        else
        {
            showMessage(tr("请选择一条曲线"),this);
        }
    }
}

void CCurveSettingForm::slotMoveUp()
{
    int currentIndex = ui->curveTableWidget->currentRow();
    if(currentIndex != -1)
    {
        if(currentIndex > 0)
        {
            int preIndex = currentIndex -1;
            if(preIndex != -1) //! not the first one
                swapTableItem(preIndex,currentIndex);
            m_list.swap(preIndex,currentIndex);
            slotHandleModified();
        }
    }
    else
    {
        showMessage(tr("请选中一条曲线"));
    }
}

void CCurveSettingForm::slotMoveDown()
{
    int currentIndex = ui->curveTableWidget->currentRow();
    int maxIndex = ui->curveTableWidget->rowCount()-1;
    if(currentIndex != -1)
    {
        if(currentIndex < maxIndex)
        {
            int nextIndex = currentIndex +1;
            if(nextIndex <= maxIndex) //!
                swapTableItem(nextIndex,currentIndex);
            m_list.swap(nextIndex,currentIndex);
            slotHandleModified();
        }
    }
    else
    {
        showMessage(tr("请选中一条曲线"));
    }
}

void CCurveSettingForm::slotCustomMenu(const QPoint &p)
{
    QPoint point = mapToGlobal(p);
    m_menu->exec(point);
}

void CCurveSettingForm::updatePreviewCurve()
{
    if(m_previewCurveForm)
    {
        m_curves->setList(m_list);
        m_previewCurveForm->setCurveObjectManager(m_curves);
    }
}

void CCurveSettingForm::removeCurves(const QStringList &itemNames)
{
    foreach (const QString &name, itemNames)
    {
        int index = findItemByName(name);
        if(index != -1)
        {
            delete m_list.takeAt(index);
        }  
    }
}

int CCurveSettingForm::findItemByName(const QString &name)
{
    for(int i = 0; i< m_list.count();i++){
        if(m_list.at(i)->name() == name)
            return i;
    }
    return -1;
}

void CCurveSettingForm::swapTableItem(int preIndex, int currentIndex)
{
    QString currentName = ui->curveTableWidget->item(currentIndex,0)->text();
    QString currentVariant = ui->curveTableWidget->item(currentIndex,1)->text();
    QString currentComment = ui->curveTableWidget->item(currentIndex,2)->text();
    Qt::CheckState currentVisible = ui->curveTableWidget->item(currentIndex,3)->checkState();
    QColor currentColor = ui->curveTableWidget->item(currentIndex,4)->backgroundColor();

    QString preName = ui->curveTableWidget->item(preIndex,0)->text();
    QString preVariant = ui->curveTableWidget->item(preIndex,1)->text();
    QString preComment = ui->curveTableWidget->item(preIndex,2)->text();
    Qt::CheckState preVisible = ui->curveTableWidget->item(preIndex,3)->checkState();
    QColor preColor = ui->curveTableWidget->item(preIndex,4)->backgroundColor();

    ui->curveTableWidget->item(currentIndex,0)->setText(preName);
    ui->curveTableWidget->item(currentIndex,1)->setText(preVariant);
    ui->curveTableWidget->item(currentIndex,2)->setText(preComment);
    ui->curveTableWidget->item(currentIndex,3)->setCheckState(preVisible);
    ui->curveTableWidget->item(currentIndex,4)->setBackgroundColor(preColor);

    ui->curveTableWidget->item(preIndex,0)->setText(currentName);
    ui->curveTableWidget->item(preIndex,1)->setText(currentVariant);
    ui->curveTableWidget->item(preIndex,2)->setText(currentComment);
    ui->curveTableWidget->item(preIndex,3)->setCheckState(currentVisible);
    ui->curveTableWidget->item(preIndex,4)->setBackgroundColor(currentColor);

    ui->curveTableWidget->selectRow(preIndex);
}

bool CCurveSettingForm::isCurveRepeat(QString str, int index)
{
    QStringList list;
    if(index == -1)
    {
        for(int i = 0;i <m_list.count(); i++)
        {
            if(m_isRealTime)
            {
                if(m_list[i]->variantDepict() == str)
                    return true;
            }
            else
            {
                if(m_list[i]->viewName()+"." +m_list[i]->variantDepict() == str)
                    return true;
            }
        }
    }
    else
    {
        for(int i = 0; i<m_list.count(); i++)
        {
            if(i == index)
                continue;
            else
            {
                if(m_isRealTime)
                {
                    list.append(m_list.at(i)->variantDepict());
                }
                else
                {
                    list.append(m_list[i]->viewName()+"." +m_list.at(i)->variantDepict());
                }
            }
        }
        bool isRepeat = false;
        for(int i = 0; i<list.count(); i++)
        {
            if(list.at(i) == str)
                isRepeat = true;
            else
                continue;
        }
        return isRepeat;
    }
    return false;
}

bool CCurveSettingForm::isVariantContained(const QString &path,const QString &viewName, const QString &variantName)
{
    QList<DBQ::ViewField *>viewFiled;
    QString str;
    viewFiled = DBQ::View_GetAllViewFieldsInViewFromDir(path+"/DBQuery",viewName,str);
    for(int i = 0 ;i<viewFiled.count();i++)
    {
        if(viewFiled.at(i)->fieldName == variantName)
            return true;
    }
    return false;
}

void CCurveSettingForm::createMenu()
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slotCustomMenu(QPoint)));// 响应右键菜单

    m_menu = new QMenu(ui->curveTableWidget);
    QAction *addAction = new QAction(tr("增加"), ui->curveTableWidget);
    m_menu->addAction(addAction);
    QAction *addDozenAction = new QAction(tr("批量增加"),ui->curveTableWidget);
    m_menu->addAction(addDozenAction);
    QAction* deleteAction = new QAction(tr("删除"), ui->curveTableWidget);
    m_menu->addAction(deleteAction);
    QAction *upAction = new QAction(tr("上移"),ui->curveTableWidget);
    m_menu->addAction(upAction);
    QAction *downAction = new QAction(tr("下移"),ui->curveTableWidget);
    m_menu->addAction(downAction);
    QAction *propertyAction = new QAction(tr("属性"),ui->curveTableWidget);
    m_menu->addAction(propertyAction);

    connect (addAction,&QAction::triggered,this,&CCurveSettingForm::slotAddOneCurve);
    connect (addDozenAction,&QAction::triggered,this,&CCurveSettingForm::slotAddDozenCurves);
    connect (deleteAction,&QAction::triggered,this,&CCurveSettingForm::slotDeleteOneCurve);
    connect (upAction,&QAction::triggered,this,&CCurveSettingForm::slotMoveUp);
    connect (downAction,&QAction::triggered,this,&CCurveSettingForm::slotMoveDown);
    connect (propertyAction,&QAction::triggered,this,&CCurveSettingForm::slotPropertyButtonClicked);
}

void CCurveSettingForm::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
    {
        slotDeleteOneCurve();
    }
    else if(event->key() == Qt::Key_Enter)
    {
        slotPropertyButtonClicked();
    }
}

void CCurveSettingForm::setPreviewCurveForm(CPreviewCurveSettingForm *previewForm)
{
    m_previewCurveForm = previewForm;
}

CPreviewCurveSettingForm *CCurveSettingForm::previewCurveForm()
{
    return m_previewCurveForm;
}


QString createName(const QStringList &nameList)
{
    int count = nameList.count();
    for(int i= 0; i < count;i++)
    {
        QString name = QString("Curve%1").arg(QString::number(i+1));
        if(!nameList.contains(name))
            return name;
    }
    return QString("Curve%1").arg(QString::number(count+1));
}

void CCurveSettingForm::setYList(const QList<CYAxis *> &yList)
{
    m_yList = yList;
}

void CCurveSettingForm::setFieldsList(const QStringList &fieldsList)
{
    m_fields = fieldsList;
}

void CCurveSettingForm::setIsRealTime(bool isRealTime)
{
    m_isRealTime = isRealTime;
}

void CCurveSettingForm::setXList(const QList<CXAxis *> &xList)
{
    m_xList = xList;
}

