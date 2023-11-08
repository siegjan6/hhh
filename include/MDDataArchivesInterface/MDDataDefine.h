#ifndef MDDATADEFINE_H
#define MDDATADEFINE_H

/*************************** alarm *********************************/
#define ALM_ID						"ID"                        // ID 系统自动生成
#define ALM_AID						"AID"                       // AID 用户定义报警ID号，一般要求全局唯一
#define ALM_NAME					"Name"                      // 名称
#define ALM_MESSAGE					"Message"                   // 消息
#define ALM_LEVEL					"AlarmLevel"                // 报警类别
#define ALM_SERVERRITY					"Serverity"                 // 报警等级
#define ALM_STARTVALUE                                  "StartValue"                // 发生值
#define ALM_ENDVALUE                                    "EndValue"                  // 结束值
#define ALM_STARTTIME                                   "StartTime"                 // 发生时间
#define ALM_STARTTIME_MS                                "StartTimeMS"               // 发生时间毫秒
#define ALM_ENDTIME					"EndTime"                   // 结束时间
#define ALM_ENDTIME_MS                                  "EndTimeMS"                 // 结束时间毫秒
#define ALM_ACKNOWLEDGETIME                             "AckTime"                   // 知悉时间
#define ALM_ACKNOWLEDGETIME_MS                          "AckTimeMS"                 // 知悉时间毫秒
#define ALM_ACKNOWLEDGESTATE                            "AckState"                  // 知悉状态
#define ALM_CONFIRMTIME                                 "ConfirmTime"               // 确认时间
#define ALM_CONFIRMTIME_MS                              "ConfirmTimeMS"             // 确认时间毫秒
#define ALM_CONFIRMSTATE                                "ConfirmState"              // 确认状态
#define ALM_SUPPRESSEDORSHELVED                         "SuppressedOrShelved"       // 抑制或者搁置状态 0-无 1-Suppressed(抑制) 2-Shelved(搁置)
#define ALM_SUPPRESSSTATE                               "SuppressState"             // 抑制状态
#define ALM_SUPPRESSTIME                                "SuppressTime"              // 抑制时间
#define ALM_SUPPRESSTIME_MS                             "SuppressTimeMS"            // 抑制时间毫秒
#define ALM_SHELVESTATE                                 "ShelveState"               // 搁置状态
#define ALM_SHELVETIME                                  "ShelveTime"                // 搁置时间
#define ALM_SHELVETIME_MS                               "ShelveTimeMS"              // 搁置时间毫秒
#define ALM_STATUS					"Status"                    // 状态
#define ALM_USER					"CurrentUser"               // 当前用户
#define ALM_COMMENT                                     "Comment"                   // 备注
#define ALM_FORECOLOR                                   "ForeColor"                 // 前景色
#define ALM_BACKCOLOR                                   "BackColor"                 // 背景色


/****************************** log *********************************/
#define LOG_RECORDTIME                                  "RecordTime"                // 记录时间
#define LOG_RECORDTIME_MS                               "RecordTimeMS"              // 记录时间毫秒
#define LOG_USER					"CurrentUser"               // 当前用户
#define LOG_MESSAGE					"Message"                   // 消息
#define LOG_TYPE					"Type"                      // 对象类型
#define LOG_NAME					"Name"                      // 对象名称
#define LOG_PROTECTMODE                                 "ProtectMode"               // 保护模式
#define LOG_OPERATER                                    "Operater"                  // 操作者
#define LOG_OPERATETIME                                 "OperateTime"               // 操作时间
#define LOG_VERIFIER                                    "Verifier"                  // 校验者
#define LOG_VERIFYTIME                                  "VerifyTime"                // 校验时间
#define LOG_OPERATECOMMENT                              "OperateComment"            // 操作说明
#define LOG_VERIFYCOMMENT                               "VerifyComment"             // 校验说明

/******************************** default group name ************************************/
#define  ALARM_DATA_GROUP			"AlarmDataGroup"
#define  LOG_DATA_GROUP             "LogDataGroup"

#endif // MDDATADEFINE_H
