/*!
\author Y2hhaWhq

\section 功能

组合框数据类
*/
#ifndef DRAWCOMBOBOXDATA_H
#define DRAWCOMBOBOXDATA_H

#include <QComboBox>
#include <QJsonObject>
#include "DrawObjDefine.h"
class CMDDrawComboBoxEvent;

class CDrawComboBoxData
{
public:
    CDrawComboBoxData();
    ~CDrawComboBoxData(){}

public:
    void operator = (const CDrawComboBoxData& other);
    //! 序列化
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

    //! 组合框
    inline QComboBox *comboBox(){return m_comboBox;}
    inline void setComboBox(QComboBox *comboBox){m_comboBox = comboBox;}
    //! 事件绑定
    inline CMDDrawComboBoxEvent *event(){return m_event;}
    inline void setEvent(CMDDrawComboBoxEvent *event){m_event = event;}
    //! 滚动条位置
    inline bool layoutDirection(){return m_layoutDirection;}
    inline void setLayoutDirection(bool layoutDirection){m_layoutDirection = layoutDirection;}
    //! 下拉类型
    inline qint32 itemStyle(){return m_itemStyle;}
    inline void setItemStyle(qint32 &itemStyle){m_itemStyle = itemStyle;}
    //! 当前索引
    inline qint32 currentIndex(){return m_currentIndex;}
    inline void setCurrentIndex(qint32 &currentIndex){m_currentIndex = currentIndex;}
    //! 当前文本
    inline QString currentText(){return m_currentText;}
    inline void setCurrentText(QString &currentText){m_currentText = currentText;}
    //! 子项排序
    inline bool sort(){return m_sort;}
    inline void setSort(bool sort){m_sort = sort;}


public:
    QStringList m_itemAdd;                  //! 添加控件子项

private:
    QComboBox* m_comboBox;                  //! 组合框
    CMDDrawComboBoxEvent* m_event;          //! 事件绑定
    bool m_layoutDirection;                 //! 滚动条位置
    qint32 m_itemStyle;                     //! 下拉类型
    qint32 m_currentIndex;                  //! 当前索引
    QString m_currentText;                  //! 当前文本
    bool m_sort;                            //! 子项排序

};

#endif // DRAWCOMBOBOXDATA_H
