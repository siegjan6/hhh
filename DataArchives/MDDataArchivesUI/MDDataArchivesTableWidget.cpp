#include "MDDataArchivesTableWidget.h"
#include "IMDDataGroup.h"
#include "IMDDataItem.h"
#include "MDMultiLanguage.h"

#include <QHeaderView>
#include <QApplication>

CMDDataArchivesTableWidget::CMDDataArchivesTableWidget(QWidget *parent)
    : QTableWidget(parent),
      m_iRow (0)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

CMDDataArchivesTableWidget::~CMDDataArchivesTableWidget()
{

}

void CMDDataArchivesTableWidget::initData(IMDDataGroup *pDataGrp)
{
    if (NULL == pDataGrp)
    {
        return;
    }

    int count = pDataGrp->itemCount();
    if (count == rowCount())
    {
        return;
    }

    IMDDataItem *pItem = NULL;
    clearContents();
    setRowCount(0);
    m_iRow = 0;
    QString strPath = "";
    setRowCount(count);
    for (int i = 0; i < count; ++i)
    {
        pItem = pDataGrp->getItem(i);
        if (NULL == pItem)
        {
            continue;
        }

        //        insertRow(0);
        strPath = ":/images/lock.png";
        if (!pItem->isFixed())
        {
            strPath = ":/images/unlock.png";
        }

        QTableWidgetItem *pTabItemName = new QTableWidgetItem(QIcon(strPath), pItem->name());;
        setItem(m_iRow, TAB_COL_ZERO, pTabItemName);
        QTableWidgetItem *pTabItemType = new QTableWidgetItem(dataTypeToString(pItem->dataType()));
        setItem(m_iRow, TAB_COL_FIRST, pTabItemType);
        // LD wait...
        QTableWidgetItem *pTabItemDecrip = new QTableWidgetItem(CMDMultiLanguage::instance()->value(pItem->description()));
        setItem(m_iRow, TAB_COL_SECOND, pTabItemDecrip);

        ++m_iRow;
    }
}

void CMDDataArchivesTableWidget::setHorizontalHeaderText(const QStringList &titleList)
{
    setColumnCount(titleList.count());
    setHorizontalHeaderLabels(titleList);
}

QString CMDDataArchivesTableWidget::dataTypeToString(MDDataType type)
{
    switch(type)
    {
    case MDDataType::Bool:
        return QString("Bool");
    case MDDataType::SByte:
        return QString("SByte");
    case MDDataType::Byte:
        return QString("Byte");
    case MDDataType::Int16:
        return QString("Int16");
    case MDDataType::UInt16:
        return QString("UInt16");
    case MDDataType::Int32:
        return QString("Int32");
    case MDDataType::UInt32:
        return QString("UInt32");
    case MDDataType::Int64:
        return QString("Int64");
    case MDDataType::UInt64:
        return QString("UInt64");
    case MDDataType::Float:
        return QString("Float");
    case MDDataType::Double:
        return QString("Double");
    case MDDataType::DateTime:
        return QString("DateTime");
    default:
        return QString("String");
    }

    return QString();
}

