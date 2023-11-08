/*!
\author enli 2015.01.08
\section 功能函数类汇总
*/
#ifndef MDDRIVERFUNCTIONS_H
#define MDDRIVERFUNCTIONS_H

#include "MDDriverLibs_global.h"
//位操作类
class MDDRIVERLIBSSHARED_EXPORT CBitFuntons
{
 public:
    //!置位(0)
    static quint32 clearBit(quint32 var, quint8 bit);
    //!取位
    static quint32 getBit(quint32 var, quint8 bit);
    //!置位(1)
    static quint32 setBit(quint32 var, quint8 bit);
};
//!CRC校验类
class MDDRIVERLIBSSHARED_EXPORT CCyclicRedundancyCheck
{
public:
    static quint16 CRC16(quint8 *btData, quint16 startIndex, quint16 endIndex);
};
#endif // MDDRIVERFUNCTIONS_H
