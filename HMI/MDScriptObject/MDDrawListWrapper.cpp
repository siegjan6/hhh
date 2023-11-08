#include "MDDrawListWrapper.h"
#include "MDDrawList.h"

CMDDrawListWrapper::CMDDrawListWrapper(void *object, const QString &name)
    :CMDDrawWidgetWrapper(object,name)
{

}

QString CMDDrawListWrapper::currentItem()
{
    return object()->currentItem();
}

void CMDDrawListWrapper::setCurrentItem(const QString& str)
{
    Q_UNUSED(str);
}

int  CMDDrawListWrapper::listCount()
{
    return object()->listCount();
}

void CMDDrawListWrapper::setListCount(int value)
{
    Q_UNUSED(value);
}

int CMDDrawListWrapper::currentIndex()
{
    return object()->currentIndex();
}

void CMDDrawListWrapper::setCurrentIndex(int value)
{
    object()->setCurrentIndex(value);
}

int CMDDrawListWrapper::borderStyle()
{
    return object()->borderStyle();
}

void CMDDrawListWrapper::setBorderStyle(int value)
{
    switch (value)
    {
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

int CMDDrawListWrapper::sorted()
{
    return object()->sorted();
}

void CMDDrawListWrapper::setSorted(int value)
{
    object()->setSorted(value);
}

int CMDDrawListWrapper::selectCount()
{
    return object()->selectCount();
}

void CMDDrawListWrapper::setSelectCount(int value)
{
    Q_UNUSED(value);
}

QString CMDDrawListWrapper::indexString(const int index)
{
    return object()->indexString(index);
}

void CMDDrawListWrapper::insertItem(int index,  const QVariant& value)
{
    object()->insertItem(index,value.toString());
}

void CMDDrawListWrapper::addItem(const QString &value)
{
   object()->addItem(value);
}

void CMDDrawListWrapper::clearItems()
{
    object()->clearItems();
}

void CMDDrawListWrapper::removeItem(int index)
{
    object()->removeItem(index);
}

int CMDDrawListWrapper::findString(int index,  QString str)
{
    return object()->findString(index,str);
}

CMDDrawList *CMDDrawListWrapper::object()
{
    return reinterpret_cast<CMDDrawList*>(MDObject());
}
