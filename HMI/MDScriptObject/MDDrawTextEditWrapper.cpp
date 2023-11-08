#include "MDDrawTextEditWrapper.h"
#include "MDDrawTextEdit.h"

CMDDrawTextEditWrapper::CMDDrawTextEditWrapper(void *object, const QString &name)
    :CMDDrawWidgetWrapper(object,name)
{
}

QString CMDDrawTextEditWrapper::text()
{
    return object()->text();
}

void CMDDrawTextEditWrapper::setText(const QString& str)
{
    //    obj->addScript("setText", QVariant(str));
    object()->setText(str);
}

bool CMDDrawTextEditWrapper::readOnly()
{
    return object()->readOnly();
}

void CMDDrawTextEditWrapper::setReadOnly(bool readOnly)
{
    object()->setReadOnly(readOnly);
}

int CMDDrawTextEditWrapper::alignment()
{
    return object()->alignment();
}

void CMDDrawTextEditWrapper::setAlignment(int value)
{
    switch (value) {
    case 0:
        object()->setAlignment(Qt::AlignLeft);
        break;
    case 1:
        object()->setAlignment(Qt::AlignCenter);
        break;
    case 2:
        object()->setAlignment(Qt::AlignRight);
        break;
    default:
        break;
    }
}

int CMDDrawTextEditWrapper::lineWrapMode()
{
    return object()->lineWrapMode();
}

void CMDDrawTextEditWrapper::setLineWrapMode(int value)
{
    switch (value) {
    case 0:
        object()->setLineWrapMode(QTextEdit::NoWrap);
        break;
    case 1:
        object()->setLineWrapMode(QTextEdit::WidgetWidth);
        break;
    default:
        break;
    }
}

int CMDDrawTextEditWrapper::borderStyle()
{
    return object()->borderStyle();
}

void CMDDrawTextEditWrapper::setBorderStyle(int value)
{
    switch (value) {
    case 0:
        object()->setBorderStyle(QFrame::NoFrame);
        break;
    case 1:
        object()->setBorderStyle(QFrame::StyledPanel);
        break;
    case 2:
        object()->setBorderStyle(QFrame::WinPanel);
        break;
    default:
        break;
    }
}

void CMDDrawTextEditWrapper::setHorizontalScrollBar(bool value)
{
    object()->setHorizontalScrollBar(value);
}

bool CMDDrawTextEditWrapper::horizontalScrollBar()
{
    return object()->horizontalScrollBar();
}

void CMDDrawTextEditWrapper::setVerticalScrollBar(bool value)
{
    object()->setVerticalScrollBar(value);
}

bool CMDDrawTextEditWrapper::verticalScrollBar()
{
    return object()->verticalScrollBar();
}

int CMDDrawTextEditWrapper::selectStart()
{
    return object()->selectStart();
}

void CMDDrawTextEditWrapper::setSelectStart(int value)
{
    object()->setSelectStart(value);
}

int CMDDrawTextEditWrapper::selectLength()
{
    return object()->selectLength();
}

void CMDDrawTextEditWrapper::setSelectLength(int value)
{
    object()->setSelectLength(value);
}

QString CMDDrawTextEditWrapper::selectText()
{
    return object()->selectText();
}

void CMDDrawTextEditWrapper::setSelectText(QString &str)
{
    object()->setSelectText(str);
    //    obj->addScript("setSelectText",QVariant(str));
}


CMDDrawTextEdit *CMDDrawTextEditWrapper::object()
{
    return reinterpret_cast<CMDDrawTextEdit*>(MDObject());
}
