/*!

\author emhhbmdsZQ==

\section 功能

    曲线预览类
*/
#ifndef CPREVIEWCURVE_H
#define CPREVIEWCURVE_H
#include <QObject>
#include <QPainter>
#include "Data.h"
class CCurveObjectManager;
class CPreviewCurveData;
class CMDBrushManager;
class CXAxisManager;

class CPreviewCurve:public QObject
{
    Q_OBJECT
public:
    CPreviewCurve();
    ~CPreviewCurve();

    enum class moveType
    {
        moveStart = 0,moveEnd,move,moveStartRoll,moveEndRoll
    };

public:
    CPreviewCurveData *previewData();
/*!
\section 鼠标移动距离
*/
public:
    void setMoveLength(const double &length);
    void setMoveType(int type);
private:
    moveType m_moveType;

public:
    void setIsStudioMode(bool isStudio);
    void setIsReal(bool isReal);

/*!
\section 预览矩形区域
*/
public:
    QRectF previewArea() const;
    void setPreviewArea(const QRectF &area);
/*!
\section 曲线组以及曲线矩形区域
*/
    QRectF curveArea() const;
    void setCurveArea(const QRectF &area);

    void setCurves(CCurveObjectManager *curves);
    CCurveObjectManager *curves();

private:
    CPreviewCurveData *m_previewData;    //! 曲线预览数据类
    QRectF             m_previewArea;    //! 整个预览部分区域
    QRectF             m_curveArea;      //! 曲线所在区域

    CCurveObjectManager *m_curves;
    CCurveVisible        m_previewPoint;
    bool                 m_isStudioMode;
    bool                 m_isReal;

public:
    void draw(QPainter *p);
    void calculate();
    void calculatePreviewPoint();

private:
    void drawBackGround(QPainter *painter);
    void drawMoveArea(QPainter *painter);
    void drawCurve(QPainter *p);

public:
      QRectF startMoveRect() const;
      QRectF endMoveRect() const;
      QRectF dragMoveRect() const;
      QRectF startRollMoveRect() const;
      QRectF endRollMoveRect() const;

private:
    /*!
    \section 曲线预览数据
    */
    QRectF m_previewBackRect;      //! 曲线预览背景区域
    QRectF m_previewCurveRect;     //! 预览曲线显示区域
    QRectF m_moveRect;             //! 移动块间区域
    QRectF m_startMoveRect;        //! 起始移动块
    QRectF m_endMoveRect;          //! 结束移动块
    QRectF m_dragMoveRect;         //! 拖动移动块

    /*!
    \section 滚动条数据
    */
    QRectF m_rollBackRect;          //! 滚动条背景区域
    QRectF m_rollRect;              //! 滚动区域
    QRectF m_startRollRect;         //! 开始滚动点击区域
    QRectF m_endRollRect;           //! 结束滚动点击区域

public:
    void startRollMove();           //! 点击左移
    void endRollMove();             //! 点击右移
    void startMove(const double &dx);      //! 拖动左移动块
    void endMove(const double &dx);        //! 拖动右移动块
    void dragMove(const double &dx);       //! 拖动滑块左右移动

public:
     void getBrushes(QList<CMDBrushManager*>& list);

signals:
     void updatePreviewArea();

public:
     void setScale(double start,double end);
     void setStartScale(double start);
     void setEndScale(double end);
     double startScale();
     double endScale();
private:
     double m_startScale;
     double m_endScale;

public:
     void setXManager(CXAxisManager *xManager);
private:
     CXAxisManager *m_xManager;

/*!
\section 序列化
*/
public:
     CPreviewCurve &operator =(const CPreviewCurve &other);
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

};

#endif // CPREVIEWCURVE_H
