/*!
\author eHVr
\section 功能
    曲线后台数据类,属性
    \a name
    \a comment
    \a variantDepict
    \a curveColor
    \a curveWidth
    \a curveStyle
    \a curveVisible
    \a upWarningLine
    \a belowWarningline
    \a normalPoint
    \a warningPoint
*/
#ifndef CCURVEOBJECT_H
#define CCURVEOBJECT_H

#include <QObject>
#include <QPainter>
#include <QPixmap>

#include "Data.h"
#include "CWarningLine.h"
#include "CFind.h"
#include <QPoint>

class CXAxis;
class CYAxis;
class CPoint;

class CCurveObject
{

public:
    CCurveObject();
    ~CCurveObject();
/*!
\section 绑定的坐标轴
*/
public:
    CXAxis *xAxis() const;
    void setXAxis(CXAxis *xAxis);

    QString xName() const;
    void setXName(const QString &xName);

    CYAxis *yAxis();
    void setYAxis(CYAxis *yAxis);

    QString yName() const;
    void setYName(const QString &yName);

    void setProjectPath(const QString &path);
    QString projectPath() const;
private:
    CXAxis  *m_xAxis; //! 绑定的X轴
    CYAxis  *m_yAxis; //! 绑定的Y轴
    QString  m_xName;
    QString  m_yName;
    QString  m_projectPath;

/*!
\section 数据
*/

public:
    CCurveData &rawData();
    void setRawData(const CCurveData &rawData);
    CCurveVisible &totalVisblePoint();

    void setPreviewData(const CCurveData &rawData);
    CCurveData &previewData();

    void setBoundedPreview(bool isBounded);
    bool isBoundedPreview() const;

private:
    CCurveData          m_rawData;
    CCurveData          m_rawPreviewData;//! 曲线预览数据
    CCurveVisible       m_visiblePoint; //! 可视数据
    CCurveVisible       m_totalVPoint;
public:
    void clearDataForZoom();
/*!
\section 曲线属性:
         线宽,线色,线型,是否显示,名称,描述,绘制区域,是否在游标中显示
*/
public:
    QString name() const;
    void setName(const QString &name);

    QString comment() const;
    void setComment(const QString &comment);

    QString variantDepict() const;
    void setVariantDepict(const QString &variantDepict);

    QString timeVariantDepict() const;
    void setTimeVariantDepict(const QString &timeVariantDepict);

    QString viewName() const;
    void setViewName(const QString &str);

    int curveWidth() const;
    void setCurveWidth(int curveWidth);

    QColor curveColor() const;
    void setCurveColor(const QColor &curveColor);

    void  setCurveStyle (Qt::PenStyle style);
    Qt::PenStyle curveStyle() const;

    QRectF curveArea() const;
    void setCurveArea(const QRectF &curveArea);

    bool curveVisible() const;
    void setCurveVisible(bool curveVisible);

    bool curveVernierVisible() const;
    void setCurveVernierVisible(bool curveVernierVisible);

    void setType(bool isReal);
    void setFresh(qint32 ms);
    void setIsRun(bool isRun);
private:
    QString             m_name;                  //! 名称
    QString             m_comment;               //! 说明
    QString             m_variantDepict;         //! 变量说明
    QString             m_timeVariantDepict;     //! 历史曲线绑定的时间字段
    QString             m_viewName;
    QColor              m_curveColor ;           //! 颜色
    qint32              m_curveWidth ;           //! 曲线线宽
    Qt::PenStyle        m_curveStyle ;           //! 线型
    QRectF              m_curveArea;             //! 曲线区域

    bool                m_curveVisible;          //! 是否显示
    bool                m_curveVernierVisible;   //! 是否在游标中显示
    bool                m_isReal;
    bool                m_isCurveRunning;
    qint32              m_freshTime;             //! 刷新时间
    bool                m_isBoundedPreview;      //! 是否绑定曲线预览


/*!
\section 上下警戒线相关属性:
         超过警戒线数据点的颜色,大小,是否显示
*/
public:
    CWarningLine *upWarningLine();
    CWarningLine *belowWarningLine();
    CPoint       *normalPoint();
    CPoint       *warningPoint();
private:
    CWarningLine *m_upWarningLine;
    CWarningLine *m_belowWarningLine;
    CPoint       *m_normalPoint;
    CPoint       *m_warningPoint;

/*!
\section 计算,绘制
*/
public:
    void draw(QPainter *p);
    void drawOnPixmap();
    void calculate();
private:
    void calculatePoint(const QPointF &p); //! 计算数据点
    bool calculateCheck();                 //! 计算前数据检测
    void calVisiblePoint();
    void clearOldData();
    void calculateWarningLine(CWarningLine *line,const QRectF& area,CYAxis *y);
    void calculateTimeType();
    void calculateValueType();

/*!
\section 序列化和反序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

    CCurveObject & operator =(const CCurveObject &other);
private:
    bool      m_isCalculated;

    QPixmap   m_pixmap;               //!当前时刻的贴图
    QPixmap   m_prePixmap;            //!上一时刻的贴图
    QDateTime m_preStartTime;
    QDateTime m_preEndTime;
    QPoint    m_lastPoint;
    double    m_picLength;
};

#endif // CCURVEOBJECT_H
