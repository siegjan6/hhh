/*!
\author aGRw 2015.08.15
\section 动态颜色数据类
*/
#ifndef DYNAMICCOLOR_H
#define DYNAMICCOLOR_H

#include "MDColorManager/MDColorManager.h"

class CMDDrawObj;

enum class DynamicType
{
    Brush,
    Pen
};

/*!
\section 动态颜色
*/
class CDynamicColor
{
public:
    CDynamicColor(DynamicType type);
    ~CDynamicColor();
/*!
\section property
*/
public:
    CMDBrushManager* brush(){return m_brush;}
    CMDPenManager* pen(){return m_pen;}
    void setMin(double value){m_min = value;}
    double min(){return m_min;}
    void setMax(double value){m_max = value;}
    double max(){return m_max;}
    void initColor(const QColor& color);
private:
    CMDBrushManager* m_brush;
    CMDPenManager* m_pen;
    double m_min;
    double m_max;
/*!
\section public funtion
*/
public:
    void getBrushes(QList<CMDBrushManager*>& list);
    void getPens(QList<CMDPenManager*>& list);
    void setImagePath(const QString& value);
    //!是否在范围内
    bool isInRange(double value);
public:
    void operator=(const CDynamicColor& other);
    void serialize(QJsonObject &json);
    void deserialize(const QJsonObject &json);
/*!
\section private funtion
*/
private:
    void release();
};

/*!
\section 动态颜色管理类
*/
class CDynamicColorManager
{
public:
    CDynamicColorManager();
    CDynamicColorManager(DynamicType type);
    ~CDynamicColorManager();
/*!
\section property
*/
public:
    QList<CDynamicColor*>* list(){return &m_list;}
    CMDBrushManager* brush(){return m_brush;}
    CMDPenManager* pen(){return m_pen;}
    DynamicType type(){return m_type;}
    CDynamicColor* insert(int index);
    void showDialog(int index);
    void setContainer(CMDDrawObj* value);
    CMDDrawObj* container();
private:
    QList<CDynamicColor*> m_list;
    CMDBrushManager* m_brush;
    CMDPenManager* m_pen;
    DynamicType m_type;
    CMDDrawObj* m_container;

/*!
\section public funtion
*/
public:
    void getBrushes(QList<CMDBrushManager*>& list);
    void getPens(QList<CMDPenManager*>& list);
    void resetValue(double value);
public:
    void operator=(const CDynamicColorManager& other);
    void serialize(QJsonObject &json);
    void deserialize(const QJsonObject &json);
/*!
\section private funtion
*/
private:
    void release();
};

#endif // DYNAMICBRUSH_H
