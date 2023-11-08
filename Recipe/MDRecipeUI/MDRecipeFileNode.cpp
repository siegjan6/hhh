#include "MDRecipeFileNode.h"

#include <QFileInfo>
#include <QDir>
#include <QAction>
#include <QMenu>
#include <QTreeWidget>
#include <QMessageBox>
#include <QKeyEvent>
#include <QLineEdit>


#include "MDRecipeUIImp.h"

//////////////////////////////////////////////////////////////////////////
QTreeWidgetItem* CMDRecipeFileNode::m_copyNode = NULL;
//////////////////////////////////////////////////////////////////////////

CMDRecipeFileNode::CMDRecipeFileNode()
{
    m_menu = NULL;
    m_treeView = NULL;
    m_operateType = NodeOperate::AddDir;

    m_actAddFile = NULL;
    m_actAddDir = NULL;
    m_actCopy = NULL;
    m_actPaste = NULL;
    m_actDel = NULL;
    m_actRename = NULL;
}
CMDRecipeFileNode::~CMDRecipeFileNode()
{
}
//////////////////////////////////////////////////////////////////////////
void CMDRecipeFileNode::initNodes(QTreeWidgetItem *root, CMDRecipeUIImp *scriptUI)
{
    Q_ASSERT(root != NULL && root->treeWidget() != NULL);

    initTreeView(root);
    initRoot(root, scriptUI);

    //file
    QString project = scriptUI->recipeFolder();
    QDir dir(project);
    if (!dir.exists())
        dir.mkdir(project);
    getNodes(project, "*" + sExtension, root, NULL);
}
void CMDRecipeFileNode::close(QTreeWidgetItem *root)
{
    resetCopyNode(root);
    clearNode(root);
}
void CMDRecipeFileNode::initTreeView(QTreeWidgetItem *root)
{
    if (m_treeView != NULL)
        return;

    m_treeView = root->treeWidget();
    //bind event
    connect(m_treeView, &QTreeWidget::doubleClicked, this, &CMDRecipeFileNode::treeViewDoubleClick);
    connect(m_treeView, &QTreeWidget::itemPressed, this, &CMDRecipeFileNode::treeViewNodeMouseClick);
    m_treeView->installEventFilter(this);

    initMenu();
}
void CMDRecipeFileNode::initRoot(QTreeWidgetItem *root, CMDRecipeUIImp *scriptUI)
{
    clearNode(root);
    root->setText(0, sRootName);
    QVariant v(sRootName);
    root->setData(0, Qt::UserRole, v);
    root->setIcon(0, QIcon(":/images/folder.png"));

    QVariant vClass((qulonglong)scriptUI);
    root->setData(0, Qt::UserRole + 1, vClass);
}
void CMDRecipeFileNode::initMenu()
{
    m_actAddFile = new QAction(QIcon(":/images/new.png"), sAddFile, this);
    m_actAddDir = new QAction(QIcon(":/images/new.png"), sAddDir, this);
    m_actCopy = new QAction(QIcon(":/images/copy.png"), sCopy, this);
    m_actPaste = new QAction(QIcon(":/images/paste.png"), sPaste, this);
    m_actDel = new QAction(QIcon(":/images/delete.png"), sDel, this);
    m_actRename = new QAction(sRename, this);

    m_actCopy->setShortcut(QKeySequence::Copy);
    m_actPaste->setShortcut(QKeySequence::Paste);
    m_actDel->setShortcut(QKeySequence::Delete);

    connect(m_actAddFile, &QAction::triggered, this, &CMDRecipeFileNode::menuItemAddFileClick);
    connect(m_actAddDir, &QAction::triggered, this, &CMDRecipeFileNode::menuItemAddDirClick);
    connect(m_actCopy, &QAction::triggered, this, &CMDRecipeFileNode::menuItemCopyClick);
    connect(m_actPaste, &QAction::triggered, this, &CMDRecipeFileNode::menuItemPasteClick);
    connect(m_actDel, &QAction::triggered, this, &CMDRecipeFileNode::menuItemDelClick);
    connect(m_actRename, &QAction::triggered, this, &CMDRecipeFileNode::menuItemRenameClick);

    m_menu = new QMenu(m_treeView);
    m_menu->addAction(m_actAddFile);
    m_menu->addAction(m_actAddDir);
    m_menu->addSeparator();
    m_menu->addAction(m_actCopy);
    m_menu->addAction(m_actPaste);
    m_menu->addAction(m_actDel);
    m_menu->addAction(m_actRename);

    m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_treeView, &QTreeWidget::customContextMenuRequested,
            this, &CMDRecipeFileNode::menuStripNodeOpening);
}
void CMDRecipeFileNode::clearNode(QTreeWidgetItem *node)
{
    for (int i = 0; i < node->childCount(); i++)
    {
        QTreeWidgetItem* child = node->child(i);
        clearNode(child);
        delete child;
    }
    node->takeChildren();
}
//////////////////////////////////////////////////////////////////////////
QTreeWidgetItem *CMDRecipeFileNode::selectNode()
{
    if (m_treeView != NULL)
    {
        QTreeWidgetItem* node = m_treeView->currentItem();
        if (node != NULL &&
                (isRoot(node) || isScriptNode(node)))
            return  node;
    }

    return NULL;
}
//////////////////////////////////////////////////////////////////////////
QTreeWidgetItem *CMDRecipeFileNode::getRoot(QTreeWidgetItem *node)
{
    while (node != NULL)
    {
        if (isRoot(node))
            return node;
        node = node->parent();
    }

    return NULL;
}
bool CMDRecipeFileNode::isRoot(QTreeWidgetItem *node)
{
    return (QString::compare(node->data(0, Qt::UserRole).toString(), sRootName) == 0);
}
bool CMDRecipeFileNode::isScriptNode(QTreeWidgetItem *node)
{
    return (QString::compare(node->data(0, Qt::UserRole).toString(), sNodeSign) == 0);
}
QTreeWidgetItem *CMDRecipeFileNode::createNode(const QString &fullPath, CMDRecipeFileNode::FileType type)
{
    //get name
    QString name = QFileInfo(fullPath).fileName();

    //create
    QTreeWidgetItem* node = new QTreeWidgetItem();
    node->setText(0, name);
    node->setData(0, Qt::UserRole, QVariant(sNodeSign));
    setNodeIcon(node, type);

    return node;
}
QTreeWidgetItem *CMDRecipeFileNode::cloneNode(QTreeWidgetItem *source)
{
    QTreeWidgetItem* node = new QTreeWidgetItem(QStringList(source->text(0)));
    node->setData(0, Qt::UserRole, QVariant(sNodeSign));

    setNodeIcon(node, getFileType(source));

    return node;
}
void CMDRecipeFileNode::insertSortedNode(QTreeWidgetItem *parent, QTreeWidgetItem *child)
{
    FileType type = getFileType(child);
    int index = 0;

    int count = parent->childCount();
    for (; index < count; index++)
    {
        QTreeWidgetItem* item = parent->child(index);

        if (getFileType(item) != type)
        {
            if (type == FileType::Dir)
                break;
            if (type == FileType::File)
                continue;
        }

        if (QString::compare(item->text(0), child->text(0), Qt::CaseInsensitive) > 0)
            break;
    }

    parent->insertChild(index, child);
}
void CMDRecipeFileNode::getNodes(const QString &path, const QString &pattern, QTreeWidgetItem *parent, QTreeWidget *tree)
{
    QDir directory = QDir(path);
    directory.setNameFilters(QStringList(pattern));
    //dir
    QFileInfoList dirs = directory.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot);
    foreach (QFileInfo dir, dirs)
    {
        QTreeWidgetItem* node = createNode(dir.absoluteFilePath(), FileType::Dir);
        if (parent != NULL)
            parent->addChild(node);
        else
            tree->addTopLevelItem(node);
        getNodes(dir.absoluteFilePath(), pattern, node, NULL);
    }

    //file
    QFileInfoList files = directory.entryInfoList(QDir::Files);
    foreach (QFileInfo file, files)
    {
        QTreeWidgetItem* node = createNode(file.absoluteFilePath(), FileType::File);
        if (parent != NULL)
            parent->addChild(node);
        else
            tree->addTopLevelItem(node);
    }
}
void CMDRecipeFileNode::copyChildNodes(QTreeWidgetItem *source, QTreeWidgetItem *target)
{
    for (int i = 0; i < source->childCount(); i++)
    {
        QTreeWidgetItem* cloneN = cloneNode(source->child(i));
        target->addChild(cloneN);
        copyChildNodes(source->child(i), cloneN);
    }
}
CMDRecipeUIImp *CMDRecipeFileNode::getscriptUI(QTreeWidgetItem *node)
{
    QTreeWidgetItem* root = getRoot(node);
    if (root != NULL)
        return reinterpret_cast<CMDRecipeUIImp*>(root->data(0, Qt::UserRole + 1).toULongLong());

    return NULL;
}
void CMDRecipeFileNode::setNodeIcon(QTreeWidgetItem *node, CMDRecipeFileNode::FileType type)
{
    if (type == FileType::Dir)
        node->setIcon(0, QIcon(":/images/folder.png"));
    else
        node->setIcon(0, QIcon(":/images/file.png"));

    node->setData(0, Qt::UserRole+1, QVariant((qulonglong)type));
}
//////////////////////////////////////////////////////////////////////////
QString CMDRecipeFileNode::getRelativeFilePath(QTreeWidgetItem *node)
{
    QString path;
    if (getFileType(node) == FileType::Dir)
        path.insert(0, "/");
    while(node != NULL)
    {
        path.insert(0, node->text(0));
        path.insert(0, "/");
        node = node->parent();
    }

    if (path.at(path.count() - 1) == '/')
        path.remove(path.count() - 1, 1);

    return path;
}
CMDRecipeFileNode::FileType CMDRecipeFileNode::getFileType(QTreeWidgetItem *node)
{
    if (isRoot(node))
        return FileType::Dir;

    return (FileType)node->data(0, Qt::UserRole+1).toULongLong();
}
QString CMDRecipeFileNode::getFilePath(QTreeWidgetItem *node)
{
    QString project = getscriptUI(node)->recipeFolder();
    QString path;
    if (getFileType(node) == FileType::Dir)
        path.insert(0, "/");
    while(!isRoot(node))
    {
        path.insert(0, node->text(0));
        path.insert(0, "/");
        node = node->parent();
    }
    path.insert(0, project);

    if (path.at(path.count() - 1) == '/')
        path.remove(path.count() - 1, 1);

    return path;
}
void CMDRecipeFileNode::copyFolder(const QString &source, const QString &target)
{
    QDir directory(target);
    if (!directory.exists())
        directory.mkdir(target);

    directory = QDir(source);
    QFileInfoList files = directory.entryInfoList(QDir::Files);
    foreach (QFileInfo file, files)
        QFile::copy(file.absoluteFilePath(), target+"/"+file.fileName());

    QFileInfoList dirs = directory.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot);
    foreach (QFileInfo dir, dirs)
        copyFolder(dir.absoluteFilePath(), target+"/"+dir.fileName());
}
void CMDRecipeFileNode::removeDirectory(const QString &fullName)
{
    QDir dir(fullName);

    QFileInfoList infos = dir.entryInfoList(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
    foreach (QFileInfo info, infos)
    {
        if (info.isDir()) //dir
            removeDirectory(info.absoluteFilePath());
        else  //file
            QFile::remove(info.absoluteFilePath());
    }

    dir.rmdir(fullName);
}
//////////////////////////////////////////////////////////////////////////
void CMDRecipeFileNode::setMenuState(CMDRecipeFileNode::NodeType type)
{
    m_actCopy->setVisible(true);
    m_actRename->setVisible(true);
    m_actDel->setVisible(true);
    m_actAddFile->setVisible(true);
    m_actAddDir->setVisible(true);
    m_actPaste->setVisible(true);

    m_actPaste->setEnabled(isValidCopy(m_copyNode));

    switch (type)
    {
        case NodeType::Root:
            m_actCopy->setVisible(false);
            m_actRename->setVisible(false);
            m_actDel->setVisible(false);
            break;
        case NodeType::Dir:
            break;
        case NodeType::File:
            m_actAddFile->setVisible(false);
            m_actAddDir->setVisible(false);
            m_actPaste->setVisible(false);
            break;
    }
}
bool CMDRecipeFileNode::isValidCopy(QTreeWidgetItem *copyNode)
{
    return copyNode != NULL && copyNode->treeWidget() != NULL;
}
QString CMDRecipeFileNode::createNewName(QTreeWidgetItem *parentNode, CMDRecipeFileNode::FileType type)
{
    int index = 1;
    bool hasRepeat = true;
    QString name;
    QString sIndex;
    while (hasRepeat)
    {
        name.clear();
        sIndex.setNum(index);
        if (type == FileType::File)
            name.append(sNewFileName+sIndex);
        else
            name.append(sNewDirName+sIndex);
        index++;

        hasRepeat = false;
        for (int i = 0; i < parentNode->childCount(); i++)
        {
            QTreeWidgetItem* node = parentNode->child(i);
            if (QString::compare(node->text(0), name, Qt::CaseInsensitive) == 0)
            {
                hasRepeat = true;
                break;
            }
        }
    }

    return name;
}
void CMDRecipeFileNode::createCopyName(QTreeWidgetItem *parentNode, QTreeWidgetItem *newNode)
{
    QString name = newNode->text(0);
    QString str = name;
    int removeCount = 0;
    int index = 1;
    //字符串符合如下格式"复件(2)"
    if (name.count() >= 5 && QString::compare(name.left(3), sCopyName+"(", Qt::CaseInsensitive) == 0)
    {
        int right = name.indexOf(")");
        if (right != -1)
        {
            QString num = name.mid(3, right - 3);
            bool success = false;
            num.toInt(&success);
            if (success)
            {
                removeCount = right + 1;
                index = 2;
            }
        }
    }

    bool hasRepeat = true;
    while (hasRepeat)
    {
        QString sIndex;
        sIndex.setNum(index);
        hasRepeat = false;
        for (int i = 0; i < parentNode->childCount(); i++)
        {
            QTreeWidgetItem* node = parentNode->child(i);
            if (QString::compare(node->text(0), str, Qt::CaseInsensitive) == 0)
            {
                hasRepeat = true;
                break;
            }
        }

        if (!hasRepeat)
            continue;
        str.clear();
        if (index == 1)
            str.append(sCopyName + name);
        else
        {
            str.append(name);
            if (removeCount > 0)
                str.remove(0, removeCount);
            str.insert(0, sCopyName + "(" + sIndex + ")");
        }
        index++;
    }

    newNode->setText(0, str);
}
void CMDRecipeFileNode::nodeAfterLabelEditEvent(CMDRecipeFileNode::NodeOperate operate, QTreeWidgetItem *node, const QString &oldFullName)
{
    QString fullName = getFilePath(node);
    switch (operate)
    {
    case NodeOperate::AddFile:
        createForm(node);
        break;
    case NodeOperate::AddDir:
    {
        QDir dir(fullName);
        if (!dir.exists())
            dir.mkdir(fullName);
    }
        break;
    case NodeOperate::Rename:
    {
        if (QString::compare(fullName, oldFullName, Qt::CaseInsensitive) == 0)	//no change
            return;

        renameForms(node, oldFullName);
        if (getFileType(node) == FileType::File)
        {
            QFile file(oldFullName);
            if (file.exists())
                file.rename(fullName);
        }
        else
        {
            QDir dir(oldFullName);
            if (dir.exists())
                dir.rename(oldFullName, fullName);
        }
    }
        break;
    default:
        break;
    }
}
bool CMDRecipeFileNode::isValidName(QTreeWidgetItem *node, QString &label, QString &message)
{
    if (label.isEmpty())
    {
        message = sNullHint;
        return false;
    }

    //check extension
    if (getFileType(node) == FileType::File)
    {
        if (label.count() <= 4)
            label += sExtension;
        else
        {
            QString ext = label.right(4);
            if (QString::compare(ext, sExtension) != 0)
                label += sExtension;
        }
    }

    //check repeat
    for (int i = 0; i < node->parent()->childCount(); i++)
    {
        QTreeWidgetItem* child = node->parent()->child(i);
        if (child != node && QString::compare(child->text(0), label) == 0)
        {
            message = sRepeatHint;
            return false;
        }
    }

    return true;
}
void CMDRecipeFileNode::addEvent(CMDRecipeFileNode::FileType type)
{
    QTreeWidgetItem* node = selectNode();
    QString fullName = getFilePath(node) + "/" + createNewName(node, type);
    QTreeWidgetItem* createN = createNode(fullName, type);
    insertSortedNode(node, createN);
    m_treeView->setCurrentItem(createN);
    m_treeView->expandItem(node);

    m_operateType = (type == FileType::File) ? NodeOperate::AddFile : NodeOperate::AddDir;

    setNodeEdit(createN);
}
void CMDRecipeFileNode::resetCopyNode(QTreeWidgetItem *deleteNode)
{
    QTreeWidgetItem* node = m_copyNode;
    while (node != NULL)
    {
        if (node == deleteNode)
        {
            m_copyNode = NULL;
            return;
        }

        node = node->parent();
    }
}
//////////////////////////////////////////////////////////////////////////
void CMDRecipeFileNode::menuStripNodeOpening(const QPoint &pos)
{
    QTreeWidgetItem* node = m_treeView->currentItem();
    if (node == NULL)
        return;
    bool root = isRoot(node);
    bool isHmi = isScriptNode(node);

    if (!(isRoot(node) || isScriptNode(node)))
        return;

    NodeType type = NodeType::Dir;
    if (isRoot(node))
        type = NodeType::Root;
    else
        type = getFileType(node) == FileType::Dir ? NodeType::Dir : NodeType::File;

    setMenuState(type);

    m_menu->exec(QCursor::pos());
}
void CMDRecipeFileNode::menuItemAddFileClick()
{
    addEvent(FileType::File);
}
void CMDRecipeFileNode::menuItemAddDirClick()
{
    addEvent(FileType::Dir);
}
void CMDRecipeFileNode::menuItemCopyClick()
{
    m_copyNode = selectNode();
}
void CMDRecipeFileNode::menuItemPasteClick()
{
    QTreeWidgetItem* parent = selectNode();

    //防止目标目录和源目录相同
    if (parent == m_copyNode)
        parent = parent->parent();
    //防止父目录拷贝到子目录中
    QTreeWidgetItem* node = parent;
    while (!isRoot(node))
    {
        if (node->parent() == m_copyNode)
        {
            QMessageBox box;
            box.setText(sSubToParentHint);
            box.exec();
            return;
        }
        node = node->parent();
    }

    //node
    QString sourceName = getFilePath(m_copyNode);
    FileType type = getFileType(m_copyNode);
    QTreeWidgetItem* newNode = createNode(sourceName, type);
    createCopyName(parent, newNode);
    copyChildNodes(m_copyNode, newNode);
    insertSortedNode(parent, newNode);
    m_treeView->expandItem(parent);

    //file
    QString destName = getFilePath(newNode);
    if (type == FileType::File)
    {
        if (QFile::exists(sourceName) && !QFile::exists(destName))
            QFile::copy(sourceName, destName);
    }
    else
    {
        QFileInfo info(sourceName);
        if (info.isDir())
            copyFolder(sourceName, destName);
    }
}
void CMDRecipeFileNode::menuItemDelClick()
{
    QTreeWidgetItem* node = selectNode();
    QString fullName = getFilePath(node);
    FileType type = getFileType(node);
    QTreeWidgetItem* nextNode = NULL;
    QTreeWidgetItem* parent = node->parent();
    int index = parent->indexOfChild(node);
    if ((index + 1) < parent->childCount())
        nextNode = parent->child(index + 1);
    else if (index > 0)
        nextNode = parent->child(index - 1);
    else
        nextNode = parent;

    if (type == FileType::File)
    {
        if (!QFile::exists(fullName))
            return;

        if (showMessage(sDelHint))
            QFile::remove(fullName);
        if (!QFile::exists(fullName))		//delete success
        {
            //要在Remove(node)之前
            closeForms(node);
            resetCopyNode(node);
            node->parent()->removeChild(node);
            m_treeView->setCurrentItem(nextNode);
            delete node;
        }
    }
    else
    {
        QDir dir(fullName);
        if (!dir.exists())
            return;

        if (showMessage(sDelHint))
            removeDirectory(fullName);
        if (!dir.exists())		//delete success
        {
            closeForms(node);
            resetCopyNode(node);
            node->parent()->removeChild(node);
            m_treeView->setCurrentItem(nextNode);
            delete node;
        }
    }
}
void CMDRecipeFileNode::menuItemRenameClick()
{
    QTreeWidgetItem* node = selectNode();
    m_operateType = NodeOperate::Rename;
    m_oldFullName = getFilePath(node);
    setNodeEdit(node);
}
bool CMDRecipeFileNode::showMessage(const QString &text)
{
    QMessageBox box;
    box.setText(text);
    box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    box.setDefaultButton(QMessageBox::Cancel);
    return box.exec() == QMessageBox::Ok;
}
void CMDRecipeFileNode::setNodeEdit(QTreeWidgetItem *node)
{
    QLineEdit* edit = new QLineEdit(m_treeView);
    connect(edit, &QLineEdit::editingFinished, this, &CMDRecipeFileNode::editingFinished);
    edit->installEventFilter(this);
    m_treeView->setItemWidget(node, 0, edit);

    edit->setText(node->text(0));
    edit->selectAll();
    edit->setFocus();
}
void CMDRecipeFileNode::editingFinished()
{
    QTreeWidgetItem *node = selectNode();
    if (node == NULL)
        return;
    QLineEdit *edit = qobject_cast<QLineEdit*>(m_treeView->itemWidget(node, 0));
    if(edit == NULL)
        return;

    treeViewAfterLabelEdit(node, edit);

    //移除edit
    m_treeView->removeItemWidget(node, 0);
    edit->deleteLater();
}
//////////////////////////////////////////////////////////////////////////
bool CMDRecipeFileNode::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_treeView)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            return treeViewKeyDown(keyEvent);
        }

        return false;
    }
    else if (obj->inherits("QLineEdit"))
    {
        //按下esc，恢复原始值，结束编辑
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Escape)
            {
                QLineEdit *edit = qobject_cast<QLineEdit*>(obj);
                QTreeWidgetItem *node = selectNode();
                if (node != NULL)
                    edit->setText(node->text(0));

                edit->editingFinished();
                return true;
            }
        }

        return false;
    }

    return QObject::eventFilter(obj, event);
}
void CMDRecipeFileNode::treeViewDoubleClick()
{
    QTreeWidgetItem* node = selectNode();
    if (node == NULL || !isScriptNode(node))
        return;

    openForm(node);
}
void CMDRecipeFileNode::treeViewAfterLabelEdit(QTreeWidgetItem *node, QLineEdit *edit)
{
    QString message;
    QString label = edit->text();
    if (!isValidName(node, label, message))
    {
        //移除edit
        m_treeView->removeItemWidget(node, 0);
        //disconnect(edit, &QLineEdit::editingFinished, this, &CMDRecipeFileNode::editingFinished);

        QMessageBox box;
        box.setText(message);
        box.setStandardButtons(QMessageBox::Ok);
        box.exec();
    }
    else
    {
        node->setText(0, label);
        //sort
        QTreeWidgetItem* parent = node->parent();
        parent->removeChild(node);
        insertSortedNode(parent, node);
        m_treeView->setCurrentItem(node);
    }

    nodeAfterLabelEditEvent(m_operateType, node, m_oldFullName);
}
void CMDRecipeFileNode::treeViewNodeMouseClick(QTreeWidgetItem *node)
{
    if (!isScriptNode(node))
        return;

    m_treeView->setCurrentItem(node);
}
bool CMDRecipeFileNode::treeViewKeyDown(QKeyEvent* e)
{
    //key事件中一定要判断节点是否存在和有效
    QTreeWidgetItem* node = selectNode();
    if (node == NULL || !isScriptNode(node))
        return false;

    switch (e->key())
    {
        case Qt::Key_Delete:
            if (!isRoot(node))
                menuItemDelClick();
            break;
        case Qt::Key_C:		//copy
            if ((e->modifiers() & Qt::ControlModifier) != 0)
            {
                if (!isRoot(node))
                    menuItemCopyClick();
            }
            break;
        case Qt::Key_V:		//paste
            if ((e->modifiers() & Qt::ControlModifier) != 0)
            {
                if (isValidCopy(m_copyNode))
                {
                    QTreeWidgetItem* old = node;
                    if (getFileType(node) == FileType::File)
                        node = node->parent();

                    m_treeView->setCurrentItem(node);
                    menuItemPasteClick();
                    m_treeView->setCurrentItem(old);
                }
            }
            break;
        default:
            break;
    }

    return false;
}
//////////////////////////////////////////////////////////////////////////
void CMDRecipeFileNode::createForm(QTreeWidgetItem *node)
{
    CMDRecipeUIImp* scriptUI = getscriptUI(node);
    if (scriptUI == NULL)
        return;
    QString fullName = getFilePath(node);

    scriptUI->createScriptFile(fullName);
}
void CMDRecipeFileNode::openForm(QTreeWidgetItem *node)
{
    if (getFileType(node) != FileType::File)
        return;

    CMDRecipeUIImp* scriptUI = getscriptUI(node);
    if (scriptUI == NULL)
        return;
    QString fullName = getFilePath(node);

    scriptUI->open(fullName);
}
void CMDRecipeFileNode::renameForm(CMDRecipeUIImp *scriptUI, QTreeWidgetItem *node, const QString &oldFullName)
{
//    if (getFileType(node) == FileType::File)
//        scriptUI->forms()->rename(getFilePath(node), oldFullName);
//    else
//    {
//        for (int i = 0; i < node->childCount(); i++)
//        {
//            QTreeWidgetItem* n = node->child(i);
//            renameForm(scriptUI, n, oldFullName + "/" + n->text(0));
//        }
//    }
}
void CMDRecipeFileNode::renameForms(QTreeWidgetItem *node, const QString &oldFullName)
{
    CMDRecipeUIImp* scriptUI = getscriptUI(node);
    if (scriptUI == NULL)
        return;

    renameForm(scriptUI, node, oldFullName);
}
void CMDRecipeFileNode::closeForm(CMDRecipeUIImp *scriptUI, QTreeWidgetItem *node)
{
//    if (getFileType(node) == FileType::File)
//        scriptUI->forms()->close(getFilePath(node));
//    else
//    {
//        for (int i = 0; i < node->childCount(); i++)
//        {
//            closeForm(scriptUI, node->child(i));
//        }
//    }
}
void CMDRecipeFileNode::closeForms(QTreeWidgetItem *node)
{
    CMDRecipeUIImp* scriptUI = getscriptUI(node);
    if (scriptUI == NULL)
        return;

    closeForm(scriptUI, node);
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////



