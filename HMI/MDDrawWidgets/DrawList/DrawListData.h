/*!
\author Y2hhaWhq

\section 功能

列表控件数据类
*/
#ifndef DRAWLISTWIDGETDATA_H
#define DRAWLISTWIDGETDATA_H

#include <QListWidget>
#include <QJsonObject>

class CMDDrawListEvent;

class CDrawListData
{
public:
    CDrawListData();
    ~CDrawListData(){}

public:
    QListWidget* m_widget;                              //! 列表控件
    CMDDrawListEvent* m_event;                          //! 事件绑定类

    bool m_hasScrollBar;                                //! 滚动条
    QFrame::Shape m_borderStyle;                        //! 边框
    QAbstractItemView::SelectionMode m_selectionMode;   //! 选择模式
    Qt::LayoutDirection m_layoutDirection;              //! 滚动条位置
    qint32 m_sorted;                                    //! 子项排序
    QStringList m_itemAdd;                              //! 子项

/*!
\section  序列化
*/
public:
    void operator = (const CDrawListData &other);

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

#endif // DRAWLISTWIDGETDATA_H
