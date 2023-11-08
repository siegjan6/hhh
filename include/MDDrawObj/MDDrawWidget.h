/*!
\author aGRw 2015.03.09
\section widget基类
*/
#ifndef MDDRAWWIDGET_H
#define MDDRAWWIDGET_H

#include <QWidget>
#include "MDDrawObj.h"

class CDrawWidgetData;
class CMDDrawWidgetEvent;

class MDDRAWOBJSHARED_EXPORT CMDDrawWidget : public CMDDrawObj
{
public:
    CMDDrawWidget();
    virtual ~CMDDrawWidget();
    void initCreate() ;

private:
    CDrawWidgetData* m_data;
/*!
\section property
*/
protected:
    void setWidget(QWidget* value, CMDDrawWidgetEvent* event = NULL);
    void setWidgetDock(QWidget* value, CMDDrawWidgetEvent* event = NULL);
public:
    QWidget* widget();
    CMDDrawWidgetEvent* event();
    virtual bool finalEnabled();
/*!
\section virtual property
*/
public:
    bool isVector() ;
    bool isWidget() ;
    void updateFormScale() ;

/*!
\section common
*/
public:
    void serialize(QJsonObject &json);
    void deserialize(const QJsonObject &json);
    CMDDrawObj* clone() ;
/*!
\section vritual function
*/
public:
    void onGeneratePath() ;
    void updateVisible();
    void updateEnabled();
    void top() ;
    void last() ;
    void onPrint(QPainter* p) ;
    void onInitialization() ;
/*!
\section event
*/
public:
    void onMouseDown(Qt::MouseButton button, qreal x, qreal y) ;
    void onMouseUp(Qt::MouseButton button, qreal x, qreal y) ;
    void onMouseMove(Qt::MouseButton button, qreal x, qreal y) ;
/*!
\section undo
*/
public:
    void undoAdd() ;
    void undoDelete() ;
};

#endif // MDDRAWWIDGET_H


