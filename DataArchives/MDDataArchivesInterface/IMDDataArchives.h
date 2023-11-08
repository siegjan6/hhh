#ifndef MDDATAARCHIVES_H
#define MDDATAARCHIVES_H

#include "MDDataGroup.h"
class  CMDDataArchives
{
public:
    CMDDataArchives();
public:
    void initial();
public:
    bool addDataGroup(CMDDataGroup *dataGroup);
    bool removeDataGroup(CMDDataGroup *dataGroup);
    void removeAllDataGroup();
    CMDDataGroup *getDataGroup(const QString &name);
    CMDDataGroup *getDataGroup(int index);
    int dataGroupCount();
private:
    QList <CMDDataGroup*> m_dataGroupList;
};

#endif // MDDATAARCHIVES_H
