/*!

\author eHVr

\section 功能

    说明标签
*/
#ifndef CFEATURETAGCOMMENTITEM_H
#define CFEATURETAGCOMMENTITEM_H
#include "CFeatureTagItem.h"

class CFeatureTagCommentItem : public CFeatureTagItem
{
public:
    CFeatureTagCommentItem();
    ~CFeatureTagCommentItem();
public:
    //! 类型
    virtual ItemType type() const;
public:
    virtual void calculate();
    virtual void drawItem(QPainter *p);
/*!
\section 序列化
*/
public:
    void serialize(QJsonObject& json);

};

#endif // CFEATURETAGCOMMENTITEM_H
