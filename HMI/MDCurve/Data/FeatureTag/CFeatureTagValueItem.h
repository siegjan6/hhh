/*!

\author eHVr

\section 功能

    数值子标签
*/
#ifndef CFEATURETAGVALUEITEM_H
#define CFEATURETAGVALUEITEM_H
#include "CFeatureTagItem.h"

class CFeatureTagValueItem : public CFeatureTagItem
{
public:
    CFeatureTagValueItem();
    virtual ~CFeatureTagValueItem();

public:
    virtual double value() const;
    virtual void setValue(double value);
private:
    double  m_value;//! 变量值
    QString m_drawText; //! 需要绘制的文本

public:
    virtual ItemType type() const; //! 类型
/*!
\section 计算，绘制
*/
public:
    virtual void calculate();
    virtual void drawItem(QPainter *p);
//! 序列化
public:
    void serialize(QJsonObject& json);

};

#endif // CFEATURETAGVALUEITEM_H
