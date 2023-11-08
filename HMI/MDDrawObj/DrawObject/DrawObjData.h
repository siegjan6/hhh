/*!
\author aGRw 2015.03.03
\section 绘图基类数据
*/
#ifndef DRAWOBJDATA_H
#define DRAWOBJDATA_H

#include <QPainterPath>
#include <QStringList>
#include <QList>

#include "DrawData.h"
#include "IHMIForm.h"
#include "MDExpInfo.h"

class CMDPropertyDrawObj;
class CMDDrawMulti;

class CDrawObjData
{
public:
    CDrawObjData();
public:
    QRectF m_bound;
    IHMIForm* m_parent;
    CMDDrawMulti* m_multiParent;
    QPainterPath m_basePath;		//!原始控件路径
    QString m_name;//!控件名称
    QRectF m_newRect;
    CDataObj m_data;		//!数据
    CCalcData m_dataBk;		//!计算数据
    QRectF m_rectBk;
    //!layer
    bool m_visible;		//!可见
    bool m_locked;//!锁定
    qint32 m_layer;
    //var
    CMDExpManager m_expManager;
    //!property
    CMDPropertyDrawObj* m_property;
    //!是否支持父控件事件响应
    bool m_supportGroupEvent;
    //!控件是否显示选中框
    bool m_showSelectedFrame;
    QString m_storageImagePath;//!图库的图片目录

    //!run
    //提示信息
    QString m_toolTip;
    //隐藏时是否支持事件
    bool m_supportHidedEvent;
    //类继承信息
    QStringList m_classInfo;
/*!
\section common
*/
public:
    void operator=(const CDrawObjData& other);
    void serialize(QJsonObject &json);
    void deserialize(const QJsonObject &json);
//////////////////////////////////////////////////////////////////////////
};

#endif // DRAWOBJDATA_H
