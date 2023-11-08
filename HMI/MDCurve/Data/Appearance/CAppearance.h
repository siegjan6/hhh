/*!

\author eHVr

\section 功能

        外观类
*/
#ifndef CAPPEARANCE_H
#define CAPPEARANCE_H

#include "CHeader.h"
#include "MDBrushManager.h"
#include <QPixmap>

class CAppearance
{
public:
    CAppearance();
    ~CAppearance();
/*!
\section   基础属性
*/
public:
    CHeader *header() const;

    bool isShowBorder() const;
    void setIsShowBorder(bool isShowBorder);
private:
    bool     m_isShowBorder;    //! 边框显示
    CHeader  *m_header;         //! 标题

/*!
\section   曲线区域
*/
public:
    QRectF curveArea() const;
    void setCurveArea(const QRectF &curveArea);

    CMDBrushManager curveBrush() const;
    void setCurveBrush(const CMDBrushManager &curveBrush);
private:
    CMDBrushManager  m_curveBrush;//! 曲线区域画刷
    QRectF           m_curveArea; //! 曲线区域
/*!
\section   整个窗体
*/
public:
    CMDBrushManager widgetBrush() const;
    void setWidgetBrush(const CMDBrushManager &widgetBrush);

    QRectF widgetArea() const;
    void setWidgetArea(const QRectF &widgetArea);
private:
    CMDBrushManager     m_widgetBrush; //! 窗体画刷
    QRectF              m_widgetArea;  //! 绘制区域

/*!
\section  计算，绘制
*/
public:
    void draw(QPainter*p, const QRect &rect);
    void drawBorder(QPainter *p,const QRect &rect); //! 绘制边框
    void calculate();

/*!
    \section  序列化和反序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

public:
    CAppearance &operator= (const CAppearance &other);
public:
    void changeLanguage();
    void collectWords();

    void getBrushes(QList<CMDBrushManager*>& list);

};

#endif // CAPPEARANCE_H
