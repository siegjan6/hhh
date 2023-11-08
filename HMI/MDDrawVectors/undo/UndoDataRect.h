/*!
\section redo操作时需要的rect数据
*/
#ifndef UNDODATARECT_H
#define UNDODATARECT_H

#include "MDUndoData.h"

class CUndoDataRect : public CMDUndoDataVector
{
public:
    CUndoDataRect();
    virtual ~CUndoDataRect(){}
public:
    qreal m_xRound;
    qreal m_yRound;
};

#endif // UNDODATARECT_H
