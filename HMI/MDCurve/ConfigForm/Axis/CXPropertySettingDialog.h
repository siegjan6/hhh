/*!

\author eHVr

\section 功能
     单一X轴属性设置界面
*/
#ifndef CXPROPERTYSETTINGDIALOG_H
#define CXPROPERTYSETTINGDIALOG_H

#include <QDialog>
#include <QVector>

class CXAxis;
class ITranslateWidgetTool;
namespace Ui {
class CXPropertySettingDialog;
}

class CXPropertySettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CXPropertySettingDialog(QWidget *parent = 0);
    ~CXPropertySettingDialog();
public:
    //! 静态函数.模态执行一个x轴属性设置窗体,并返回一个x轴指针
    static CXAxis *getXAxis(bool *ok, CXAxis *initial, QWidget *parent = 0);

    void setData(CXAxis *x);
/*!
\section 初始化
*/
private:
    void initialize();
    void initializeData();
    void initializeUI();

    void initializeCommon();
    void initializeValueType();
    void initializeTimeType();

/*!
\section 应用内容修改
*/
private:
    void applyChanges();
    void applyValueChanges();
    void applyTimeChanges();

/*!
\section 信号槽
*/
private:
    void createConnections();
private slots:
    void slotSetAxisType(int);     //! 设置X轴类型
    void slotOkButtonClicked();
    void slotCancelButtonClicked();
    void slotSetFont();
    void slotSetFontColor();
    void slotSetAxisLineProperty();

/*!
\section tools functions
*/
private:
    void setColor( QColor &initialColor, QPushButton &colorButton);
private:
    Ui::CXPropertySettingDialog *ui;
    QVector<int> m_time;
    QFont        m_font;                          //! 字体
    CXAxis       *m_axis;                         //! 轴
    QColor       m_fontColor;
    ITranslateWidgetTool  *m_translateTool; //! 多语言
};

#endif // CXPROPERTYSETTINGDIALOG_H
