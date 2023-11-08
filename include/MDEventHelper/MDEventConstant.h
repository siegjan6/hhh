/*!

\author dGFuZ3p3

\section 功能

定义事件常量。

*/

#ifndef MDEVENTCONSTANT_H
#define MDEVENTCONSTANT_H

enum class MDEventType
{
    null_event                                          = 0,

    // 1 - 32
    combined                                          = 1,

    // 33 - 64
    project_Loaded                               = 33,
    project_BeforeUnload                    = 34,

    // 65 - 96
    variable_ValueChanged                    = 65,
    variable_ValueChangedInRange     = 66,
    variable_QualityChanged                 = 67,
    variable_ValueRead                            = 68,
    variable_ValueWrite                           = 69,

    // 97 - 128
    timer_Cycle                                        = 97,
    timer_Moment                                  = 98,

    // 129 - 160
    recipe_Uploaded                 = 129,
    recipe_Downloaded           = 130,
    recipe_BeforeView             = 131,
    recipe_BeforeModify         = 132,
    recipe_BeforeDelete          = 133,

    // 161 - 192
    window_Opened                   = 161,
    window_Closed                      = 162
};

enum class MDOperator
{
    null_operator                           = 0,

    greater_than                            = 1,    // >
    less_than                                   = 2,    // <
    equal_to                                    = 3,    // ==
    greater_than_or_equal_to     = 4,    // >=
    less_than_or_equal_to            = 5,   // <=
    not_equal_to                            = 6,    // !=
    take_a_bit                                  = 7     // bit[]
};

enum class MDRelationship
{
    null_relationship  = 0,
    relationship_no   = 1,
    relationship_or    = 2,
    relationship_and = 3
};

#endif // MDEVENTCONSTANT_H
