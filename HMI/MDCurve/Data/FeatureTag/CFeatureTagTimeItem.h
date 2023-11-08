/*!

\author eHVr

\section 功能

    时间子标签
*/
#ifndef CFEATURETAGTIMEITEM_H
#define CFEATURETAGTIMEITEM_H
#include "CFeatureTagItem.h"
#include <QDateTime>

class CFeatureTagTimeItem : public CFeatureTagItem
{
public:
    CFeatureTagTimeItem();
    virtual ~CFeatureTagTimeItem();

    QDateTime dateTime() const;
    void setDateTime(const QDateTime &dateTime);
private:
    QDateTime m_dateTime; //! 时间值
    QString   m_drawText; //! 需要绘制的文本

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

#endif // CFEATURETAGTIMEITEM_H
