/*!

\author dGFuZ3p3

\section 功能

定义报警常量。

*/

#ifndef MDALARMCONSTANT_H
#define MDALARMCONSTANT_H

/*!
\section 报警模式
*/
enum class MDAlarmMode
{
    common   = 0,    //! 普通
    extend       = 1    //! 扩展
};

/*!
\section 报警确认状态
*/
enum class MDAlarmConfirmStatus
{
    occur                   = 0,    //! 发生
    confirmed_not_end       = 1,    //! 已确认，未结束
    confirmed_then_end     = 2,    //! 先确认，后结束
    end_not_confirmed       = 3,    //! 已结束，未确认
    end_then_confirmed     = 4    //! 先结束，后确认
};

/*!
\section 报警知悉状态
*/
enum class MDAlarmAcknowledgeStatus
{
    occur                               = 0,    //! 发生
    acknowledged_not_end       = 1,    //! 已知悉，未结束
    acknowledged_then_end     = 2,    //! 先知悉，后结束
    end_not_acknowledged       = 3,    //! 已结束，未知悉
    end_then_acknowledged     = 4    //! 先结束，后知悉
};

/*!
\section 报警抑制状态
*/
enum class MDAlarmSuppressStatus
{
    occur                               = 0,    //! 发生
    suppressed_not_end       = 1,    //! 已抑制，未结束
    suppressed_then_end     = 2,    //! 先抑制，后结束
    end_not_suppressed       = 3,    //! 已结束，未抑制
    end_then_suppressed     = 4    //! 先结束，后抑制
};

/*!
\section 报警抑制状态
*/
enum class MDAlarmShelveStatus
{
    occur                               = 0,    //! 发生
    shelved_not_end       = 1,    //! 已搁置，未结束
    shelved_then_end     = 2,    //! 先搁置，后结束
    end_not_shelved       = 3,    //! 已结束，未搁置
    end_then_shelved     = 4    //! 先结束，后搁置
};


/*!
\section 报警总状态
*/
enum class MDAlarmStatus
{
    null                        =   -1, //! 无
    occur                       =   0,  //! 发生
    end                         =   1,  //! 结束自动知悉，无需确认
    occur_need_ack              =   2,  //! 发生需要人为知悉，未结束
    occur_acked                 =   3,  //! 发生且已经人为知悉，未结束
    end_need_ack                =   4,  //! 结束需要人为知悉
    end_acked                   =   5,  //! 结束已经人为知悉
    occur_acked_need_confirm    =   6,  //! 发生已经人为知悉，需要人为确认，未结束
    occur_acked_confirmed       =   7,  //! 发生已经人为知悉，且已经人为确认，未结束
    end_acked_need_confirm      =   8,  //! 结束已经人为知悉，需要人为确认
    end_acked_confirmed         =   9,  //! 结束已经人为知悉，且已经人为确认
    override                    =   10, //! 此报警被更高级别的报警覆盖
};


#endif // MDALARMCONSTANT_H
