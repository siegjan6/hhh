#include "MDPropertyDrawList.h"
#include "MDDrawList.h"
#include "DrawListData.h"
#include "AddItemDialog.h"
#include "undo/DrawListCommand.h"

/*!
\section 属性函数
*/
void CMDPropertyDrawList::setHasScrollBar(bool value)
{
    if(!widget())
        return;
    if(isBatch())
        setBatchHasScrollBar(value);
    else
        undo()->push(new DrawListCommand(
                         widget(),widget()->hasScrollBar(),value,(int)DrawListId::HasScrollBar));

    setModify(true);
}
bool CMDPropertyDrawList::hasScrollBar()
{
    if(widget())
        return widget()->hasScrollBar();
    return false;
}

void CMDPropertyDrawList::setBorderStyle(PropertyBorderStyle value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchBorderStyle(value);
    else
    {
        QFrame::Shape factValue = QFrame::NoFrame;
        switch (value)
        {
        case PropertyBorderStyle::NoFrame:
            factValue = QFrame::NoFrame;
            break;
        case PropertyBorderStyle::StyledPanel:
            factValue = QFrame::StyledPanel;
            break;
        case PropertyBorderStyle::WinPanel:
            factValue = QFrame::WinPanel;
            break;
        default:
            break;
        }
        undo()->push(new DrawListCommand(
                         widget(),widget()->borderStyle(),factValue,(int)DrawListId::BorderStyle));
    }
    setModify(true);
}

CMDPropertyDrawList::PropertyBorderStyle CMDPropertyDrawList::borderStyle()
{
    if(widget())
    {
        switch (widget()->borderStyle())
        {
        case QFrame::NoFrame:
            return PropertyBorderStyle::NoFrame;
        case QFrame::StyledPanel:
            return PropertyBorderStyle::StyledPanel;
        case QFrame::WinPanel:
            return PropertyBorderStyle::WinPanel;
        default:
            break;
        }
    }
    return PropertyBorderStyle::NoFrame;
}

void CMDPropertyDrawList::setSelectionMode(SelectionMode value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchSelectionMode(value);
    else
    {
        QAbstractItemView::SelectionMode factValue = QAbstractItemView::NoSelection;
        switch (value)
        {
        case SelectionMode::NoSelection:
            factValue = QAbstractItemView::NoSelection;
            break;
        case SelectionMode::SingleSelection:
            factValue = QAbstractItemView::SingleSelection;
            break;
        case SelectionMode::MultiSelection:
            factValue = QAbstractItemView::MultiSelection;
            break;
        default:
            break;
        }
        undo()->push(new DrawListCommand(
                         widget(),widget()->selectionMode(),factValue,(int)DrawListId::SelectionMode));
    }

    setModify(true);
}

CMDPropertyDrawList::SelectionMode CMDPropertyDrawList::selectionMode()
{
    if(widget())
    {
        switch (widget()->selectionMode())
        {
        case QAbstractItemView::NoSelection:
            return SelectionMode::NoSelection;
        case QAbstractItemView::SingleSelection:
            return SelectionMode::SingleSelection;
        case QAbstractItemView::MultiSelection:
            return SelectionMode::MultiSelection;
        default:
            break;
        }
    }
    return SelectionMode::NoSelection;
}

void CMDPropertyDrawList::setLayoutDirection(LayoutDirection value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchLayoutDirection(value);
    else
    {
        Qt::LayoutDirection factValue = Qt::LeftToRight;
        switch (value)
        {
        case LayoutDirection::LeftToRight:
            factValue = Qt::LeftToRight;
            break;
        case LayoutDirection::RightToLeft:
            factValue = Qt::RightToLeft;
            break;
        default:
            break;
        }

        undo()->push(new DrawListCommand(
                         widget(),widget()->layoutDirection(),factValue,(int)DrawListId::LayoutDirection));
    }
    setModify(true);
}

CMDPropertyDrawList::LayoutDirection CMDPropertyDrawList::layoutDirection()
{
    if(widget())
    {
        switch (widget()->layoutDirection())
        {
        case Qt::LeftToRight:
            return LayoutDirection::LeftToRight;
        case Qt::RightToLeft:
            return LayoutDirection::RightToLeft;
        default:
            break;
        }
    }
    return LayoutDirection::LeftToRight;
}

void CMDPropertyDrawList::setDILItem(const QString &value)
{
    Q_UNUSED(value)
    CAddItemDialog dialog(dynamic_cast<QWidget*>(QObject::parent()));
    dialog.setItems(widget()->itemAdd());
    if(dialog.exec() == QDialog::Accepted)
    {
        QStringList items = dialog.items().split(breakString(), QString::SkipEmptyParts);
        for(int i=0;i<items.count(breakString());i++)
        {
            if(items.at(i) == breakString())
                items.removeAt(i);
        }
        if(isBatch())
            setBatchDILItem(items);
        else
            widget()->setItemAdd(items);
    }
    setModify(true);
}

QString CMDPropertyDrawList::DILitem()
{
    QString itemsIn;
    if(widget()->itemAdd().count())
        itemsIn.append(widget()->itemAdd().at(0));
    return itemsIn;
}

void CMDPropertyDrawList::setSorted(PropertyItemSortedStyle value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchSorted(value);
    else
    {
        int factValue = 0;
        switch (value) {
        case PropertyItemSortedStyle::NoOrder:
            factValue = 0;
            break;
        case PropertyItemSortedStyle::AscendingOrder:
            factValue = 1;
            break;
        case PropertyItemSortedStyle::DescendingOrder:
            factValue = 2;
            break;
        default:
            break;
        }
        undo()->push(new DrawListCommand(
                         widget(),widget()->sorted(),factValue,(int)DrawListId::Sorted));
    }
    setModify(true);
}

