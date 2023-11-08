/****************************************************************************
*
*  Copyright (C) 2002-2014 Helta Ltd. - NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: office@nocisoft.com
*  Web: www.nocisoft.com
*
*  This file is part of the NCReport reporting software
*  Created: 2014.07.25. (nocisoft)
*
*  Licensees holding a valid NCReport License Agreement may use this
*  file in accordance with the rights, responsibilities, and obligations
*  contained therein. Please consult your licensing agreement or contact
*  office@nocisoft.com if any conditions of this licensing are not clear
*  to you.
*
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#include "ncreportdatasourcerelation.h"
#include "ncreportdatasource.h"
#include "ncreportdirector.h"

#include <QSet>

//NCReportDataSourceRelation::NCReportDataSourceRelation(NCReportDataSourceRelation *parent)
//    : m_parentItem(parent), m_dataSource(0), m_currentChildIndex(0)
//{
//}

NCReportDataSourceRelation::NCReportDataSourceRelation(NCReportDirector *director, NCReportDataSource *dataSource, NCReportDataSourceRelation *parent)
    : m_parentItem(parent), m_dataSource(dataSource), m_director(director), m_currentChildIndex(0), m_lastFoundItem(0)
{
}

NCReportDataSourceRelation::~NCReportDataSourceRelation()
{
    qDeleteAll(m_childItems);
}

/*!
 * \brief NCReportDataSourceRelation::insertDataSources
 * \param dataSources
 * Inserts all members of the \param dataSources and builds the relation hierarchy.
 */
bool NCReportDataSourceRelation::insertDataSources(const QHash<QString, NCReportDataSource *> &dataSources)
{
    // Check invalid children
    QSet<NCReportDataSource*> dslist;
    QHash<QString, NCReportDataSource*>::const_iterator it = dataSources.constBegin();
    while (it != dataSources.constEnd()) {
        NCReportDataSource *ds = it.value();

        if (ds->openRole()==NCReportDataSource::ChildDataSource && !dataSources.contains(ds->parentId()))
            return false;   // invalid child, wrong parent id

        dslist << ds;
        ++it;
    }
    while ( dslist.count()>0 ) {
        // repeat until all data sources get inserted

        QHash<QString, NCReportDataSource*>::const_iterator it = dataSources.constBegin();
        while (it != dataSources.constEnd()) {
            NCReportDataSource *ds = it.value();

            if (dslist.contains(ds)) {

                switch(ds->openRole()) {
                case NCReportDataSource::BeginReport:
                case NCReportDataSource::ChildDataSource:
                {
                    if (insertDataSource(ds))
                        dslist.remove(ds);
                    break;
                }
                case NCReportDataSource::GroupChange:
                case NCReportDataSource::SubReport:
                {
                    dslist.remove(ds);  // we don't care GroupChange datasources here
                    break;
                }

                }
            }

            ++it;
        }
    }
    return true;
}

/*!
 * \brief NCReportDataSourceRelation::insertDataSource
 * \param dataSource
 * Inserts data source to the parent/child hierarchy. Finds its parent if has.
 * \return true if succeeded.
 */
bool NCReportDataSourceRelation::insertDataSource(NCReportDataSource *dataSource)
{
    bool ok=false;
    switch (dataSource->openRole()) {
    case NCReportDataSource::ChildDataSource:
    {
        // it has a parent, we have to find it
        for (int i=0; i<childCount(); i++) {
            if (child(i)->dataSource()->id() == dataSource->parentId()) {
                // found parent
                NCReportDataSourceRelation *relation = new NCReportDataSourceRelation(m_director, dataSource, child(i));
                child(i)->appendChild(relation);
                ok = true;
                break;
            } else {
                // not found, search lower level
                ok = child(i)->insertDataSource(dataSource);
            }
        }
        break;
    }
    case NCReportDataSource::BeginReport:
    {
        // ROOT ELEMENT
        NCReportDataSourceRelation *relation = new NCReportDataSourceRelation(m_director, dataSource, this);
        appendChild(relation);
        ok = true;
        break;
    }
    case NCReportDataSource::SubReport:
    case NCReportDataSource::GroupChange:
        // NOT VALID TYPES FOR RELATION
        break;
    }

    return ok;
}

//void NCReportDataSourceRelation::setDataSource(NCReportDataSource *dataSource)
//{
//    m_dataSource = dataSource;
//}

NCReportDataSource *NCReportDataSourceRelation::dataSource()
{
    return m_dataSource;
}

bool NCReportDataSourceRelation::open()
{
    return updateDataSource(First, this);
}

//bool NCReportDataSourceRelation::nextRecord()
//{
//    return recordPositionChange(Next);
//}

bool NCReportDataSourceRelation::nextRecord()
{
    return recordPositionChange(Next);
}
bool NCReportDataSourceRelation::nextRecord( NCReportDataSource *ds, bool isSamplingPageBreak)
{
    return recordPositionChange(Next, ds, isSamplingPageBreak);
}

bool NCReportDataSourceRelation::previousRecord()
{
    return recordPositionChange(Previous);
}

bool NCReportDataSourceRelation::previousRecord( NCReportDataSource *ds)
{
    return recordPositionChange(Previous, ds);
}

bool NCReportDataSourceRelation::firstRecord()
{
    return recordPositionChange(First);
}

bool NCReportDataSourceRelation::firstRecord( NCReportDataSource *ds)
{
    return recordPositionChange(First, ds);
}

bool NCReportDataSourceRelation::lastRecord()
{
    return recordPositionChange(Last);
}

bool NCReportDataSourceRelation::lastRecord( NCReportDataSource *ds)
{
    return recordPositionChange(Last, ds);
}

