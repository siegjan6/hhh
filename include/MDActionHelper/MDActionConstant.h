/*!

\author dGFuZ3p3

\section 功能

定义动作常量。

*/

#ifndef MDACTIONCONSTANT_H
#define MDACTIONCONSTANT_H

enum class MDActionType
{
    null_action                              = 0,

    // 1 - 32
    reserved                                 = 1,

    // 33 - 40
    project_Load                         = 33,
    project_Unload                    = 34,
    project_ExitFS                     = 35,

    // 41 - 56
    variable_WriteValue               = 41,
    variable_ReadValue                = 42,

    // 57 - 64
    timer_Delay                       = 57,
    timer_Start                         = 58,
    timer_Stop                        = 59,

    // 65 - 80
    recipe_Upload                 = 65,
    recipe_Download           = 66,
    recipe_View                     = 67,
    recipe_Modify                   = 68,
    recipe_Delete                   = 69,

    // 81 - 96
    window_Open                 = 81,
    window_Close                 = 82,
    window_Move                 = 83,
    window_Hide                 = 84,
    window_RunScript            = 85,
    window_ShowByPid            = 86,
    window_HideByPid            = 87,
    window_CloseByPid           = 88,

    // 97 - 104
    account_Modify                 = 97,
    account_ModifyPassword = 98,
    account_Delete                 = 99,

    // 105 - 112
    powerGroup_Modify        = 105,
    powerGroup_Delete         = 106,

    // 113 - 144
    action_Execute           = 113,
    alarm_Confirm           = 114,
    historyGroup_Save     = 115,
    tts_Play                        = 116,
    account_powergroup_Manage = 117,
    alarm_Acknowledge           = 118,

    // 999
    custom                      = 999
};

enum class MDWriteMode
{
    null_mode  = 0,
    mode_fixed  = 1,
    mode_relative    = 2,
    mode_switch_0_1 = 3
};

#endif // MDACTIONCONSTANT_H
