/*!

\author eHVr

\section 功能

    曲线后台数据类，包含各个模块
\class CAppearance          外观
\class CGridRegion          绘图网格
\class CCurveObjectManager  曲线管理
\class CXAxisManager        x轴管理
\class CYAxisManager        Y轴管理
\class CDrawMode            绘制模式
\class CLabelManager        标签
\class CVernier             游标
\class CVernierWidget       游标界面
\class CAuxiliary           辅助框
\class CFeatureTagGroup     特征标签
\class CTools               工具
*/
#ifndef CCURVEPROPERTY_H
#define CCURVEPROPERTY_H

#include <QRectF>
#include <QDateTime>
#include <QPainter>
#include "CDataProperty.h"

#include <QPixmap>

class CXAxis;
class CYAxis;
class CAppearance;
class CGridRegion;
class CCurveObject;
class CCurveObjectManager;
class CXAxisManager;
class CYAxisManager;
class CDrawMode;
class CLabelManager;
class CVernier;
class CVernierWidget;
class CAuxiliary;
class IDrawEnvrionment;
class CHistoryTagManager;
class CTagManager;
//class CRealTimeTagManager;
class CTools;

class CReferenceCurveManager;
class CReferenceCurve;
class CPreviewCurve;

class CCurveProperty:public QObject
{
    Q_OBJECT
public:
    CCurveProperty(QObject *parent = 0);
    ~CCurveProperty();
/*!
\section 基本属性
*/
public:

    //! 绘制区域
    QRectF drawArea() const;
    void setDrawArea(const QRectF &drawArea);

    QRectF curveArea() const;

    //! 预览区域
    QRectF previewArea();

    void setBorderLineColor(const QColor &color);
private:
    QRectF  m_drawArea;
    QRectF  m_previewArea;

/*!
\section 功能函数
*/
public:
    void addXAxis(CXAxis *axis);
    void addYAxis(CYAxis *axis);
    void addCurve(CCurveObject *curve);
    void addReferenceCurve(CReferenceCurve *reCurve);

    void setIsRealTime(bool);
    void setDrawEnvironment(IDrawEnvrionment* value); //! 环境指针
    void setCurrentMousePoint(const QPoint &point);
    void setParentWidget(QWidget *parent);

    void setVernierClosed();
    void applyChanges();
public slots:
    void refresh();
    void refreshForTools();
private:
    bool m_isRealTime; //! 是否为实时版

/*!
\section 脚本接口
*/
public:
    void setHistoryDataSearchFilter(const QString &filter);
    void setHistoryTagSearchFilter(const QString &filter);
    void query();
/*!
\section 各个模块指针
*/
public:
    CAppearance            *appearance() const;
    CLabelManager          *label() const;
    CDataProperty          *data() const;
    CCurveObjectManager    *curves() const;
    CXAxisManager          *xs() const;
    CYAxisManager          *ys() const;
    CVernier               *vernier() const;
    CHistoryTagManager     *historyTag() const;
    CGridRegion            *gridRegion() const;
    CDrawMode              *drawMode() const;
    CRealTimeTagManager    *realTag() const;
    CTools                 *tools() const;
    CReferenceCurveManager *referenceCurves() const;
    CPreviewCurve          *previewCurve() const;
private:
    CAppearance             *m_appearance ;            //! 外观
    CGridRegion             *m_gridRegion ;            //! 网格区域
    CCurveObjectManager     *m_curveObjects ;          //! 曲线组
    CReferenceCurveManager  *m_referenceCurves ;        //! 基准曲线组
    CPreviewCurve           *m_previewCurve ;           //! 曲线预览
    CXAxisManager           *m_xsManager ;             //! X轴组
    CYAxisManager           *m_ysManager ;             //! Y轴组
    CDrawMode               *m_drawMode ;              //! 绘制模式
    CDataProperty           *m_data ;                  //! 数据
    CLabelManager           *m_label ;                 //! 标签
    CVernier                *m_vernier ;               //! 游标
    CVernierWidget          *m_vernierForm ;           //! 游标界面
    CAuxiliary              *m_auxiliary ;             //! 线,点
    CHistoryTagManager      *m_historyTag ;            //! 历史特征标签
    CRealTimeTagManager     *m_realTag ;               //! 实时特征标签
    CTools                  *m_tools ;                 //! 放大缩小等操作
signals:
    void updateWidget();

/*!
\section 操作函数
    移动,放大,缩小
*/
public:
    void setIsRun(bool isRun);
    void move(const double &length);
    void moveHeight(const double &height);
    void zoomIn(const QRect &rect, const QRect &curve);
    void buttonRun();
    bool buttonZoomIn();
    bool buttonZoomOut();
    bool buttonNormal();
    bool buttonSelectZoomIn();

    bool isRun() const;
    bool isMoveButtonChecked() const;
    bool isZoomSelectChecked() const;
    bool isStudioMode() const;
/*!
\section 计算，绘制
*/
public:
    void calculate();
    void calculatePreviewCurve();
    void draw(QPainter *p, const QRect &rect);
/*!
\section 序列化和反序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

public:
    CCurveProperty *clone(QWidget *parent);
/*!
\section 内部功能函数
*/
private:
    void setXYPointers();                   //! 针对各条曲线，设置 xy轴指针。
    void setOrientation();                  //! 设置绘制模式
    void setEndTime(const QDateTime &time); //! X轴结束时间

    void setLabelPoint(double x);
    void showVernier(const QPoint &point);

private:
    bool   m_isCalculated;   //! 是否经过计算
    bool   m_isStudioMode;   //! 是否为编辑版

public:
    void changeLanguage();
    void collectWords(); 

public:
    void setPreviewMoveLength(const double &dx);
    void setStartMove(const double &dx);      //! 拖动左移动块
    void setEndMove(const double &dx);        //! 拖动右移动块
    void setDragMove(const double &dx);       //! 拖动滑块左右移动

    QRectF startMoveRect() const;
    QRectF endMoveRect()const;
    QRectF dragMoveRect() const;
    QRectF startRollRect() const;
    QRectF endRollRect() const;

};

#endif // CCURVEPROPERTY_H
