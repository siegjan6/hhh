/*!
\author aGRw 2016.03.08
\section 按钮控件
*/
#ifndef MDDRAWBUTTON_H
#define MDDRAWBUTTON_H

#include "MDDrawText.h"

#if defined(MDDRAWVECTORS_LIBRARY)
#  define MDDRAWVECTORSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWVECTORSSHARED_EXPORT Q_DECL_IMPORT
#endif

class MDDRAWVECTORSSHARED_EXPORT CMDDrawButton : public CMDDrawText
{
public:
    CMDDrawButton();
    virtual ~CMDDrawButton();
/*!
\section virtual property
*/
public:
     DrawType type();
     QString className(){return "CMDDrawButton";}
/*!
\section protected function
*/
protected:
    CMDDrawObj *createSelf();
    void onPaint(QPainter* p);
    void onMouseDown(Qt::MouseButton button, qreal x, qreal y);
    void onMouseUp(Qt::MouseButton button, qreal x, qreal y);
    void onMouseLeave();
/*!
\section common
*/
public:
    CMDDrawObj* clone() ;
};

#endif // MDDRAWBUTTON_H





