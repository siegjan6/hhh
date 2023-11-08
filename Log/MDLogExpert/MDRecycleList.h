#ifndef MDRECYCLELIST_H
#define MDRECYCLELIST_H

#include <QList>
#include <QLinkedList>

class CMDLogRecord;
class CMDRecycleList
{
public:
   CMDRecycleList();
   CMDRecycleList(QList<CMDLogRecord*>* rawList);
   virtual ~CMDRecycleList();
public:
   void init(QList<CMDLogRecord*>* rawList, int maxCount);
   void release();
public:
   QList<CMDLogRecord*>* rawList() const;
   int count() const;
   CMDLogRecord* at(int index) const;
public:
   void append(CMDLogRecord* t);
   void prepend(CMDLogRecord* t);
public:
   CMDLogRecord* takeFirst();
   CMDLogRecord* takeLast();
   void ensureMaxCount_FromFirst();
protected:
   void appendToRecycle(CMDLogRecord* t);
   void prependToRecycle(CMDLogRecord* t);
protected:
   QList<CMDLogRecord*>* m_rawList;
   QLinkedList<CMDLogRecord*> m_recycleList;
   int m_maxCount;
};

#endif // MDRECYCLELIST_H
