/*!
\author aGRw 2015.07.22
\section 节点操作类，应用于文件操作
*/
#ifndef STORAGEFILENODE_H
#define STORAGEFILENODE_H

#include <QObject>
#include <QTreeWidget>
#include <QAction>
#include <QMenu>
#include "MDMultiLanguage.h"

class CStorageFileNode : public QObject
{
    Q_OBJECT
public:
    explicit CStorageFileNode(QObject *parent = 0);
    ~CStorageFileNode();
//////////////////////////////////////////////////////////////////////////
private:
    QTreeWidget* m_tree;
    QString m_basePath;//!初始路径
    bool m_canEvent;
    QAction* m_actAddDir;
    QAction* m_actDel;
    QAction* m_actRename;
    QMenu* m_menu;
    QString m_oldFullName;//!原始目录名称
    bool m_isRename;
//////////////////////////////////////////////////////////////////////////
private:
    QString sRoot;
    QString sAddDir;
    QString sDel;
    QString sRename;
    QString sNewDirName;
    QString sNullHint;
    QString sRepeatHint;
    QString sDelHint;
//////////////////////////////////////////////////////////////////////////
public:
    void init(QTreeWidget* tree, const QString& path);

private:
    //!获取节点
    void getNodes(const QString &path, QTreeWidgetItem *parent);
    //!创建节点
    QTreeWidgetItem* createNode(const QString &fullPath);
    //!获取路径
    QString getFilePath(QTreeWidgetItem *node);
    void currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem*);
    void initMenu();

signals:
    void currentPathChanged(const QString& path);

/*!
\section menu event
*/
private:
    void menuStripNodeOpening(const QPoint &pos);
    void menuItemAddDirClick();
    void menuItemDelClick();
    void menuItemRenameClick();

    bool showMessage(const QString& text);
    void setNodeEdit(QTreeWidgetItem* node);
    void editingFinished();

    QString createNewName(QTreeWidgetItem *parentNode);
    void insertSortedNode(QTreeWidgetItem *parent, QTreeWidgetItem *child);
    void treeViewAfterLabelEdit(QTreeWidgetItem *node, QLineEdit *edit);
    bool isValidName(QTreeWidgetItem *node, QString &label, QString &message);
    void nodeAfterLabelEditEvent(QTreeWidgetItem *node, const QString& oldFullName, bool isRename = false);
    void removeDirectory(const QString &fullName);
    void setMenuState(bool isRoot);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
/*!
\section language
*/
public:
    void changeLanguage();
private:
    ITranslateWidgetTool* m_transTool;


};

#endif // STORAGEFILENODE_H
