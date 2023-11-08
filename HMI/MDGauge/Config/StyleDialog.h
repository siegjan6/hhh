/*!
\author emhhbmdsZQ==
\section  仪表各组件的风格窗体类对话框。
*/

#ifndef STYLEDIALOG_H
#define STYLEDIALOG_H

#include <QDialog>
#include <QLabel>
#include "DrawDefine.h"

namespace Ui {
class CStyleDialog;
}

class CStyleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CStyleDialog(QWidget *parent = 0);
    ~CStyleDialog();

private:
    Ui::CStyleDialog *ui;

protected:
    void paintEvent(QPaintEvent*);     //!  给各个风格窗体绘制背景以及选中状态显示
    void mousePressEvent(QMouseEvent* event);  //!  鼠标按下事件确定选中的是哪种风格

public:
    qint32 m_index;   //!  选中按钮索引
private: 
    qint32 m_count;  //!  按钮数量
    QLabel* m_labArray[12];   //!  按钮数组
    //!  按钮大小
    qint32 m_width;
    qint32 m_height;
private:
    void buttonDown(QPoint point);
    void showStyle();
    /*!
     \section 确定选择的窗体类型。
    */
public:
    void setStyle(styleType type);
    styleType getStyle();
    styleType m_type;

};

#endif // STYLEDIALOG_H
