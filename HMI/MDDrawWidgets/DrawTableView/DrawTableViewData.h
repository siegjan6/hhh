/*!
\author aGRw 2015.12.16
\section 表格数据类
*/
#ifndef DRAWTABLEVIEWDATA_H
#define DRAWTABLEVIEWDATA_H

#include <QTableView>
#include <QStandardItemModel>
#include <QItemDelegate>

class CDrawTableViewData
{
public:
    CDrawTableViewData();
    ~CDrawTableViewData();
public:
    QTableView* m_widget;
    QStandardItemModel m_model;

    qint32 m_colCount;//!列数
    qint32 m_rowCount;//!行数
    qint32 m_colWidth;//!列宽
    qint32 m_rowHeight;//!行高
    bool m_hideRowHeader;//!隐藏行头
    bool m_hideColHeader;//!隐藏列头
    bool m_readOnly;//！只读

    QList<QItemDelegate*> m_delegateList;
/*!
\section  序列化
*/
public:
    void operator=(const CDrawTableViewData& other);
};

#endif // DRAWTABLEVIEWDATA_H
