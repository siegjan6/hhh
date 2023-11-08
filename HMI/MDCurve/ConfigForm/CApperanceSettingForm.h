/*!

\author xuk
\class CApperanceSettingForm
\section 功能
    外观设置界面
*/
#ifndef CAPPEARANCESETTINGFORM_H
#define CAPPEARANCESETTINGFORM_H

#include <QWidget>

#include "MDBrushManager.h"
#include "MDBrushSelectDialog.h"


namespace Ui {
class CAppearanceSettingForm;
}

class CAppearance;
class CGridRegion;
class CLabelManager;
class CMDBrushManager;

class CAppearanceSettingForm : public QWidget
{
    Q_OBJECT
public:
    explicit CAppearanceSettingForm(QWidget *parent = 0);
    ~CAppearanceSettingForm();
public:
    /*!
     \brief 获取后台数据指针

     \param apperance 界面数据指针
     \param gridRegion 曲线框数据
     \param labels     标签
    */
    void setData(CAppearance *apperance,CGridRegion *gridRegion,CLabelManager *labels);
private:

/*!
\section 窗体控件修改标志
*/
public:
    void resetModified(); //!重置编辑标志位
private slots:
    void slotHandleModified(); //! 捕捉窗体内容编辑信号
signals:
    void itemModified(); //! 发出窗体中控件内容编辑的信号
private:
    bool m_isModified = false; //! 窗体是否编辑标志位

/*!
\section 应用修改内容
*/
public:
    void applyChanges();
private:
    void applyApperanceChanges();
    void applyGridRegionChanges();
    void applyLabelChanges();
/*!
\section 槽
*/
private slots:   
    void slotSetTitleFont();  //!设置标题字体
    void slotSetBorderColor(); //!设置边框颜色
    void slotSetLabelFont(); //!设置标签字体
    void slotSetWidgetPannelVisible(bool); //!设置窗体边框可见
    void slotSetCustomMenuEnable(bool); //!设置右键菜单有效
    void slotSetLabelVisible(bool); //! 设置标签可见
    void slotSetCurrentCurveValueVisible(bool); //! 设置当前曲线值是否可见
    void slotSetCurveGradientColor(); //!   设置曲线区域背景色
    void slotSetWidgetGradientColor(); //!   设置窗体背景色
    void slotSetTitleFontColor();//! 设置窗体字体颜色

/*!
\section 创建信号槽链接
*/
private:  
    void createConnections();
    void breakConnections();
/*!
\section 初始化窗体数据
*/
private:
    void initialize();
    void initializeUI();
    void initializeAppearance();
    void initializeGridRegion();
    void initializeLabels();

/*!
\section 内部功能函数
*/
private:
    void setFont(QFont &);
    void setColor(QColor &initialColor, QPushButton &colorButton);
    void setButtonColor(const QColor &color, QPushButton &button);
    void setButtonBackGroundColor(const QBrush &brush, QPushButton &button);
private:

    Ui::CAppearanceSettingForm *ui;

    bool m_widgetPanelVisible = false;  //! 窗体边框可见
    bool m_customMenuEnable = false;             //! 是否允许右键菜单
    bool m_labelVisible = false;                 //! 标签是否可见
    bool m_currentCurveValueVisible = false;     //! 是否显示鼠标当前位置的值

    CAppearance    *m_appearance = nullptr; //! 外观
    CGridRegion    *m_gridRegion = nullptr; //! 网格区域
    CLabelManager  *m_labels = nullptr;     //! 标签
    QFont          m_titleFont;             //! 标题字体
    QColor         m_titleFontColor;        //! 标题字体颜色
    QFont          m_labelFont;             //! 标签字体
    QColor         m_borderColor;           //! 边框色

    CMDBrushManager           m_widgetBrush;                    //! 窗体背景画刷
    CMDBrushManager           m_curveBrush;                     //! 曲线区域背景画刷
    CMDBrushSelectDialog      *m_curveDialog = nullptr;         //! 曲线区域背景画刷设置对话框
    CMDBrushSelectDialog      *m_widgetDialog = nullptr;        //! 窗体区域背景画刷设置对话框
};

#endif // CAPPEARANCESETTINGFORM_H
