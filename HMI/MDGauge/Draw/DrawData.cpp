#include <QJsonObject>
#include <QJsonArray>
#include "DrawData.h"
#include "MDJsonConvert.h"
/*!
\section  背景色数据类。
*/
CBackColor::CBackColor()
{
    m_color1 = QColor(255,255,255);
    m_color2 = QColor(255,255,255);
    m_gradientStyle = gsSolid;
}

void CBackColor::operator=(const CBackColor& data)
{
    this->m_color1 = data.m_color1;
    this->m_color2 = data.m_color2;
    this->m_gradientStyle = data.m_gradientStyle;
}

void CBackColor::serialize(QJsonObject &json)
{
    json.insert("Color1",(double)m_color1.rgba());
    json.insert("Color2",(double)m_color2.rgba());
    json.insert("GradientStyle",(int)m_gradientStyle);
}

void CBackColor::deserialize(const QJsonObject &json)
{
    m_color1 = QColor::fromRgba(json.value("Color1").toDouble());
    m_color2 = QColor::fromRgba(json.value("Color2").toDouble());
    int gradient = json.value("GradientStyle").toInt();
    m_gradientStyle = (gradientStyle)gradient;

}
//////////////////////////////////////////////////////////////////////////
/*!
\section   边缘线数据类。
*/
CEdgeLine::CEdgeLine()
{
    m_width = 1;
    m_color = QColor(0,0,0,255);
    m_lineStyle = lsSolid;
}
void CEdgeLine::operator=(const CEdgeLine& data)
{
    this->m_color = data.m_color;
    this->m_lineStyle = data.m_lineStyle;
    this->m_width = data.m_width;
}
void CEdgeLine::setWidth(qint32 width)
{
    if (width >= 1 && width <= 10)
        m_width = width;
}
qint32 CEdgeLine::getWidth()
{
    return m_width;
}

void CEdgeLine::serialize(QJsonObject &json)
{
    json.insert("Color",(double)m_color.rgba());
    json.insert("Width",m_width);
    json.insert("LineStyle",(int)m_lineStyle);
}

void CEdgeLine::deserialize(const QJsonObject &json)
{
    m_color = QColor::fromRgba(json.value("Color").toDouble());
    m_width = json.value("Width").toInt();
    int line = json.value("LineStyle").toInt();
    m_lineStyle = (lineStyle)line;
}
//////////////////////////////////////////////////////////////////////////
/*!
\section
控制层数据类。
*/
CDataLayer::CDataLayer()
{
    m_rect = QRectF(3,3,94,94);
    m_startAngle = 30;
    m_angleRange = 300;
    m_minValue = 0;
    m_valueRange = 100;
    m_scaleCount = 11;
    m_index = 1;
    m_isHori = true;
}
CDataLayer::~CDataLayer()
{
    for (qint32 i = 0; i < m_item.count(); i++)
        delete m_item.at(i);
    m_item.clear();
}

drawType CDataLayer::getDrawType()
{
    return dtLayer;
}

void CDataLayer::operator=(const CDataLayer& data)
{
    CDataLayer* d = (CDataLayer*)&data;

    this->m_rect = d->m_rect;
    this->m_startAngle = d->m_startAngle;
    this->m_angleRange = d->m_angleRange;
    this->m_minValue = d->m_minValue;
    this->m_valueRange = d->m_valueRange;
    this->m_scaleCount = d->m_scaleCount;
    this->m_index = d->m_index;
    this->m_isHori = d->m_isHori;

    while(!this->m_item.isEmpty())
            qDeleteAll(m_item);
    CData* pData = NULL;
    CData* pNewData = NULL;
    qint32 count = d->m_item.count();

    for(qint32 i = count-1;i>=0;i--)
    {
          pData = d->m_item.at(i);
          pNewData = d->createData(pData->getDrawType());
          *pNewData = *pData;
          this->m_item.prepend(pNewData);
      }
}

void CDataLayer::operator=(const CData& data)
{
    CDataLayer* d = (CDataLayer*)&data;

    this->m_rect = d->m_rect;
    this->m_startAngle = d->m_startAngle;
    this->m_angleRange = d->m_angleRange;
    this->m_minValue = d->m_minValue;
    this->m_valueRange = d->m_valueRange;
    this->m_scaleCount = d->m_scaleCount;
    this->m_index = d->m_index;
    this->m_isHori = d->m_isHori;

    while(!this->m_item.isEmpty())
         qDeleteAll(m_item);

    CData* pData = NULL;
    CData* pNewData = NULL;
    qint32 count = d->m_item.count();
   for(qint32 i = count-1;i>=0;i--)
   {
        pData = d->m_item.at(i);
        pNewData = d->createData(pData->getDrawType());
        *pNewData = *pData;
        this->m_item.prepend(pNewData);
    }
}

void CDataLayer::serialize(QJsonObject &json)
{
    QJsonObject layerJson;
    layerJson.insert("Rect",CMDJsonConvert::toJson(m_rect));
    layerJson.insert("Index",m_index);
    layerJson.insert("IsHori",m_isHori);
    layerJson.insert("StartAngle",m_startAngle);
    layerJson.insert("AngleRange",m_angleRange);
    layerJson.insert("MinValue",m_minValue);
    layerJson.insert("ValueRange",m_valueRange);
    layerJson.insert("ScaleCount",m_scaleCount);
    json.insert("DataLayer",layerJson);
    QJsonArray array;
    foreach(CData* data,m_item)
    {
        QJsonObject myJson;
        drawType type = data->getDrawType();
        myJson.insert("Type",(int)type);
        data->serialize(myJson);
        array.append(myJson);
    }
    json.insert("Items", array);
}

void CDataLayer::deserialize(const QJsonObject &json)
{
    m_item.clear();
    QJsonObject layerJson = json.value("DataLayer").toObject();
    m_rect = CMDJsonConvert::toQRectF(layerJson.value("Rect"));
    m_index = layerJson.value("Index").toInt();
    m_isHori = layerJson.value("IsHori").toBool();
    m_startAngle = layerJson.value("StartAngle").toInt();
    m_angleRange = layerJson.value("AngleRange").toInt();
    m_minValue = layerJson.value("MinValue").toDouble();
    m_valueRange = layerJson.value("ValueRange").toDouble();
    m_scaleCount = layerJson.value("ScaleCount").toInt();

    QJsonArray array = json.value("Items").toArray();
    foreach(QJsonValue value,array)
    {
        QJsonObject myJson = value.toObject();
        int temp = myJson.value("Type").toInt();
        drawType type;
        type = (drawType)temp;
        CData* pData = NULL;
        pData = createData(type);
        if(pData == NULL)
        {
            return;
        }
        pData->deserialize(myJson);
        m_item.append(pData);
    }
}

