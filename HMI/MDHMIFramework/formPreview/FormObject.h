/*!
\author aGRw 2016.03.14
\section 图形窗体对象，用于显示以及设置大小
*/
#ifndef FORMOBJECT_H
#define FORMOBJECT_H

#include <QRect>
#include <QPixmap>
#include <QPainter>
#include "MDHMIForm.h"

class CFormObject
{
public:
    CFormObject(CMDHMIForm* form);
//////////////////////////////////////////////////////////////////////////
public:
    QString fullName(){return m_fullName;}
    void setRect(const QRect& rect);
    QRect rect(){return m_rect;}
    bool hasChanged(){return m_hasChanged;}
    void setIsVisible(bool value){m_isVisible = value;}
    bool isVisible(){return m_isVisible;}
private:
    QRect m_rect;//!窗体大小
    QString m_fullName;//窗体路径
    QPixmap m_image;//!窗体图像
    bool m_hasChanged;//!m_rect是否修改过
    bool m_isVisible;//!是否显示
//////////////////////////////////////////////////////////////////////////
public:
    void paint(QPainter* p);
private:
    void init(CMDHMIForm* form);
};

#endif // FORMOBJECT_H
