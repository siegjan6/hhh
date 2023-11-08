/*!
\author aGRw 2016.09.28
\section DockWidget配置
*/
#ifndef DOCKCONFIG_H
#define DOCKCONFIG_H

#include <QDockWidget>
#include <QMenu>
#include <QObject>

class CDockConfig : public QObject
{
    Q_OBJECT
public:
    CDockConfig(QMainWindow* parent);
    ~CDockConfig();
private:
    QList<QDockWidget*> m_list;
    QStringList m_titles;
public:
    void loadWidget(QList<QWidget *> *widgets);
    void fillDockMenu(QMenu* m);

private:
    void init(QMainWindow *parent);
    void showDock();
};

#endif // DOCKCONFIG_H