void CDataLayer::setRectF(const QRectF& rect)
{
    m_rect = rect;
}
QRectF CDataLayer::getRectF()
{
    return m_rect;
}
void CDataLayer::setStartAngle(qint32 startAngle)
{
    if (startAngle >= 0 && startAngle <= 360)
        m_startAngle = startAngle;
}
qint32 CDataLayer::getStartAngle()
{
    return m_startAngle;
}
void CDataLayer::setAngleRange(qint32 angleRange)
{
    if (angleRange >= 0 && angleRange <= 360)
        m_angleRange = angleRange;
}
qint32 CDataLayer::getAngleRange()
{
    return m_angleRange;
}
void CDataLayer::setMinValue(double minValue)
{
    m_minValue = minValue;
//!   确保当前值在取值范围内
    CData* pData = NULL;
    qint32 count = m_item.count();
    for(qint32 i = 0;i<count;i++)
    {
        pData = m_item.at(i);
        drawType t = pData->getDrawType();

        if (t == dtNeedle || t == dtLed || t == dtRNeedle || t == dtRMarker || t == dtTemperature)
        {
            CDataValue* n = (CDataValue*)pData;
            if (n->m_value < m_minValue)
            {
                n->m_value = m_minValue;
                n->m_currentValue = m_minValue;
            }
        }
    }
}
double CDataLayer::getMinValue()
{
    return m_minValue;
}
void CDataLayer::setValueRange(double valueRange)
{
    if (valueRange > 0)
    {
       m_valueRange  = valueRange;
//!     确保当前值在取值范围内
       CData* pData = NULL;
       qint32 count = m_item.count();
       for(qint32 i = 0;i<count;i++)
       {
            pData = m_item.at(i);
            drawType t = pData->getDrawType();

            if (t == dtNeedle || t == dtLed || t == dtRNeedle || t == dtRMarker || t == dtTemperature)
            {
                CDataValue* n = (CDataValue*)pData;
                if (n->m_value > (m_minValue + valueRange))
                {
                    n->m_value = m_minValue + valueRange;
                    n->m_currentValue = m_minValue + valueRange;
                }
            }
        }
      }
}

double CDataLayer::getValueRange()
{
    return m_valueRange;
}

void CDataLayer::setScaleCount(qint32 scaleCount)
{
    if (scaleCount >= 2 && scaleCount <= 16)
        m_scaleCount = scaleCount;
}
qint32 CDataLayer::getScaleCount()
{
    return m_scaleCount;
}

void CDataLayer::setIndex(const int &index)
{
    m_index = index;
}

int CDataLayer::getIndex() const
{
   return m_index;
}
//////////////////////////////////////////////////////////////////
CData* CDataLayer::createData(drawType type)
{
    CData* d = NULL;

    switch(type)
    {
    case dtCap:
        d = (CData*)(new CDataCap());
        break;
    case dtNeedle:
        d = (CData*)(new CDataNeedle());
        break;
    case dtRange:
        d = (CData*)(new CDataRange());
        break;
    case dtScaleText:
        d = (CData*)(new CDataScaleText());
        break;
    case dtScale:
        d = (CData*)(new CDataScale());
        break;
    case dtScaleBack:
        d = (CData*)(new CDataScaleBack());
        break;
    case dtBack:
        d = (CData*)(new CDataBack());
        break;
    case dtLabel:
        d = (CData*)(new CDataLabel());
        break;
    case dtLed:
        d = (CData*)(new CDataLed());
        break;
    case dtLayer:
        d = (CData*)(new CDataLayer());
        break;
    case dtHalfBack:
        d = (CData*)(new CDataHalfBack());
        break;
    case dtTemperature:
        d = (CData*)(new CDataTemperature());
        break;
    case dtRNeedle:
        d = (CData*)(new CRulerNeedle());
        break;
    case dtRRange:
        d = (CData*)(new CRulerRange());
        break;
    case dtRScaleText:
        d = (CData*)(new CRulerScaleText());
        break;
    case dtRScale:
        d = (CData*)(new CRulerScale());
        break;
    case dtRScaleBack:
        d = (CData*)(new CRulerScaleBack());
        break;
    case dtRMarker:
        d = (CData*)(new CRulerMarker());
        break;

    default:
        d = NULL;
    }

    return d;
}
//////////////////////////////////////////////////////////////////////////
/*!
\section 绘图项中心点的数据类。
*/
CDataCap::CDataCap()
{
    m_radius = 6;
    m_style = 0;
    m_backColor.m_color1 = QColor(245, 245, 245);
    m_backColor.m_color2 = QColor(220, 220, 220);
    m_backColor.m_gradientStyle = gs4;
    m_edgeLine.m_color = QColor(0, 0, 0, 64);
}

drawType CDataCap::getDrawType()
{
    return dtCap;
}
void CDataCap::operator=(const CData& data)
{
    CDataCap* d = (CDataCap*)&data;

    this->m_style = d->m_style;
    this->m_radius = d->m_radius;
    this->m_backColor = d->m_backColor;
    this->m_edgeLine = d->m_edgeLine;
}
void CDataCap::setRadius(qreal radius)
{
    if (radius >= 0 && radius <= 36.0)
        m_radius = radius;
}
qreal CDataCap::getRadius()
{
    return m_radius;
}

void CDataCap::serialize(QJsonObject &json)
{
    QJsonObject capJson;
    capJson.insert("Style",m_style);
    capJson.insert("Radius",m_radius);
    m_edgeLine.serialize(capJson);
    m_backColor.serialize(capJson);
    json.insert("DataCap",capJson);
}

void CDataCap::deserialize(const QJsonObject &json)
{
    QJsonObject capJson = json.value("DataCap").toObject();
    m_style = capJson.value("Style").toInt();
    m_radius = capJson.value("Radius").toDouble();
    m_edgeLine.deserialize(capJson);
    m_backColor.deserialize(capJson);

}
//////////////////////////////////////////////////////////////////////////
/*!
\section 绘图项指针的数据类。
*/
CDataNeedle::CDataNeedle()
{
    m_width = 7;
    m_len = 38;
    m_radius = 37;
    m_step = 0;
    m_style = 0;
    m_index = 1;
    m_expression = "";
    m_value = 0;
    m_backColor.m_color1 = QColor(255,255,255);
    m_backColor.m_color2 = QColor(255,0,0);
    m_backColor.m_gradientStyle = gs1;
    m_edgeLine.m_color = QColor(0, 0, 0, 64);
    m_currentValue = m_value;
    m_currentStep = 0;
}

