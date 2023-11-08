/*!
author Y2hvdWps 2016.10.24
section 数值映射表表格数据模型类
详细说明
*/
#ifndef VALUEMAPMODEL_H
#define VALUEMAPMODEL_H

#include <QStandardItemModel>
#include "ValueMapManager.h"
#include <QSet>

class CValueMapView;

class CValueMapModel : public QStandardItemModel
{
public:
    explicit CValueMapModel(int rows, int columns, CValueMapView * parent);
    virtual ~CValueMapModel(){}

public:
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool dropMimeData (const QMimeData *, Qt::DropAction, int, int, const QModelIndex &parent);
private:
    CValueMapView* m_mainView;
    QSet<QString> m_nameSet;    //!用于检查值是否重名
};

#endif // VALUEMAPMODEL_H
