/*!
\author aGRw 2015.08.17
\section 动态颜色配置对话框
*/
#ifndef DYNAMICCOLORDIALOG_H
#define DYNAMICCOLORDIALOG_H


#include <QDialog>
#include "DynamicColor.h"

namespace Ui {
class CDynamicColorDialog;
}

class CDynamicColorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CDynamicColorDialog(QWidget *parent = 0);
    ~CDynamicColorDialog();
private slots:
    void on_pushButtonAdd_clicked();
    void on_pushButtonDel_clicked();
    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();
private:
    Ui::CDynamicColorDialog *ui;
/*!
\section property
*/
public:
    CDynamicColorManager* colors(){return &m_colors;}
private:
    CDynamicColorManager m_colors;//!数据
    int m_width;
    int m_height;
    bool hasChangedEvent;//!防止在初始化时触发cellChanged
/*!
\section function
*/
public:
    void init(CDynamicColorManager* colors);
private:
    void initTableWidget();
    void cellChanged(int row, int col);
    void cellDoubleClicked(int row, int col);
    void fillItem(CDynamicColor *color, int row);
};

#endif // DYNAMICCOLORDIALOG_H
