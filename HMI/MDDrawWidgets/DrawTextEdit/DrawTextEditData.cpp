#include "DrawTextEditData.h"
//////////////////////////////////////////////////////////////////////////


CDrawTextEditData::CDrawTextEditData()
{
    m_textEdit = NULL;
    m_event = NULL;
    m_horizontalScrollBar = false;
    m_verticalScrollBar = false;
    m_borderStyle = QFrame::Shape::NoFrame;
    m_lineWrapMode = QTextEdit::LineWrapMode::WidgetWidth;
    m_readOnly = false;
    m_alignment = Qt::AlignLeft;
    m_selectStart = 0;
    m_selectLength = 4;
    m_selectText = "";
    m_text = "编辑框";
    m_VarMaxLength = 50;
    m_decimal = 0;//! 小数位数
    m_dateFormat = "yyyy-MM-dd hh:mm:ss";//! 日期格式
    m_writeVarEnable = true;
    m_enterSetVarEnable = true;
    m_hasPassword = false;
}

QTextEdit *CDrawTextEditData::textEdit() const
{
    return m_textEdit;
}

void CDrawTextEditData::setTextEdit(QTextEdit *textEdit)
{
    m_textEdit = textEdit;
}

CMDDrawTexEditEvent *CDrawTextEditData::event() const
{
    return m_event;
}

void CDrawTextEditData::setEvent(CMDDrawTexEditEvent *event)
{
    m_event = event;
}

/*!
\section  序列化
*/
void CDrawTextEditData::operator=(const CDrawTextEditData &other)
{
    m_text = other.m_text;
    m_readOnly = other.m_readOnly;
    m_horizontalScrollBar = other.m_horizontalScrollBar;
    m_verticalScrollBar = other.m_verticalScrollBar;
    m_alignment = other.m_alignment;
    m_borderStyle = other.m_borderStyle;
    m_lineWrapMode = other.m_lineWrapMode;
    m_decimal = other.m_decimal;
    m_dateFormat = other.m_dateFormat;
    m_writeVarEnable = other.m_writeVarEnable;
    m_enterSetVarEnable = other.m_enterSetVarEnable;
    m_hasPassword = other.m_hasPassword;
}

void CDrawTextEditData::serialize(QJsonObject &json)
{
    json.insert("Text",m_text);
    json.insert("ReadOnly",m_readOnly);
    json.insert("Alignment",(int)m_alignment);
    json.insert("BorderStyle",m_borderStyle);
    json.insert("LineWrapMode",m_lineWrapMode);
    json.insert("HorizontalScrollBar",m_horizontalScrollBar);
    json.insert("VerticalScrollBar",m_verticalScrollBar);
    json.insert("VarMaxLength",m_VarMaxLength);
    json.insert("Decimal",m_decimal);
    json.insert("DateFormat",m_dateFormat);
    json.insert("WriteVarEnable",m_writeVarEnable);
    json.insert("EnterSetVarEnable",m_enterSetVarEnable);
    json.insert("HasPassword",m_hasPassword);
}

void CDrawTextEditData::deserialize(const QJsonObject &json)
{
    m_text = json.value("Text").toString();
    m_readOnly = json.value("ReadOnly").toBool();
    m_alignment = (Qt::Alignment)json.value("Alignment").toInt();
    m_borderStyle = (QFrame::Shape)json.value("BorderStyle").toInt();
    m_lineWrapMode = (QTextEdit::LineWrapMode)json.value("LineWrapMode").toInt();
    m_horizontalScrollBar = json.value("HorizontalScrollBar").toBool();
    m_verticalScrollBar = json.value("VerticalScrollBar").toBool();
    m_VarMaxLength = json.value("VarMaxLength").toInt();
    m_decimal = json.value("Decimal").toInt();
    m_dateFormat = json.value("DateFormat").toString();
    m_writeVarEnable = json.value("WriteVarEnable").toBool();
    m_enterSetVarEnable = json.value("EnterSetVarEnable").toBool();
    m_hasPassword = json.value("HasPassword").toBool();
}
