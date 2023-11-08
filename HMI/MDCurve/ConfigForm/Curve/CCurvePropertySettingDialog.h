/*!

\author eHVr

\section 功能
     单一曲线属性设置窗体
     功能：曲线的上下警戒值设置，警戒线设置，数据点形状、颜色设置等
*/
#ifndef CCURVESETTINGDIALOG_H
#define CCURVESETTINGDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QColor>

class CPoint;
class CXAxis;
class CYAxis;
class QComboBox;
class CCurveObject;
class CAbstractAxis;
class CXAxisManager;
class CYAxisManager;
class CWarningLine;
class IMDRTDBManagerUI;
class ITranslateWidgetTool;

namespace Ui {
class CCurvePropertySettingDialog;
}

class CCurvePropertySettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CCurvePropertySettingDialog(QWidget *parent = 0);
    ~CCurvePropertySettingDialog();

/*!
\section  设置曲线属性,返回曲线的指针
*/
public:

    /*!
     \brief getRealCurve
        实时曲线设置
     \param ok      状态
     \param initial 初始曲线
     \param xList   x轴链表
     \param yList   y轴链表
     \param variantForm 变量选择界面
     \param parent  父窗体
     \return
    */
    static  CCurveObject *getRealCurve(bool *ok, CCurveObject *initial,
                   const QList<CXAxis *> &xList, const QList<CYAxis *> &yList,
                   IMDRTDBManagerUI *variantForm, QWidget *parent = 0);


    /*!
     \brief getRealCurve
        历史曲线设置
     \param ok      状态
     \param initial 初始曲线
     \param xList   x轴链表
     \param yList   y轴链表
     \param variantForm 字段链表
     \param parent  父窗体
     \return
    */
    static  CCurveObject *getHistoryCurve(bool *ok, CCurveObject *initial,
                   const QList<CXAxis *> &xList, const QList<CYAxis *> &yList,
                   const QStringList &fields, QWidget *parent = 0);

/*!
\section 数据
*/
public:
    void setData(CCurveObject *curve,const QList<CXAxis *> &xList, const QList<CYAxis *> &yList);

    void setVariantForm(IMDRTDBManagerUI *variantForm);
    void setFields(const QStringList &fields);
    void setIsRealTime(bool isRealTime);
/*!
\section 应用内容更改
*/
private:
    void applyChanges();
    void applyCurveChanges();
    void applyUpWarningLineChanges();
    void applyBelowWarningLineChanges();
    void applyNormalPointChanges();
    void applyWarningPointChanges();
/*!
\section 初始化数据
*/
private:
    void initialize();
    void initializeData();
    void initializeUI();
    void initializeCurve();
    void initializeUpWarningLine();
    void initializeBelowWarningLine();
    void initializeNormalPoint();
    void initializeWarningPoint();
    void initializeAxisComboBox(QList<CXAxis *> axisList , QComboBox *comboBox);//! X
    void initializeAxisComboBox(QList<CYAxis *> axisList , QComboBox *comboBox);//! Y

    void setLineStyle(QComboBox *comboBox);  //! 设置线样式
    void setPointStyle(QComboBox *comboBox); //! 设置点样式
/*!
\section 创建信号槽链接
*/
private:
    void createConnections();
    void createUIConnections();
/*!
\section 槽函数
*/
private slots:
    void slotSetCurveAndWarning(int index);   //! 曲线和警戒线配置选择
    void slotSetVarinatName();                //! 变量名
    void slotSetTimeVariantName();            //! 时间变量名
    void slotOkButtonClicked();               //! 确定按钮
    void slotCancelButtonClicked();           //! 取消按钮
    void slotSetCurveColor();                 //! 曲线色
    void slotSetUpWarningLineColor();         //! 上警戒线颜色
    void slotSetBelowWarningLineColor();      //! 下警戒线颜色
    void slotSetNormalPointColor();           //! 正常点颜色
    void slotSetWarningPointColor();          //! 警戒点颜色
    void slotSetUpWarningLineEnable(bool);    //! 上警戒线可见
    void slotSetBelowWarningLineEnable(bool); //! 下警戒线可见
    void slotSetNormalPointEnable(bool);      //! 正常点可见
    void slotSetWarningPointEnable(bool);     //! 警戒点可见
    void slotSetCurveLineStyle(int);          //! 曲线样式
    void slotSetUpWarningLineStyle(int);      //! 上警戒线样式
    void slotSetBelowWarningLineStyle(int);   //! 下警戒线样式
    void slotSetViewName();                   //! 视图名

/*!
\section 功能函数
*/
private:
    void setColor(QColor &initialColor, QPushButton &colorButton);
private:
    Ui::CCurvePropertySettingDialog *ui;

    CCurveObject     *m_curve;          //! 曲线数据
    QList<CXAxis *>   m_xList;
    QList<CYAxis *>   m_yList;

    bool              m_isRealTime;
    QStringList       m_fields;

    CWarningLine      *m_upWarningLine;   //! 上警戒线
    CWarningLine      *m_belowWarningLine;//! 下警戒线
    CPoint            *m_normalPoint;     //! 正常数据点
    CPoint            *m_warningPoint;    //! 警戒点
    IMDRTDBManagerUI  *m_variantForm;

    QColor m_curveColor;             //! 曲线颜色
    QColor m_upWarningLineColor;     //! 上警戒线颜色
    QColor m_belowWarningLineColor;  //! 下警戒线颜色
    QColor m_normalPointColor;       //! 正常点颜色
    QColor m_warningPointColor;      //! 警戒点颜色

    int m_curveLineStyle;           //! 曲线线型
    int m_upWarningLineStyle;       //! 上警戒线线型
    int m_belowWarningLineStyle;    //! 下警戒线线型


private:
    ITranslateWidgetTool *m_translateTool;//! 多语言

};


#endif // CCURVESETTINGDIALOG_H