drawType CDataNeedle::getDrawType()
{
    return dtNeedle;
}
void CDataNeedle::operator=(const CData& data)
{
    CDataNeedle* d = (CDataNeedle*)&data;

    this->m_style = d->m_style;
    this->m_index = d->m_index;
    this->m_expression = d->m_expression;
    this->m_value = d->m_value;
    this->m_backColor = d->m_backColor;
    this->m_edgeLine = d->m_edgeLine;
    this->m_width = d->m_width;
    this->m_len = d->m_len;
    this->m_radius = d->m_radius;
    this->m_step = d->m_step;
    this->m_currentValue = d->m_currentValue;
    this->m_currentStep = d->m_currentStep;
}

void CDataNeedle::serialize(QJsonObject &json)
{
    QJsonObject myJson;
    myJson.insert("Style",m_style);
    myJson.insert("Index",m_index);
    myJson.insert("Expression",m_expression);
    myJson.insert("Width",m_width);
    myJson.insert("Len",m_len);
    myJson.insert("Radius",m_radius);
    myJson.insert("Step",m_step);
    QJsonObject edgeLineJson;
    m_edgeLine.serialize(edgeLineJson);
    myJson.insert("EdgeLine",edgeLineJson);
    m_backColor.serialize(myJson);
    json.insert("DataNeedle",myJson);

}

void CDataNeedle::deserialize(const QJsonObject &json)
{
    QJsonObject myJson = json.value("DataNeedle").toObject();
    m_style = myJson.value("Style").toInt();
    m_index = myJson.value("Index").toInt();
    m_expression = myJson.value("Expression").toString();
    m_width = myJson.value("Width").toDouble();
    m_len = myJson.value("Len").toDouble();
    m_radius = myJson.value("Radius").toDouble();
    m_step = myJson.value("Step").toInt();
    QJsonObject edgeLineJson = myJson.value("EdgeLine").toObject();
    m_edgeLine.deserialize(edgeLineJson);
    m_backColor.deserialize(myJson);
}


void CDataNeedle::setWidth(qreal width)		//! 1.0-25.0
{
    if (width >= 1 && width <= 25)
        m_width = width;
}
qreal CDataNeedle::getWidth()
{
    return m_width;
}
void CDataNeedle::setLen(qreal len)			//! 1.0-100.0
{
    if (len >= 1 && len <= 100)
        m_len = len;
}
qreal CDataNeedle::getLen()
{
    return m_len;
}
void CDataNeedle::setRadius(qreal radius)	//! -55.0-55.0
{
    if (radius >= -55.0 && radius <= 55.0)
        m_radius = radius;
}
qreal CDataNeedle::getRadius()
{
    return m_radius;
}
void CDataNeedle::setStep(qint32 step)			//! 0-10
{
    if (step >= 0 && step <= 10)
        m_step = step;
}
qint32 CDataNeedle::getStep()
{
    return m_step;
}
/////////////////////////////////////////////////////////////////
/*!
\section 绘图项进度条的数据类。
*/
CDataRange::CDataRange()
{
    m_radius = 25;
    m_width = 5;
    m_minValue = 70;
    m_valueRange = 30;
    m_arrangement = 0;
    m_edgeLine.m_lineStyle = lsNull;
    m_backColor.m_color1 = QColor(255,255,255);
    m_backColor.m_color2 = QColor(0, 255, 0);
    m_backColor.m_gradientStyle = gs2;
}

drawType CDataRange::getDrawType()
{
    return dtRange;
}
void CDataRange::operator=(const CData& data)
{
    CDataRange* d = (CDataRange*)&data;

    this->m_arrangement = d->m_arrangement;
    this->m_edgeLine = d->m_edgeLine;
    this->m_backColor = d->m_backColor;
    this->m_radius = d->m_radius;
    this->m_width = d->m_width;
    this->m_minValue = d->m_minValue;
    this->m_valueRange = d->m_valueRange;
}

void CDataRange::serialize(QJsonObject &json)
{
    QJsonObject rangeJson;
    rangeJson.insert("ArrangeMent",m_arrangement);
    rangeJson.insert("Radius",m_radius);
    rangeJson.insert("Width",m_width);
    rangeJson.insert("MinValue",m_minValue);
    rangeJson.insert("ValueRange",m_valueRange);
    QJsonObject edgeLineJson;
    m_edgeLine.serialize(edgeLineJson);
    rangeJson.insert("EdgeLine",edgeLineJson);
    m_backColor.serialize(rangeJson);
    json.insert("DataRange",rangeJson);
}

void CDataRange::deserialize(const QJsonObject &json)
{
    QJsonObject rangeJson = json.value("DataRange").toObject();
    m_arrangement = rangeJson.value("ArrangeMent").toInt();
    m_radius = rangeJson.value("Radius").toDouble();
    m_width = rangeJson.value("Width").toDouble();
    m_minValue = rangeJson.value("MinValue").toDouble();
    m_valueRange = rangeJson.value("ValueRange").toDouble();
    QJsonObject edgeLineJson = rangeJson.value("EdgeLine").toObject();
    m_edgeLine.deserialize(edgeLineJson);
    m_backColor.deserialize(rangeJson);

}

void CDataRange::setRadius(qreal radius)		//!  1.0-50.0
{
    if (radius >= 1.0 && radius <= 50.0)
        m_radius = radius;
}
qreal CDataRange::getRadius()
{
    return m_radius;
}
void CDataRange::setWidth(qreal width)		//!  1.0-25.0
{
    if (width >= 1.0 && width <= 25.0)
        m_width = width;
}
qreal CDataRange::getWidth()
{
    return m_width;
}
void CDataRange::setMinValue(double minValue)
{
    m_minValue = minValue;
}

double CDataRange::getMinValue()
{
    return m_minValue;
}
void CDataRange::setValueRange(double valueRange)   	//!  >0
{
    if (valueRange >= 0)
        m_valueRange = valueRange;
}
double CDataRange::getValueRange()
{
    return m_valueRange;
}

