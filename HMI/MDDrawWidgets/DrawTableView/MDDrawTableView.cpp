#include <QHeaderView>
#include "MDDrawTableView.h"
#include "DrawTableViewData.h"
#include "ItemDelegate.h"
#include "PropertyDrawTableView.h"
/////////////////////////////////////////////////////////////////
CMDDrawTableView::CMDDrawTableView()
{
    m_data = new CDrawTableViewData;
    setWidget(m_data->m_widget, NULL);
}
CMDDrawTableView::~CMDDrawTableView()
{
    delete m_data;
}
/////////////////////////////////////////////////////////////////
QTableView *CMDDrawTableView::widget()
{
    return m_data->m_widget;
}
void CMDDrawTableView::setColCount(int value)
{
    if (value < 0)
        return;

    m_data->m_colCount = value;
    m_data->m_model.setColumnCount(value);
}
int CMDDrawTableView::colCount()
{
    return m_data->m_colCount;
}
void CMDDrawTableView::setRowCount(int value)
{
    if (value < 0)
        return;

    m_data->m_rowCount = value;
    m_data->m_model.setRowCount(value);
}
int CMDDrawTableView::rowCount()
{
    return m_data->m_rowCount;
}
void CMDDrawTableView::setColWidth(int value)
{
    if (value < 0)
        return;
    m_data->m_colWidth = value;
    for(int i = 0; i < m_data->m_model.columnCount(); i++)
        widget()->setColumnWidth(i, value);
}
int CMDDrawTableView::colWidth()
{
    return m_data->m_colWidth;
}
void CMDDrawTableView::setRowHeight(int value)
{
    if (value < 0)
        return;
    m_data->m_rowHeight = value;
    for(int i = 0; i < m_data->m_model.rowCount(); i++)
        widget()->setRowHeight(i, value);
}
int CMDDrawTableView::rowHeight()
{
    return m_data->m_rowHeight;
}
void CMDDrawTableView::setHideRowHeader(bool value)
{
    m_data->m_hideRowHeader = value;
    if (m_data->m_hideRowHeader)
        widget()->verticalHeader()->hide();
    else
        widget()->verticalHeader()->show();
}
bool CMDDrawTableView::hideRowHeader()
{
    return m_data->m_hideRowHeader;
}
void CMDDrawTableView::setHideColHeader(bool value)
{
    m_data->m_hideColHeader = value;
    if (m_data->m_hideColHeader)
        widget()->horizontalHeader()->hide();
    else
        widget()->horizontalHeader()->show();
}
bool CMDDrawTableView::hideColHeader()
{
    return m_data->m_hideColHeader;
}
void CMDDrawTableView::setReadOnly(bool value)
{
    m_data->m_readOnly = value;
    if (m_data->m_readOnly)
        widget()->setEditTriggers(QAbstractItemView::NoEditTriggers);
    else
        widget()->setEditTriggers(
            static_cast<QAbstractItemView::EditTrigger>(QAbstractItemView::DoubleClicked
            & QAbstractItemView::EditKeyPressed
            & QAbstractItemView::AnyKeyPressed));
}
bool CMDDrawTableView::readOnly()
{
    return m_data->m_readOnly;
}
/////////////////////////////////////////////////////////////////
void CMDDrawTableView::setItemTypeRange(int row, int rowCount, int col, int colCount, int type)
{
    bool r0 = checkIndex(row, rowCount, true);
    bool r1 = checkIndex(col, colCount, false);
    if (!r0 || !r1)
        return;

    for(int i = row; i < rowCount; i++)
        for(int j = col; j < colCount; j++)
            setItemType(i, j, (int)type);
}
int CMDDrawTableView::itemType(int row, int col)
{
    QStandardItem* it = item(row, col);
    if (it == NULL)
        return 0;

    return it->data().toInt();
}
void CMDDrawTableView::setItemValueRange(int row, int rowCount, int col, int colCount, const QVariant &value)
{
    bool r0 = checkIndex(row, rowCount, true);
    bool r1 = checkIndex(col, colCount, false);
    if (!r0 || !r1)
        return;

    for(int i = row; i < rowCount; i++)
        for(int j = col; j < colCount; j++)
            setItemValue(i, j, value.toString());
}
QVariant CMDDrawTableView::itemValue(int row, int col)
{
    QStandardItem* it = item(row, col);
    if (it == NULL)
        return QVariant();

    return it->text();
}
void CMDDrawTableView::setRowHeaderText(int index, const QString &value)
{
    QStandardItem* it = rowHeader(index);
    if (it == NULL)
        return;

    it->setText(value);
}
void CMDDrawTableView::setColHeaderText(int index, const QString &value)
{
    QStandardItem* it = colHeader(index);
    if (it == NULL)
        return;

    it->setText(value);
}
void CMDDrawTableView::setSingleRowHeight(int index, int value)
{
    int count = m_data->m_model.rowCount();
    if (index < 0 || index >= count)
        return;
    if (value < 0)
        return;

    widget()->setRowHeight(index, value);
}
void CMDDrawTableView::setSingleColWidth(int index, int value)
{
    int count = m_data->m_model.columnCount();
    if (index < 0 || index >= count)
        return;
    if (value < 0)
        return;

    widget()->setColumnWidth(index, value);
}
void CMDDrawTableView::setReadOnlyRowRange(int row, int rowCount, bool value)
{
    DelegateType type = DelegateType::Normal;
    if (value)
       type = DelegateType::ReadOnly;

    setDelegate(type, true, row, rowCount);
}
void CMDDrawTableView::setReadOnlyColRange(int col, int colCount, bool value)
{
    DelegateType type = DelegateType::Normal;
    if (value)
       type = DelegateType::ReadOnly;

    setDelegate(type, false, col, colCount);
}
void CMDDrawTableView::setDecimalRowRange(int row, int rowCount, int value)
{
    setDelegate(DelegateType::Double, true, row, rowCount, value);
}
void CMDDrawTableView::setDecimalColRange(int col, int colCount, int value)
{
    setDelegate(DelegateType::Double, false, col, colCount, value);
}
void CMDDrawTableView::setItemType(int row, int col, int type)
{
    QStandardItem* it = item(row, col);
    if (it == NULL)
        return;
    it->setData((int)type);
}
void CMDDrawTableView::setItemValue(int row, int col, const QVariant &value)
{
    QStandardItem* it = item(row, col);
    if (it == NULL)
        return;
    it->setText(value.toString());
}
QStandardItem *CMDDrawTableView::item(int row, int col)
{
    int rowCount = m_data->m_model.rowCount();
    int colCount = m_data->m_model.columnCount();
    if ((row < 0 || row >= rowCount)
        ||(col < 0 || col >= colCount))
        return NULL;

    QStandardItem* item = m_data->m_model.item(row, col);
    if (item == NULL)
    {
        item = new QStandardItem();
        item->setData((int)ItemType::String);
        m_data->m_model.setItem(row, col, item);
    }

    return item;
}
QStandardItem *CMDDrawTableView::rowHeader(int row)
{
    int rowCount = m_data->m_model.rowCount();
    if (row < 0 || row >= rowCount)
        return NULL;
    QStandardItem* item = m_data->m_model.verticalHeaderItem(row);
    if (item == NULL)
    {
        item = new QStandardItem();
        m_data->m_model.setVerticalHeaderItem(row, item);
    }

    return item;
}
QStandardItem *CMDDrawTableView::colHeader(int col)
{
    int colCount = m_data->m_model.columnCount();
    if (col < 0 || col >= colCount)
        return NULL;
    QStandardItem* item = m_data->m_model.horizontalHeaderItem(col);
    if (item == NULL)
    {
        item = new QStandardItem();
        m_data->m_model.setHorizontalHeaderItem(col, item);
    }

    return item;
}
bool CMDDrawTableView::checkIndex(int &start, int &count, bool isRow)
{
    int c = 0;
    if (isRow)
        c = m_data->m_model.rowCount();
    else
        c = m_data->m_model.columnCount();
    if (c == 0)
        return false;

    start = (start < 0) ? 0 : start;
    start = (start > (c - 1)) ? (c - 1) : start;
    count = (count < 1) ? 1 : count;
    count += start;
    count = (count > c) ? c : count;

    return true;
}
void CMDDrawTableView::setDelegate(CMDDrawTableView::DelegateType type, bool isRow,
    int start, int count, int decimal/* = 0*/)
{
    bool r = checkIndex(start, count, isRow);
    if (!r)
        return;
    decimal = (decimal < 0) ? 0 : decimal;

    QItemDelegate* delegate = NULL;
    switch (type)
    {
    case DelegateType::Normal:
        delegate = new QItemDelegate();
        break;
    case DelegateType::ReadOnly:
        delegate = new CReadOnlyDelegate();
        break;
    case DelegateType::Double:
        delegate = new CDoubleDelegate(decimal);
        break;
    default:
        break;
    }

    for(int i = start; i < count; i++)
    {
        if (isRow)
            widget()->setItemDelegateForRow(i, delegate);
        else
            widget()->setItemDelegateForColumn(i, delegate);
    }

    //deleteList
    m_data->m_delegateList.append(delegate);
}
/////////////////////////////////////////////////////////////////
DrawType CMDDrawTableView::type()
{
    return DrawType::TableView;
}
CMDPropertyDrawObj *CMDDrawTableView::createPropertyDrawObj()
{
    return new CPropertyDrawTableView(this);
}
void CMDDrawTableView::onInitialization()
{
    //resetColor会导致字体设置失效
    CMDDrawBasicWidget::onInitialization();

    m_data->m_widget->setModel(&m_data->m_model);
    m_data->m_model.setColumnCount(m_data->m_colCount);
    m_data->m_model.setRowCount(m_data->m_rowCount);
    int count = m_data->m_model.columnCount();
    for(int i = 0; i < count; i++)
        widget()->setColumnWidth(i, m_data->m_colWidth);
    count = m_data->m_model.rowCount();
    for(int i = 0; i < count; i++)
        widget()->setRowHeight(i, m_data->m_rowHeight);
    if (m_data->m_hideRowHeader)
        widget()->verticalHeader()->hide();
    else
        widget()->verticalHeader()->show();
    if (m_data->m_hideColHeader)
        widget()->horizontalHeader()->hide();
    else
        widget()->horizontalHeader()->show();
    if (m_data->m_readOnly)
        widget()->setEditTriggers(QAbstractItemView::NoEditTriggers);
    else
        widget()->setEditTriggers(
            QAbstractItemView::DoubleClicked
            | QAbstractItemView::EditKeyPressed
            | QAbstractItemView::AnyKeyPressed);
}
CMDDrawObj *CMDDrawTableView::createSelf()
{
    return new CMDDrawTableView();
}
/////////////////////////////////////////////////////////////////
CMDDrawObj *CMDDrawTableView::clone()
{
    CMDDrawTableView* obj = static_cast<CMDDrawTableView*>(CMDDrawBasicWidget::clone());
    *obj->m_data = *m_data;
    return obj;
}
/////////////////////////////////////////////////////////////////

