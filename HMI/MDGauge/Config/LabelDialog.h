/*!
\author emhhbmdsZQ==
\section  仪表标签的属性配置对话框类
*/

#ifndef LABELDIALOG_H
#define LABELDIALOG_H
#include <QDialog>
#include "DrawData.h"
#include "PaintButton.h"

class  CColorDialog;
namespace Ui {
class CLabelDialog;
}

class CLabelDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CLabelDialog(QWidget *parent = 0);
    ~CLabelDialog();

private:
    Ui::CLabelDialog *ui;

protected:
    void initDialog();
    void paintEvent(QPaintEvent*);  //!  在各个图标外绘制矩形框

public:
    void setData(CData* data);  //!  将保存的仪表标签的各属性的数值显示到界面
    CPaintButton* m_color;
    //! 标签的位置坐标
    qreal m_x;
    qreal m_y;

private:
    CDataLabel* m_label;     //!  仪表标签的数据类指针

private:
    bool eventFilter(QObject *watched, QEvent *event);

/*!
\section 对标签的各个属性进行配置。
 */
private slots:
    void setLeft(qint32 value);
    void setTop(qint32 value);
    void setSize(qint32 value);
    void setFontName(QString str);
    void setString(QString str);
    void setTextBold(qint32 state);

    void leftWarning();
    void topWarning();
    void sizeWarning();

    void on_colorBtn_clicked();

};

#endif // LABELDIALOG_H
