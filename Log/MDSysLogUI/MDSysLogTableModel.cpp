#include "MDSysLogTableModel.h"
#include "MDSysLogger.h"
#include "MDSysLogData.h"
#include <QSize>
#include "MDStringResource.h"

CMDSysLogTableModel::CMDSysLogTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    m_logger = NULL;
}

void CMDSysLogTableModel::setLogger(CMDSysLogger* logger)
{
    beginResetModel();
    m_logger = logger;
    endResetModel();
}

void CMDSysLogTableModel::update()
{
    beginResetModel();
    endResetModel();
}

QString CMDSysLogTableModel::levelComment(int level) const
{
    QString comment;
    switch (level) {
    case 128: comment = STRANS(STRING_INFO);
        break;
    case 150: comment = STRANS(STRING_WARN);
        break;
    case 182: comment = STRANS(STRING_ERROR);
        break;
    default:;
        break;
    }
    return comment;
}

int CMDSysLogTableModel::rowCount(const QModelIndex & /* parent */) const
{
    return m_logger->logCount();
}

int CMDSysLogTableModel::columnCount(const QModelIndex & /* parent */) const
{
    return 4;
}

QVariant CMDSysLogTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    CMDSysLogData* log = m_logger->log(index.row());
    if(!log)
         return QVariant();

    switch (index.column()) {
    case 0:
        return log->timeStamp_ISO8601();
        break;
    case 1:
        return levelComment(log->level());
        break;
    case 2:
        return log->message();
        break;
    case 3:
        return log->source();
        break;
    default:
        break;
    }

    return QVariant();
}

QVariant CMDSysLogTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if(orientation == Qt::Vertical)
        {
            int rowNum = m_logger->logCount() - section;
            QChar ch = '0';
            return QString(QString("%1").arg(rowNum,0,10,ch));
        } else if(orientation == Qt::Horizontal)
        {
            switch(section)
            {
            case 0:return STRANS(STRING_LOG_TIME);break;
            case 1:return STRANS(STRING_LOG_LEVEL);break;
            case 2:return STRANS(STRING_LOG_EVENT);break;
            case 3:return STRANS(STRING_LOG_SOURCE);break;
            default: return tr("");
            }
        }
    }
    if (role == Qt::DecorationRole)
    {
        //return QVariant::fromValue(services);
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}

