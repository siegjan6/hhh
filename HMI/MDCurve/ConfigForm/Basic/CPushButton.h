/*!

\author eHVr

\section 功能

    重绘PushButton，使之具有边框
    使用： 外观设置界面中，背景色填充选项按钮
*/
#ifndef CPUSHBUTTON_H
#define CPUSHBUTTON_H

#include <QPushButton>
#include <QPaintEvent>

class CPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CPushButton(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent*);
};

#endif // CPUSHBUTTON_H
