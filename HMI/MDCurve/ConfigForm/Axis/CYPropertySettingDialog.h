/*!

\author eHVr

\section 功能

    单一Y轴属性设置对话框

*/
#ifndef CYPROPERTYSETTINGDIALOG_H
#define CYPROPERTYSETTINGDIALOG_H

#include <QDialog>

class CYAxis;
class ITranslateWidgetTool;
namespace Ui {
class CYPropertySettingDialog;
}

class CYPropertySettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CYPropertySettingDialog(QWidget *parent = 0);
    ~CYPropertySettingDialog();


public:
    //!  静态函数.模态执行一个Y轴属性设置窗体,并返回一个Y轴指针
    static CYAxis * getYAxis(bool *ok, CYAxis *initial, QWidget *parent = 0);
    void setData(CYAxis *y);
/*!
\section 初始化
*/
private:
    void initialize();
    void initializeData();
    void initializeUI();

private:
    void applyChanges();//! 应用内容修改
/*!
\section 信号槽
*/
private:
    void createConnections();
private slots:
    void slotOkButtonClicked();
    void slotCancelButtonClicked();
    void slotSetAxisLineVisible(bool);
    void slotSetAxisLineProperty();
    void slotSetFont();
    void slotSetFontColor();

private:
    void setColor( QColor &initialColor, QPushButton &colorButton);
private:
    Ui::CYPropertySettingDialog *ui;

    CYAxis   *m_yAxis;
    QFont     m_font;
    QColor    m_fontColor;
private:
    ITranslateWidgetTool *m_translatTool;//! 多语言
};

#endif // CYPROPERTYSETTINGDIALOG_H
