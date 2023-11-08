/*!
\author emhhbmdsZQ==
\section  直表刻度的属性配置对话框类
*/
#ifndef RULERSCALEDIALOG_H
#define RULERSCALEDIALOG_H
#include <QDialog>
#include "DrawData.h"
#include "PaintButton.h"

class CColorDialog;
namespace Ui {
class CRulerScaleDialog;
}

class CRulerScaleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CRulerScaleDialog(QWidget *parent = 0);
    ~CRulerScaleDialog();

private:
    Ui::CRulerScaleDialog *ui;

protected:
    void initDialog();
    void paintEvent(QPaintEvent*);

public:
    void setData(CData* data);    //!  将保存的直表刻度的各属性值显示到界面

private:
    CRulerScale* m_rScale;   //!  直表刻度数据类指针
    CPaintButton* m_rScaleStyle;
    CPaintButton* m_rScaleArrange;
    CPaintButton* m_edgeStyle;
    CPaintButton* m_edgeColor;
    CPaintButton* m_gradient;
    CPaintButton* m_color1;
    CPaintButton* m_color2;

private:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    /*!
     \section 对直表刻度的各个基本属性进行配置。
    */
    void setScaleWid(qint32 value);
    void setLen(qint32 value);
    void setOffset(qint32 value);
    void setSubScale(qint32 value);
    void setEdgeWide(qint32 value);
    void setSub(qint32 state);
    /*!
     \section 对直表刻度的各个基本属性设置超限的警告。
    */
    void widWarning();
    void lenWarning();
    void offsetWarning();
    void subWarning();
    void edgeWarning();
    /*!
     \section 对直表刻度的布局以及边缘线的风格和填充风格进行设置。
    */
    void on_rScaleStyleBtn_clicked();
    void on_rScaleArrangeBtn_clicked();
    void on_redgeStyleBtn_clicked();
    void on_rgradStyleBtn_clicked();
    /*!
     \section 对直表刻度的各部分颜色进行设置。
    */
    void on_redgeColorBtn_clicked();
    void on_rcolor1Btn_clicked();
    void on_rcolor2Btn_clicked();
};

#endif // RULERSCALEDIALOG_H
