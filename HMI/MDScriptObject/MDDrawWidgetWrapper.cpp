#include "MDDrawWidgetWrapper.h"
#include "MDDrawBasicWidget.h"

CMDDrawWidgetWrapper::CMDDrawWidgetWrapper(void *Object, const QString &name)
    :CMDDrawObjWrapper(Object,name)
{
}

QString CMDDrawWidgetWrapper::fontName()
{
    return object()->font()->family();
}

void CMDDrawWidgetWrapper::setFontName(const QString &name)
{
    QFont* font = object()->font();
    font->setFamily(name);
    object()->setFont(font);
}

bool CMDDrawWidgetWrapper::fontBold()
{
    return object()->font()->bold();
}

void CMDDrawWidgetWrapper::setFontBold(bool bold)
{
    QFont* f = object()->font();
    f->setBold(bold);
    return object()->setFont(f);
}

int CMDDrawWidgetWrapper::fontSize()
{
    return object()->font()->pointSize();
}

void CMDDrawWidgetWrapper::setFontSize(int size)
{
    QFont* f = object()->font();
    f->setPointSize(size);
    return object()->setFont(f);
}

bool CMDDrawWidgetWrapper::fontItalic()
{
    CMDDrawBasicWidget* obj = (CMDDrawBasicWidget*)MDObject();
    bool bItalic = obj->font()->italic();
    return bItalic;
}

void CMDDrawWidgetWrapper::setFontItalic(bool italic)
{
    QFont* f = object()->font();
    f->setItalic(italic);
    return object()->setFont(f);
}

bool CMDDrawWidgetWrapper::fontUnderline()
{
    return object()->font()->underline();
}

void CMDDrawWidgetWrapper::setFontUnderline(bool underLine)
{
    QFont* f = object()->font();
    f->setUnderline(underLine);
    return object()->setFont(f);
}

bool CMDDrawWidgetWrapper::fontStrikeOut()
{
    return object()->font()->strikeOut();
}

void CMDDrawWidgetWrapper::setFontStrikeOut(bool strikeOut)
{
    QFont* f = object()->font();
    f->setStrikeOut(strikeOut);
    return object()->setFont(f);
}

int CMDDrawWidgetWrapper::backColor()
{
    QRgb rgba = object()->backColor().rgba();
    return rgba;
}

void CMDDrawWidgetWrapper::setBackColor(int rgba)
{
    QColor color = QColor(rgba);
    object()->setBackColor(color);
}

int CMDDrawWidgetWrapper::textColor()
{
    QRgb rgba = object()->textColor().rgba();
    return rgba;
}

void CMDDrawWidgetWrapper::setTextColor(int rgba)
{
    QColor color = QColor(rgba);
    object()->setTextColor(color);
}

CMDDrawBasicWidget *CMDDrawWidgetWrapper::object()
{
    return reinterpret_cast<CMDDrawBasicWidget*>(MDObject());
}
