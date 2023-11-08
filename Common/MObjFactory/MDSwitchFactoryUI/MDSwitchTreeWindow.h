#ifndef MDSWITCHTREEWINDOW_H
#define MDSWITCHTREEWINDOW_H

#include "../../include/MDObjectManagerUI/MDObjectTreeWindow.h"

class CMDSwitchFactory;
class CMDMObj;
class CMDSwitchTreeWindow : public CMDObjectTreeWindow
{
    Q_OBJECT
public:
    explicit CMDSwitchTreeWindow(QWidget *parent = 0);
    ~CMDSwitchTreeWindow();

/*!
\section 初始化
*/
public:
    void init(CMDSwitchFactory *f);
protected:
    CMDSwitchFactory *m_f;
    QVariant m_actionIcon;

/*!
\section 重载的接口
*/
public:
    virtual bool save(); //! 保存到文件
public:
    virtual void buildObjectTree(CMDObjectTreeItem* rootItem);
    virtual bool addObject(void* objectUI, void *objectConfiger, qint16 subType);
    virtual bool deleteObject(QList<CMDObjectTreeItem*>& selectedTreeItems);
    virtual void objectDeleted(QList<CMDObjectTreeItem*>& selectedTreeItems);
    virtual bool configObject(void* objectUI, void *objectConfiger, QList<CMDObjectTreeItem*>& selectedTreeItems);
    virtual bool configObjectInBrowser(void* objectUI, void *objectConfiger, CMDObjectTreeItem *selectedTreeItem);
    virtual bool pasteObject(void* objectPtr);
public:
    virtual QString objectName(void* objectPtr);
    virtual bool resultOnSetData(qint16 objectType);
    virtual QVariant objectData(bool enableChecked, CMDObjectTreeItem *item, int column, int role) const;
    virtual bool isTopObject(qint16 objectType);
public:
    virtual void initTreeColumnWidth(bool enableChecked, QTreeView *treeView);
    virtual void initHeader(bool enableChecked, QStringList& headerList);
public:
    virtual void onMenuItemVisible(CMDObjectTreeItem *selectedTreeItem);
    virtual bool initAddAction();
    virtual void initSubMenuOfAdd(QMenu* subMenu);
public:
    virtual bool batchAddObject(void* objectUI, void *objectConfiger, CMDObjectTreeItem *selectedTreeItem);
protected:
    QVariant actionData(bool enableChecked, CMDMObj* mobj, int propIndex) const;

/*!
\section 多语言
*/
public:
    void changeLanguage();
};

#endif // MDSWITCHTREEWINDOW_H
