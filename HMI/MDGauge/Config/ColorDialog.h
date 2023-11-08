/*!
\author emhhbmdsZQ==
\section  颜色对话框类，通过调用这个累类来对仪表组件中各部分进行颜色设置。
*/

#ifndef COLORDIALOG_H
#define COLORDIALOG_H
#include <QDialog>

namespace Ui {
class CColorDialog;
}

class CColorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CColorDialog(QWidget *parent = 0);
    ~CColorDialog();
private:
    Ui::CColorDialog *ui;

protected:
    void initDialog();
public:
    /*!
    \section 设定颜色的初始值
    */
    QColor m_color;
    qint32 m_R ;
    qint32 m_G ;
    qint32 m_B ;
    qint32 m_A ;
public:
    void setColor(QColor c);

private:
    QRect m_rect;   //!  显示当前颜色的矩形框

private:
    bool eventFilter(QObject *watched, QEvent *event);   //! 手动输入颜色的各分量值

protected:
    void paintEvent(QPaintEvent*);   //!   绘制矩形框并用当前选择的颜色进行填充
    void mousePressEvent(QMouseEvent* event);   //!   点击矩形框区域调用系统颜色对话框

private slots:
/*!
  \section 设定颜色的各个分量的值
 */
    void setAColor(qint32 value);
    void setRColor(qint32 value);
    void setGColor(qint32 value);
    void setBColor(qint32 value);
    void on_OKBtn_clicked();
    void on_CancelBtn_clicked();

};

#endif // COLORDIALOG_H
