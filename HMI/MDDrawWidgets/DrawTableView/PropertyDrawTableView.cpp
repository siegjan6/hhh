#include "PropertyDrawTableView.h"
#include "MDDrawTableView.h"
/////////////////////////////////////////////////////////////////
CPropertyDrawTableView::CPropertyDrawTableView(CMDDrawObj *parent):
    CMDPropertyDrawBasicWidget(parent)
{
    insertTranslateMap(tr("readOnly"),tr("只读"));
    insertTranslateMap(tr("rowCount"),tr("行数"));
    insertTranslateMap(tr("colCount"),tr("列数"));
    insertTranslateMap(tr("rowHeight"),tr("行高"));
    insertTranslateMap(tr("colWidth"),tr("列宽"));
    insertTranslateMap(tr("hideRowHeader"),tr("隐藏行头"));
    insertTranslateMap(tr("hideColHeader"),tr("隐藏列头"));
}
CMDDrawTableView *CPropertyDrawTableView::object()
{
    return dynamic_cast<CMDDrawTableView*>(m_object);
}
/////////////////////////////////////////////////////////////////
void CPropertyDrawTableView::setReadOnly(bool value)
{
    object()->setReadOnly(value);
    setModify(true);
}
bool CPropertyDrawTableView::readOnly()
{
    return object()->readOnly();
}
void CPropertyDrawTableView::setRowCount(int value)
{
    object()->setRowCount(value);
    setModify(true);
}
int CPropertyDrawTableView::rowCount()
{
    return object()->rowCount();
}
void CPropertyDrawTableView::setColCount(int value)
{
    object()->setColCount(value);
    setModify(true);
}
int CPropertyDrawTableView::colCount()
{
    return object()->colCount();
}
void CPropertyDrawTableView::setRowHeight(int value)
{
    object()->setRowHeight(value);
    setModify(true);
}
int CPropertyDrawTableView::rowHeight()
{
    return object()->rowHeight();
}
void CPropertyDrawTableView::setColWidth(int value)
{
    object()->setColWidth(value);
    setModify(true);
}
int CPropertyDrawTableView::colWidth()
{
    return object()->colWidth();
}
void CPropertyDrawTableView::setHideRowHeader(bool value)
{
    object()->setHideRowHeader(value);
    setModify(true);
}
bool CPropertyDrawTableView::hideRowHeader()
{
    return object()->hideRowHeader();
}
void CPropertyDrawTableView::setHideColHeader(bool value)
{
    object()->setHideColHeader(value);
    setModify(true);
}
bool CPropertyDrawTableView::hideColHeader()
{
    return object()->hideColHeader();
}
/////////////////////////////////////////////////////////////////