//////////////////////////////////////////////////////////////////////////
/*!
\section 绘图项刻度的数据类。
*/
CDataScale::CDataScale()
{
    m_width = 3;
    m_len = 6;
    m_radius = 37;
    m_subCount = 3;
    m_style = 0;
    m_arrangement = 1;
    m_isSubScale = false;
    m_backColor.m_color1 = QColor(65, 140, 240,128);
    m_backColor.m_color2 = QColor(0, 0, 0, 32);
    m_backColor.m_gradientStyle = gs4;
    m_edgeLine.m_color = QColor(0, 0, 0, 80);
}

drawType CDataScale::getDrawType()
{
    return dtScale;
}
void CDataScale::operator=(const CData& data)
{
    CDataScale* d = (CDataScale*)&data;

    this->m_style = d->m_style;
    this->m_arrangement = d->m_arrangement;
    this->m_isSubScale = d->m_isSubScale;
    this->m_backColor = d->m_backColor;
    this->m_edgeLine = d->m_edgeLine;
    this->m_width = d->m_width;
    this->m_len = d->m_len;
    this->m_radius = d->m_radius;
    this->m_subCount = d->m_subCount;
}

void CDataScale::serialize(QJsonObject &json)
{
    QJsonObject scaleJson;
    scaleJson.insert("Style",m_style);
    scaleJson.insert("ArrangeMent",m_arrangement);
    scaleJson.insert("IsSubScale",m_isSubScale);
    scaleJson.insert("Width",m_width);
    scaleJson.insert("Len",m_len);
    scaleJson.insert("Radius",m_radius);
    scaleJson.insert("SubCount",m_subCount);
    QJsonObject edgeLineJson;
    m_edgeLine.serialize(edgeLineJson);
    scaleJson.insert("EdgeLine",edgeLineJson);
    m_backColor.serialize(scaleJson);
    json.insert("DataScale",scaleJson);
}

void CDataScale::deserialize(const QJsonObject &json)
{
    QJsonObject scaleJson = json.value("DataScale").toObject();
    m_style = scaleJson.value("Style").toInt();
    m_arrangement = scaleJson.value("ArrangeMent").toInt();
    m_isSubScale = scaleJson.value("IsSubScale").toBool();
    m_width = scaleJson.value("Width").toDouble();
    m_len = scaleJson.value("Len").toDouble();
    m_radius = scaleJson.value("Radius").toDouble();
    m_subCount = scaleJson.value("SubCount").toInt();
    QJsonObject edgeLineJson = scaleJson.value("EdgeLine").toObject();
    m_edgeLine.deserialize(edgeLineJson);
    m_backColor.deserialize(scaleJson);

}

void CDataScale::setWidth(qreal width)			//!  1.0-25.0
{
    if (width >= 1 && width <= 25)
        m_width = width;
}
qreal CDataScale::getWidth()
{
    return m_width;
}
void CDataScale::setLen(qreal len)				//!  1.0-36.0
{
    if (len >= 1 && len <= 36)
        m_len = len;
}
qreal CDataScale::getLen()
{
    return m_len;
}
void CDataScale::setRadius(qreal radius)		//!  10.0-80.0
{
    if (radius >= 10 && radius <= 80)
        m_radius = radius;
}
qreal CDataScale::getRadius()
{
    return m_radius;
}
void CDataScale::setSubCount(qint32 subCount)		//!  1-10
{
    if (subCount >= 1 && subCount <= 10)
        m_subCount = subCount;
}
qint32 CDataScale::getSubCount()
{
    return m_subCount;
}

//////////////////////////////////////////////////////////////////////////
/*!
\section 绘图项刻度背景的数据类。
*/
CDataScaleBack::CDataScaleBack()
{
    m_width = 2;
    m_radius = 37;
    m_arrangement = 1;
    m_backColor.m_color1 = QColor(0, 0,0,45);
    m_backColor.m_color2 = QColor(0, 0,0,128);
    m_backColor.m_gradientStyle = gsSolid;
    m_edgeLine.m_lineStyle = lsNull;
}

drawType CDataScaleBack::getDrawType()
{
    return dtScaleBack;
}
void CDataScaleBack::operator=(const CData& data)
{
    CDataScaleBack* d = (CDataScaleBack*)&data;

    this->m_arrangement = d->m_arrangement;
    this->m_backColor = d->m_backColor;
    this->m_edgeLine = d->m_edgeLine;
    this->m_width = d->m_width;
    this->m_radius = d->m_radius;
}

void CDataScaleBack::serialize(QJsonObject &json)
{
    QJsonObject scaleBackJson;
    scaleBackJson.insert("ArrangeMent",m_arrangement);
    scaleBackJson.insert("Width",m_width);
    scaleBackJson.insert("Radius",m_radius);
    QJsonObject edgeLineJson;
    m_edgeLine.serialize(edgeLineJson);
    scaleBackJson.insert("EdgeLine",edgeLineJson);
    m_backColor.serialize(scaleBackJson);
    json.insert("Width",m_width);
    json.insert("DataScaleBack",scaleBackJson);
}

void CDataScaleBack::deserialize(const QJsonObject &json)
{
    QJsonObject scaleBackJson = json.value("DataScaleBack").toObject();
    m_arrangement = scaleBackJson.value("ArrangeMent").toInt();
    m_width = scaleBackJson.value("Width").toDouble();
    m_radius = scaleBackJson.value("Radius").toDouble();
    QJsonObject edgeLineJson = scaleBackJson.value("EdgeLine").toObject();
    m_edgeLine.deserialize(edgeLineJson);
    m_backColor.deserialize(scaleBackJson);
}
void CDataScaleBack::setWidth(qreal width)			//1.0-25.0
{
    if (width >= 1 && width <= 25)
        m_width = width;
}
qreal CDataScaleBack::getWidth()
{
    return m_width;
}
void CDataScaleBack::setRadius(qreal radius)		//1.0-50.0
{
    if (radius >= 1 && radius <= 50)
        m_radius = radius;
}
qreal CDataScaleBack::getRadius()
{
    return m_radius;
}
///////////////////////////////////////////////////////////////////////////////
/*!
\section 绘图项刻度文本的数据类。
*/
CDataScaleText::CDataScaleText()
{
    m_decimals = 0;
    m_radius = 33;
    m_size = 5;
    m_style = 0;
    m_name = "Tahoma";
    m_color = QColor(0,0,0);
    m_isBold = false;
}
drawType CDataScaleText::getDrawType()
{
    return dtScaleText;
}
void CDataScaleText::operator=(const CData& data)
{
    CDataScaleText* d = (CDataScaleText*)&data;

    this->m_style = d->m_style;
    this->m_name = d->m_name;
    this->m_color = d->m_color;
    this->m_isBold = d->m_isBold;
    this->m_decimals = d->m_decimals;
    this->m_radius = d->m_radius;
    this->m_size = d->m_size;
}

