#include <QDir>
#include <QHeaderView>
#include <QLineEdit>
#include <QMessageBox>
#include <QEvent>
#include <QKeyEvent>
#include "StorageFileNode.h"

CStorageFileNode::CStorageFileNode(QObject *parent/* = 0*/) : QObject(parent)
{
    m_tree = NULL;
    m_canEvent = false;
    m_actAddDir = NULL;
    m_actDel = NULL;
    m_actRename = NULL;
    m_menu = NULL;
    m_transTool = NULL;
    m_isRename = false;

    sRoot = tr("图库");
    sAddDir = tr("添加");
    sDel = tr("删除");
    sRename = tr("重命名");
    sNewDirName = tr("新建类别");
    sNullHint = tr("名称不能为空");
    sRepeatHint = tr("名称已存在");
    sDelHint = tr("是否确认删除？");
}
CStorageFileNode::~CStorageFileNode()
{
    if(m_transTool != NULL)
        delete m_transTool;
}
//////////////////////////////////////////////////////////////////////////
void CStorageFileNode::init(QTreeWidget *tree, const QString &path)
{
    m_canEvent = false;

    if (m_tree == NULL)
    {
        m_tree = tree;
        m_basePath = path;

        connect(m_tree, &QTreeWidget::currentItemChanged, this, &CStorageFileNode::currentItemChanged);
        m_tree->installEventFilter(this);
        initMenu();

        m_transTool = CMDMultiLanguage::createWidgetTool();
        m_transTool->init(this);
    }
    else
    {
        for (int i = 0; i < m_tree->topLevelItemCount(); i++)
            delete m_tree->topLevelItem(i);
        m_tree->clear();
    }

    //root
    QTreeWidgetItem* root = new QTreeWidgetItem(m_tree);
    root->setText(0, sRoot);
    root->setIcon(0, QIcon(":/rs/image/root.png"));

    //node
    getNodes(m_basePath, root);

    //tree
    m_tree->header()->hide();
    m_tree->expandAll();

    m_canEvent = true;
}
////////////////////////////////////////////////////////////////////////////
void CStorageFileNode::getNodes(const QString &path, QTreeWidgetItem *parent)
{
    QDir directory = QDir(path);
    //dir
    QFileInfoList dirs = directory.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot);
    for(int i = 0; i < dirs.count(); i++)
    {
        QFileInfo dir = dirs.at(i);
        QTreeWidgetItem* node = createNode(dir.absoluteFilePath());
        parent->addChild(node);
        getNodes(dir.absoluteFilePath(), node);
    }
}
QTreeWidgetItem *CStorageFileNode::createNode(const QString &fullPath)
{
    //get name
    QString name = QFileInfo(fullPath).fileName();

    //create
    QTreeWidgetItem* node = new QTreeWidgetItem();
    node->setText(0, name);
    node->setIcon(0, QIcon(":/rs/image/dir.png"));

    return node;
}
QString CStorageFileNode::getFilePath(QTreeWidgetItem *node)
{
    QString path;
    while (node->parent() != NULL)
    {
        path.insert(0, "/" + node->text(0));
        node = node->parent();
    }
    path.insert(0, m_basePath);

    return path;
}
void CStorageFileNode::currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *)
{
    if (!m_canEvent)
        return;
    if (current == NULL)
        return;

    QString path = getFilePath(current);
    emit currentPathChanged(path);
}
//////////////////////////////////////////////////////////////////////////
void CStorageFileNode::initMenu()
{
    m_actAddDir = new QAction(QIcon(":/rs/image/AddDir"), sAddDir, this);
    m_actDel = new QAction(QIcon(":/rs/image/delete"), sDel, this);
    m_actRename = new QAction(sRename, this);

    m_actDel->setShortcut(QKeySequence::Delete);

    connect(m_actAddDir, &QAction::triggered, this, &CStorageFileNode::menuItemAddDirClick);
    connect(m_actDel, &QAction::triggered, this, &CStorageFileNode::menuItemDelClick);
    connect(m_actRename, &QAction::triggered, this, &CStorageFileNode::menuItemRenameClick);

    m_menu = new QMenu(m_tree);
    m_menu->addAction(m_actAddDir);
    m_menu->addAction(m_actDel);
    m_menu->addAction(m_actRename);

    m_tree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_tree, &QTreeWidget::customContextMenuRequested,
            this, &CStorageFileNode::menuStripNodeOpening);
}
void CStorageFileNode::menuItemAddDirClick()
{
    QTreeWidgetItem* node = m_tree->currentItem();
    if (node == NULL)
        return;

    m_isRename = false;
    QString fullName = getFilePath(node) + "/" + createNewName(node);
    QTreeWidgetItem* createN = createNode(fullName);
    insertSortedNode(node, createN);
    m_tree->setCurrentItem(createN);
    m_tree->expandItem(node);

    setNodeEdit(createN);
}
QString CStorageFileNode::createNewName(QTreeWidgetItem *parentNode)
{
    int index = 1;
    bool hasRepeat = true;
    QString name;
    QString sIndex;
    while (hasRepeat)
    {
        name.clear();
        sIndex.setNum(index);
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
void CStorageFileNode::insertSortedNode(QTreeWidgetItem *parent, QTreeWidgetItem *child)
{
    int index = 0;

    int count = parent->childCount();
    for (; index < count; index++)
    {
        QTreeWidgetItem* item = parent->child(index);
        if (QString::compare(item->text(0), child->text(0), Qt::CaseInsensitive) > 0)
            break;
    }

    parent->insertChild(index, child);
}
void CStorageFileNode::setNodeEdit(QTreeWidgetItem *node)
{
    QLineEdit* edit = new QLineEdit(m_tree);
    connect(edit, &QLineEdit::editingFinished, this, &CStorageFileNode::editingFinished);
    edit->installEventFilter(this);
    m_tree->setItemWidget(node, 0, edit);

    edit->setText(node->text(0));
    edit->selectAll();
    edit->setFocus();
}
void CStorageFileNode::editingFinished()
{
    QTreeWidgetItem *node = m_tree->currentItem();
    if (node == NULL)
        return;
    QLineEdit *edit = qobject_cast<QLineEdit*>(m_tree->itemWidget(node, 0));
    if(edit == NULL)
        return;

    treeViewAfterLabelEdit(node, edit);

    //移除edit
    m_tree->removeItemWidget(node, 0);
    edit->deleteLater();
}
void CStorageFileNode::treeViewAfterLabelEdit(QTreeWidgetItem *node, QLineEdit *edit)
{
    QString message;
    QString label = edit->text();
    if (!isValidName(node, label, message))
    {
        //移除edit
        m_tree->removeItemWidget(node, 0);

        QMessageBox box(m_tree);
        box.setText(message);
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
    }
    else
    {
        node->setText(0, label);
        //sort
        QTreeWidgetItem* parent = node->parent();
        parent->removeChild(node);
        insertSortedNode(parent, node);
        m_tree->setCurrentItem(node);
    }

    nodeAfterLabelEditEvent(node, m_oldFullName, m_isRename);
}
bool CStorageFileNode::isValidName(QTreeWidgetItem *node, QString &label, QString &message)
{
    if (label.isEmpty())
    {
        message = sNullHint;
        return false;
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
void CStorageFileNode::nodeAfterLabelEditEvent(QTreeWidgetItem *node, const QString& oldFullName, bool isRename/* = false*/)
{
    QString fullName = getFilePath(node);

    if(isRename)//rename
    {
        if (QString::compare(fullName, oldFullName, Qt::CaseInsensitive) == 0)	//no change
            return;

        QDir dir(oldFullName);
        if (dir.exists())
            dir.rename(oldFullName, fullName);

        currentItemChanged(node, NULL);
    }
    else//add dir
    {
        QDir dir(fullName);
        if (!dir.exists())
            dir.mkdir(fullName);
    }
}
void CStorageFileNode::menuItemDelClick()
{
    QTreeWidgetItem* node = m_tree->currentItem();
    if (node == NULL)
        return;

    QString fullName = getFilePath(node);
    QTreeWidgetItem* nextNode = NULL;
    QTreeWidgetItem* parent = node->parent();
    int index = parent->indexOfChild(node);
    if ((index + 1) < parent->childCount())
        nextNode = parent->child(index + 1);
    else if (index > 0)
        nextNode = parent->child(index - 1);
    else
        nextNode = parent;


    QDir dir(fullName);
    if (!dir.exists())
        return;

    if (showMessage(sDelHint))
        removeDirectory(fullName);
    if (!dir.exists())		//delete success
    {
        node->parent()->removeChild(node);
        m_tree->setCurrentItem(nextNode);
        delete node;
    }

}
void CStorageFileNode::menuItemRenameClick()
{
    QTreeWidgetItem* node = m_tree->currentItem();
    m_isRename = true;
    m_oldFullName = getFilePath(node);
    setNodeEdit(node);
}
bool CStorageFileNode::showMessage(const QString &text)
{
    QMessageBox box(m_tree);
    box.setText(text);
    box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    box.setDefaultButton(QMessageBox::Cancel);
    CMDMultiLanguage::instance()->translateWidget(&box);
    return box.exec() == QMessageBox::Ok;
}
void CStorageFileNode::removeDirectory(const QString &fullName)
{
    QDir dir(fullName);

    QFileInfoList infos = dir.entryInfoList(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
    for(int i = 0; i < infos.count(); i++)
    {
        QFileInfo info = infos.at(i);
        if (info.isDir()) //dir
            removeDirectory(info.absoluteFilePath());
        else  //file
            QFile::remove(info.absoluteFilePath());
    }

    dir.rmdir(fullName);
}
void CStorageFileNode::setMenuState(bool isRoot)
{
    m_actDel->setVisible(!isRoot);
    m_actRename->setVisible(!isRoot);
    m_actAddDir->setVisible(true);
}
void CStorageFileNode::menuStripNodeOpening(const QPoint &pos)
{
    QTreeWidgetItem* node = m_tree->itemAt(pos);
    if (node == NULL)
        return;

    m_tree->setCurrentItem(node);
    setMenuState((node->parent() == NULL));

    m_menu->exec(QCursor::pos());
}
bool CStorageFileNode::eventFilter(QObject *obj, QEvent *event)
{
    if (obj->inherits("QLineEdit"))
    {
        //按下esc，恢复原始值，结束编辑
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Escape)
            {
                QLineEdit *edit = qobject_cast<QLineEdit*>(obj);
                QTreeWidgetItem *node = m_tree->currentItem();
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
//////////////////////////////////////////////////////////////////////////
void CStorageFileNode::changeLanguage()
{
    m_transTool->changeLanguage();
    if(m_tree->topLevelItem(0) != NULL)
        m_tree->topLevelItem(0)->setText(0, CMDMultiLanguage::instance()->value(sRoot));
}
//////////////////////////////////////////////////////////////////////////



