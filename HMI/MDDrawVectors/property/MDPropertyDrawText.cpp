#include <QDebug>
#include "MDPropertyDrawText.h"
#include "DrawText/FormatDialog.h"
#include "DrawTextCommand.h"

CMDPropertyDrawText::CMDPropertyDrawText(CMDDrawObj *parent) :
    CMDPropertyDrawVector(parent)
{
    insertTranslateMap("textBrush", tr("字体颜色"));
    insertTranslateMap("font", tr("字体"));
    insertTranslateMap("hAlignment", tr("水平对齐"));
    insertTranslateMap("vAlignment", tr("垂直对齐"));
    insertTranslateMap("lineWrap", tr("换行"));
    insertTranslateMap("buttonFrame", tr("按钮边框"));
    insertTranslateMap("text", tr("文本"));
    insertTranslateMap("textFormat", tr("文本格式"));

    insertTranslateMap("Left", tr("左"));
    insertTranslateMap("Center", tr("中"));
    insertTranslateMap("Right", tr("右"));
    insertTranslateMap("Top", tr("上"));
    insertTranslateMap("Middle", tr("中"));
    insertTranslateMap("Bottom", tr("下"));
}
//////////////////////////////////////////////////////////////////////////
void CMDPropertyDrawText::setText(const QString &value)
{
    if(isBatch())
        setBatchText(value);
    else
        undo()->push(new CDrawTextCommand(
            object(), object()->text(), QVariant(value), (int)DrawTextId::Text));
    setModify(true);
}
QString CMDPropertyDrawText::text()
{
    return object()->text();
}
void CMDPropertyDrawText::setTextBrush(const QString &/*value*/)
{
    CMDBrushManager* oldValue = new CMDBrushManager;
    *oldValue = *object()->textBrush();

    if (object()->textBrush()->showDialog())
    {
        CMDBrushManager* newValue = new CMDBrushManager;
        *newValue = *object()->textBrush();

        if(isBatch())
        {
            *object()->textBrush() = *oldValue;
            setBatchTextBrush(newValue);
            delete oldValue;
            delete newValue;
        }
        else
        {
            undo()->push(new CDrawTextCommand(
                object(), (qulonglong)oldValue, (qulonglong)newValue, (int)DrawTextId::TextBrush));
            object()->invalidate();
        }

        setModify(true);
    }
    else
        delete oldValue;
}
QString CMDPropertyDrawText::textBrush()
{
    return object()->textBrush()->text();
}
void CMDPropertyDrawText::setFont(const QFont &value)
{
    if(isBatch())
        setBatchFont(value);
    else
    {
        QFont* oldValue = new QFont;
        *oldValue = *object()->font();
        QFont* newValue = new QFont;
        *newValue = value;

        undo()->push(new CDrawTextCommand(
            object(), (qulonglong)oldValue, (qulonglong)newValue, (int)DrawTextId::Font));
    }

    setModify(true);
}
QFont CMDPropertyDrawText::font()
{
    return *object()->font();
}
void CMDPropertyDrawText::setHAlignment(CMDPropertyDrawText::PropertyHAlignment value)
{
    if(isBatch())
        setBatchHAlignment(value);
    else
        undo()->push(new CDrawTextCommand(
            object(), (int)object()->hAlignment(), (int)value, (int)DrawTextId::HAlignment));

    setModify(true);
}
CMDPropertyDrawText::PropertyHAlignment CMDPropertyDrawText::hAlignment()
{
    return (CMDPropertyDrawText::PropertyHAlignment)((int)object()->hAlignment());
}
void CMDPropertyDrawText::setVAlignment(CMDPropertyDrawText::PropertyVAlignment value)
{
    if(isBatch())
        setBatchVAlignment(value);
    else
        undo()->push(new CDrawTextCommand(
            object(), (int)object()->vAlignment(), (int)value, (int)DrawTextId::VAlignment));
    setModify(true);
}
CMDPropertyDrawText::PropertyVAlignment CMDPropertyDrawText::vAlignment()
{
    return (CMDPropertyDrawText::PropertyVAlignment)((int)object()->vAlignment());
}
void CMDPropertyDrawText::setLineWrap(bool value)
{
    if(isBatch())
        setBatchLineWrap(value);
    else
        undo()->push(new CDrawTextCommand(
            object(), object()->isWrap(), value, (int)DrawTextId::LineWrap));
    setModify(true);
}
bool CMDPropertyDrawText::lineWrap()
{
    return object()->isWrap();
}
void CMDPropertyDrawText::setTextFormat(const QString &value)
{
    CFormatDialog dlg(container());
    dlg.init(object());
    if (dlg.exec() == QDialog::Accepted)
    {
        QJsonObject* v = dlg.value();

        if(isBatch())
            setBatchTextFormat(v);
        else
        {
            object()->setDecimal(v->value("Decimal").toInt());
            object()->setDateFormat(v->value("DateFormat").toString());
            object()->setMaxLength(v->value("MaxLength").toInt());
        }

        setModify(true);
    }
}
QString CMDPropertyDrawText::textFormat()
{
    return object()->textFormatString();
}
//////////////////////////////////////////////////////////////////////////
CMDDrawText *CMDPropertyDrawText::object()
{
    return dynamic_cast<CMDDrawText*>(m_object);
}
//////////////////////////////////////////////////////////////////////////
void CMDPropertyDrawText::setBatchText(const QString &value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawText* v = dynamic_cast<CMDDrawText*>(o);
        undo()->push(new CDrawTextCommand(
            v, v->text(), value, (int)DrawTextId::Text));
    }

    undo()->endMacro();
}
void CMDPropertyDrawText::setBatchTextBrush(CMDBrushManager *value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawText* v = dynamic_cast<CMDDrawText*>(o);

        CMDBrushManager* oldValue = new CMDBrushManager;
        *oldValue = *v->textBrush();
        CMDBrushManager* newValue = new CMDBrushManager;
        *newValue =*value;

        undo()->push(new CDrawTextCommand(
            v, (qulonglong)oldValue, (qulonglong)newValue, (int)DrawTextId::TextBrush));
        v->invalidate();
    }

    undo()->endMacro();
}
void CMDPropertyDrawText::setBatchFont(const QFont &value)
{
    undo()->beginMacro("");

    FontType type = getFontChange(value, *object()->font());
    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawText* v = dynamic_cast<CMDDrawText*>(o);
        QFont f = *(v->font());
        switch(type)
        {
        case FontType::Family:
            f.setFamily(value.family());
            break;
        case FontType::PointSize:
            f.setPointSize(value.pointSize());
            break;
        case FontType::Bold:
            f.setBold(value.bold());
            break;
        case FontType::Italic:
            f.setItalic(value.italic());
            break;
        case FontType::Underline:
            f.setUnderline(value.underline());
            break;
        case FontType::Strikeout:
            f.setStrikeOut(value.strikeOut());
            break;
        case FontType::Kerning:
            f.setKerning(value.kerning());
            break;
        case FontType::All:
            f = value;
            break;
        default:
            break;
        }

        QFont* oldValue = new QFont;
        *oldValue = *v->font();
        QFont* newValue = new QFont;
        *newValue = f;

        undo()->push(new CDrawTextCommand(
            v, (qulonglong)oldValue, (qulonglong)newValue, (int)DrawTextId::Font));
    }

    undo()->endMacro();
}
void CMDPropertyDrawText::setBatchHAlignment(CMDPropertyDrawText::PropertyHAlignment value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawText* v = dynamic_cast<CMDDrawText*>(o);
        undo()->push(new CDrawTextCommand(
            v, (int)v->hAlignment(), (int)value, (int)DrawTextId::HAlignment));
    }

    undo()->endMacro();
}
void CMDPropertyDrawText::setBatchVAlignment(CMDPropertyDrawText::PropertyVAlignment value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawText* v = dynamic_cast<CMDDrawText*>(o);
        undo()->push(new CDrawTextCommand(
            v, (int)v->vAlignment(), (int)value, (int)DrawTextId::VAlignment));
    }

    undo()->endMacro();
}
void CMDPropertyDrawText::setBatchLineWrap(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawText* v = dynamic_cast<CMDDrawText*>(o);
        undo()->push(new CDrawTextCommand(
            v, v->isWrap(), value, (int)DrawTextId::LineWrap));
    }

    undo()->endMacro();
}
void CMDPropertyDrawText::setBatchTextFormat(QJsonObject *value)
{
    int decimal = value->value("Decimal").toInt();
    QString dateFormat = value->value("DateFormat").toString();
    int maxLength = value->value("MaxLength").toInt();

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawText* v = dynamic_cast<CMDDrawText*>(o);
        v->setDecimal(decimal);
        v->setDateFormat(dateFormat);
        v->setMaxLength(maxLength);
    }
}
//////////////////////////////////////////////////////////////////////////
