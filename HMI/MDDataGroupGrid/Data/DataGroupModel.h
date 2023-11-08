/*!
\author bWVpeGw=
\section 报警日志数据模型
*/
#ifndef DATAGROUPMODEL_H
#define DATAGROUPMODEL_H

#include <QList>
#include <QColor>
#include <QFont>
#include <QObject>
#include <QSqlTableModel>
#include "DataGroupData.h"
#include "IDrawEnvironment.h"

class IMDDataGroup;
class CMDDatabaseRecordset;

class CDataGroupModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit CDataGroupModel(QObject *parent = 0);

    virtual int rowCount (const QModelIndex &parent) const;
    virtual int columnCount (const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData (int section, Qt::Orientation orientation, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
/*!
\section 设置字段与数据
*/
public:
    void setFields(QList<CDataGroupField>* fields);     //! 字段内容
    void setDatas(CMDDatabaseRecordset* datas);         //! 数据内容
    void resetModel();                                  //! 刷新模型
    void setPropertyData(CDataGroupBasicData * value);  //! 属性变量
    void setStudioMode(bool isStudioMode);              //! 是否是编辑时
private:
    QList<CDataGroupField>* m_fields;                   //! 字段
    CMDDatabaseRecordset* m_datas;                      //! 字段数据
    CDataGroupBasicData* m_propertyData;                //! 属性数据
    bool m_studioMode;                                  //! 是否是编辑时
};

#endif // DATAGROUPMODEL_H
