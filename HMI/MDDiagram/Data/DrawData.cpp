#include <QJsonObject>
#include <QJsonArray>
#include "DrawData.h"
#include "MDJsonConvert.h"

CDrawData::CDrawData()
{
    m_backColor = Qt::white;
    m_interColor = Qt::white;
    m_diagramStyle = 1;
    m_diagramFont = QFont("Arial", 10);
    m_typePosition = 0;
    m_typeValueFont = QFont("Arial", 10);
    m_toolPosition = 0;
    m_stringPosition = 1;
    m_stringArrange = 1;
    m_stringFont = QFont("Arial", 10);
    m_stringColor = Qt::black;
    m_stringText = "";
    m_isSetEdge = 1;

    m_category = 0;
    m_decimals = 2;
    m_setYValue = 0;
    m_maxValue = 150;
    m_minValue = -100;
    m_mainScaleNum = 5;
    m_subScaleNum = 2;
    m_lineColor = Qt::black;

    m_string1 = "2011年,2012年,2013年,2014年,2015年";
    m_string2 = "一月,二月,三月";
    m_string3 = "A,B,C,D";
    m_valueString =  "10,20,30,40,50,26,37,48,50,20,11,52,34,44,45,"
            "34,21,18,37,47,38,21,33,40,42,12,19,28,45,39,"
            "21,44,34,24,39,46,50,34,45,33,24,47,44,35,33,"
            "21,42,13,34,49,32,37,13,29,45,44,21,29,39,28,"
            "32,44,32,24,41,33,27,34,45,30,29,21,44,34,22"
            ;
}

CDrawData::~CDrawData()
{
}

CDrawData &CDrawData::operator =(const CDrawData &other)
{
    this->m_backColor = other.m_backColor;
    this->m_interColor = other.m_interColor;
    this->m_diagramStyle = other.m_diagramStyle;
    this->m_diagramFont = other.m_diagramFont;
    this->m_typePosition = other.m_typePosition;
    this->m_typeValueFont = other.m_typeValueFont;
    this->m_toolPosition = other.m_toolPosition;
    this->m_stringPosition = other.m_stringPosition;
    this->m_stringArrange = other.m_stringArrange;
    this->m_stringFont = other.m_stringFont;
    this->m_stringColor = other.m_stringColor;
    this->m_stringText = other.m_stringText;
    this->m_isSetEdge = other.m_isSetEdge;

    this->m_category = other.m_category;
    this->m_decimals = other.m_decimals;
    this->m_setYValue = other.m_setYValue;
    this->m_maxValue = other.m_maxValue;
    this->m_minValue = other.m_minValue;
    this->m_mainScaleNum = other.m_mainScaleNum;
    this->m_subScaleNum =other.m_subScaleNum;
    this->m_lineColor = other.m_lineColor;

    this->m_string1 = other.m_string1;
    this->m_string2 = other.m_string2;
    this->m_string3 = other.m_string3;
    this->m_valueString = other.m_valueString;
    return *this;
}

void CDrawData::serialize(QJsonObject &json)
{
    json.insert("DiagramStyle",m_diagramStyle);
    json.insert("StringColor",(double)m_stringColor.rgba());
    json.insert("BackColor",(double)m_backColor.rgba());
    json.insert("InterColor",(double)m_interColor.rgba());
    json.insert("DiagramFont",CMDJsonConvert::toJson(m_diagramFont));
    json.insert("TypePosition",m_typePosition);
    json.insert("TypeValueFont",CMDJsonConvert::toJson(m_typeValueFont));
    json.insert("ToolPosition",m_toolPosition);
    json.insert("StringPosition",m_stringPosition);
    json.insert("StringArrange",m_stringArrange);
    json.insert("StringFont",CMDJsonConvert::toJson(m_stringFont));
    json.insert("StringText",m_stringText);
    json.insert("IsSetEdge",m_isSetEdge);
    json.insert("Category",m_category);
    json.insert("Decimals",m_decimals);
    json.insert("SetYValue",m_setYValue);
    json.insert("MaxValue",m_maxValue);
    json.insert("MinValue",m_minValue);
    json.insert("MainScaleNum",m_mainScaleNum);
    json.insert("SubScaleNum",m_subScaleNum);
    json.insert("LineColor",(double)m_lineColor.rgba());
    json.insert("String1",m_string1);
    json.insert("String2",m_string2);
    json.insert("String3",m_string3);
    json.insert("ValueString",m_valueString);
}

void CDrawData::deserialize(const QJsonObject &json)
{
    m_diagramStyle = json.value("DiagramStyle").toInt();
    m_stringColor = QColor::fromRgba(json.value("StringColor").toDouble());
    m_backColor = QColor::fromRgba(json.value("BackColor").toDouble());
    m_interColor = QColor::fromRgba(json.value("InterColor").toDouble());
    m_diagramFont =CMDJsonConvert::toQFont(json.value("DiagramFont"));
    m_typePosition = json.value("TypePosition").toInt();
    m_typeValueFont = CMDJsonConvert::toQFont(json.value("TypeValueFont"));
    m_toolPosition = json.value("ToolPosition").toInt();
    m_stringPosition = json.value("StringPosition").toInt();
    m_stringArrange = json.value("StringArrange").toInt();
    m_stringFont = CMDJsonConvert::toQFont(json.value("StringFont"));
    m_stringText = json.value("StringText").toString();
    m_isSetEdge = json.value("IsSetEdge").toBool();
    m_category = json.value("Category").toInt();
    m_decimals = json.value("Decimals").toInt();
    m_setYValue = json.value("SetYValue").toInt();
    m_maxValue = json.value("MaxValue").toInt();
    m_minValue = json.value("MinValue").toInt();
    m_mainScaleNum = json.value("MainScaleNum").toInt();
    m_subScaleNum = json.value("SubScaleNum").toInt();
    m_lineColor = QColor::fromRgba(json.value("LineColor").toDouble());
    m_string1 = json.value("String1").toString();
    m_string2 = json.value("String2").toString();
    m_string3 = json.value("String3").toString();
    m_valueString = json.value("ValueString").toString();
}

