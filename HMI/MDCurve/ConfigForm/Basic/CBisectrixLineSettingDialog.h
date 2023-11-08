/*!

\author eHVr
\class CBisectrixLineSettingDialog
\section 功能
    基础线条属性设置对话框
    \a lineColor
    \a lineWidth
    \a lineStyle
*/
#ifndef CBISECTRIXLINESETTINGDIALOG_H
#define CBISECTRIXLINESETTINGDIALOG_H

#include <QDialog>

class ITranslateWidgetTool;
class CBisectrixLine;
namespace Ui {
class CBisectrixLineSettingDialog;
}


class CBisectrixLineSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CBisectrixLineSettingDialog(QWidget *parent = 0);
    ~CBisectrixLineSettingDialog();

public:
    /*!
     \brief
            获取基本线指针
     \param ok      状态
     \param initial 初始的基础线
     \param parent  父类
    */
    static CBisectrixLine *getLine(bool *ok,CBisectrixLine *initial,QWidget *parent = 0);
    void setData(CBisectrixLine *line); //!  获取后台数据
/*!
\section 初始化数据
*/
private:
    void initialize();
    void initializeData();
    void initializeUI();
    void initializeUIData();
private:
    void applyChanges();       //! 应用修改内容
private:
    void setLineStyles();      //! 设置线型
    void createConnections();  //! 创建信号槽链接

/*!
\section 槽函数
*/
private slots:
    void slotLineStyleChanges(int);
    void slotSetLineColor();
    void slotSetLineWidth(int);
    void slotSetLineStyle(int);

    void slotAccept();
    void slotCancel();

private:
    Ui::CBisectrixLineSettingDialog *ui;

    CBisectrixLine  *m_line;          //! 线
    QColor          m_lineColor;      //! 颜色
    int             m_lineWidth;      //! 宽度
    int             m_lineStyle;      //! 线型
private:
    ITranslateWidgetTool *m_transTool; //! 多语言
};

#endif // CBISECTRIXLINESETTINGDIALOG_H
