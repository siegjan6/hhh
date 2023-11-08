#ifndef CPREVIEWCURVESETTINGFORM_H
#define CPREVIEWCURVESETTINGFORM_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include "MDBrushManager.h"
class CPreviewCurve;
class CCurveObjectManager;

namespace Ui {
class CPreviewCurveSettingForm;
}

class CPreviewCurveSettingForm : public QWidget
{
    Q_OBJECT

public:
    explicit CPreviewCurveSettingForm(QWidget *parent = 0);
    ~CPreviewCurveSettingForm();

private:
    Ui::CPreviewCurveSettingForm *ui;


/*!
\brief 获取后台数据指针
\param previewCurve 界面数据指针
*/
public:
    void setData(CPreviewCurve *previewCurve,CCurveObjectManager *curves);
    void setProjectPath(const QString &path);
    void setCurveObjectManager(CCurveObjectManager *curves);

/*!
\section 窗体控件修改标志
*/
public:
    void resetModified();      //! 重置编辑标志位
private slots:
    void slotHandleModified(); //! 捕捉窗体内容编辑信号
signals:
    void itemModified();       //! 发出窗体中控件内容编辑的信号
private:
    bool m_isModified;         //! 窗体是否编辑标志位

/*!
\section 初始化窗体数据
*/
private:
    void initialize();
    void initializeUI();
    void initializePreviewArea();
    void initializeScrollBar();
    void initializeCurveAndLabel();
    void initializeComboBox(CCurveObjectManager *curves , QComboBox *comboBox);

/*!
\section 槽
*/
private slots:
    void slotSetPreviewVisible(bool checked);
    void slotSetCurveVisible(bool checked);
    void slotSetCurveComboBox(int index);
    void slotSetPreviewBack();
    void slotSetHighLight();
    void slotSetMainColor();
    void slotSetSubColor();
    void slotSetStartBackColor();
    void slotSetStartScaleColor();
    void slotStartLineMarkColor();
    void slotSetEndBackColor();
    void slotSetEndScaleColor();
    void slotEndLineMarkColor();

    void slotSetRollBack();
    void slotSetRollMoveBack();
    void slotSetRollStartBack();
    void slotSetRollEndBack();
    void slotSetRollScaleColor();
    void slotSetStartArrowColor();
    void slotSetEndArrrowColor();

/*!
\section 应用修改内容
*/
public:
    void applyChanges();
private:
    void applyPreviewAreaChanges();      //! 预览区域修改
    void applyScrollBarChanges();        //! 滚动条修改
    void applyCurveAndLabelChanges();    //! 曲线与标签修改

/*!
\section 创建信号槽链接
*/
private:
    void createConnections();
    void breakConnections();

/*!
\section 内部功能函数
*/
private:
    void setColor(QColor &initialColor, QPushButton &colorButton);
    void setButtonBackGroundColor(const QBrush &brush, QPushButton &button);
private:
    CCurveObjectManager *m_curves ;        //! 曲线管理器
    CPreviewCurve  *m_previewCurve;
    bool           m_isPreviewVisible;     //! 曲线预览是否可见
    QString        m_projectPath;          //! 项目工程路径

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

    int            m_mainBisectrix;            //! 主等分线个数(1-20)
    int            m_subBisectrix;             //! 次等分线个数(1-10)
    int            m_subBisectrixheight;       //! 次等分线高(3-8)

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

};

#endif // CPREVIEWCURVESETTINGFORM_H
