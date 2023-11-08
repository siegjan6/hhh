#include "DataGroupModel.h"
#include "IMDDataGroup.h"
#include "IMDDataItem.h"
#include "MDDatabaseRecordset.h"
#include <QDebug>

CDataGroupModel::CDataGroupModel(QObject *parent) : QSqlTableModel(parent)
{
    m_fields = NULL;
    m_datas = NULL;
    m_propertyData = new CDataGroupBasicData;
}

int CDataGroupModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if(!m_studioMode)
        return (m_datas ? m_datas->rowCount(): 0);
    else
        return 1;
}

int CDataGroupModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return (m_fields ? m_fields->count() : 0);
}

QVariant CDataGroupModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }
    if(index.column() > m_fields->count())
    {
        return QVariant();
    }
    int row = index.row();
    int col = index.column();
    QString showFields = m_fields->at(col).name();
    if(role  ==  Qt::DisplayRole)
    {
        if(!m_studioMode)
        {
            bool ok = false;
            if(m_datas->value(row,showFields).type() == QVariant::DateTime)
                return QVariant(m_datas->value(row,showFields).toDateTime().toString(m_fields->at(col).dataFormat()));
            else if(m_datas->value(row,showFields).type() == QVariant::Double)
                return QVariant(QString::number(m_datas->value(row,showFields).toDouble(&ok),'f',m_fields->at(col).dataFormat().mid(1,1).toInt(&ok,10)));
            else if(m_datas->value(row,showFields).type() == QMetaType::Float)
                return QVariant(QString::number(m_datas->value(row,showFields).toFloat(&ok),'f',m_fields->at(col).dataFormat().mid(1,1).toInt(&ok,10)));
            else
               return QVariant(m_datas->value(row,showFields));
        }
        else
        {
            if(showFields == "RecordTime" || showFields == "StartTime")
                return QVariant(QDateTime::currentDateTime().toString(m_fields->at(col).dataFormat()));
            else
                return QVariant(QString(""));
        }
    }
    else if(role == Qt::ForegroundRole)
    {
        if( !(m_propertyData->m_gridType == GridType::Alarm) || !(m_propertyData->m_isFieldColor))
        {
            return QVariant(QColor(m_propertyData->m_dataColor));
        }
        else
        {
            if(!m_studioMode)
            {
                if(m_propertyData->m_gridType == GridType::Alarm)
                {
                    bool ok = false;
                    int frontCol = m_propertyData->m_allFieldsName.indexOf(m_propertyData->m_frontColorField);
                    return QVariant(QColor(m_datas->value(row,frontCol).toUInt(&ok)));
                }
            }
        }
    }
    else if(role == Qt::BackgroundRole)
    {
        if(!(m_propertyData->m_gridType == GridType::Alarm) ||!(m_propertyData->m_isFieldColor))
        {
            return QVariant(QColor(m_propertyData->m_tabColor));
        }
        else
        {
            if(!m_studioMode){
                if(m_propertyData->m_gridType == GridType::Alarm)
                {
                    bool ok = false;
                    int backCol = m_propertyData->m_allFieldsName.indexOf(m_propertyData->m_backColorField);
                    qDebug() << "backCol ="<< backCol << "row ="<<row;
                    return QVariant(QColor(m_datas->value(row,backCol).toUInt(&ok)));
                }
            }
        }
    }
    else if(role == Qt::FontRole)
    {
        return m_propertyData->m_dataFont;
    }
    else if(role == Qt::TextAlignmentRole)
    {
        return Qt::AlignCenter;
    }
    return QVariant();
}

QVariant CDataGroupModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Vertical)
    {
        if(role == Qt::DisplayRole)
        {
            QChar ch = '0';
            return QString(QString("%1").arg(section + 1,4,10,ch));
        }
        else if(role == Qt::FontRole)
        {
            return m_propertyData->m_colFont;
        }
        else if(role == Qt::ForegroundRole)
        {
            return m_propertyData->m_colForeColor;
        }
    }
    else if(orientation == Qt::Horizontal)
    {
        if(role == Qt::DisplayRole)
        {
            if(m_fields)
            {
                if(!m_fields->at(section).comment().isEmpty())
                    return m_fields->at(section).comment();
                else
                    return m_fields->at(section).name();
            }
        }
        else if(role == Qt::FontRole)
        {
            return m_propertyData->m_rowFont;
        }
        else if(role == Qt::ForegroundRole)
        {
            return m_propertyData->m_rowForeColor;
        }
    }
    return QSqlTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags CDataGroupModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractTableModel::flags(index)|Qt::ItemIsSelectable;
}

void CDataGroupModel::setFields(QList<CDataGroupField> *fields)
{
    m_fields = fields;
}

void CDataGroupModel::setDatas(CMDDatabaseRecordset *datas)
{
    m_datas = datas;
    resetModel();
}

void CDataGroupModel::resetModel()
{
    beginResetModel();
    endResetModel();
}

void CDataGroupModel::setPropertyData(CDataGroupBasicData *value)
{
    m_propertyData = value;
}

void CDataGroupModel::setStudioMode(bool isStudioMode)
{
    m_studioMode = isStudioMode;
}
