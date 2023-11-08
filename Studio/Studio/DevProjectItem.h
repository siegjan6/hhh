#ifndef DEVPROJECTITEM_H
#define DEVPROJECTITEM_H

#include <QObject>
#include <QMdiSubWindow>
#include <QMainWindow>
#include <QList>

#include "MDMultiLanguage.h"
#include "ITranslateWidgetTool.h"
#include "IStudioModule.h"
#include "IStudioInterface.h"
#include "IHMIFramework.h"
#include "IDictionaryManager.h"

class CDevProjectManager;
class MainWindow;

class CDevProjectItem : public QObject, public IStudioInterface
{
    Q_OBJECT
public:
    CDevProjectItem();
    virtual ~CDevProjectItem();
public:
/*!
\section property
*/
public:
    QString fullName();                 //项目全名
    CMDProjectConfiger *projectConfiger(){return m_configer;}
    CDevProjectManager *manager(){return m_manager;}
    void setManager(CDevProjectManager *value){m_manager = value;}
    QTreeWidget *treeWidget();
    QTreeWidgetItem *projectRoot(){return m_projectRoot;}
    QMdiArea* mdiArea();                //Mdi区域
    MainWindow *mainWindow();           //主窗口
    IMDRTDBManagerUI *rtdbManagerUI();  //实时数据库UI
    IHMIFramework* hmiFramework();      //图形界面
    CPropertyBrowser *propertyBrowser();//属性框
private:
    CMDProjectConfiger* m_configer;    //项目配置器
    CDevProjectManager* m_manager;      //管理器
    QTreeWidgetItem* m_projectRoot;     //项目根节点
/*!
\section field
*/
private:
    QList<IStudioModule*> m_modules;
/*!
\section interface
*/
public:
    //激活窗口
    bool setActiveForm(CBaseForm *form);
    //打开窗口
    void openMDIWindow(CBaseForm *form);
/*!
\section public function
*/
public:
    bool load(const QString &fullName);
    bool unload();
    static CBaseForm* getBaseForm(QMdiSubWindow *sub);
/*!
\section private function
*/
private:
    void initModules();
    void clear();
/*!
\section event
*/
protected:
    bool eventFilter(QObject* watched, QEvent * event);
/*!
\section language
*/
public:
    void initLanguage();
    void releaseLanguage();
    void changeLanguage();
    void collectWords();
private:
    IDictionaryManager* m_dictManager;
};

#endif // DEVPROJECTITEM_H
