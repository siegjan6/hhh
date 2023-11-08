/*!
\author bWVpeGw=
\section  报警日志公共导出类
*/
#ifndef MDDATAGROUPGRID_H
#define MDDATAGROUPGRID_H

#include <QtCore/qglobal.h>
#include <QWidget>
#include <QJsonObject>
#include "IDrawEnvironment.h"
#include "MDGroupGridDefine.h"
class CDataGroupWidget;
class IMDDBQueryEngine;

#if defined(MDDATAGROUPGRID_LIBRARY)
#  define MDDATAGROUPGRID_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDATAGROUPGRID_SHARED_EXPORT Q_DECL_IMPORT
#endif

class MDDATAGROUPGRID_SHARED_EXPORT CMDDataGroupGrid
{
public:
    CMDDataGroupGrid();
    ~CMDDataGroupGrid();

/*!
\section 初始化
*/
public:
    QWidget* widget();
    void onInitialization();
    void setGridType(GridType type);                    //! 控件类型设置
    void setGridName(const QString &name);              //! 获取控件名称
    void setDrawEnvrionment(IDrawEnvrionment* value);   //! 设置控件框架指针
private:
    CDataGroupWidget* m_widget;

/*!
\section 事件触发通知
*/
public:
    void setNotify(bool notify);
    void setQueryEngine(IMDDBQueryEngine * pEngine);
    void query();
/*!
\section 打开配置框
*/
public:
    bool openConfigDialog();
    void onDoubleClick();
/*!
\section 序列化
*/
public:
    //! 一般情况下几乎总要遵循operator=输入和返回的都是类对象的引用的原则
    CMDDataGroupGrid& operator =(const CMDDataGroupGrid &other);
    void serialize(QDataStream& stream, bool isOrigVersion = false);
    void deserialize(QDataStream& stream);
    void serialize_1(QDataStream& stream);

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

#endif // MDDATAGROUPGRID_H
