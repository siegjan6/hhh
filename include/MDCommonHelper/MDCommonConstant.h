/*!

\author dGFuZ3p3

\section 功能

定义对象类型常量。

*/

#ifndef MDCOMMONCONSTANT_H
#define MDCOMMONCONSTANT_H

enum class MDObjectType
{
    null_obj = 0,

    project             = 1,
    variable            = 2,
    window            = 3,
    timer                = 4,
    recipe               = 5,
    logGroup         = 6,
    alarmGroup    = 7,
    historyGroup   = 8,
    action               = 9,
    event                = 10,
    account             = 11,
    powerGroup  = 12,
    alarm               = 13,
    alarmLevel      = 14,
    alarmCondition   = 15,
    mdswitch               = 16,

    custom           = 999
};

#endif // MDCOMMONCONSTANT_H
