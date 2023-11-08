#include "CViewFieldDialog.h"
#include "ui_CViewFieldDialog.h"
#include <QListWidgetItem>


CViewFieldDialog::CViewFieldDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CViewFieldDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("视图-字段列表");
    connect(ui->okButton,SIGNAL(clicked()),this,SLOT(slotOk()));
    connect(ui->cancleButton,SIGNAL(clicked()),this,SLOT(slotCancel()));
    connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem *, int)),this,SLOT(slotSetChecked(QTreeWidgetItem *, int)));
}

CViewFieldDialog::~CViewFieldDialog()
{
    delete ui;
}

QStringList CViewFieldDialog::getField(bool *ok, const QStringList &viewList, QMap<QString, QList<DBQ::ViewField *> > map, QWidget *parent)
{
    CViewFieldDialog dialog(parent);
    dialog.setHeaderList(viewList);
    dialog.setViewVariantMap(map);
    dialog.initialize();
    int ret = dialog.exec();

    if (ok)
        *ok = !!ret;

    if (ret)
    {
        return dialog.selectField();
    }
    return QStringList();
}

void CViewFieldDialog::setHeaderList(const QStringList &viewList)
{
    m_viewList = viewList;
}

void CViewFieldDialog::setViewVariantMap(QMap<QString, QList<DBQ::ViewField *> > map)
{
    m_map = map;
}

void CViewFieldDialog::initialize()
{
    QStringList header;
    header<<"姓名"<<"字段类型"<<"字段别名"<<"字段描述";
    ui->treeWidget->setHeaderLabels(header);
    ui->treeWidget->setColumnWidth(0,150);
    for(int i = 0 ;i< m_viewList.count();++i)
    {
        QTreeWidgetItem *topItem = new QTreeWidgetItem;
        topItem->setCheckState(0,Qt::Unchecked);
        topItem->setText(0,m_viewList.at(i));
        ui->treeWidget->insertTopLevelItem(i,topItem);
    }

    QList<QString>keys = m_map.keys();
    for(int i = 0;i<keys.count();i++)
    {
        int index = 0;
        QTreeWidgetItem *topItem = ui->treeWidget->topLevelItem(i);
        QString key = keys.at(i);
        QList<DBQ::ViewField *>viewField = m_map.value(key);
        for(int j = 0; j<viewField.count();j++)
        {
            DBQ::ViewField *view = viewField.at(j);
            if(typeToString(view->type) != "dateTime")
            {
                QStringList columItemList;
                columItemList<<view->fieldName<<typeToString(view->type)<<view->alias<<view->description;
                QTreeWidgetItem *item = new QTreeWidgetItem(columItemList);
                item->setCheckState(0,Qt::Unchecked);
                topItem->insertChild(index,item);
                index++;
            }
        }
    }
}

QStringList CViewFieldDialog::selectField() const
{
    QStringList selectFields;
    for(int i = 0; i < ui->treeWidget->topLevelItemCount();i++)
    {
        QTreeWidgetItem *topItem = ui->treeWidget->topLevelItem(i);
        for(int j=0; j <topItem->childCount(); j++)
        {
            QTreeWidgetItem *item = topItem->child(j);
            if(item->checkState(0) == Qt::Checked)
            {
               selectFields.append(topItem->text(0)+"."+ item->text(0));
            }
        }
    }
    return selectFields;

}

QString CViewFieldDialog::typeToString(DBQ::VIEW_FIELD_TYPE type)
{
    QString str;
    switch(type)
    {
    case DBQ::VIEW_FIELD_TYPE_UNKNOWN:
        str = "unknown";
        break;
    case DBQ::VIEW_FIELD_TYPE_BOOL:
        str = "bool";
        break;
    case DBQ::VIEW_FIELD_TYPE_SBYTE:
        str = "sbyte";
        break;
    case DBQ::VIEW_FIELD_TYPE_BYTE:
        str = "byte";
        break;
    case DBQ::VIEW_FIELD_TYPE_INT16:
        str = "int16";
        break;
    case DBQ::VIEW_FIELD_TYPE_UINT16:
        str = "Uint16";
        break;
    case DBQ::VIEW_FIELD_TYPE_INT32:
        str = "int32";
        break;
    case DBQ::VIEW_FIELD_TYPE_UINT32:
        str = "Uint32";
        break;

    case DBQ::VIEW_FIELD_TYPE_INT64:
        str = "int64";
        break;
    case DBQ::VIEW_FIELD_TYPE_UINT64:
        str = "Uint64";
        break;

    case DBQ::VIEW_FIELD_TYPE_FLOAT:
        str = "float";
        break;
    case DBQ::VIEW_FIELD_TYPE_DOUBLE:
        str = "double";
        break;

    case DBQ::VIEW_FIELD_TYPE_STRING:
        str = "string";
        break;
    case DBQ::VIEW_FIELD_TYPE_DATETIME:
        str = "dateTime";
        break;
    default:
        break;
    }
    return str;

}

void CViewFieldDialog::slotOk()
{
    accept();
}

void CViewFieldDialog::slotCancel()
{
    close();
}

void CViewFieldDialog::slotSetChecked(QTreeWidgetItem *item, int column)
{
    if(!item->parent() && column == 0)
    {
        for(int i = 0; i <item->childCount();i++)
        {
            if(item->checkState(column))
            {
                QTreeWidgetItem *childItem = item->child(i);
                childItem->setCheckState(column,Qt::Checked);
            }
            if(!item->checkState(column))
            {
                QTreeWidgetItem *childItem = item->child(i);
                childItem->setCheckState(column,Qt::Unchecked);
            }
        }
    }
}


