/*!
\author aGRw 2015.03.03
\section 绘图矢量类数据
*/
#ifndef DRAWVECTORDATA_H
#define DRAWVECTORDATA_H

#include <QtGui>
#include <QList>
#include "MDDrawObj.h"
#include "MDFill.h"
#include "MDColorManager/MDColorManager.h"
#include "DrawData.h"
#include "MDHMIActionManager.h"
#include "DynamicColor.h"

class CDrawVectorData
{
public:
    CDrawVectorData();
    ~CDrawVectorData();
public:
    CDataVector m_dataVector;			//!矢量数据
    QPainterPath m_matrixPath;		//!矩阵转换后的控件路径
    QPainterPath m_path;			//!混合父控件矩阵后的最终路径
    QTransform m_matrix;
    CMDPenManager m_penManager;						//!画笔
    CMDBrushManager m_brushManager;					//!画刷
    EditMode m_editMode;
    CMDFill m_fill;//!填充配置
    QString m_inputVariable;//!输入变量
    QTransform m_multiMatrix;//!原始矩阵和成组矩阵相乘后的实际矩阵
    QTransform m_baseMatrix;//!原始矩阵
    QString sMouseDown;
    QString sMouseUp;
    QString sRMouseDown;
    QString sRMouseUp;

    //!action
    CMDHMIActionManager m_mouseDownActions;
    CMDHMIActionManager m_mouseUpActions;
    CMDHMIActionManager m_rightMouseDownActions;
    CMDHMIActionManager m_rightMouseUpActions;

    //!dynamic color
    CDynamicColorManager m_dynamicBrush;
    CDynamicColorManager m_dynamicPen;

/*!
\section common
*/
public:
    void operator=(const CDrawVectorData& other);
    void serialize(QJsonObject &json);
    void deserialize(const QJsonObject &json);
//////////////////////////////////////////////////////////////////////////
};

#endif // DRAWVECTORDATA_H
