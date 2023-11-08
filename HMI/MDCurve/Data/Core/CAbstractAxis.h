/*!

\author eHVr

\section 功能

    基础坐标轴,基本属性
    \a name             名称
    \a comment          说明
    \a valuePrecision   数值精度
    \a axisVisible      轴可见
    \a axisLineVisible  轴线可见
    \a font             字体
    \a fontColor        字体颜色
*/
#ifndef CABSTRACTAXIS_H
#define CABSTRACTAXIS_H

#include <QObject>
#include <QColor>
#include <QFont>
#include <QSet>
#include "CBisectrixLine.h"

class QPainter;
class CCurveObject;
class CAbstractAxis
{
public:
    CAbstractAxis();
    virtual ~CAbstractAxis();
/*!
\section   基础属性
*/
    //! 名称
public:
    void setName(const QString &name);
    QString name () const;
private:
    QString m_name;

    //! 说明
public:
    void setComment(const QString &comment);
    QString comment() const;
private:
    QString m_comment;

    //! 数值精度
public:
    int valuePrecision() const;
    void setValuePrecision(int valuePrecision);
private:
    int m_valuePrecision;

    //! 轴是否可见(包含字符,轴线)
public:
    void setAxisVisible(bool);
    bool axisVisible() const;
private:
    bool m_axisVisible;

    //! 轴线是否可见
public:
    bool axisLineVisible() const;
    void setAxisLineVisible(bool axisLineVisible);
private:
    bool m_axisLineVisible;
    //! 字体
public:
    QFont font() const;
    void setFont(const QFont &font);
private:
    QFont m_font;

    //! 字体颜色
public:
    QColor fontColor() const;
    void setFontColor(const QColor &fontColor);
private:
    QColor m_fontColor;

public:
    void setScale(double start,double end);
    double endScale() const;
    void setEndScale(double endScale);

    double startScale() const;
    void setStartScale(double startScale);
private:
    double          m_endScale ;
    double          m_startScale ;
    //! 基础线条
public:
    CBisectrixLine *axisLine() const;
private:
    CBisectrixLine *m_axisLine ;

/*!
\section   曲线相关
*/
public:
    void addCurve(CCurveObject *curve);
    void removeCurve(CCurveObject *curve);

    QList<CCurveObject *> curves() const;
    void setCurves(const QList<CCurveObject *> &curves);

    bool isBoundCurve() const;
    bool allBindCurvesInvisible() const;
private:
    QList<CCurveObject*> m_curves;

/*!
\section   绘制
*/
public:
    virtual void draw(QPainter *) = 0;
    virtual void drawAxisLine(QPainter *) = 0;

/*!
\section   序列化和反序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

public:
    CAbstractAxis &operator =(const CAbstractAxis &other);//! clone

public:
    void changeLanguage();
    void collectWords();
private:
    QString m_translateComment;
};
#endif // CABSTRACTAXIS_H
