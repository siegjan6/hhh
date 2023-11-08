#ifndef DEVPROJECTMANAGER_H
#define DEVPROJECTMANAGER_H

#include <QObject>
#include <QList>
#include "DevProjectItem.h"
#include "MDHMICreator.h"

class MainWindow;

class CDevProjectManager: public QObject
{
    Q_OBJECT
public:
    CDevProjectManager();
    virtual ~CDevProjectManager();
/*!
\section property
*/
public:
    void setTreeWidget(QTreeWidget *value);
    QTreeWidget* treeWidget(){return m_treeWidget;}
    void setMDIArea(QMdiArea *value){m_mdiArea = value;}
    QMdiArea* mdiArea(){return m_mdiArea;}
    IHMIStudio *hmiStudio(){return m_hmiStudio;}
    void setMainWindow(MainWindow *value);
    MainWindow *mainWindow();
    void setActiveProjectItem(CDevProjectItem *value);
    CDevProjectItem* activeProjectItem(){return m_activeProjectItem;}
private:
    IHMIStudio* m_hmiStudio;
    QTreeWidget *m_treeWidget;
    QMdiArea *m_mdiArea;
    MainWindow *m_mainWindow;
    CDevProjectItem *m_activeProjectItem;//当前活动项目
/*!
\section field
*/
private:
    QList<CDevProjectItem*> m_projectItems;
    QMenu *m_popupMenu;
/*!
\section private function
*/
private:
    void init();
    void activeProject();
    void runProject();
    void virtualRunProject();
    void locateProject();
    void closeProjectFun();
    void reloadProject();
    void callTreeMenu(const QPoint &pos);

    bool removeItem(CDevProjectItem *item);
    void removeAllItem();
    CDevProjectItem *findItem(const QString &fullName);
    CDevProjectItem *findItem(QTreeWidgetItem *item);
    void resetCurrentProject();
    void updateActiveUI();
    void changeItemFont(CDevProjectItem *project, bool isSelect = false);
public:
    void closeProject(CDevProjectItem *item);
    void closeAllProject();
    bool addProject(const QString &fullName);
/*!
\section language
*/
public:
    void changeLanguage();
    void collectWords();
};

#endif // DEVPROJECTMANAGER_H
