/*!

\author dGFuZ3p3

\section 功能

日志数据的“表”模型类。

*/

#ifndef MDSYSLOGTABLEMODEL_H
#define MDSYSLOGTABLEMODEL_H

#include <QAbstractTableModel>

class CMDSysLogger;
class CMDSysLogTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CMDSysLogTableModel(QObject *parent = 0);

public:
    void setLogger(CMDSysLogger* logger);
    void update();
private:
    CMDSysLogger* m_logger ;// NULL;
    QString levelComment(int level) const;

public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
};

#endif // MDSYSLOGTABLEMODEL_H
