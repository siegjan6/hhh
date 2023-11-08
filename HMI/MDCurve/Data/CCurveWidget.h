/*!

\author eHVr

\section 功能
    \class CCurveWidget 曲线显示界面类
*/
#ifndef CCURVEWIDGET_H
#define CCURVEWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QRubberBand>
#include <QMenu>
#include "CCurveProperty.h"
#include "MDColorManager.h"

class IMDDataBaseParam; //! database
class IDrawEnvrionment;
class CCurveWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CCurveWidget(QWidget *parent = 0);
    ~CCurveWidget();

    int setDILProperty();//! 属性设置对话框
/*!
\section 脚本相关接口，过滤条件，查询
*/
public:

    enum class DataSourceParameters{
        None = 0,
        DBType,
        DBIP,
        DBPort,
        DBName,
        DBUser,
        DBPassword
    };
    void setHistoryDataSearchFilter(const QString &);
    void setHistoryTagSearchFilter(const QString &);
    void setDataSourceParameter(int type, const QString &parameter);
    void query();
private:
    int stringToType(const QString &);
    /*!
    \section 创建查询菜单
    */
private:
     void createMenu();
/*!
\section 环境相关函数
*/
public:
    void setIsRealTime(bool);
    void setDrawEnvironment(IDrawEnvrionment*);
private:
    bool m_isRealTime;
    IDrawEnvrionment *m_environment;

/*!
\section 鼠标移动时的相关函数
*/
private:
    void moveLength(const double &length);
    void moveHeight(const double &height);
    void setCurrentMousePoint(const QPoint &);
/*!
\section 拷贝相关函数
*/
public:
    void setCurve(CCurveProperty *);
    void setCurveParentWidget(QWidget *parent);
    CCurveProperty *clone(QWidget *parent); //! 克隆函数
/*!
\section 重载窗体事件
*/
protected:
    virtual void paintEvent(QPaintEvent*);
    virtual void resizeEvent(QResizeEvent *);

    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
private slots:
    void slotCustomMenu(const QPoint &p);
    void queryHistory();
    void setConfig();

/*!
\section 序列化和反序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
private:
    CCurveProperty *m_curve;             //! 曲线
    QRubberBand    *m_rubberBand;        //! 弹簧区域
    QPoint          m_startPoint;        //! 起始点
    QPoint          m_endPoint;          //! 结束点

    bool            m_rubberBandVisble;  //! 弹簧是否可见
    bool            m_isPressed;         //! 鼠标是否按下
    bool            m_isPreview;         //! 是否预览

    QStringList     m_dataSource;
    QMenu*          m_menu;             //! 查询菜单
public:
    void changeLanguage();
    void collectWords();

public:
    void getBrushes(QList<CMDBrushManager*>& list);

    /*!
    \section 基准曲线设置起始值
    */
public:
    //! 设置起始数值
    void setValue(int index, QVariant value);
    //! 获取表达式和与之对应的索引
    void getExpressionList(QList<int> &indexs, QList<QString> &expressions);
    //! 设置起始时间
public:
    void setStartTime(const QString &curveName,const QVariant &startTime);
public:
   void setReferenceCurveData(const QString &curveName, const QVariant &xValue, const QVariant &yValue);

private:
   enum class moveType
      {
          moveLeft = 0,moveRight,move,moveStartRoll,moveEndRoll
      };
    moveType m_moveType;
    QRectF m_startMoveRect;        //! 起始移动块
    QRectF m_endMoveRect;          //! 结束移动块
    QRectF m_dragMoveRect;         //! 拖动移动块

    QRectF m_startArrows;       //! 开始点击滚动
    QRectF m_endArrows;         //! 结束点击滚动
};

#endif // CCURVEWIDGET_H
