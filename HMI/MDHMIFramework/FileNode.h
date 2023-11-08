/*!
\author aGRw 2014.11.14
\section 节点操作类，用于图形窗体管理
*/

#ifndef FILENODE_H
#define FILENODE_H

#include <QTreeWidgetItem>
#include "ITranslateWidgetTool.h"

class CHMIFramework;

class CFileNode : public QObject
{
    Q_OBJECT
public:
    CFileNode();
    virtual ~CFileNode();
/*!
\section const
*/
private:
    QString sExtension;
    QString sRootName;
    QString sAddFile;
    QString sAddDir;
    QString sCopy;
    QString sPaste;
    QString sDel;
    QString sRename;
    QString sCopyName;
    QString sNewDirName;
    QString sNewFileName;
    QString sNodeSign;
    static QString sRootSign;
    QString sNullHint;
    QString sRepeatHint;
    QString sSubToParentHint;
    QString sVectorTabName;
    QString sDelHint;
/*!
\section initalize
*/
public:
    //!根据root和framework生成一个文件树
    void initNodes(QTreeWidgetItem* root, CHMIFramework* framework);
    //!清空root所在的节点
    void close(QTreeWidgetItem* root);
private:
    void initTreeView(QTreeWidgetItem* root);
    void initRoot(QTreeWidgetItem* root, CHMIFramework* framework);
    void initMenu();

    void clearNode(QTreeWidgetItem* node);
private:
    QMenu* m_menu;
/*!
\section enum
*/
public:
    enum class FileType { Dir, File };
    enum class NodeType { Root, Dir, File };
    enum class NodeOperate { AddFile, AddDir, Rename };
/*!
\section field
*/
private:
    QTreeWidget* m_treeView;
/*!
\section property
*/
public:
    QTreeWidgetItem* selectNode();
    QWidget* view();
/*!
\section node operate
*/
public:
    void getNodes(const QString& path, const QString& pattern, QTreeWidgetItem* parent, QTreeWidget* tree);
    static CHMIFramework* getFramework(QTreeWidgetItem* node);
private:
    static QTreeWidgetItem* getRoot(QTreeWidgetItem* node);
    static bool isRoot(QTreeWidgetItem* node);
    bool isHmiNode(QTreeWidgetItem* node);
    QTreeWidgetItem* createNode(const QString& fullPath, FileType type);
    QTreeWidgetItem* cloneNode(QTreeWidgetItem* source);
    void insertSortedNode(QTreeWidgetItem* parent, QTreeWidgetItem* child);
    void copyChildNodes(QTreeWidgetItem* source, QTreeWidgetItem* target);
    void setNodeIcon(QTreeWidgetItem* node, FileType type);
/*!
\section file operate
*/
public:
    //!获取相对路径
    QString getRelativeFilePath(QTreeWidgetItem* node);
    static FileType getFileType(QTreeWidgetItem* node);
    static QString getFilePath(QTreeWidgetItem* node);
private:
    void copyFolder(const QString& source, const QString& target);
    void removeDirectory(const QString& fullName);
/*!
\section menu
*/
private:
    void setMenuState(NodeType type);
    bool isValidCopy(QTreeWidgetItem* copyNode);
    QString createNewName(QTreeWidgetItem* parentNode, FileType type);
    void createCopyName(QTreeWidgetItem* parentNode, QTreeWidgetItem* newNode);
    void nodeAfterLabelEditEvent(NodeOperate operate, QTreeWidgetItem* node, const QString& oldFullName);
    bool isValidName(QTreeWidgetItem* node, QString& label, QString& message);
    void addEvent(FileType type);
    //!如果copyNode包含在deleteNode中，则copyNode置为NULL
    void resetCopyNode(QTreeWidgetItem* deleteNode);
private:
    static QTreeWidgetItem* m_copyNode;
    NodeOperate m_operateType;

    QAction* m_actAddFile;
    QAction* m_actAddDir;
    QAction* m_actCopy;
    QAction* m_actPaste;
    QAction* m_actDel;
    QAction* m_actRename;
/*!
\section menu event
*/
private:
    void menuStripNodeOpening(const QPoint &pos);
    void menuItemAddFileClick();
    void menuItemAddDirClick();
    void menuItemCopyClick();
    void menuItemPasteClick();
    void menuItemDelClick();
    void menuItemRenameClick();

    bool showMessage(const QString& text);
    void setNodeEdit(QTreeWidgetItem* node);
    void editingFinished();
/*!
\section treeview event
*/
protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    void treeViewDoubleClick();
    void treeViewAfterLabelEdit(QTreeWidgetItem *node, QLineEdit* edit);
    void treeViewNodeMouseClick(QTreeWidgetItem* node);
    bool treeViewKeyDown(QKeyEvent *e);

private:
    QString m_oldFullName;
/*!
\section form
*/
private:
    void createForm(QTreeWidgetItem* node);
    void openForm(QTreeWidgetItem* node);
    void renameForm(CHMIFramework* framework, QTreeWidgetItem* node, const QString& oldFullName);
    void renameForms(QTreeWidgetItem *node, const QString& oldFullName);
    void closeForm(CHMIFramework* framework, QTreeWidgetItem *node);
    void closeForms(QTreeWidgetItem* node);
/*!
\section file
*/
public:
    void getFileNames(const QString& path, QStringList& names);
/*!
\section language
*/
public:
    void changeLanguage(QTreeWidgetItem* root);
private:
    ITranslateWidgetTool* m_transMenu;
};

#endif // FILENODE_H
