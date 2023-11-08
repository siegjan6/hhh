/*!
\author enli 2015.01.08
\section 选择变量对话框，用于中间变量+tang：selectVariables
*/
#ifndef SELECTVARIABLEDIALOG_H
#define SELECTVARIABLEDIALOG_H

#include <QTreeView>
namespace Ui {
class CTreeViewEx;
}

class CTreeViewEx : public QTreeView
{
    Q_OBJECT

public:
    explicit CTreeViewEx(QWidget *parent = 0);
    ~CTreeViewEx();

public:
     //重载鼠标按下事件
	 void mousePressEvent(QMouseEvent* event);
signals:
	 void mousePressed();
};

#endif // SELECTVARIABLEDIALOG_H
