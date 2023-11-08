#ifndef QTGROUPBOXPROPERTYBROWSER_H
#define QTGROUPBOXPROPERTYBROWSER_H

#include "qtpropertybrowser.h"

#if QT_VERSION >= 0x040400
QT_BEGIN_NAMESPACE
#endif

/*!
    \class QtGroupBoxPropertyBrowser

    \brief The QtGroupBoxPropertyBrowser class provides a QGroupBox
    based property browser.

    A property browser is a widget that enables the user to edit a
    given set of properties. Each property is represented by a label
    specifying the property's name, and an editing widget (e.g. a line
    edit or a combobox) holding its value. A property can have zero or
    more subproperties.

    QtGroupBoxPropertyBrowser provides group boxes for all nested
    properties, i.e. subproperties are enclosed by a group box with
    the parent property's name as its title. For example:

    \image qtgroupboxpropertybrowser.png

    Use the QtAbstractPropertyBrowser API to add, insert and remove
    properties from an instance of the QtGroupBoxPropertyBrowser
    class. The properties themselves are created and managed by
    implementations of the QtAbstractPropertyManager class.

    \sa QtTreePropertyBrowser, QtAbstractPropertyBrowser
*/
class QtGroupBoxPropertyBrowserPrivate;
class QT_QTPROPERTYBROWSER_EXPORT QtGroupBoxPropertyBrowser : public QtAbstractPropertyBrowser
{
    Q_OBJECT
public:
    QtGroupBoxPropertyBrowser(QWidget *parent = 0);
    ~QtGroupBoxPropertyBrowser();
protected:
    virtual void itemInserted(QtBrowserItem *item, QtBrowserItem *afterItem);
    virtual void itemRemoved(QtBrowserItem *item);
    virtual void itemChanged(QtBrowserItem *item);
private:
    QtGroupBoxPropertyBrowserPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtGroupBoxPropertyBrowser)
    Q_DISABLE_COPY(QtGroupBoxPropertyBrowser)
    Q_PRIVATE_SLOT(d_func(), void slotUpdate())
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed())

};

#if QT_VERSION >= 0x040400
QT_END_NAMESPACE
#endif

#endif
