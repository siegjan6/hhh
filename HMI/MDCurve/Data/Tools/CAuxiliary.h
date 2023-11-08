/*!

\author eHVr

\section 功能

    绘制辅助线和曲线上的方块
*/

#ifndef CAUXILIARY_H
#define CAUXILIARY_H
#include <QPainter>

class CCurveObjectManager;
class CReferenceCurveManager;
class CAuxiliary
{
public:
    CAuxiliary();
    ~CAuxiliary();

public:
    void setCurves(CCurveObjectManager *curve); //!相关曲线
    void setReCurves(CReferenceCurveManager* reCurve);

    bool visble() const;
    void setVisble(bool visble);

    //! 当前坐标x
    double mousePointX() const;
    void setMousePointX(double mousePointX);
private:
    bool                   m_visible;           //! 是否显示
    double                 m_mousePointX;      //! 当前鼠标的X坐标
    CCurveObjectManager    *m_curves;           //! 曲线管理器
    CReferenceCurveManager *m_reCurves;         //! 参考曲线管理器
/*!
  \section 计算，绘制
*/
public:
    void calculate();
    void draw(QPainter *p);
private:
    void drawMoveLine(QPainter *p);
private:
    QPainterPath *m_path;            //! 绘制路径
    QRectF        m_curveArea;
    bool          m_isCalculated;    //! 是否经过计算

};

#endif // CAUXILIARY_H
