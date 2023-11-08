#include "MDPropertyDrawBasicWidget.h"
#include "MDDrawBasicWidget.h"
#include "DrawBasicWidgetData.h"
#include "MDRTDBManagerUI/IMDRTDBManagerUI.h"
#include "IDrawEnvironment.h"
#include "undo/DrawBasicWidgetCommand.h"

void CMDPropertyDrawBasicWidget::setEnable(bool value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchEnable(value);
    else
        undo()->push(new CDrawBasicWidgetCommand(
            widget(), widget()->enable(), value, (int)DrawBasicWidgetId::Enable));

    setModify(true);
}

bool CMDPropertyDrawBasicWidget::enable()
{
    if(widget())
        return widget()->enable();
    return true;
}

void CMDPropertyDrawBasicWidget::setTextColor(const QColor &value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchTextColor(value);
    else
        undo()->push(new CDrawBasicWidgetCommand(
            widget(), widget()->textColor(), value, (int)DrawBasicWidgetId::TextColor));

    setModify(true);
}

QColor CMDPropertyDrawBasicWidget::textColor()
{
    if(widget())
        return widget()->textColor();
    return QColor();
}

void CMDPropertyDrawBasicWidget::setBackColor(const QColor &value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchBackColor(value);
    else
        undo()->push(new CDrawBasicWidgetCommand(
            widget(), widget()->backColor(), value, (int)DrawBasicWidgetId::BackColor));

    setModify(true);
}

QColor CMDPropertyDrawBasicWidget::backColor()
{
    if(widget())
        return widget()->backColor();
    return QColor();
}

void CMDPropertyDrawBasicWidget::setDisableBackColor(const QColor &value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchDisableBackColor(value);
    else
        undo()->push(new CDrawBasicWidgetCommand(
            widget(), widget()->disableBackColor(), value, (int)DrawBasicWidgetId::DisableBackColor));

    setModify(true);
}

QColor CMDPropertyDrawBasicWidget::disableBackColor()
{
    if(widget())
        return widget()->disableBackColor();
    return QColor();
}

void CMDPropertyDrawBasicWidget::setFont(const QFont &value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchFont(value);
    else
    {
        QFont* oldValue = new QFont;
        *oldValue = *widget()->font();
        QFont* newValue = new QFont;
        *newValue = value;

        undo()->push(new CDrawBasicWidgetCommand(
            widget(), (qulonglong)oldValue, (qulonglong)newValue, (int)DrawBasicWidgetId::Font));
    }

    setModify(true);
}

QFont CMDPropertyDrawBasicWidget::font()
{
    if(widget())
        return *widget()->font();
    return QFont();
}

CMDPropertyDrawBasicWidget::CMDPropertyDrawBasicWidget(CMDDrawObj *parent):
    CMDPropertyDrawObj(parent)
{
    insertTranslateMap("enable",tr("可用"));

    insertTranslateMap("textColor",tr("文本色"));
    insertTranslateMap("backColor",tr("背景色"));
    insertTranslateMap("disableBackColor",tr("不可用时背景色"));
    insertTranslateMap("font",tr("字体"));
}

CMDDrawBasicWidget *CMDPropertyDrawBasicWidget::widget()
{
    return dynamic_cast<CMDDrawBasicWidget*>(m_object);
}


//////////////////////////////////////////////////////////////////////////
void CMDPropertyDrawBasicWidget::resetBatchProperty(CPropertyBrowser *browser)
{
    if(!isBatch())
        return;

    CMDPropertyDrawObj::resetBatchProperty(browser);
}

void CMDPropertyDrawBasicWidget::setBatchEnable(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawBasicWidget* v = dynamic_cast<CMDDrawBasicWidget*>(o);
        undo()->push(new CDrawBasicWidgetCommand(
            v, v->enable(), value, (int)DrawBasicWidgetId::Enable));
    }

    undo()->endMacro();
}

void CMDPropertyDrawBasicWidget::setBatchTextColor(const QColor &value)
{
    undo()->beginMacro("");

    ColorType type = getColorChanged(value, widget()->textColor());
    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawBasicWidget* v = dynamic_cast<CMDDrawBasicWidget*>(o);
        QColor c = v->textColor();

        undo()->push(new CDrawBasicWidgetCommand(
            v, v->textColor(), getChangedColor(value,c,type), (int)DrawBasicWidgetId::TextColor));
    }

    undo()->endMacro();
}

void CMDPropertyDrawBasicWidget::setBatchBackColor(const QColor &value)
{
    undo()->beginMacro("");

    ColorType type = getColorChanged(value, widget()->backColor());
    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawBasicWidget* v = dynamic_cast<CMDDrawBasicWidget*>(o);
        QColor c = v->backColor();

        undo()->push(new CDrawBasicWidgetCommand(
            v, v->backColor(), getChangedColor(value,c,type), (int)DrawBasicWidgetId::BackColor));
    }

    undo()->endMacro();
}

void CMDPropertyDrawBasicWidget::setBatchDisableBackColor(const QColor &value)
{
    undo()->beginMacro("");

    ColorType type = getColorChanged(value, widget()->disableBackColor());
    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawBasicWidget* v = dynamic_cast<CMDDrawBasicWidget*>(o);
        QColor c = v->disableBackColor();

        undo()->push(new CDrawBasicWidgetCommand(
            v, v->disableBackColor(),getChangedColor(value,c,type) , (int)DrawBasicWidgetId::DisableBackColor));
    }

    undo()->endMacro();
}

void CMDPropertyDrawBasicWidget::setBatchFont(const QFont &value)
{
    undo()->beginMacro("");

    FontType type = getFontChange(value, *widget()->font());
    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawBasicWidget* v = dynamic_cast<CMDDrawBasicWidget*>(o);
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

        undo()->push(new CDrawBasicWidgetCommand(
            v, (qulonglong)oldValue, (qulonglong)newValue, (int)DrawBasicWidgetId::Font));
    }

    undo()->endMacro();
}

CMDPropertyDrawBasicWidget::ColorType CMDPropertyDrawBasicWidget::getColorChanged(const QColor &c1, const QColor &c2)
{
    int changeCount = 0;
    ColorType type = ColorType::Red;

    if(c1.red() != c2.red())
    {
        type = ColorType::Red;
        changeCount++;
    }
    if(c1.green() != c2.green())
    {
        type = ColorType::Green;
        changeCount++;
    }
    if(c1.blue() != c2.blue())
    {
        type = ColorType::Blue;
        changeCount++;
    }
    if(c1.alpha() != c2.alpha())
    {
        type = ColorType::Alpha;
        changeCount++;
    }

    if(changeCount > 1)
        return ColorType::All;

    return type;
}

QColor CMDPropertyDrawBasicWidget::getChangedColor(QColor value, QColor c, CMDPropertyDrawBasicWidget::ColorType type)
{
    switch(type)
    {
    case ColorType::Red:
        c.setRed(value.red());
        break;
    case ColorType::Green:
        c.setGreen(value.green());
        break;
    case ColorType::Blue:
        c.setBlue(value.blue());
        break;
    case ColorType::Alpha:
        c.setAlpha(value.alpha());
        break;
    case ColorType::All:
        c = value;
        break;
    default:
        break;
    }
    return c;
}

//////////////////////////////////////////////////////////////////////////