void CDataScaleText::serialize(QJsonObject &json)
{
    QJsonObject scaleTextJson;
    scaleTextJson.insert("Style",m_style);
    scaleTextJson.insert("Name",m_name);
    scaleTextJson.insert("Color",(double)m_color.rgba());
    scaleTextJson.insert("IsBold",m_isBold);
    scaleTextJson.insert("Decimals",m_decimals);
    scaleTextJson.insert("Radius",m_radius);
    scaleTextJson.insert("Size",m_size);
    json.insert("DataScaleText",scaleTextJson);

}

void CDataScaleText::deserialize(const QJsonObject &json)
{
    QJsonObject scaleTextJson = json.value("DataScaleText").toObject();
    m_style = scaleTextJson.value("Style").toInt();
    m_name = scaleTextJson.value("Name").toString();
    m_color = QColor::fromRgba(scaleTextJson.value("Color").toDouble());
    m_isBold = scaleTextJson.value("IsBold").toBool();
    m_decimals = scaleTextJson.value("Decimals").toInt();
    m_radius = scaleTextJson.value("Radius").toDouble();
    m_size = scaleTextJson.value("Size").toDouble();
}
void CDataScaleText::setDecimals(qint32 decimals)			//!  0-5
{
    if (decimals >= 0 && decimals <= 5)
        m_decimals = decimals;
}
qint32 CDataScaleText::getDecimals()
{
    return m_decimals;
}
void CDataScaleText::setRadius(qreal radius)			//!  1.0-80.0
{
    if (radius >= 10 && radius <= 80)
        m_radius = radius;
}
qreal CDataScaleText::getRadius()
{
    return m_radius;
}
void CDataScaleText::setSize(qreal size)				//!  5.0-20.0
{
    if (size >= 1.0 && size <= 20.0)
        m_size = size;
}
qreal CDataScaleText::getSize()
{
    return m_size;
}

////////////////////////////////////////////////////////////////////////////////////
/*!
\section 绘图项仪表背景的数据类。
*/
CDataBack::CDataBack()
{
    m_edgeWidth = 6.5;
    m_style = 2;
    m_backColor.m_color1 = QColor(255,255,255,255);
    m_backColor.m_color2 = QColor(255,255,255,255);
    m_backColor.m_gradientStyle = gs4;
    m_edgeColor.m_color1 = QColor(131, 181, 255,100);
    m_edgeColor.m_color2 = QColor(0, 81, 191,100);
    m_edgeColor.m_gradientStyle = gs4;
    m_edgeLine.m_lineStyle = lsNull;
}

drawType CDataBack::getDrawType()
{
    return dtBack;
}
void CDataBack::operator=(const CData& data)
{
    CDataBack* d = (CDataBack*)&data;
    this->m_style = d->m_style;
    this->m_edgeLine = d->m_edgeLine;
    this->m_backColor = d->m_backColor;
    this->m_edgeColor = d->m_edgeColor;
    this->m_edgeWidth = d->m_edgeWidth;
}

void CDataBack::serialize(QJsonObject &json)
{
    QJsonObject backJson;
    backJson.insert("Style",m_style);
    backJson.insert("EdgeWidth",m_edgeWidth);
    QJsonObject edgeLineJson;
    m_edgeLine.serialize(edgeLineJson);
    json.insert("EdgeLine",edgeLineJson);
    QJsonObject edgeColorJson;
    m_edgeColor.serialize(edgeColorJson);
    json.insert("EdgeColor",edgeColorJson);
    QJsonObject backColorJson;
    m_backColor.serialize(backColorJson);
    json.insert("BackColor",backColorJson);
    json.insert("DataBack",backJson);
}

void CDataBack::deserialize(const QJsonObject &json)
{
    QJsonObject backJson = json.value("DataBack").toObject();
    m_style = backJson.value("Style").toInt();
    m_edgeWidth = backJson.value("EdgeWidth").toDouble();
    QJsonObject edgeLineJson = json.value("EdgeLine").toObject();
    m_edgeLine.deserialize(edgeLineJson);
    QJsonObject edgeColorJson = json.value("EdgeColor").toObject();
    m_edgeColor.deserialize(edgeColorJson);
    QJsonObject backColorJson = json.value("BackColor").toObject();
    m_backColor.deserialize(backColorJson);
}
void CDataBack::setEdgeWidth(qreal edgeWidth)			//!  1.0-50.0
{
    if (edgeWidth >= 1 && edgeWidth <= 50)
        m_edgeWidth = edgeWidth;
}
qreal CDataBack::getEdgeWidth()
{
    return m_edgeWidth;
}
//////////////////////////////////////////////////////////////////////////////////
/*!
\section 绘图项半圆背景的数据类。
*/
CDataHalfBack::CDataHalfBack()
{
    m_style = 2;
    m_arrangement = 0;
}

drawType CDataHalfBack::getDrawType()
{
    return dtHalfBack;
}
void CDataHalfBack::operator=(const CData& data)
{
    CDataBack::operator =(data);

    CDataHalfBack* d = (CDataHalfBack*)&data;
    this->m_arrangement = d->m_arrangement;
}

void CDataHalfBack::serialize(QJsonObject &json)
{
    CDataBack::serialize(json);
    QJsonObject halfBackJson;
    halfBackJson.insert("ArrangeMent",m_arrangement);
    json.insert("HalfBack",halfBackJson);
}

void CDataHalfBack::deserialize(const QJsonObject &json)
{
    CDataBack::deserialize(json);
    QJsonObject halfBackJson = json.value("HalfBack").toObject();
    m_arrangement = halfBackJson.value("ArrangeMent").toInt();

}
//////////////////////////////////////////////////////////////////
/*!
\section 绘图项标签的数据类。
*/
CDataLabel::CDataLabel()
{
    m_size = 10;
    m_text = "Label";
    m_showText = "Label";
    m_pos = QPointF(50, 40);
    m_name = "Tahoma";
    m_color = QColor(0,0,0);
    m_isBold = false;
}
drawType CDataLabel::getDrawType()
{
    return dtLabel;
}
void CDataLabel::operator=(const CData& data)
{
    CDataLabel* d = (CDataLabel*)&data;

    this->m_pos = d->m_pos;
    this->m_name = d->m_name;
    this->m_color = d->m_color;
    this->m_isBold = d->m_isBold;
    this->m_text = d->m_text;
    this->m_showText = d->m_showText;
    this->m_size = d->m_size;
}

