/****************************************************************************
*
*  Copyright (C) 2002-2008 Helta Kft. / NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: nszabo@helta.hu, info@nocisoft.com
*  Web: www.nocisoft.com
*
*  This file is part of the NCReport reporting software
*
*  Licensees holding a valid NCReport License Agreement may use this
*  file in accordance with the rights, responsibilities, and obligations
*  contained therein. Please consult your licensing agreement or contact
*  nszabo@helta.hu if any conditions of this licensing are not clear
*  to you.
*
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/
#include "ncreportgroup.h"
#include "ncreportsection.h"
#include "ncreportdatasource.h"

NCReportGroup::NCReportGroup() :
    m_header(0),
    m_footer(0),
    m_detail(0),
    m_state(Closed),
    m_headerState(On),
    m_enabled(false),
    m_groupchanged(false),
    m_startsOnNewPage(false),
    m_repeatHeader(false),
    m_initialPrintDone(false),
    m_printLock(false),
    m_nestedLevel(0),
    m_counter(0)
{
    reset();
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("NCReportGroup init.");
#endif
}

NCReportGroup::~NCReportGroup()
{
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("NCReportGroup delete.");
#endif
}

void NCReportGroup::reset()
{
    m_currentValue = "**CC**";
    m_groupchanged = true;
    m_currentMetaInfo.firstValue.clear();
    m_currentMetaInfo.lastValue.clear();
    m_counter = 0;
}

void NCReportGroup::setValue(const QString & value)
{
    m_groupchanged = ( value != m_currentValue );
    if (m_groupchanged) {

        m_lastMetaInfo = m_currentMetaInfo; // save state on group change

        m_currentMetaInfo.firstValue = value;
        m_currentMetaInfo.firstRow = detail()->dataSource()->currentRow();
        m_counter = 1;
    } else {
        m_currentMetaInfo.lastValue = value;
        m_currentMetaInfo.lastRow = detail()->dataSource()->currentRow();
        m_counter++;
    }
    m_currentValue = value;

#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug( "GROUP: %s setValue(%s) changed=%i",qPrintable(m_id), qPrintable(m_currentValue), m_groupchanged );
#endif
}

bool NCReportGroup::isGroupChanged()
{
    //return false;
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug( "GROUP: %s isGroupChanged() = %i (currentValue=%s)",qPrintable(m_id), m_groupchanged, qPrintable(m_currentValue) );
#endif
    return m_groupchanged;
}

NCReportSection * NCReportGroup::header()
{
    return m_header;
}

NCReportSection * NCReportGroup::footer()
{
    return m_footer;
}

const QString & NCReportGroup::expression() const
{
    return m_groupExp;
}

const QString &NCReportGroup::pageBreakExp() const
{
    return m_pageBreakExp;
}

void NCReportGroup::setId(const QString & id )
{
    m_id = id;
}

NCReportSection * NCReportGroup::detail()
{
    return m_detail;
}

bool NCReportGroup::repeatHeaderOnNextPage() const
{
    return m_repeatHeader;
}

const QString& NCReportGroup::resetVarList() const
{
    return m_resetVariables;
}

const QString & NCReportGroup::groupId() const
{
    return m_id;
}

void NCReportGroup::setGroupExp(const QString & exp)
{
    m_groupExp = exp;
}

void NCReportGroup::setResetVarList(const QString & varList)
{
    m_resetVariables = varList;
}

void NCReportGroup::setReprintHeaderOnNextPage(bool set)
{
    m_repeatHeader = set;
}

void NCReportGroup::setStartsOnNewPage(bool set )
{
    m_startsOnNewPage = set;
}

void NCReportGroup::setPageBreakExp(const QString &expression)
{
    m_pageBreakExp = expression;
}

bool NCReportGroup::startsOnNewPage() const
{
    return m_startsOnNewPage;
}

void NCReportGroup::setInitialPrintDone(bool set)
{
    m_initialPrintDone = set;
}

bool NCReportGroup::initialPrintDone() const
{
    return m_initialPrintDone;
}

void NCReportGroup::setGroupChanged(bool set)
{
    m_groupchanged = set;
}

void NCReportGroup::setPrintLock(bool set)
{
    m_printLock = set;
}

bool NCReportGroup::printLock()
{
    return m_printLock;
}

const QString & NCReportGroup::value() const
{
    return m_currentValue;
}

void NCReportGroup::setNestedLevel(short level)
{
    m_nestedLevel = level;
}

short NCReportGroup::nestedLevel() const
{
    return m_nestedLevel;
}

NCReportGroup::HeaderState NCReportGroup::headerState() const
{
    return m_headerState;
}

void NCReportGroup::setHeaderState( NCReportGroup::HeaderState state )
{
    m_headerState = state;
}

const QString &NCReportGroup::currentValue() const
{
    return m_currentValue;
}

const NCReportGroup::GroupMetaInfo &NCReportGroup::currentMetaInfo() const
{
    return m_currentMetaInfo;
}

const NCReportGroup::GroupMetaInfo &NCReportGroup::lastMetaInfo() const
{
    return m_lastMetaInfo;
}

int NCReportGroup::counter() const
{
    return m_counter;
}
