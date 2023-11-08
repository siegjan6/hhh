#include <QHeaderView>

#include "FileNode.h"
#include "HMIFramework.h"
#include "PreviewNode.h"
#include "PreviewDialog.h"
//////////////////////////////////////////////////////////////////////////
CPreviewNode::CPreviewNode(QTreeWidget *tree, QObject *parent)
    :QObject(parent)
{
    m_root = NULL;
    m_tree = tree;
    m_tree->header()->setVisible(false);
    connect(m_tree, &QTreeWidget::doubleClicked, this, &CPreviewNode::treeWidgetDoubleClick);
}
CPreviewNode::~CPreviewNode()
{
}
//////////////////////////////////////////////////////////////////////////
void CPreviewNode::init(QTreeWidgetItem *fileNode)
{
    while(m_tree->topLevelItemCount() > 0)
        delete m_tree->takeTopLevelItem(0);

    m_root = fileNode->clone();

    m_tree->insertTopLevelItem(0, m_root);
    m_tree->expandAll();
}
//////////////////////////////////////////////////////////////////////////
void CPreviewNode::treeWidgetDoubleClick()
{
    QTreeWidgetItem* node = m_tree->currentItem();
    if(node == NULL)
        return;

   loadForm(node);
}
void CPreviewNode::loadForm(QTreeWidgetItem *node)
{
    if (CFileNode::getFileType(node) != CFileNode::FileType::File)
        return;

    QString fullName = CFileNode::getFilePath(node);
    qobject_cast<CPreviewDialog*>(parent())->loadForm(fullName);
}
//////////////////////////////////////////////////////////////////////////
