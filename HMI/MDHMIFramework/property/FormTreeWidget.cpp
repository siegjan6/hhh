#include <QDir>
#include "FormTreeWidget.h"
#include "ui_FormTreeWidget.h"
#include "HMIFramework.h"
#include "FormOperation.h"
#include "MDHMIForm.h"
//////////////////////////////////////////////////////////////////////////
CFormTreeWidget::CFormTreeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFormTreeWidget)
{
    ui->setupUi(this);
    m_framework = NULL;
}
CFormTreeWidget::~CFormTreeWidget()
{
    delete ui;
}
//////////////////////////////////////////////////////////////////////////
void CFormTreeWidget::init(CHMIFramework *framework)
{
    m_framework = framework;
    CFormOperation* f = dynamic_cast<CFormOperation*>(m_framework->forms());
    initNodes(ui->treeWidget->topLevelItem(0), m_framework);
    connect(ui->treeWidget, &QTreeWidget::itemDoubleClicked, this, &CFormTreeWidget::itemDoubleClicked);
    connect(f, &CFormOperation::formOpenedEvent, this, &CFormTreeWidget::formOpened);
    connect(f, &CFormOperation::formClosedEvent, this, &CFormTreeWidget::formClosed);
}
//////////////////////////////////////////////////////////////////////////
void CFormTreeWidget::initNodes(QTreeWidgetItem *root, CHMIFramework *framework)
{
    initRoot(root, framework);

    //file
    QString project = framework->hmiPath();
    QDir dir(project);
    if (!dir.exists())
        return;
    getNodes(project, "*.gdi", root);
    initOpenedState();

    ui->treeWidget->expandAll();
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
}
void CFormTreeWidget::initRoot(QTreeWidgetItem *root, CHMIFramework *framework)
{
    root->setText(0, "图形");
    root->setIcon(0, QIcon(":/rs/image/root.png"));
}
void CFormTreeWidget::initOpenedState()
{
    CFormOperation* fo = dynamic_cast<CFormOperation*>(m_framework->forms());
    foreach (CMDHMIForm* f, *fo->openedList())
        formOpened(f->common()->fullName());
}
void CFormTreeWidget::getNodes(const QString &path, const QString &pattern, QTreeWidgetItem *parent)
{
    QDir directory = QDir(path);
    directory.setNameFilters(QStringList(pattern));
    //dir
    QFileInfoList dirs = directory.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot);
    for(int i = 0; i < dirs.count(); i++)
    {
        QFileInfo dir = dirs.at(i);

        QTreeWidgetItem* node = createNode(dir.absoluteFilePath(), FileType::Dir);
        parent->addChild(node);
        getNodes(dir.absoluteFilePath(), pattern, node);
    }

    //file
    QFileInfoList files = directory.entryInfoList(QDir::Files);
    for(int i = 0; i < files.count(); i++)
    {
        QFileInfo file = files.at(i);

        QTreeWidgetItem* node = createNode(file.absoluteFilePath(), FileType::File);
        parent->addChild(node);
    }
}
QTreeWidgetItem *CFormTreeWidget::createNode(const QString &fullPath, FileType type)
{
    //get name
    QString name = QFileInfo(fullPath).fileName();

    //create
    QTreeWidgetItem* node = new QTreeWidgetItem();
    node->setText(0, name);
    setNodeIcon(node, type);

    return node;
}
void CFormTreeWidget::setNodeIcon(QTreeWidgetItem *node, FileType type)
{
    if (type == FileType::Dir)
        node->setIcon(0, QIcon(":/rs/image/dir.png"));
    else
        node->setIcon(0, QIcon(":/rs/image/file.png"));

    node->setData(0, Qt::UserRole, (qulonglong)type);
}
CFormTreeWidget::FileType CFormTreeWidget::getFileType(QTreeWidgetItem *node)
{
    return (FileType)node->data(0, Qt::UserRole).toULongLong();
}
QString CFormTreeWidget::getFilePath(QTreeWidgetItem *node)
{
    QString project = m_framework->hmiPath();
    QString path;
    while(node->parent())
    {
        path.insert(0, node->text(0));
        path.insert(0, "/");
        node = node->parent();
    }
    path.insert(0, project);

    return path;
}
QTreeWidgetItem *CFormTreeWidget::findNode(const QString &fullName)
{
    QTreeWidgetItem* node = ui->treeWidget->topLevelItem(0);
    QString project = m_framework->hmiPath();
    QString name = fullName;
    name.replace(project, "", Qt::CaseInsensitive);
    QStringList paths = name.split("/", QString::SkipEmptyParts);

    foreach(QString p, paths)
    {
        for(int i = 0; i < node->childCount(); i++)
        {
            QTreeWidgetItem* n = node->child(i);
            if(p.compare(n->text(0), Qt::CaseInsensitive) == 0)
            {
                node = n;
                break;
            }

        }
    }

    if(getFileType(node) == FileType::File)
        return node;
    return NULL;
}
void CFormTreeWidget::setOpenedState(QTreeWidgetItem* node, bool isOpened)
{
    if(node == NULL)
        return;

    if(isOpened)
        node->setIcon(1, QIcon(":/rs/image/openFolder.png"));
    else
        node->setIcon(1, QIcon());
}
//////////////////////////////////////////////////////////////////////////
void CFormTreeWidget::openForm(QTreeWidgetItem *node)
{
    m_framework->forms()->open(getFilePath(node));
}
void CFormTreeWidget::closeForm(QTreeWidgetItem *node)
{
    m_framework->forms()->close(getFilePath(node), true);
}
//////////////////////////////////////////////////////////////////////////
void CFormTreeWidget::itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(getFileType(item) != FileType::File)
        return;

    if(column == 0)
        openForm(item);
    else if(column == 1)
        closeForm(item);
}
void CFormTreeWidget::formOpened(const QString &fullName)
{
    setOpenedState(findNode(fullName), true);
}
void CFormTreeWidget::formClosed(const QString &fullName)
{
    setOpenedState(findNode(fullName), false);
}
//////////////////////////////////////////////////////////////////////////
