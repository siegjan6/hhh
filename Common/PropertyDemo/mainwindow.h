/*!

\author xuk

\section 功能

测试窗体类，用于测试属性栏
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QMap>
#include <QIcon>

#include "PropertyBrowser.h"
#include "ctest.h"
#include "csubtest.h"
class QtBrowserItem;


namespace Ui {
class MainWindow;
}

typedef QMap<int,QIcon> QtIconMap;
/*!
 \brief class MainWindow

*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initUI(); //! 初始化窗体
private:
    void createDock();//! 创建Dock窗体
private slots:
    void setDynamicProperty();

    void setHide();

    void on_changeBlueBtn_clicked();

    void on_changeGreenBtn_clicked();

    void on_setHideBtn_clicked();

    void on_setShowBtn_clicked();

    void on_setROColorBtn_clicked();

    void on_setTipBtn_clicked();

    void on_setBackColorBtn_clicked();

    void on_SetAllROBtn_clicked();

    void on_reSetAllROBtn_clicked();

    void on_setDynamicBtn_clicked();

    void on_reSetDynamicBtn_clicked();

    void on_insertEnumsBtn_clicked();

    void on_expandAllBtn_clicked();

private:
    Ui::MainWindow *ui;

    CPropertyBrowser    *m_objectController;//!  属性栏
    CPropertyBrowser    *m_showController;
    QObject             *m_controlledObject;//!  QObject 指针，存放属性管理对象的指针
    CTest               *m_test;        //!  测试类
    CSubTest            *m_subTest;//! 测试子类

};

Q_DECLARE_METATYPE(QIcon)
Q_DECLARE_METATYPE(QtIconMap)      //! 使用枚举类图标时，务必声明,否则在Windows下报错
#endif // MAINWINDOW_H
