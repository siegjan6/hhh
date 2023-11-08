#include "DrawTableViewData.h"
/////////////////////////////////////////////////////////////////////
CDrawTableViewData::CDrawTableViewData()
{
    m_widget = new QTableView;

    m_colCount = 3;
    m_rowCount = 5;
    m_colWidth = 80;
    m_rowHeight = 50;
    m_hideRowHeader = false;
    m_hideColHeader = false;
    m_readOnly = false;
}
CDrawTableViewData::~CDrawTableViewData()
{
    qDeleteAll(m_delegateList.begin(), m_delegateList.end());
    m_delegateList.clear();
}
/////////////////////////////////////////////////////////////////////
void CDrawTableViewData::operator=(const CDrawTableViewData &other)
{
    m_colCount = other.m_colCount;
    m_rowCount = other.m_rowCount;
    m_colWidth = other.m_colWidth;
    m_rowHeight = other.m_rowHeight;
    m_hideRowHeader = other.m_hideRowHeader;
    m_hideColHeader = other.m_hideColHeader;
    m_readOnly = other.m_readOnly;
}
/////////////////////////////////////////////////////////////////////
