/*!

\author dGFuZ3p3

\section 功能

脚本文件界面管理类，以树结构管理脚本文件。
来自 houdp ，不做过多注释。

*/

#ifndef MDSCRIPTFILENODE_H
#define MDSCRIPTFILENODE_H

#include <QTreeWidgetItem>

class CMDScriptUIImp;

const QString sExtension  = ".js";
const QString sRootName = "全局脚本";
const QString sAddFile = "添加脚本";
const QString sAddDir = "添加组";
const QString sCopy = "复制";
const QString sPaste = "粘帖";
const QString sDel = "删除";
const QString sRename = "重命名";
const QString sCopyName = "复件";
const QString sNewDirName = "subdir";
const QString sNewFileName = "ScriptCode";
const QString sNodeSign = "globalScript";
const QString sNullHint = "名称不能为空";
const QString sRepeatHint = "名称已存在";
const QString sSubToParentHint = "父目录不能拷贝到子目录中";
const QString sVectorTabName = "矢量控件";
const QString sDelHint = "是否确认删除？";

//节点操作类，用于图形窗体管理
class CMDScriptFileNode : public QObject
{
    Q_OBJECT
public:
    CMDScriptFileNode();
    ~CMDScriptFileNode();
//////////////////////////////////////////////////////////////////////////
//const
private:
//////////////////////////////////////////////////////////////////////////
//initialize
public:
    //根据root和scriptUI生成一个文件树
    void initNodes(QTreeWidgetItem* root, CMDScriptUIImp* scriptUI);
    //清空root所在的节点
    void close(QTreeWidgetItem* root);
private:
    void initTreeView(QTreeWidgetItem* root);
    void initRoot(QTreeWidgetItem* root, CMDScriptUIImp* scriptUI);
    void initMenu();

    void clearNode(QTreeWidgetItem* node);
private:
    QMenu* m_menu ;// NULL;
//////////////////////////////////////////////////////////////////////////
//enum
public:
    enum class FileType { Dir, File };
    enum class NodeType { Root, Dir, File };
    enum class NodeOperate { AddFile, AddDir, Rename };
//////////////////////////////////////////////////////////////////////////
//field
private:
    QTreeWidget* m_treeView ;// NULL;
//////////////////////////////////////////////////////////////////////////
//property
public:
    QTreeWidgetItem* selectNode();
//////////////////////////////////////////////////////////////////////////
//node operate
public:
    void getNodes(const QString& path, const QString& pattern, QTreeWidgetItem* parent, QTreeWidget* tree);
private:
    QTreeWidgetItem* getRoot(QTreeWidgetItem* node);
    bool isRoot(QTreeWidgetItem* node);
    bool isScriptNode(QTreeWidgetItem* node);
    QTreeWidgetItem* createNode(const QString& fullPath, FileType type);
    QTreeWidgetItem* cloneNode(QTreeWidgetItem* source);
    void insertSortedNode(QTreeWidgetItem* parent, QTreeWidgetItem* child);
    void copyChildNodes(QTreeWidgetItem* source, QTreeWidgetItem* target);
    CMDScriptUIImp* getscriptUI(QTreeWidgetItem* node);
    void setNodeIcon(QTreeWidgetItem* node, FileType type);
//////////////////////////////////////////////////////////////////////////
//file operate
public:
    //获取相对路径
    QString getRelativeFilePath(QTreeWidgetItem* node);
private:
    FileType getFileType(QTreeWidgetItem* node);
    QString getFilePath(QTreeWidgetItem* node);
    void copyFolder(const QString& source, const QString& target);
    void removeDirectory(const QString& fullName);
//////////////////////////////////////////////////////////////////////////
//menu
private:
    void setMenuState(NodeType type);
    bool isValidCopy(QTreeWidgetItem* copyNode);
    QString createNewName(QTreeWidgetItem* parentNode, FileType type);
    void createCopyName(QTreeWidgetItem* parentNode, QTreeWidgetItem* newNode);
    void nodeAfterLabelEditEvent(NodeOperate operate, QTreeWidgetItem* node, const QString& oldFullName);
    bool isValidName(QTreeWidgetItem* node, QString& label, QString& message);
    void addEvent(FileType type);
    //如果copyNode包含在deleteNode中，则copyNode置为NULL
    void resetCopyNode(QTreeWidgetItem* deleteNode);
private:
    static QTreeWidgetItem* m_copyNode;
    NodeOperate m_operateType ;// NodeOperate::AddDir;

    QAction* m_actAddFile ;// NULL;
    QAction* m_actAddDir ;// NULL;
    QAction* m_actCopy ;// NULL;
    QAction* m_actPaste ;// NULL;
    QAction* m_actDel ;// NULL;
    QAction* m_actRename ;// NULL;
//////////////////////////////////////////////////////////////////////////
//menu event
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
//////////////////////////////////////////////////////////////////////////
//treeview event
protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    void treeViewDoubleClick();
    void treeViewAfterLabelEdit(QTreeWidgetItem *node, QLineEdit* edit);
    void treeViewNodeMouseClick(QTreeWidgetItem* node);
    bool treeViewKeyDown(QKeyEvent *e);

private:
    QString m_oldFullName;
//////////////////////////////////////////////////////////////////////////
//form
private:
    void createForm(QTreeWidgetItem* node);
    void openForm(QTreeWidgetItem* node);
    void renameForm(CMDScriptUIImp* scriptUI, QTreeWidgetItem* node, const QString& oldFullName);
    void renameForms(QTreeWidgetItem *node, const QString& oldFullName);
    void closeForm(CMDScriptUIImp* scriptUI, QTreeWidgetItem *node);
    void closeForms(QTreeWidgetItem* node);
//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
};

#endif // MDSCRIPTFILENODE_H
