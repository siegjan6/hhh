#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QMenuBar>
#include <QList>
#include "contentqsplitter.h"

namespace Ui {
class MainWidget;
}

class ITestorInterface;
class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();
    void DisplayReSize();
public slots:
    void slotNewProject();
    void slotOpenProject();
private:
    void CreateMenus();
    void  InitLayout();
    void loadPlugins();
private:
    Ui::MainWidget *ui;
    QMenuBar * m_pMenuBar;
    QMenu * m_pFileMenu;
    QAction * m_pNewProjectAction;
    QAction * m_pOpenProjectAction;
    ContentQSplitter * m_pContentWidget;
    QList< ITestorInterface *> m_Plugins;
};

#endif // MAINWIDGET_H
