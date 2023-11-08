/*!

\author emhhbmdsZQ==

\section 功能

    曲线预览数据类
*/
#ifndef CPREVIEWCURVEDATA_H
#define CPREVIEWCURVEDATA_H
#include <QObject>
#include "MDBrushManager.h"

class CPreviewCurveData
{
public:
    CPreviewCurveData();
    ~CPreviewCurveData();

    /*!
    \section 曲线预览基础数据
    */

public:
    /*!
    \section 曲线预览是否可见
    */
    bool isPreviewVisible() const;
    void setPreviewVisible(bool visible);

    /*!
    \section 预览区域数据
    */
    void setPreviewBackBrush(const CMDBrushManager &previewBackBrush);
    CMDBrushManager previewBackBrush() const;

    void setPreviewHightLightBrush(const CMDBrushManager &previewHightLightBrush);
    CMDBrushManager previewHightLightBrush() const;

    void setMainBisectrixColor(const QColor &mainBisectrixColor);
    QColor mainBisectrixColor() const;

    void setSubBisectrixColor(const QColor &subBisectrixColor);
    QColor subBisectrixColor() const;

    void setStartBackColor(const QColor &startBackColor);
    QColor startBackColor() const;

    void setStartScaleColor(const QColor &startScaleColor);
    QColor startScaleColor() const;

    void setStartLineMarkColor(const QColor &startLineMarkColor);
    QColor startLineMarkColor() const;

    void setEndBackColor(const QColor &endBackColor);
    QColor endBackColor() const;

    void setEndScaleColor(const QColor &endScaleColor);
    QColor endScaleColor() const;

    void setEndLineMarkColor(const QColor &endLineMarkColor);
    QColor endLineMarkColor() const;

    void setMainBisectrix(const int &mainCount);
    int mainBisectrix() const;

    void setSubBisectrix(const int &subCount);
    int subBisectrix() const;

    void setSubBisectrixHeight(const int &height);
    int subBisectrixHeight() const;

    /*!
    \section 滚动条数据
    */
    void setRollBackBrush(const CMDBrushManager &rollBackBrush);
    CMDBrushManager rollBackBrush() const;

    void setRollMoveBackBrush(const CMDBrushManager &rollMoveBackBrush);
    CMDBrushManager rollMoveBackBrush() const;

    void setRollStartBackBrush(const CMDBrushManager &rollStartBackBrush);
    CMDBrushManager rollStartBackBrush() const;

    void setRollEndBackBrush(const CMDBrushManager &rollEndBackBrush);
    CMDBrushManager rollEndBackBrush() const;

    void setRollScaleColor(const QColor &rollScaleColor);
    QColor rollScaleColor() const;

    void setRollStartArrowsColor(const QColor &rollStartArrowsColor);
    QColor rollStartArrowsColor() const;

    void setRollEndArrowsColor(const QColor &rollEndArrowsColor);
    QColor rollEndArrowsColor() const;

    /*!
    \section 曲线与标签数据
    */
    void setCurveVisible(bool visible);
    bool isCurveVisible() const;

    void setBoundedCurve(const QString &string);
    QString boundedCurve() const;


private:
    bool     m_isPreviewVisible;               //! 曲线预览是否可见

    /*!
    \section 预览区域
    */
    CMDBrushManager m_previewBackBrush;        //! 背景画刷
    CMDBrushManager m_previewHightLightBrush;  //! 高亮色画刷
    QColor         m_mainBisectrixColor;       //! 主等分线颜色
    QColor         m_subBisectrixColor;        //! 次等分线颜色
    QColor         m_startBackColor;           //! 起始移动块背景色
    QColor         m_startScaleColor;          //! 起始移动块刻度颜色
    QColor         m_startLineMarkColor;       //! 起始移动块线条标识色

    QColor         m_endBackColor;             //! 结束移动块背景色
    QColor         m_endScaleColor;            //! 结束移动块刻度颜色
    QColor         m_endLineMarkColor;         //! 结束移动块线条标识色

    qint32            m_mainBisectrix;            //! 主等分线个数(1-20)
    qint32            m_subBisectrix;             //! 次等分线个数(1-10)
    qint32            m_subBisectrixheight;       //! 次等分线高(3-8)

    /*!
    \section 滚动条
    */
    CMDBrushManager        m_rollBackBrush;        //! 背景色画刷
    CMDBrushManager        m_rollMoveBackBrush;    //! 滑块背景色画刷
    CMDBrushManager        m_rollStartBackBrush;   //! 起始箭头标识背景色画刷
    CMDBrushManager        m_rollEndBackBrush;     //! 结束箭头标识背景色画刷
    QColor                 m_rollScaleColor;       //! 滑块刻度颜色
    QColor                 m_rollStartArrowsColor; //! 起始箭头标识颜色
    QColor                 m_rollEndArrowsColor;   //! 结束箭头标识颜色

    /*!
    \section 曲线与标签
    */
    bool         m_isCurveVisible;    //! 曲线是否显示
    QString      m_boundedCurve;      //! 绑定曲线

    qint32 m_version;                    //! 版本号
  /*!
   \section 序列化
  */
public:
      CPreviewCurveData &operator =(const CPreviewCurveData &other);
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

public:
     void getBrushes(QList<CMDBrushManager*>& list);

};

#endif // CPREVIEWCURVEDATA_H