bool NCReportDataSourceRelation::recordPositionChange(NCReportDataSourceRelation::RecordAction action, bool isSamplingPageBreak)
{
    if (!dataSource())
        return false;

    bool done = false;
#if 1
    // walk through all chlidren
    while (m_currentChildIndex<childCount() && m_currentChildIndex>=0) {
        // position change of its child
        done = child(m_currentChildIndex)->recordPositionChange(action, isSamplingPageBreak);
        if (done)
            break;
        else {
            /*
             * PREVIOUS ISSUE: The child data source should update + bottom
            */
            switch (action) {
            case Next: m_currentChildIndex++; break;
            case Previous: m_currentChildIndex--; break;
            case First: m_currentChildIndex = 0; break;
            case Last: m_currentChildIndex = childCount()-1; break;
            }
        }
    }
    switch (action) {
    case Next:
        if ( m_currentChildIndex>=childCount() )
            m_currentChildIndex = childCount()-1;
        break;
    case Previous:
        if ( m_currentChildIndex<0 )
            m_currentChildIndex = 0;
        break;
    case First:
    case Last:
        break;
    }
#else
    // walk through all chlidren
    if (childCount() > m_currentChildIndex) {
        result = child(m_currentChildIndex)->recordPositionChange(action, isSamplingPageBreak);
    }
#endif

    if (done) {
        // ok we have done it by its child.
    } else {
        switch (action) {
        case Next: done = dataSource()->next(isSamplingPageBreak); break;
        case Previous: done = dataSource()->previous(); break;
        case First: done = dataSource()->first(); break;
        case Last: done = dataSource()->last(); break;
        }
#ifdef NCREPORT_DEBUG_OUTPUT
        qDebug("NCReportDataSourceRelation::recordPositionChange(%i) Result:%i ID:%s PK:%s FK:%s", action, done,
                qPrintable(dataSource()->id()),
                qPrintable(dataSource()->value(dataSource()->primaryKeyColumn()).toString()),
                qPrintable(dataSource()->foreignKeyValue())
                          );
#endif
        if (done) {
            if (childCount()>0) {
                //done = updateDataSource(child(0));
                updateDataSource(action, child(m_currentChildIndex));
            }
        } else {
//            if (dataSource()->forceUpdateOnStepBackwordInRelation() && parent() && action == Previous ) {
//                // this is a child data source. It needs to update query and go bottom.
//                done = updateDataSource(action, this);
//            }

            // nothing to do
        }
    }
    return done;

}

bool NCReportDataSourceRelation::recordPositionChange(NCReportDataSourceRelation::RecordAction action,  NCReportDataSource *ds, bool isSamplingPageBreak)
{
    if ( !ds )
        return false;
    NCReportDataSourceRelation *rel = find(ds);
    if (rel) {
        return rel->recordPositionChange(action, isSamplingPageBreak);
    }
    return false;
}

bool NCReportDataSourceRelation::updateDataSource(RecordAction action, NCReportDataSourceRelation *rel)
{
    bool ok = true;
    if (rel->dataSource()) {

        if (rel->parent() && rel->parent()->dataSource() ) {
            QString pk = rel->parent()->dataSource()->value( rel->parent()->dataSource()->primaryKeyColumn() ).toString();
            rel->dataSource()->setForeignKeyValue( pk );
#ifdef NCREPORT_DEBUG_OUTPUT
            qDebug("SET FK to %s:%s",qPrintable(rel->dataSource()->id()),qPrintable(pk));
#endif
        }
        if (rel->dataSource()->isOpened()) {
            rel->dataSource()->evaluate( m_director->evaluator() );
            ok = rel->dataSource()->update();
            if (ok) {
                switch (action) {
                case Next:
                case First: ok = rel->dataSource()->first(); break;
                case Previous:
                case Last: ok = rel->dataSource()->last(); break;
                }
                //----------------------------------------
                // return true: if empty data source is accetable and we would like the parent to be rendered.
                // return false: if empty data source is not accetable and we would like to hide parent.
                //----------------------------------------
                if (rel->dataSource()->allowEmpty())
                    ok = true;
            }
        } else {
            ok = m_director->openDataSource(rel->dataSource());
        }
    }
//    if (ok && rel->childCount()>0)
//        ok = updateDataSource( rel->child(0) );    // recursively update

    if (ok) {
        for (int i=0; i<rel->childCount(); i++) {
            ok = updateDataSource(action, rel->child(i) );
            if (!ok) // cancel if !ok
                break;
        }
    }
    return ok;
}

void NCReportDataSourceRelation::appendChild(NCReportDataSourceRelation *item)
{
    m_childItems.append(item);
}

NCReportDataSourceRelation *NCReportDataSourceRelation::find(NCReportDataSource *ds)
{
    if (dataSource()==ds)
        return this;
    if (m_lastFoundItem && m_lastFoundItem->dataSource() == ds)
        return m_lastFoundItem;

    for (int i=0; i<childCount(); ++i) {
        if ( ds == child(i)->dataSource() ) {
            m_lastFoundItem = child(i);
            return m_lastFoundItem;
        }
    }
    return 0;
}

NCReportDataSourceRelation *NCReportDataSourceRelation::child(int row)
{
    return m_childItems.value(row);
}

int NCReportDataSourceRelation::childCount() const
{
    return m_childItems.count();
}

NCReportDataSourceRelation *NCReportDataSourceRelation::parent()
{
    return m_parentItem;
}

int NCReportDataSourceRelation::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<NCReportDataSourceRelation*>(this));

    return 0;
}
