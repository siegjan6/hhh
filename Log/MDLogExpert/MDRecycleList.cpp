#include "MDRecycleList.h"
#include "MDLogRecord.h"

CMDRecycleList::CMDRecycleList()
{
    m_rawList = NULL;
    m_maxCount = 0;
}

CMDRecycleList::CMDRecycleList(QList<CMDLogRecord*>* rawList)
{
    m_rawList = rawList;
    m_maxCount = 0;
}

CMDRecycleList::~CMDRecycleList()
{
    release();
}

void CMDRecycleList::init(QList<CMDLogRecord*>* rawList, int maxCount)
{
    m_rawList = rawList;
    m_maxCount = maxCount;
}

void CMDRecycleList::release()
{
    if(m_rawList)
    {
        while(!m_rawList->isEmpty())
            delete m_rawList->takeFirst();

        m_rawList = NULL;
    }

    while(!m_recycleList.isEmpty())
        delete m_recycleList.takeFirst();
}

QList<CMDLogRecord*>* CMDRecycleList::rawList() const
{
    return m_rawList;
}

int CMDRecycleList::count() const
{
    return (m_rawList ? m_rawList->count() : 0);
}

CMDLogRecord* CMDRecycleList::at(int index) const
{
    return (m_rawList ? m_rawList->at(index) : NULL);
}

void CMDRecycleList::append(CMDLogRecord* t)
{
    if(!m_rawList)
        return;

    m_rawList->append(t);
    if(m_rawList->size() > m_maxCount)
    {
        appendToRecycle(m_rawList->takeFirst());
    }
}

void CMDRecycleList::prepend(CMDLogRecord* t)
{
    if(!m_rawList)
        return;

    m_rawList->prepend(t);
    if(m_rawList->size() > m_maxCount)
    {
        prependToRecycle(m_rawList->takeLast());
    }
}

void CMDRecycleList::appendToRecycle(CMDLogRecord* t)
{
    m_recycleList.append(t);

    if(m_recycleList.count() > m_maxCount)
        delete m_recycleList.takeFirst();
}

void CMDRecycleList::prependToRecycle(CMDLogRecord* t)
{
    m_recycleList.prepend(t);

    if(m_recycleList.count() > m_maxCount)
        delete m_recycleList.takeLast();
}

void CMDRecycleList::ensureMaxCount_FromFirst()
{
    if(!m_rawList)
        return;

    while (m_rawList->size() > m_maxCount)
        prepend(m_rawList->takeLast());
}

CMDLogRecord* CMDRecycleList::takeFirst()
{
    return (!m_recycleList.isEmpty() ? m_recycleList.takeFirst() : NULL);
}

CMDLogRecord* CMDRecycleList::takeLast()
{
    return (!m_recycleList.isEmpty() ? m_recycleList.takeLast() : NULL);
}
