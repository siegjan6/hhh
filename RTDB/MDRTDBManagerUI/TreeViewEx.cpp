#include "TreeViewEx.h"
CTreeViewEx::CTreeViewEx(QWidget *parent) :
    QTreeView(parent)
{
}

CTreeViewEx::~CTreeViewEx()
{
}
void CTreeViewEx::mousePressEvent(QMouseEvent* event)
{
    QTreeView::mousePressEvent(event);
	emit mousePressed();
}

