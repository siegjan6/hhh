#ifndef UNDODATABASICARC_H
#define UNDODATABASICARC_H

#include "MDUndoData.h"

class CUndoDataBasicArc : public CMDUndoDataVector
{
public:
    CUndoDataBasicArc();
    virtual ~CUndoDataBasicArc(){}
public:
    qreal m_startAngle;
    qreal m_spanAngle;
};

#endif // UNDODATABASICARC_H