void CDataLabel::serialize(QJsonObject &json)
{
    QJsonObject labelJson;
    labelJson.insert("Pos",CMDJsonConvert::toJson(m_pos));
    labelJson.insert("Color",(double)m_color.rgba());
    labelJson.insert("Name",m_name);
    labelJson.insert("IsBold",m_isBold);
    labelJson.insert("Text",m_text);
    labelJson.insert("ShowText",m_showText);
    labelJson.insert("Size",m_size);
    json.insert("Label",labelJson);
}

void CDataLabel::deserialize(const QJsonObject &json)
{
    QJsonObject labelJson = json.value("Label").toObject();
    m_pos = CMDJsonConvert::toQPointF(labelJson.value("Pos"));
    m_color = QColor::fromRgba(labelJson.value("Color").toDouble());
    m_name = labelJson.value("Name").toString();
    m_isBold = labelJson.value("IsBold").toBool();
    m_text = labelJson.value("Text").toString();
    m_showText = labelJson.value("ShowText").toString();
    m_size = labelJson.value("Size").toDouble();

}

void CDataLabel::setSize(qreal size)				//!  5.0-20.0
{
    if (size >= 5 && size <= 20)
        m_size = size;
}
qreal CDataLabel::getSize()
{
    return m_size;
}
void CDataLabel::setString(QString str)
{
    m_text = str ;
}
QString CDataLabel::getString()
{
    return m_text;
}
//////////////////////////////////////////////////////////////////
/*!
\section 绘图项LED数字的数据类。
*/
CDataLed::CDataLed()
{
    m_decimals = 1;
    m_size = 15;
    m_pos = QPointF(50, 75);
    m_color = QColor(0,0,0);
    m_index = 1;
    m_expression = "";
    m_value = 0;
}

drawType CDataLed::getDrawType()
{
    return dtLed;
}
void CDataLed::operator=(const CData& data)
{
    CDataLed* d = (CDataLed*)&data;

    this->m_index = d->m_index;
    this->m_expression = d->m_expression;
    this->m_value = d->m_value;
    this->m_pos = d->m_pos;
    this->m_color = d->m_color;
    this->m_size = d->m_size;
    this->m_decimals = d->m_decimals;
}

void CDataLed::serialize(QJsonObject &json)
{
    QJsonObject ledJson;
    ledJson.insert("Pos",CMDJsonConvert::toJson(m_pos));
    ledJson.insert("Color",(double)m_color.rgba());
    ledJson.insert("Size",m_size);
    ledJson.insert("Decimals",m_decimals);
    ledJson.insert("Index",m_index);
    ledJson.insert("Expression",m_expression);
    json.insert("Led",ledJson);

}

void CDataLed::deserialize(const QJsonObject &json)
{
    QJsonObject ledJson = json.value("Led").toObject();
    m_pos = CMDJsonConvert::toQPointF(ledJson.value("Pos"));
    m_color = QColor::fromRgba(ledJson.value("Color").toDouble());
    m_size = ledJson.value("Size").toDouble();
    m_decimals = ledJson.value("Decimals").toInt();
    m_index = ledJson.value("Index").toInt();
    m_expression = ledJson.value("Expression").toString();

}
void CDataLed::setSize(qreal size)	 //!  5.0-20.0
{
    if (size >= 5 && size <= 20)
        m_size = size;
}
qreal CDataLed::getSize()
{
    return m_size;
}
void CDataLed::setDecimals(qint32 decimals)		//!  0-6
{
    if (decimals >= 0 && decimals <= 6)
        m_decimals = decimals;
}
qint32 CDataLed::getDecimals()
{
    return m_decimals;
}
void CDataLed::setStep(qint32 step)		//!  0-10
{
    m_index = step;
}
qint32 CDataLed::getStep()
{
    return m_index;
}
//////////////////////////////////////////////////////////////////////////
/*!
\section 绘图项温度计的数据类。
*/
CDataTemperature::CDataTemperature()
{
    m_width1 = 10;
    m_width2 = 20;
    m_height1 = 10;
    m_height2 = 5;
    m_offset = 0;
    m_step = 0;
    m_arrangement = 1;
    m_index = 1;
    m_currentStep = 0;
    m_value = 0;
    m_currentValue = 0;
    m_edgeLine.m_color = QColor(0, 0, 0, 64);
    m_edgeLine.m_lineStyle = lsSolid;
    m_edgeLine.setWidth(1);
    m_temColor.m_color1 = QColor(255, 255, 255, 255);
    m_temColor.m_color2 = QColor(255, 0, 0,255);
    m_temColor.m_gradientStyle = gs4;
    m_backColor.m_color1 = QColor(255, 255, 255,100);
    m_backColor.m_color2 = QColor(80, 80, 80,128);
    m_backColor.m_gradientStyle = gs1;
}

drawType CDataTemperature::getDrawType()
{
    return dtTemperature;
}
void CDataTemperature::operator=(const CData& data)
{
    CDataTemperature* d = (CDataTemperature*)&data;

    this->m_arrangement = d->m_arrangement;
    this->m_index = d->m_index;
    this->m_expression = d->m_expression;
    this->m_value = d->m_value;
    this->m_backColor = d->m_backColor;
    this->m_edgeLine = d->m_edgeLine;
    this->m_temColor = d->m_temColor;
    this->m_width1 = d->m_width1;
    this->m_width2 = d->m_width2;
    this->m_height1 = d->m_height1;
    this->m_height2 = d->m_height2;
    this->m_offset = d->m_offset;
    this->m_step = d->m_step;
    this->m_currentValue = d->m_currentValue;
    this->m_currentStep = d->m_currentStep;
}

