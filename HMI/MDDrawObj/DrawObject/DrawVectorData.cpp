#include "DrawVectorData.h"

//////////////////////////////////////////////////////////////////////////
//todo add variable

//////////////////////////////////////////////////////////////////////////
CDrawVectorData::CDrawVectorData()
{
    m_editMode = EditMode::Normal;

    sMouseDown = "onMouseDown";
    sMouseUp = "onMouseUp";
    sRMouseDown = "onRButtonMouseDown";
    sRMouseUp = "onRButttonMouseUp";

    m_dynamicBrush = CDynamicColorManager(DynamicType::Brush);
    m_dynamicPen = CDynamicColorManager(DynamicType::Pen);
}

CDrawVectorData::~CDrawVectorData()
{
}
//////////////////////////////////////////////////////////////////////////
void CDrawVectorData::operator=(const CDrawVectorData &other)
{
    if (this == &other)
        return;

    m_dataVector = other.m_dataVector;
    m_matrixPath = other.m_matrixPath;
    m_path = other.m_path;
    m_baseMatrix = other.m_baseMatrix;
    m_brushManager = other.m_brushManager;
    m_penManager = other.m_penManager;
    m_fill = other.m_fill;
    m_inputVariable = other.m_inputVariable;

    m_mouseDownActions = other.m_mouseDownActions;
    m_mouseUpActions = other.m_mouseUpActions;
    m_rightMouseDownActions = other.m_rightMouseDownActions;
    m_rightMouseUpActions = other.m_rightMouseUpActions;

    m_dynamicBrush = other.m_dynamicBrush;
    m_dynamicPen = other.m_dynamicPen;
}
void CDrawVectorData::serialize(QJsonObject &json)
{
    json.insert("InputVariable", m_inputVariable);

    QJsonObject jsDataVector;
    m_dataVector.serialize(jsDataVector);
    json.insert("DataVector", jsDataVector);
    QJsonObject jsBrushManager;
    m_brushManager.serialize(jsBrushManager);
    json.insert("BrushManager", jsBrushManager);
    QJsonObject jsPenManager;
    m_penManager.serialize(jsPenManager);
    json.insert("PenManager", jsPenManager);
    QJsonObject jsFill;
    m_fill.serialize(jsFill);
    json.insert("Fill", jsFill);

    QJsonObject jsDynamicBrush;
    m_dynamicBrush.serialize(jsDynamicBrush);
    json.insert("DynamicBrush", jsDynamicBrush);
    QJsonObject jsDynamicPen;
    m_dynamicPen.serialize(jsDynamicPen);
    json.insert("DynamicPen", jsDynamicPen);

    QJsonObject jsMouseDown;
    m_mouseDownActions.serialize(jsMouseDown);
    json.insert("MouseDownActions", jsMouseDown);
    QJsonObject jsMouseUp;
    m_mouseUpActions.serialize(jsMouseUp);
    json.insert("MouseUpActions", jsMouseUp);
    QJsonObject jsRightMouseDown;
    m_rightMouseDownActions.serialize(jsRightMouseDown);
    json.insert("RightMouseDownActions", jsRightMouseDown);
    QJsonObject jsRightMouseUp;
    m_rightMouseUpActions.serialize(jsRightMouseUp);
    json.insert("RightMouseUpActions", jsRightMouseUp);
}
void CDrawVectorData::deserialize(const QJsonObject &json)
{
    m_inputVariable = json.value("InputVariable").toString();

    m_dataVector.deserialize(json.value("DataVector").toObject());
    m_brushManager.deserialize(json.value("BrushManager").toObject());
    m_penManager.deserialize(json.value("PenManager").toObject());
    m_fill.deserialize(json.value("Fill").toObject());

    m_dynamicBrush.deserialize(json.value("DynamicBrush").toObject());
    m_dynamicPen.deserialize(json.value("DynamicPen").toObject());

    m_mouseDownActions.deserialize(json.value("MouseDownActions").toObject());
    m_mouseUpActions.deserialize(json.value("MouseUpActions").toObject());
    m_rightMouseDownActions.deserialize(json.value("RightMouseDownActions").toObject());
    m_rightMouseUpActions.deserialize(json.value("RightMouseUpActions").toObject());
}
//////////////////////////////////////////////////////////////////////////


