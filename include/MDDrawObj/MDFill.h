/*!
\author aGRw 2015.01.25
\section 填充设置类
*/
#ifndef MDFILL_H
#define MDFILL_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWOBJ_LIBRARY)
#  define MDDRAWOBJSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWOBJSHARED_EXPORT Q_DECL_IMPORT
#endif

#include "MDColorManager/MDColorManager.h"
#include "MDExpInfo.h"

class CDynamicColorManager;
class CMDDrawObj;

class MDDRAWOBJSHARED_EXPORT CMDFill
{
public:
    CMDFill();
    ~CMDFill();
/*!
\section enum
*/
enum class FillDirection
{
    Top, Bottom, Left, Right
};
/*!
\section public function
*/
public:
    void draw(QPainter* painter, const QRectF& rect, QPainterPath* path);
/*!
\section common
*/
public:
    void operator=(const CMDFill& other);
    void serialize(QJsonObject &json);
    void deserialize(const QJsonObject &json);
/*!
\section property
*/
public:
    void setScaleValue(qreal value);
    void setDirection(FillDirection value){m_direction = value;}
    FillDirection direction(){return m_direction;}
    CMDBrushManager* brush(){return &m_brushManager;}
    void setMax(qreal value){m_max = value;}
    qreal max(){return m_max;}
    void setMin(qreal value){m_min = value;}
    qreal min(){return m_min;}
    void setDynamicBrush(CDynamicColorManager* value);
    CDynamicColorManager* dynamicBrush();
    void setContainer(CMDDrawObj* value);
    CMDDrawObj* container();
    QString text();
private:
    //!run
    qreal m_scaleValue;//！填充比例值
    //!studio
    FillDirection m_direction;
    CMDBrushManager m_brushManager;
    qreal m_max;//!最大值范围
    qreal m_min;//!最小值范围
    CDynamicColorManager* m_dynamicBrush;
/*!
\section public funtion
*/
public:
    void getBrushes(QList<CMDBrushManager*>& list);
    void generateContent(const QRectF& rect);
    //!变量变化事件
    void loadDataChangedEvent(const QRectF &rect, CMDExpInfo *expression);

/*!
\section private funtion
*/
private:
    QRectF calculateRect(const QRectF& rect, qreal scaleValue, FillDirection direction);
};

#endif // MDFILL_H