void CDataTemperature::serialize(QJsonObject &json)
{
    QJsonObject temperatureJson;
    temperatureJson.insert("ArrangeMent",m_arrangement);
    temperatureJson.insert("Index",m_index);
    temperatureJson.insert("Expression",m_expression);
    temperatureJson.insert("Width1",m_width1);
    temperatureJson.insert("Width2",m_width2);
    temperatureJson.insert("Height1",m_height1);
    temperatureJson.insert("Height2",m_height2);
    temperatureJson.insert("Offset",m_offset);
    temperatureJson.insert("Step",m_step);
    m_edgeLine.serialize(temperatureJson);
    QJsonObject backColorJson;
    m_backColor.serialize(backColorJson);
    json.insert("BackColor",backColorJson);
    QJsonObject temColorJson;
    m_temColor.serialize(temColorJson);
    json.insert("TemColor",temColorJson);
//    m_backColor.serialize(temperatureJson);
//    m_temColor.serialize(temperatureJson);
    json.insert("Temperature",temperatureJson);
}

void CDataTemperature::deserialize(const QJsonObject &json)
{
    QJsonObject temJson = json.value("Temperature").toObject();
    m_arrangement = temJson.value("ArrangeMent").toInt();
    m_index = temJson.value("Index").toInt();
    m_expression = temJson.value("Expression").toString();
    m_width1 = temJson.value("Width1").toDouble();
    m_width2 = temJson.value("Width2").toDouble();
    m_height1 = temJson.value("Height1").toDouble();
    m_height2 = temJson.value("Height2").toDouble();
    m_offset = temJson.value("Offset").toDouble();
    m_step = temJson.value("Step").toInt();
    m_edgeLine.deserialize(temJson);
//    m_backColor.deserialize(temJson);
//    m_temColor.deserialize(temJson);
    QJsonObject backColorJson = json.value("BackColor").toObject();
    m_backColor.deserialize(backColorJson);
    QJsonObject temColorJson = json.value("TemColor").toObject();
    m_temColor.deserialize(temColorJson);
}
void CDataTemperature::setWidth1(double width)		//!  1.0-50.0
{
    if (width >= 1 && width <= 50)
        m_width1 = width;
}
double CDataTemperature::getWidth1()
{
    return m_width1;
}
void CDataTemperature::setWidth2(double width)		//!  1.0-50.0
{
    if (width >= 1 && width <= 50)
        m_width2 = width;
}
double CDataTemperature::getWidth2()
{
    return m_width2;
}
void CDataTemperature::setHeight1(double height)		//!  0-30.0
{
    if (height >= 0 && height <= 30)
        m_height1 = height;
}
double CDataTemperature::getHeight1()
{
    return m_height1;
}
void CDataTemperature::setHeight2(double height)		//!  0-30.0
{
    if (height >= 0 && height <= 30)
        m_height2 = height;
}
double CDataTemperature::getHeight2()
{
    return m_height2;
}
void CDataTemperature::setOffset(double offset)		//!  -55.0-55.0
{
    if (offset >= -55 && offset <= 55)
        m_offset = offset;
}
double CDataTemperature::getOffset()
{
    return m_offset;
}
void CDataTemperature::setStep(qint32 step)				//!  0-10
{
    if (step >= 0 && step <= 10)
        m_step = step;
}
qint32 CDataTemperature::getStep()
{
    return m_step;
}


/*!
\section 直表组件的数据类。
*/
////////////////////////////////////////////////////////////////////////
/*!
\section 绘图项直表刻度背景的数据类。
*/
CRulerScaleBack::CRulerScaleBack()
{
    m_offset = 0;
    m_arrangement = 1;
    setWidth(19.7f);
    m_edgeLine.m_lineStyle = lsNull;
    m_backColor.m_color1 = QColor(220, 220, 220, 255);
    m_backColor.m_color2 = QColor(0,85, 198, 255);
    m_backColor.m_gradientStyle = gsSolid;
}

void CRulerScaleBack::serialize(QJsonObject &json)
{
    CDataScaleBack::serialize(json);
    QJsonObject rulerScaleBack;
    rulerScaleBack.insert("Offset",m_offset);
    json.insert("RulerScaleBack",rulerScaleBack);
}

void CRulerScaleBack::deserialize(const QJsonObject &json)
{
    CDataScaleBack::deserialize(json);
    QJsonObject RSBackJson = json.value("RulerScaleBack").toObject();
    m_offset = RSBackJson.value("Offset").toDouble();

}

drawType CRulerScaleBack::getDrawType()
{
    return dtRScaleBack;
}
void CRulerScaleBack::operator=(const CData& data)
{
    CDataScaleBack::operator =(data);

    CRulerScaleBack* d = (CRulerScaleBack*)&data;
    this->m_offset = d->m_offset;
}
void CRulerScaleBack::setOffset(double offset)			//-55.0-55.0
{
    if (offset >= -55.0 && offset <= 55.0)
        m_offset = offset;
}
double CRulerScaleBack::getOffset()
{
    return m_offset;
}
//////////////////////////////////////////////////////////////////////////
/*!
\section 绘图项直表刻度的数据类。
*/
CRulerScale::CRulerScale()
{
    m_offset = 0;
    m_style = 0;
    m_arrangement = 1;
    setWidth(3.6f);
    setLen(16.8f);
    m_isSubScale = false;
    setSubCount(9);
    m_edgeLine.m_lineStyle = lsNull;
    m_backColor.m_color1 = QColor(140, 174, 231, 160);
    m_backColor.m_color2 = QColor(140, 166, 214, 160);
    m_backColor.m_gradientStyle = gs6;
}

void CRulerScale::serialize(QJsonObject &json)
{
    CDataScale::serialize(json);
    QJsonObject rulerScaleJson;
    rulerScaleJson.insert("Offset",m_offset);
    json.insert("RulerScale",rulerScaleJson);

}

void CRulerScale::deserialize(const QJsonObject &json)
{
    CDataScale::deserialize(json);
    QJsonObject RSCaleJson;
    RSCaleJson = json.value("RulerScale").toObject();
    m_offset = RSCaleJson.value("Offset").toDouble();

}

drawType CRulerScale::getDrawType()
{
    return dtRScale;
}
void CRulerScale::operator=(const CData& data)
{
    CDataScale::operator =(data);

    CRulerScale* d = (CRulerScale*)&data;
    this->m_offset = d->m_offset;
}
void CRulerScale::setOffset(double offset)			//!  -55.0-55.0
{
    if (offset >= -55.0 && offset <= 55.0)
        m_offset = offset;
}
double CRulerScale::getOffset()
{
    return m_offset;
}
//////////////////////////////////////////////////////////////////////////
/*!
\section 绘图项直表刻度文字的数据类。
*/
CRulerScaleText::CRulerScaleText()
{
    m_offset = -13.8;
    m_arrangement = 2;
    m_color = QColor(33, 60, 107, 255);
    setSize(10);
}

