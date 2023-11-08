#include "MDDrawComboBoxWrapper.h"
#include "MDDrawComboBox.h"

CMDDrawComboBoxWrapper::CMDDrawComboBoxWrapper(void *object, const QString &name)
    :CMDDrawWidgetWrapper(object, name)
{

}

int CMDDrawComboBoxWrapper::listCount()
{
    return object()->listCount();
}

void CMDDrawComboBoxWrapper::setListCount(int value)
{
    Q_UNUSED(value);
}

int CMDDrawComboBoxWrapper::currentIndex()
{
    return object()->getCurrentIndex();
}

void CMDDrawComboBoxWrapper::setCurrentIndex(int value)
{
    object()->setCurIndex(value);
}

int CMDDrawComboBoxWrapper::listStyle()
{
    return object()->itemStyle();
}

void CMDDrawComboBoxWrapper::setListStyle(int value)
{
    object()->setItemStyle(value);
}

QString CMDDrawComboBoxWrapper::currentText()
{
    return object()->getCurText();
}

void CMDDrawComboBoxWrapper::setCurrentText(const QString &str)
{
    object()->setCurText(str);
}

bool CMDDrawComboBoxWrapper::sorted()
{
    return object()->Sorted();
}

void CMDDrawComboBoxWrapper::setSorted(bool sort)
{
    object()->setSorted(sort);
}

QString CMDDrawComboBoxWrapper::indexString(int index)
{
    return object()->indexString(index);
}

void CMDDrawComboBoxWrapper::insertItem(int index, const QVariant& value)
{
    object()->insertItem(index,value.toString());
}

void CMDDrawComboBoxWrapper::addItem(const QString &value)
{
    object()->addItem(value);
}

void CMDDrawComboBoxWrapper::clearItems()
{
    object()->clearItems();
}

void CMDDrawComboBoxWrapper::removeItem(int index)
{
    object()->removeItem(index);
}

int CMDDrawComboBoxWrapper::findString(int index,  QString value)
{
    return object()->findString(index,value);
}

int CMDDrawComboBoxWrapper::selectString(int index, QString value)
{
    return object()->selectString(index,value);
}

CMDDrawComboBox *CMDDrawComboBoxWrapper::object()
{
    return reinterpret_cast<CMDDrawComboBox*>(MDObject());
}
