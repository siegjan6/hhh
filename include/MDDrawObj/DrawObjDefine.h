/*!
\author aGRw 2014.04.29
\section 控件的数据定义
*/
#ifndef DRAWOBJDEFINE_H
#define DRAWOBJDEFINE_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWOBJ_LIBRARY)
#  define MDDRAWOBJSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWOBJSHARED_EXPORT Q_DECL_IMPORT
#endif

/*!
 * \section 控件类型
*/
enum class DrawType
{
    Obj = 100,          //!基类
    Vector,             //!矢量基类
    Widget,             //!Wiget基类
    Multi,              //!多个控件,redo已占用0-2999
    Form,               //!窗体

    Group = 0,          //!组
    Combine,            //!联合

    Rect,				//!矩形
    Ellipse,			//!椭圆
    Arc,                //!弧线
    Sector,             //!扇形
    Shear,              //!切
    Text,				//!文本
    Button,             //!按钮

    StraightLine,		//!直线
    FoldLine,			//!折线   10
    Bezier,             //!曲线
    Polygon,			//!多边形
    ClosedBezier,       //!曲线多边形

    TextEdit,           //!编辑框
    ComboBox,           //!组合框
    List,               //!列表
    RadioButton,        //!单选按钮
    CheckBox,           //!复选框
    SpinBox,            //!微调按钮
    DateTime,           //!日期时间 20
    Calendar,           //!月历

    Alarm,              //!报警
    Log,                //!日志
    Gauge,              //!表盘
    Curve,              //!曲线
    Report,             //!报表

    Pipe,               //!流动管道
    DataArchives,       //!数据归档
    Diagram,            //!统计图
    HistoryCurve,       //!历史曲线 30
    TableView,          //!表格
    RecipeGrid,         //!配方
    ProcDocker,         //!进程主窗体停靠
    PdfViewer,          //!pdf阅读器
};

/*!
 * \section 控件活动状态
*/
enum class ControlState
{
    None,				//!无
    Move,				//!平移
    XScale,             //!X轴缩放
    YScale,             //!Y轴缩放
    Shear,              //!倾斜
    Center,             //!旋转中心点
    Rotate,             //!旋转
    FrameMove,          //!边框移动
    BoundMove,          //!矩形边框移动
    MultiRotate,		//!成组旋转
    Custom,             //!自定义点
    MoveNode,			//!移动节点
    AddNode,			//!增加节点
    DeleteNode,         //!删除节点
    Segment,			//!路径切割
    FormWidth,          //!窗体宽度
    FormHeight          //!窗体高度
};

/*!
 * \section 鼠标事件类型
*/
enum class MouseType { Down, Up, Move };
/*!
 * \section 节点编辑状态
*/
enum class NodeState{ Move, Add, Delete };

/*!
 * \section 编辑模式
*/
enum class EditMode
{
    Normal,             //!常用
    Node,               //!节点
    Segment				//!切割
};

/*!
 * \section 正交编辑模式
*/
enum class OrthoMode
{
    Square,				//!正方形
    HoriOrVert,			//!水平或垂直
    Invalid				//!无效
};

/*!
 * \section 通知事件类型
*/
enum class NotifyType
{
    Log,//!日志
    Alarm//!报警
};

#endif // DRAWOBJDEFINE_H
