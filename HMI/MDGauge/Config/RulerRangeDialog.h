/*!
\author emhhbmdsZQ==
\section  直表临界区的属性配置对话框类
*/

#ifndef RULERRANGEDIALOG_H
#define RULERRANGEDIALOG_H
#include <QDialog>
#include "DrawData.h"
#include "PaintButton.h"

class CColorDialog;
namespace Ui {
class CRulerRangeDialog;
}

class CRulerRangeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CRulerRangeDialog(QWidget *parent = 0);
    ~CRulerRangeDialog();

private:
    Ui::CRulerRangeDialog *ui;

private:
    CRulerRange* m_rRange;     //!  直表临界区的数据类指针
    CPaintButton* m_rRangeStyle;
    CPaintButton* m_rRangeArrange;
    CPaintButton* m_edgeStyle;
    CPaintButton* m_edgeColor;
    CPaintButton* m_gradient;
    CPaintButton* m_color1;
    CPaintButton* m_color2;

protected:
    void initDialog();
    void paintEvent(QPaintEvent*);

public:
    void setData(CData* data);    //!  将保存的直表临界区的各属性值显示到界面

private:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    /*!
     \section 对直表临界区的各个基本属性进行配置。
    */
   void setRangeWid(qint32 value);
   void setOffset(qint32 value);
   void setStartValue(qint32 value);
   void setRangeValue(qint32 value);
   void setEdgeWid(qint32 value);
   /*!
    \section 对直表临界区的各个基本属性设置超限的警告。
   */
   void widWarning();
   void offsetWarning();
   void startWarning();
   void rangeWarning();
   void edgeWarning();
   /*!
    \section 对直表临界区的风格，布局以及边缘线的风格和填充风格进行设置。
   */
   void on_rRangeStyBtn_clicked();
   void on_rRangeArrangeBtn_clicked();
   void on_redgeStyleBtn_clicked();
   void on_rgradStyleBtn_clicked();
   /*!
    \section 对直表临界区的各部分颜色进行设置。
   */
   void on_redgeColorBtn_clicked();
   void on_rcolor1Btn_clicked();
   void on_rcolor2Btn_clicked();
};

#endif // RULERRANGEDIALOG_H
