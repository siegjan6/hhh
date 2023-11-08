#include "CToolsSettingForm.h"
#include "ui_CToolsSettingForm.h"
#include "CTools.h"

#include <QTableWidgetItem>
#include <QButtonGroup>

CToolsSettingForm::CToolsSettingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CToolsSettingForm),
    m_isModified(false),
    m_tools(0)
{
    ui->setupUi(this);
    ui->buttonTableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->buttonTableWidget->horizontalHeader()->resizeSection(0,120);
    ui->buttonTableWidget->horizontalHeader()->resizeSection(1,200);

}

CToolsSettingForm::~CToolsSettingForm()
{
    delete ui;
}

void CToolsSettingForm::setData(CTools *tools)
{
    if(tools){
        m_tools = tools;
        initialize();
    }
}

void CToolsSettingForm::applyChanges()
{
    if(m_tools){
        int checked = 0;
        for(int i= 0;i<ui->buttonTableWidget->rowCount();i++)
        {
            if(ui->buttonTableWidget->item(i,0)->checkState() == Qt::Checked)
            {
               checked++;
            }
        }
        if(checked != 0)
        {
            QSize size(ui->buttonWidthSpinBox->value(),ui->buttonHeigghtSpinBox->value());
            m_tools->setButtonSize(size);
        }
        else
        {
            QSize size(0,0);
            m_tools->setButtonSize(size);
        }

        for(int i = 0; i < ui->buttonTableWidget->rowCount();i++)
            setToolButtonChanges(i);
    }
}

void CToolsSettingForm::resetModified()
{
    m_isModified = false;
}

void CToolsSettingForm::slotHandleModified()
{
    if(!m_isModified){
        m_isModified = true;
        emit itemModified();
    }
}

void CToolsSettingForm::initialize()
{
    if(m_tools){
        breakConnections();
        QSize buttonSize = m_tools->buttonSize();
        QButtonGroup *buttonGroup = m_tools->buttonGroup();
        ui->buttonHeigghtSpinBox->setValue(buttonSize.height());
        ui->buttonWidthSpinBox->setValue(buttonSize.width());
        parseButtons(*buttonGroup);
        createConnections();
    }
}

void CToolsSettingForm::initializeData()
{

}

void CToolsSettingForm::initializeUI()
{
}

void CToolsSettingForm::createConnections()
{
    connect(ui->buttonTableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(slotSetItemVisible(QTableWidgetItem*)));
    connect(ui->moveUpButton,SIGNAL(clicked()),this,SLOT(slotMoveUp()));
    connect(ui->moveDownButton,SIGNAL(clicked()),this,SLOT(slotMoveDown()));

    connect(ui->buttonHeigghtSpinBox,SIGNAL(valueChanged(int)),this,SLOT(slotHandleModified()));
    connect(ui->buttonWidthSpinBox,SIGNAL(valueChanged(int)),this,SLOT(slotHandleModified()));
}

void CToolsSettingForm::breakConnections()
{
    disconnect(ui->buttonTableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(slotSetItemVisible(QTableWidgetItem*)));
    disconnect(ui->moveUpButton,SIGNAL(clicked()),this,SLOT(slotMoveUp()));
    disconnect(ui->moveDownButton,SIGNAL(clicked()),this,SLOT(slotMoveDown()));

    disconnect(ui->buttonHeigghtSpinBox,SIGNAL(valueChanged(int)),this,SLOT(slotHandleModified()));
    disconnect(ui->buttonWidthSpinBox,SIGNAL(valueChanged(int)),this,SLOT(slotHandleModified()));
}

void CToolsSettingForm::slotSetItemVisible(QTableWidgetItem *item)
{
    if(item && Qt::ItemIsUserCheckable == item->flags()){
        if(item->checkState() == Qt::Checked){
            item->setCheckState(Qt::Unchecked);
        }else if(item->checkState() == Qt::Unchecked){
            item->setCheckState(Qt::Checked);
        }
        slotHandleModified();
    }
}

void CToolsSettingForm::slotMoveUp()
{
    int currentIndex = ui->buttonTableWidget->currentRow();
    if(currentIndex != -1) {
        int preIndex = currentIndex -1;
        if(preIndex != -1) //! not the first one
            swapTableItem(preIndex,currentIndex);
        slotHandleModified();
    } else {
        showMessage(tr("请选中一个按钮"));
    }
}

