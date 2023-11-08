/*!

\author dGFuZ3p3

\section 功能

脚本界面类的实现类，管理脚本界面。

*/

#ifndef MDSCRIPTUIIMP_H
#define MDSCRIPTUIIMP_H

#include <QtWidgets/qtreewidget.h>
#include <QObject>
#include "IStudioInterface.h"
#include "MDScriptFileNode.h"

class CMDJavaScriptCodeEditor;
class CMDProjectConfiger;
class CMDJavaScriptCodeEditor2;
class CMDScriptUIImp : public QObject
{
    Q_OBJECT
public:
    CMDScriptUIImp();
    ~CMDScriptUIImp();

/*!
\section 初始化
*/
public:
    QTreeWidgetItem *m_rootInStudio ;// NULL;
    IStudioInterface* m_studioInterface;
public:
    void load();
    void unLoad();
    QString scriptFolder();
    CMDScriptFileNode m_fileNode;
    bool createScriptFile(const QString& fileName);

/*!
\section 与开发器交互；通过树控件管理对象
*/
public:
    QString fileItemPath(QTreeWidgetItem *pItem,int column);
private:
    void scriptWindowClose();
protected:
    QTreeWidget *treeWidget();
    QTreeWidgetItem *treeRoot();
private slots:
    void on_actionTreeWindow_destroyed(QObject* obj);

/*!
\section 管理打开的脚本编辑器界面
*/
public:
    void open(const QString& fileName);
    bool close(CMDJavaScriptCodeEditor2* editor);
    void closeAll();
    CMDJavaScriptCodeEditor2* find(const QString& fileName);
protected:
    QMap<QString, CMDJavaScriptCodeEditor2*> m_editorMap;
};

#endif // MDSCRIPTUIIMP_H
