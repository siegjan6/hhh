#include "ConfigData.h"

/*
    enum DockWidgetArea {
        LeftDockWidgetArea = 0x1,
        RightDockWidgetArea = 0x2,
        TopDockWidgetArea = 0x4,
        BottomDockWidgetArea = 0x8,

        DockWidgetArea_Mask = 0xf,
        AllDockWidgetAreas = DockWidgetArea_Mask,
        NoDockWidgetArea = 0
    };
*/

/*
id：类型
title：标题
area：位置，参考DockWidgetArea
defaultVisible:默认是否显示
isTab:是否tab页显示
*/
const QString CConfigData::Dock = R"(
[
    {
        "id": "Workspace",
        "title": "工作区",
        "area": 1,
        "defaultVisible": true,
        "isTab":true
    },
    {
        "id": "Toolkit",
        "title": "工具箱",
        "area": 1,
        "defaultVisible": true,
        "isTab":true
    },
    {
        "id": "Log",
        "title": "日志",
        "area": 8
    },
    {
        "id": "Property",
        "title": "属性栏",
        "area": 2,
        "defaultVisible": true
    },
    {
        "id": "Object",
        "title": "对象",
        "defaultVisible": true,
        "area": 2
    }
]
)";
