#ifndef DATAARCHIVESTABMODEL_H
#define DATAARCHIVESTABMODEL_H

/*!

\author luochao

\section 功能
1.数据归档tableview模型派生实现类。
2.通过传入表指针或者数据组指针改变table的实现

*/

#include <QAbstractTableModel>
#include <QVariant>
#include "../../include/interface/Common/IMDDataType.h"


class IMDDataTable;
class IMDDataGroup;

class CDataArchivesTabModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CDataArchivesTabModel(QObject *parent = 0);
    ~CDataArchivesTabModel();
    /*!
    \section 属性配置
    */
public:
    int insertColumn(int nCol,const QString &name);
    int getColumnCount();

    void setItemCount(int iRows);
    int getItemCount();
    void setDataTable(IMDDataTable *pTable);
    void setDataGroup(IMDDataGroup *pDataGrp);

    /*!
    \section 派生类重新实现方法
    */
private:
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    /*!
    \section 根据传入不同的指针更改界面呈现
    */
private:
    QVariant onFieldDisplayInfo(const QModelIndex &index, int /*role*/) const;
    QVariant onDataGrpDisplayInfo(const QModelIndex &index, int /*role*/) const;
    QString dataTypeToString(MDDataType type) const;

private:
    QStringList m_Cols;                 //! 列名集合
    int m_iCurRows                 ;   //! 行数
    IMDDataTable *m_pDataTable     ;  //! 表
    IMDDataGroup *m_pDataGroup     ;  //! 数据组
};

#endif // DATAARCHIVESTABMODEL_H