void CToolsSettingForm::slotMoveDown()
{
    int currentIndex = ui->buttonTableWidget->currentRow();
    int maxIndex = ui->buttonTableWidget->rowCount()-1;
    if(currentIndex != -1) {
        int nextIndex = currentIndex +1;
        if(nextIndex <= maxIndex) //!
            swapTableItem(nextIndex,currentIndex);
        slotHandleModified();
    } else {
        showMessage(tr("请选中一个按钮"));
    }
}


void CToolsSettingForm::parseButtons(const QButtonGroup &buttonGroup)
{
    int count = buttonGroup.buttons().count();
    ui->buttonTableWidget->setRowCount(count);
    for(int i = 0;i< count;i++){
        CToolButton *button  = qobject_cast<CToolButton*>(buttonGroup.button(i));
        if(button){
            int row = button->buttonPos();
            insertTableItem(row,0,createVisibleItem(button->isVisible()));
            insertTableItem(row,1,createNameItem(button->buttonName()));
            insertTableItem(row,2,createIconItem(button->icon()));
        }
    }
}

void CToolsSettingForm::setToolButtonChanges(int rowIndex)
{
    bool buttonVisible;
    if(ui->buttonTableWidget->item(rowIndex,0)->checkState() == Qt::Checked){
        buttonVisible = true;
    }else if(ui->buttonTableWidget->item(rowIndex,0)->checkState() == Qt::Unchecked){
        buttonVisible = false;
    }
    QString buttonName = ui->buttonTableWidget->item(rowIndex,1)->text();
    CToolButton *button = m_tools->button(buttonName);
    if(button){
        button->setButtonVisible(buttonVisible);
        button->setButtonPos(rowIndex);
    }
}

QTableWidgetItem *CToolsSettingForm::createNameItem(const QString &name)
{
    QTableWidgetItem *nameItem = new QTableWidgetItem;
    nameItem->setTextAlignment(Qt::AlignCenter);
    nameItem->setText(name);

    return nameItem;
}
QTableWidgetItem *CToolsSettingForm::createVisibleItem(bool visible)
{
    QTableWidgetItem *visibleItem = new QTableWidgetItem;
    visibleItem->setFlags(Qt::ItemIsUserCheckable);
    if(visible)
        visibleItem->setCheckState(Qt::Checked);
    else
        visibleItem->setCheckState(Qt::Unchecked);

    return visibleItem;
}

QTableWidgetItem *CToolsSettingForm::createIconItem(const QIcon &icon)
{
    QTableWidgetItem *iconItem = new QTableWidgetItem;
    iconItem->setIcon(icon);

    return iconItem;
}

void CToolsSettingForm::insertTableItem(int row,int column,QTableWidgetItem* item)
{
    ui->buttonTableWidget->setItem(row,column,item);
}

int CToolsSettingForm::showMessage(const QString &comment, QMessageBox::StandardButtons buttons)
{
    return QMessageBox::warning(this,tr("警告"),comment,buttons);
}

void CToolsSettingForm::swapTableItem(int preIndex, int currentIndex)
{
    Qt::CheckState currentState = ui->buttonTableWidget->item(currentIndex,0)->checkState();
    QString currentName = ui->buttonTableWidget->item(currentIndex,1)->text();
    QIcon currentIcon = ui->buttonTableWidget->item(currentIndex,2)->icon();

    Qt::CheckState preState = ui->buttonTableWidget->item(preIndex,0)->checkState();
    QString preName = ui->buttonTableWidget->item(preIndex,1)->text();
    QIcon preIcon = ui->buttonTableWidget->item(preIndex,2)->icon();

    ui->buttonTableWidget->item(currentIndex,0)->setCheckState(preState);
    ui->buttonTableWidget->item(currentIndex,1)->setText(preName);
    ui->buttonTableWidget->item(currentIndex,2)->setIcon(preIcon);

    ui->buttonTableWidget->item(preIndex,0)->setCheckState(currentState);
    ui->buttonTableWidget->item(preIndex,1)->setText(currentName);
    ui->buttonTableWidget->item(preIndex,2)->setIcon(currentIcon);

    ui->buttonTableWidget->selectRow(preIndex);
}