void CRulerScaleText::serialize(QJsonObject &json)
{
    CDataScaleText::serialize(json);
    QJsonObject rulerScaleText;
    rulerScaleText.insert("ArrangeMent",m_arrangement);
    rulerScaleText.insert("Offset",m_offset);
    json.insert("RulerScaleText",rulerScaleText);

}

void CRulerScaleText::deserialize(const QJsonObject &json)
{
    CDataScaleText::deserialize(json);
    QJsonObject RSText = json.value("RulerScaleText").toObject();
    m_arrangement = RSText.value("ArrangeMent").toInt();
    m_offset = RSText.value("Offset").toDouble();
}

drawType CRulerScaleText::getDrawType()
{
    return dtRScaleText;
}
void CRulerScaleText::operator=(const CData& data)
{
    CDataScaleText::operator =(data);

    CRulerScaleText* d = (CRulerScaleText*)&data;
    this->m_arrangement = d->m_arrangement;
    this->m_offset = d->m_offset;
}
void CRulerScaleText::setOffset(double offset)			//!   -55.0-55.0
{
    if (offset >= -55.0 && offset <= 55.0)
        m_offset = offset;
}
double CRulerScaleText::getOffset()
{
    return m_offset;
}
///////////////////////////////////////////////////////////////////////////////
/*!
\section 绘图项直表临界区的数据类。
*/
CRulerRange::CRulerRange()
{
    m_offset = 12.5;
    m_minPercent = 20;
    m_percentRange = 80;
    m_style = 0;
    m_arrangement = 0;
    setWidth(10.2);
    m_edgeLine.m_lineStyle = lsNull;
    m_backColor.m_color1 = QColor(255, 255, 255, 255);
    m_backColor.m_color2 = QColor(0, 255, 0, 255);
    m_backColor.m_gradientStyle = gs1;
}

void CRulerRange::serialize(QJsonObject &json)
{
    CDataRange::serialize(json);
    QJsonObject rulerRangeJson;
    rulerRangeJson.insert("Style",m_style);
    rulerRangeJson.insert("MinPercent",m_minPercent);
    rulerRangeJson.insert("PercentRange",m_percentRange);
    json.insert("RulerRange",rulerRangeJson);
}

void CRulerRange::deserialize(const QJsonObject &json)
{
    CDataRange::deserialize(json);
    QJsonObject rulerJson = json.value("RulerRange").toObject();
    m_style = rulerJson.value("Style").toInt();
    m_minPercent = rulerJson.value("MinPercent").toDouble();
    m_percentRange = rulerJson.value("PercentRange").toDouble();

}

drawType CRulerRange::getDrawType()
{
    return dtRRange;
}
void CRulerRange::operator=(const CData& data)
{
    CDataRange::operator =(data);

    CRulerRange* d = (CRulerRange*)&data;
    this->m_style = d->m_style;
    this->m_offset = d->m_offset;
    this->m_minPercent = d->m_minPercent;
    this->m_percentRange = d->m_percentRange;
}
void CRulerRange::setOffset(double offset)			//!  -55.0-55.0
{
    if (offset >= -55.0 && offset <= 55.0)
        m_offset = offset;
}
double CRulerRange::getOffset()
{
    return m_offset;
}
void CRulerRange::setMinPercent(double min)			//!  0.0-100.0
{
    if (min >= 0 && min <= 100)
        m_minPercent = min;
}
double CRulerRange::getMinPercent()
{
    return m_minPercent;
}
void CRulerRange::setPercentRange(double range)		//!  0.0-100.0
{
    if (range >= 0 && range <= 100)
        m_percentRange = range;
}
double CRulerRange::getPercentRange()
{
    return m_percentRange;
}
//////////////////////////////////////////////////////////////////////////
/*!
\section 绘图项直表指针的数据类。
*/
CRulerNeedle::CRulerNeedle()
{
    m_offset = 0;
    m_arrangement = 1;
    m_value = 100;
    setWidth(8.8f);
    m_edgeLine.m_lineStyle = lsNull;
    m_backColor.m_color1 = QColor(255, 255, 255, 100);
    m_backColor.m_color2 = QColor(255, 25, 25, 255);
    m_backColor.m_gradientStyle = gs4;

}

void CRulerNeedle::serialize(QJsonObject &json)
{
    CDataNeedle::serialize(json);
    QJsonObject rulerNeedleJson;
    rulerNeedleJson.insert("ArrangeMent",m_arrangement);
    rulerNeedleJson.insert("Offset",m_offset);
    json.insert("RulerNeedle",rulerNeedleJson);
}

void CRulerNeedle::deserialize(const QJsonObject &json)
{
    CDataNeedle::deserialize(json);
    QJsonObject rulerNeedleJson = json.value("RulerNeedle").toObject();
    m_arrangement = rulerNeedleJson.value("ArrangeMent").toInt();
    m_offset = rulerNeedleJson.value("Offset").toDouble();

}

drawType CRulerNeedle::getDrawType()
{
    return dtRNeedle;
}
void CRulerNeedle::operator=(const CData& data)
{
    CDataNeedle::operator =(data);

    CRulerNeedle* d = (CRulerNeedle*)&data;
    this->m_arrangement = d->m_arrangement;
    this->m_offset = d->m_offset;
}
void CRulerNeedle::setOffset(double offset)			//!   -55.0-55.0
{
    if (offset >= -55.0 && offset <= 55.0)
        m_offset = offset;
}
double CRulerNeedle::getOffset()
{
    return m_offset;
}
//////////////////////////////////////////////////////////////////////////
/*!
\section 绘图项直表指示块的数据类。
*/
CRulerMarker::CRulerMarker()
{
    m_style = 1;
    m_arrangement = 1;
    m_value = 0;
    setWidth(10);
    setLen(10);
    m_edgeLine.m_lineStyle = lsNull;
    m_backColor.m_color1 = QColor(255, 255, 255, 255);
    m_backColor.m_color2 = QColor(255, 0, 0, 255);
    m_backColor.m_gradientStyle = gs4;
}
drawType CRulerMarker::getDrawType()
{
    return dtRMarker;
}
//////////////////////////////////////////////////////////////////////////



