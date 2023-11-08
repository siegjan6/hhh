#ifndef UNDODATANODE_H
#define UNDODATANODE_H

#include "MDUndoData.h"

class CUndoDataNode : public CMDUndoDataVector
{
public:
    virtual ~CUndoDataNode(){}

public:
    QList<QPointF> m_nodes;
};

#endif // UNDODATANODE_H
