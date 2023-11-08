#include "DataArchivesTabModel.h"
#include "IMDDataTable.h"
#include "IMDDataField.h"
#include "IMDDataGroup.h"
#include "IMDDataItem.h"
#include "MDMultiLanguage.h"

#include <QIcon>
#include <QTableWidget>

CDataArchivesTabModel::CDataArchivesTabModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    m_iCurRows       = 0;     //! 行数
    m_pDataTable     = NULL;  //! 表
    m_pDataGroup     = NULL;  //! 数据组
    m_Cols.clear();
}

CDataArchivesTabModel::~CDataArchivesTabModel()
{

}
int CDataArchivesTabModel::insertColumn(int nCol,const QString &name)
{
    m_Cols.insert(nCol, name);
    return nCol + 1;
}

int CDataArchivesTabModel::getColumnCount()
{
    return m_Cols.size();
}

void CDataArchivesTabModel::setItemCount(int iRows)
{
    beginResetModel();
    m_iCurRows = iRows;
    endResetModel();
}

int CDataArchivesTabModel::getItemCount()
{
    return m_iCurRows;
}

void CDataArchivesTabModel::setDataTable(IMDDataTable *pTable)
{
    if (NULL == pTable)
    {
        return;
    }

    m_pDataTable = pTable;
    setItemCount(m_pDataTable->dataFieldCount());
}

void CDataArchivesTabModel::setDataGroup(IMDDataGroup *pDataGrp)
{
    if (NULL == pDataGrp)
    {
        return;
    }

    m_pDataGroup = pDataGrp;
    setItemCount(m_pDataGroup->itemCount());
}

QVariant CDataArchivesTabModel::onDataGrpDisplayInfo(const QModelIndex &index, int /*role*/) const
{
    if (NULL == m_pDataGroup)
    {
        return QVariant();
    }

    QString strTxt = "";
    int iRow = index.row();
    int iCol = index.column();

    IMDDataItem *pItem = m_pDataGroup->getItem(iRow);
    if(NULL == pItem)
    {
        return QVariant();
    }

    switch (iCol)
    {
    case 0:
        strTxt = pItem->name();
        break;
    case 1:
        strTxt = dataTypeToString(pItem->dataType());
        break;
    case 2:
        strTxt = QString("%1").arg(pItem->isFixed() ? "系统" : "自定义");
        break;
    case 3:
        strTxt = CMDMultiLanguage::instance()->value(pItem->description());
        break;
    default:
        break;
    }

    return strTxt;
}

QString CDataArchivesTabModel::dataTypeToString(MDDataType type) const
{
    switch(type)
    {
    case MDDataType::Bool:
    case MDDataType::SByte:
    case MDDataType::Byte:
    case MDDataType::Int16:
    case MDDataType::UInt16:
    case MDDataType::Int32:
    case MDDataType::UInt32:
    case MDDataType::Int64:
    case MDDataType::UInt64:
        return QString("Int");
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

QVariant CDataArchivesTabModel::onFieldDisplayInfo(const QModelIndex &index, int /*role*/) const
{
    if (NULL == m_pDataTable)
    {
        return QVariant();
    }

    int iRow = index.row();
    int iCol = index.column();
    QString strText = "";

    IMDDataField *pField = m_pDataTable->getDataField(iRow);
    if(NULL == pField)
    {
        return QVariant();
    }

    switch (iCol)
    {
    case 0://字段名
        strText = pField->name();
        break;
    case 1://对象名
        strText = pField->dataItemName();
        break;
    case 2://对象数据类型
        strText = dataTypeToString(pField->dataItemDataType());
        break;
    case 3:
        strText = QString("%1").arg(pField->isPrimarykey() ? "True" : "False");
        break;
    case 4:
        strText = QString("%1").arg(pField->isEnableNull() ? "Yes" : "No");
        break;
    case 5:
        strText = QString("%1").arg(pField->description());
        break;
    case 6:
        strText = QString("%1").arg(pField->isRtCurveEnable() ? "Yes" : "No");
        break;
    case 7:
        strText = QString("%1").arg(pField->isHisCurveEnable() ? "Yes" : "No");
        break;
    case 8:
        strText = QString("%1").arg(pField->isHisTableEnable() ? "Yes" : "No");
        break;
    case 9:
        strText = QString("%1").arg(pField->isHisFilterEnable() ? "Yes" : "No");
        break;
    case 10:
        strText = QString("%1").arg(pField->order());
        break;
    case 11:
        strText = QString("%1").arg(pField->lineWidth());
        break;
    case 12:
        strText = QString("%1").arg(pField->color());
        break;
    case 13:
        strText = QString("%1").arg(pField->ext());
        break;
    default:
        break;
    }

    return strText;
}

int CDataArchivesTabModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_iCurRows;
}

int CDataArchivesTabModel::columnCount(const QModelIndex & /*parent*/) const
{
    return m_Cols.size();
}

QVariant CDataArchivesTabModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    if (m_pDataTable)
    {
        return onFieldDisplayInfo(index, role);
    }

    if (m_pDataGroup)
    {
        return onDataGrpDisplayInfo(index, role);
    }

    return QVariant();
}

QVariant CDataArchivesTabModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if(orientation == Qt::Vertical)
        {
            QChar ch = '0';
            return QString(QString("%1").arg(section + 1,3,10,ch));
        }
        else if(orientation == Qt::Horizontal)
        {
            if(m_Cols.size() > section)
            {
                return m_Cols.at(section);
            }
            else
            {
                return "";
            }
        }
    }

    if (role == Qt::DecorationRole)
    {
        //return QVariant::fromValue(services);
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}
