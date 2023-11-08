/*!
\author emhhbmdsZQ==
\section  仪表临界区的属性配置对话框类
*/
#ifndef RANGEDIALOG_H
#define RANGEDIALOG_H
#include <QDialog>
#include "DrawData.h"
#include "PaintButton.h"

class CColorDialog;
namespace Ui {
class CRangeDialog;
}

class CRangeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CRangeDialog(QWidget *parent = 0);
    ~CRangeDialog();

private:
    Ui::CRangeDialog *ui;

protected:
    void initDialog();
    void paintEvent(QPaintEvent*);  //! 在对话框中的各个图标外绘制矩形框
public:   
    void setData(CData* data);    //!  将保存的仪表临界区的各属性值显示到界面

private:
    CDataRange* m_range;   //! 仪表临界区数据类指针
    CPaintButton* m_rangeStyle;
    CPaintButton* m_edgeStyle;
    CPaintButton* m_edgeColor;
    CPaintButton* m_gradient;
    CPaintButton* m_color1;
    CPaintButton* m_color2;

private:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    /*!
     \section 对临界区的各个基本属性进行配置。
    */
    void setRadius(qint32 value);
    void setWidth(qint32 value);
    void setEdgeWidth(qint32 value);
    void setStartValue(QString str);
    void setRangeValue(QString str);
    /*!
     \section 对临界区的各个基本属性设置超限的警告。
    */
    void radiusWarning();
    void widWarning();
    void edgeWarning();
    /*!
     \section 对临界区的风格，边缘线风格以及填充样式进行配置。
    */
    void on_rangeStyleBtn_clicked();
    void on_edgeStyleBtn_clicked();
    void on_gradStyleBtn_clicked();
    /*!
     \section 对临界区的各部分颜色进行设置。
    */
    void on_edgeColorBtn_clicked();
    void on_color1Btn_clicked();
    void on_color2Btn_clicked();
};

#endif // RANGEDIALOG_H
