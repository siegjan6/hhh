#include "CFilterDialog.h"
#include "ui_CFilterDialog.h"
#include "MDFilterTreePlugin.h"
#include "MDMultiLanguage.h"
#include "CContants.h"

CFilterDialog::CFilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CFilterDialog),
    m_filterManager(NULL)
{
    ui->setupUi(this);


    setWindowTitle(Translator::findTranslation( tr("过滤条件设置")));
}

CFilterDialog::~CFilterDialog()
{
    delete ui;
}

QTableWidgetItem *CFilterDialog::createTableItem(const QString &text) const
{
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(text);
    item->setTextAlignment(Qt::AlignCenter);
    return item;
}

QWidget *CFilterDialog::createTab(const QList<CMDFilterTreeDataItem*> &items,const QString &title) const
{
    CTableWidget *widget = new CTableWidget();
    widget->setWindowTitle(title);
    widget->setRowCount(items.count());

    for(int i = 0;i < items.count();i++){
        CMDFilterTreeDataItem *item = items.at(i);
        widget->setItem(i,(int)TableColumn::Name,createTableItem(itemName(item)));
        widget->setItem(i,(int)TableColumn::Value,createTableItem(item->getValue()));
    }

    return widget;
}

void CFilterDialog::setTableInfor(const QMap<QString, QStringList> &datas)
{
    QMapIterator<QString,QStringList> iter(datas);
    while(iter.hasNext()){
        iter.next();        
        if(m_filterManager){
            QList<CMDFilterTreeDataItem*> items = m_filterManager->dynamicInputList(iter.key());
            if(!items.isEmpty()){
                m_filters.insert(iter.key(),items);
                QWidget *widget = createTab(items,iter.key());
                ui->tabWidget->addTab(widget,widget->windowTitle());
            }
        }
    }
}

void CFilterDialog::setApplyButtonStatus()
{
    if(m_filters.count() <= 1){
        ui->applyAllButton->setVisible(false);
    }
}

QString CFilterDialog::itemName(CMDFilterTreeDataItem *item) const
{
    return item->getHintInfo().isEmpty()? item->getName():item->getHintInfo();
}


QMap<QString, QStringList> CFilterDialog::dataInfor() const
{
    return m_dataInfor;
}

void CFilterDialog::setDataInfor(const QMap<QString, QStringList> &dataInfor)
{
    m_dataInfor = dataInfor;
    if(m_dataInfor.isEmpty())
        return;
    setTableInfor(m_dataInfor);
    setApplyButtonStatus();
}

void CFilterDialog::setFilterPluginManager(CMDFilterTreeDataManager *manager)
{
    m_filterManager = manager;
}


void CFilterDialog::on_okButton_clicked()
{
    QString filter;
    for(int i=0; i < ui->tabWidget->count();i++){
        CTableWidget *table = qobject_cast<CTableWidget*>(ui->tabWidget->widget(i));
        if(table){
            QString name = table->windowTitle();
            QList<CMDFilterTreeDataItem*> tableInfors = m_filters[name];
            for(int i = 0 ; i < table->rowCount();i++){
                tableInfors[i]->setValue(table->item(i,(int)TableColumn::Value)->text());
            }
            if(m_filterManager){
                filter = m_filterManager->updateCondition(name,tableInfors);
                QStringList list = m_dataInfor.value(name);
                list.removeLast();
                list.append(filter);
                m_dataInfor[name] = list;
            }
        }
    }
    accept();
}

void CFilterDialog::on_cancelButton_clicked()
{
    reject();
}

void CFilterDialog::on_applyAllButton_clicked()
{
    QStringList values;
    values.clear();
    CTableWidget *table = qobject_cast<CTableWidget*>(ui->tabWidget->currentWidget());
    if(table){
        for(int i = 0 ; i < table->rowCount();i++){
            QString value = table->item(i,(int)TableColumn::Value)->text();
            values.append(value);
        }

        for(int i=0; i < ui->tabWidget->count();i++){
            CTableWidget *widget = qobject_cast<CTableWidget*>(ui->tabWidget->widget(i));
            if(widget){
                if(table == widget){
                    continue; // jump the current widget
                }else{
                    for(int j = 0; j  < qMin(widget->rowCount(),values.count());j++){
                        widget->item(j,(int)(TableColumn::Value))->setText(values.at(j));
                    }
                }
            }

        }
    }
}


QWidget *ItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    Q_UNUSED(parent);
    Q_UNUSED(option);
    Q_UNUSED(index);
    return 0;

}


CTableWidget::CTableWidget(QWidget *parent):
    QTableWidget(parent)
{
    using namespace Translator;
    setColumnCount(2);

    QStringList headers;
    headers << findTranslation(tr("参数"))<<findTranslation(tr("值"));
    setHorizontalHeaderLabels(headers);
    verticalHeader()->setVisible(false);

    setItemDelegateForColumn(0,new ItemDelegate(this));
}
