/*!
\author aGRw 2016.03.14
\section 图形窗体文件节点，是CFileNode的简化版
*/
#ifndef PRIEVEWNODE_H
#define PRIEVEWNODE_H

#include <QTreeWidgetItem>
#include <QTreeWidget>

class CPreviewNode : public QObject
{
    Q_OBJECT
public:
    CPreviewNode(QTreeWidget* tree, QObject * parent = 0);
    virtual ~CPreviewNode();
/*!
\section field
*/
private:
    QTreeWidgetItem* m_root;//!根节点
    QTreeWidget* m_tree;
/*!
\section public function
*/
public:
    void init(QTreeWidgetItem* fileNode);
/*!
\section private function
*/
private:
    void treeWidgetDoubleClick();
    //!打开窗体
    void loadForm(QTreeWidgetItem* node);
};

#endif // PRIEVEWNODE_H
