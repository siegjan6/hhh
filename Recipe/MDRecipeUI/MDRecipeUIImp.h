#ifndef MDRECIPEUIIMP_H
#define MDRECIPEUIIMP_H

#include "MDDownloadDialog.h"
#include "MDRecipeContacter.h"

/*!
 * \brief 实现 配方专家 的联系人类
 */
class CMDRecipeUIImp;
class CMDRecipeContacterImp_dl : public CMDRecipeContacter
{
public:
    CMDRecipeContacterImp_dl();
    CMDRecipeUIImp* m_UIImp ;// NULL;
public:
    virtual bool writeVarGeneral(CMDRecipe* recipe);
    virtual bool writeVarQuick(CMDRecipe* recipe);
    virtual QVariant varValue(const QString &varName);
    virtual bool isQuickVar(const QString &varName);
public:
    virtual void onDownloadTaskAdded(const QString &name, int showProcessDialog=1);
    virtual void onBeginDownloading(const QString &name, int total);
    virtual void onEndDownloading(const QString &name, DownloadFinishFlag finishFlag);
    virtual void onDownloadProcess(const QString &msg, int total, int completed);
};

#include <QtWidgets/qtreewidget.h>
#include <QObject>
#include "IStudioInterface.h"
#include "MDRecipeFileNode.h"
class CMDJavaScriptCodeEditor2;
class CMDRecipeExpert;
class CMDRecipeUIImp : public QObject
{
    Q_OBJECT
public:
    CMDRecipeUIImp(QObject *parent = 0);
    void load();
    void unload();
    CMDRecipeExpert* m_recipeExpert;
    CMDRecipeContacterImp_dl m_contacter;
    CMDDownloadDialog m_downloadDialog;
public:
    void onDownloadTaskAdded(const QString &name, int showProcessDialog=1);
    void onBeginDownloading(const QString &name, int total);
    void onDownloadProcess(const QString &msg, int total, int completed);
signals:
    void downloadTaskAdded(const QString &name, int showProcessDialog=1);
    void beginDownloading(const QString &name, int total);
    void downloadProcess(const QString &msg, int total, int completed);

/*!
\section 初始化
*/
public:
    QTreeWidgetItem *m_rootInStudio ;// NULL;
    IStudioInterface* m_studioInterface;
public:
    QString recipeFolder();
    CMDRecipeFileNode m_fileNode;
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
\section 管理打开的配方编辑器界面
*/
public:
    void open(const QString& fileName);
};

#endif // MDRECIPEUIIMP_H
