/*!
\author Y2hhaWhq

\section 功能

单选按钮控件数据类
*/
#ifndef MDDRAWRADIOBUTTONDATA_H
#define MDDRAWRADIOBUTTONDATA_H

#include <QRadioButton>
#include <QButtonGroup>
#include <QMap>
#include <QJsonObject>
#include "MDExpInfo.h"

class CMDDrawRadioButtonEvent;

class CDrawRadioButtonData
{
public:
    CDrawRadioButtonData();
    ~CDrawRadioButtonData(){}

public:
    QRadioButton* m_widget;                             //! 单选按钮
    CMDDrawRadioButtonEvent* m_event;                   //! 事件绑定类

    bool m_checked;                                     //! 选中
    qint32 m_groupId;                                   //! 分组
    static QMap<QString, QButtonGroup*> m_buttonGroups; //! 分组设置
    QString m_text;                                     //! 文本
    qreal m_checkValue;                                 //! 选择值
    QString m_showText;                                 //! 翻译文本

    CMDExpInfo* m_checkExpression;

/*!
\section  序列化
*/
public:
    void operator=(const CDrawRadioButtonData& other);

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

#endif  //MDDRAWRADIOBUTTONDATA_H