CMDPropertyDrawList::PropertyItemSortedStyle CMDPropertyDrawList::Sorted()
{
    if(widget())
    {
        switch (widget()->sorted()) {
        case 0:
            return PropertyItemSortedStyle::NoOrder;
        case 1:
            return PropertyItemSortedStyle::AscendingOrder;
        case 2:
            return PropertyItemSortedStyle::DescendingOrder;
        default:
            break;
        }
    }
    return PropertyItemSortedStyle::NoOrder;
}

CMDPropertyDrawList::CMDPropertyDrawList(CMDDrawObj *parent)
    :CMDPropertyDrawBasicWidget(parent)
{
    insertTranslateMap("selectionMode",tr("列表模式"));
    insertTranslateMap("hasScrollBar",tr("显示滚动条"));
    insertTranslateMap("layoutDirection",tr("滚动条位置"));
    insertTranslateMap("borderStyle",tr("边框样式"));
    insertTranslateMap("Sorted",tr("子项排序"));
    insertTranslateMap("item",tr("子项"));
    insertTranslateMap("NoOrder",tr("无序"));
    insertTranslateMap("AscendingOrder",tr("升序"));
    insertTranslateMap("DescendingOrder",tr("降序"));
    insertTranslateMap("LeftToRight",tr("居右"));
    insertTranslateMap("RightToLeft",tr("居左"));
    insertTranslateMap("NoSelection",tr("不可选"));
    insertTranslateMap("SingleSelection",tr("单项选择"));
    insertTranslateMap("MultiSelection",tr("多项选择"));
    insertTranslateMap("NoFrame",tr("无"));
    insertTranslateMap("StyledPanel",tr("平面"));
    insertTranslateMap("WinPanel",tr("立体"));
}

CMDDrawList *CMDPropertyDrawList::widget()
{
    return dynamic_cast<CMDDrawList*>(m_object);
}
/*!
\section 设置属性值
*/
void CMDPropertyDrawList::resetAttribute()
{
    CMDPropertyDrawObj::resetAttribute();
}

//////////////////////////////////////////////////////////////////////////
void CMDPropertyDrawList::setBatchHasScrollBar(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawList* v = dynamic_cast<CMDDrawList*>(o);
        undo()->push(new DrawListCommand(
                         v,v->hasScrollBar(),value,(int)DrawListId::HasScrollBar));
    }

    undo()->endMacro();
}

void CMDPropertyDrawList::setBatchBorderStyle(CMDPropertyDrawList::PropertyBorderStyle value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawList* v = dynamic_cast<CMDDrawList*>(o);

        QFrame::Shape factValue = QFrame::NoFrame;
        switch (value)
        {
        case PropertyBorderStyle::NoFrame:
            factValue = QFrame::NoFrame;
            break;
        case PropertyBorderStyle::StyledPanel:
            factValue = QFrame::StyledPanel;
            break;
        case PropertyBorderStyle::WinPanel:
            factValue = QFrame::WinPanel;
            break;
        default:
            break;
        }
        undo()->push(new DrawListCommand(
                         v,v->borderStyle(),factValue,(int)DrawListId::BorderStyle));
    }

    undo()->endMacro();
}

void CMDPropertyDrawList::setBatchSelectionMode(SelectionMode value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawList* v = dynamic_cast<CMDDrawList*>(o);

        QAbstractItemView::SelectionMode factValue = QAbstractItemView::NoSelection;
        switch (value)
        {
        case SelectionMode::NoSelection:
            factValue = QAbstractItemView::NoSelection;
            break;
        case SelectionMode::SingleSelection:
            factValue = QAbstractItemView::SingleSelection;
            break;
        case SelectionMode::MultiSelection:
            factValue = QAbstractItemView::MultiSelection;
            break;
        default:
            break;
        }

        undo()->push(new DrawListCommand(
                         v,v->selectionMode(),factValue,(int)DrawListId::SelectionMode));
    }

    undo()->endMacro();
}

void CMDPropertyDrawList::setBatchLayoutDirection(LayoutDirection value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawList* v = dynamic_cast<CMDDrawList*>(o);

        Qt::LayoutDirection factValue = Qt::LeftToRight;
        switch (value)
        {
        case LayoutDirection::LeftToRight:
            factValue = Qt::LeftToRight;
            break;
        case LayoutDirection::RightToLeft:
            factValue = Qt::RightToLeft;
            break;
        default:
            break;
        }

        undo()->push(new DrawListCommand(
                         v,v->layoutDirection(),factValue,(int)DrawListId::LayoutDirection));
    }

    undo()->endMacro();
}

void CMDPropertyDrawList::setBatchDILItem(QStringList list)
{
    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawList* v = dynamic_cast<CMDDrawList*>(o);
        v->setItemAdd(list);
    }
}

void CMDPropertyDrawList::setBatchSorted(CMDPropertyDrawList::PropertyItemSortedStyle value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawList* v = dynamic_cast<CMDDrawList*>(o);
        int factValue = 0;
        switch (value) {
        case PropertyItemSortedStyle::NoOrder:
            factValue = 0;
            break;
        case PropertyItemSortedStyle::AscendingOrder:
            factValue = 1;
            break;
        case PropertyItemSortedStyle::DescendingOrder:
            factValue = 2;
            break;
        default:
            break;
        }
        undo()->push(new DrawListCommand(
                         v,v->sorted(),factValue,(int)DrawListId::Sorted));
    }

    undo()->endMacro();
}
//////////////////////////////////////////////////////////////////////////
