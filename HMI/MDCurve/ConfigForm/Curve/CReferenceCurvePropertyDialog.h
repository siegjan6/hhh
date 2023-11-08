#ifndef CREFERENCECURVEPROPERTYDIALOG_H
#define CREFERENCECURVEPROPERTYDIALOG_H

#include <QDialog>
#include <QComboBox>

class CPoint;
class CXAxis;
class CYAxis;
class CAbstractAxis;
class CXAxisManager;
class CYAxisManager;
class QComboBox;
class CReferenceCurve;
class IMDRTDBManagerUI;
class ITranslateWidgetTool;

namespace Ui {
class CReferenceCurvePropertyDialog;
}

class CReferenceCurvePropertyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CReferenceCurvePropertyDialog(QWidget *parent = 0);
    ~CReferenceCurvePropertyDialog();

private:
    Ui::CReferenceCurvePropertyDialog *ui;


    /*!
    \section  设置曲线属性,返回曲线的指针
    */
public:

    static  CReferenceCurve *getReferenceCurve(bool *ok, CReferenceCurve *initial,IMDRTDBManagerUI *variantForm,
                                           const QList<CXAxis *> &xList, const QList<CYAxis *> &yList,
                                           QWidget *parent = 0);

    /*!
    \section 数据
    */
public:
    void setData(CReferenceCurve *reCurve, const QList<CXAxis *> &xList, const QList<CYAxis *> &yList);
    void setReCureColor(QColor c);
    void setVariantForm(IMDRTDBManagerUI *variantForm);
    void setIsRealTime(bool isRealTime);
    /*!
    \section 应用内容更改
    */
private:
    void applyChanges();
    void applyReCurveChanges();
    void applyNormalPointChanges();
    /*!
    \section 初始化数据
    */
private:
    void initialize();
    void initializeData();
    void initializeUI();
    void initializeReCurve();
    void initializeNormalPoint();

    void initializeAxisComboBox(QList<CXAxis *> axisList , QComboBox *comboBox);//! X
    void initializeAxisComboBox(QList<CYAxis *> axisList , QComboBox *comboBox);//! Y
    void setLineStyle(QComboBox *comboBox);  //! 设置线样式
    void setPointStyle(QComboBox *comboBox); //! 设置点样式
    void setColor(QColor &initialColor, QPushButton &colorButton);

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
    void slotOkButtonClicked();          //!确定按钮
    void slotCancelButtonClicked();      //! 取消按钮
    void slotSetReCurveColor();          //! 曲线色
    void slotSetNormalPointColor();      //! 数据点颜色
    void slotSetNormalPointEnable(bool); //! 数据点可见
    void slotSetReCurveLineStyle(int);   //! 曲线样式
    void slotSetVarinatName();           //! 变量名

    /*!
    \section 功能函数
    */
private:
    CReferenceCurve      *m_reCurve;     //! 曲线数据
    QList<CXAxis *>       m_xList;
    QList<CYAxis *>       m_yList;
    bool                  m_isRealTime;  //! 是否实时

    CPoint               *m_normalPoint; //! 数据点

    QColor m_reCurveColor;                //! 曲线颜色
    QColor m_normalPointColor;            //! 点颜色
    int m_reCurveLineStyle;              //! 曲线线型
    IMDRTDBManagerUI  *m_variantForm;


private:
    ITranslateWidgetTool *m_translateTool;//!多语言

};

#endif // CREFERENCECURVEPROPERTYDIALOG_H
