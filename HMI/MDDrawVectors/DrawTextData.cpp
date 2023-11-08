#include <QJsonObject>
#include "DrawTextData.h"
#include "MDColorManager/MDPenManager.h"
#include "MDJsonConvert.h"
//////////////////////////////////////////////////////////////////////////
CDrawTextData::CDrawTextData()
{

    m_text = "Text";   //!文本

    m_alignment = Qt::AlignLeft | Qt::AlignTop;//!布局
    m_wrapMode = QTextOption::WrapAnywhere;//!换行
    m_isButtonFrame = false;//!是否绘制按钮边框
    m_isDownFrame = false;//!按钮边框是否是按下状态
    m_decimal = 0;//!小数位数
    m_dateFormat = "yyyy-MM-dd hh:mm:ss";//!日期格式
    m_maxLength = 50;
}

void CDrawTextData::operator=(const CDrawTextData &other)
{
    if (this == &other)
        return;

    m_font = other.m_font;
    m_text = other.m_text;
    m_textBrushManager = other.m_textBrushManager;
    m_option = other.m_option;
    m_alignment = other.m_alignment;
    m_wrapMode = other.m_wrapMode;
    m_isButtonFrame = other.m_isButtonFrame;
    m_decimal = other.m_decimal;
    m_dateFormat = other.m_dateFormat;
    m_maxLength = other.m_maxLength;
}
void CDrawTextData::serialize(QJsonObject &json)
{
   json.insert("Font", CMDJsonConvert::toJson(m_font));
   json.insert("Text", m_text);
   json.insert("Alignment", (int)m_alignment);
   json.insert("WrapMode", m_wrapMode);
   json.insert("IsButtonFrame", m_isButtonFrame);
   json.insert("Decimal", m_decimal);
   json.insert("DateFormat", m_dateFormat);
   json.insert("MaxLength", m_maxLength);

   QJsonObject jsBrush;
   m_textBrushManager.serialize(jsBrush);
   json.insert("TextBrushManager", jsBrush);
}
void CDrawTextData::deserialize(const QJsonObject &json)
{
    m_font = CMDJsonConvert::toQFont(json.value("Font"));
    m_text = json.value("Text").toString();
    m_alignment = (Qt::Alignment)json.value("Alignment").toInt();
    m_wrapMode = (QTextOption::WrapMode)json.value("WrapMode").toInt();
    m_isButtonFrame = json.value("IsButtonFrame").toBool();
    m_decimal = json.value("Decimal").toInt();
    m_dateFormat = json.value("DateFormat").toString();
    m_maxLength = json.value("MaxLength").toInt();

    m_textBrushManager.deserialize(json.value("TextBrushManager").toObject());
}
//////////////////////////////////////////////////////////////////////////
