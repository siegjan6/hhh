#ifndef QTPROPERTYBROWSER_H
#define QTPROPERTYBROWSER_H

#include <QWidget>
#include <QSet>
#include <QLineEdit>
#include <QVariant>
#include "../../include/MDPropertyBrowser/PropertyBrowser.h"

#if QT_VERSION >= 0x040400
QT_BEGIN_NAMESPACE
#endif
/**
 * \brief 说明
 *     此文件中包含属性类（QtProperty）的声明，
 * 属性数据管理基类（QtAbstractPropertyManager）的声明，
 * 属性工厂模型的抽象基类（QtAbstractEditorFactoryBase）、
 * 工程模型模板类（QtAbstractEditorFactory）的声明和实现，
 * 属性浏览器基类（QtAbstractPropertyBrowser）的声明、
 * 属性浏览器子项（QtBrowserItem）的声明。
 */
#define IMDIDECHANGE     0
#define OUTSIDECHANGE    1

typedef QLineEdit::EchoMode EchoMode;

class QtAbstractPropertyManager;
class QtPropertyPrivate;

/*!
    \class QtProperty

    \brief The QtProperty class encapsulates an instance of a property.

    Properties are created by objects of QtAbstractPropertyManager
    subclasses; a manager can create properties of a given type, and
    is used in conjunction with the QtAbstractPropertyBrowser class. A
    property is always owned by the manager that created it, which can
    be retrieved using the propertyManager() function.
*/
class QT_QTPROPERTYBROWSER_EXPORT QtProperty
{
public:
    virtual ~QtProperty();

    QList<QtProperty *> subProperties() const;

    QtAbstractPropertyManager *propertyManager() const;

    QSet<QtProperty *> parentItems() const;

    int source() const;
    QString toolTip() const;
    QString statusTip() const;
    QString whatsThis() const;
    QString propertyName() const;
    bool isEnabled() const;
    bool isModified() const;

    bool hasValue() const;
    QIcon valueIcon() const;
    QString valueText() const;
    QString displayText() const;
    QString markText() const;

    void setSource(int source);
    void setToolTip(const QString &text);
    void setStatusTip(const QString &text);
    void setWhatsThis(const QString &text);
    void setPropertyName(const QString &text);
    void setEnabled(bool enable);
    void setModified(bool modified);
    void setMarkText(const QString &text); //! mark

    void addSubProperty(QtProperty *property);
    void insertSubProperty(QtProperty *property, QtProperty *afterProperty);
    void removeSubProperty(QtProperty *property);

protected:
    explicit QtProperty(QtAbstractPropertyManager *manager);
    void propertyChanged();
private:
    friend class QtAbstractPropertyManager;
    QtPropertyPrivate *d_ptr;
};

/*!
    \class QtAbstractPropertyManager

    \brief The QtAbstractPropertyManager provides an interface for
    property managers.

    A manager can create and manage properties of a given type, and is
    used in conjunction with the QtAbstractPropertyBrowser class.
*/
class QtAbstractPropertyManagerPrivate;

class QT_QTPROPERTYBROWSER_EXPORT QtAbstractPropertyManager : public QObject
{
    Q_OBJECT
public:

    explicit QtAbstractPropertyManager(QObject *parent = 0);
    ~QtAbstractPropertyManager();

    QSet<QtProperty *> properties() const;
    void clear() const;

    QtProperty *addProperty(const QString &name = QString(),const QString &markText = QString());
Q_SIGNALS:

    void propertyInserted(QtProperty *property,
                QtProperty *parent, QtProperty *after);
    void propertyChanged(QtProperty *property);
    void propertyRemoved(QtProperty *property, QtProperty *parent);
    void propertyDestroyed(QtProperty *property);
protected:
    virtual bool hasValue(const QtProperty *property) const;
    virtual QIcon valueIcon(const QtProperty *property) const;
    virtual QString valueText(const QtProperty *property) const;
    virtual QString displayText(const QtProperty *property) const;
    virtual EchoMode echoMode(const QtProperty *) const;
    virtual void initializeProperty(QtProperty *property) = 0;
    virtual void uninitializeProperty(QtProperty *property);
    virtual QtProperty *createProperty();
private:
    friend class QtProperty;
    QtAbstractPropertyManagerPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtAbstractPropertyManager)
    Q_DISABLE_COPY(QtAbstractPropertyManager)
};

class QT_QTPROPERTYBROWSER_EXPORT QtAbstractEditorFactoryBase : public QObject
{
    Q_OBJECT
public:
    virtual QWidget *createEditor(QtProperty *property, QWidget *parent) = 0;
protected:
    explicit QtAbstractEditorFactoryBase(QObject *parent = 0)
        : QObject(parent) {}

    virtual void breakConnection(QtAbstractPropertyManager *manager) = 0;
protected Q_SLOTS:
    virtual void managerDestroyed(QObject *manager) = 0;

    friend class QtAbstractPropertyBrowser;
};

/*!
    \class QtAbstractPropertyBrowser

    \brief QtAbstractPropertyBrowser provides a base class for
    implementing property browsers.

    A property browser is a widget that enables the user to edit a
    given set of properties.  Each property is represented by a label
    specifying the property's name, and an editing widget (e.g. a line
    edit or a combobox) holding its value. A property can have zero or
    more subproperties.
*/
template <class PropertyManager>
class QtAbstractEditorFactory : public QtAbstractEditorFactoryBase
{
public:
    explicit QtAbstractEditorFactory(QObject *parent) : QtAbstractEditorFactoryBase(parent) {}
    QWidget *createEditor(QtProperty *property, QWidget *parent)
    {
        QSetIterator<PropertyManager *> it(m_managers);
        while (it.hasNext()) {
            PropertyManager *manager = it.next();
            if (manager == property->propertyManager()) {
                return createEditor(manager, property, parent);//connlaio
            }
        }
        return 0;
    }
    void addPropertyManager(PropertyManager *manager)
    {
        if (m_managers.contains(manager))
            return;
        m_managers.insert(manager);
        connectPropertyManager(manager);
        connect(manager, SIGNAL(destroyed(QObject *)),
                    this, SLOT(managerDestroyed(QObject *)));
    }
    void removePropertyManager(PropertyManager *manager)
    {
        if (!m_managers.contains(manager))
            return;
        disconnect(manager, SIGNAL(destroyed(QObject *)),
                    this, SLOT(managerDestroyed(QObject *)));
        disconnectPropertyManager(manager);
        m_managers.remove(manager);
    }
    QSet<PropertyManager *> propertyManagers() const
    {
        return m_managers;
    }
    PropertyManager *propertyManager(QtProperty *property) const
    {
        QtAbstractPropertyManager *manager = property->propertyManager();
        QSetIterator<PropertyManager *> itManager(m_managers);
        while (itManager.hasNext()) {
            PropertyManager *m = itManager.next();
            if (m == manager) {
                return m;
            }
        }
        return 0;
    }
protected:
    virtual void connectPropertyManager(PropertyManager *manager) = 0;
    virtual QWidget *createEditor(PropertyManager *manager, QtProperty *property,
                QWidget *parent) = 0;
    virtual void disconnectPropertyManager(PropertyManager *manager) = 0;
    void managerDestroyed(QObject *manager)
    {
        QSetIterator<PropertyManager *> it(m_managers);
        while (it.hasNext()) {
            PropertyManager *m = it.next();
            if (m == manager) {
                m_managers.remove(m);
                return;
            }
        }
    }
private:
    void breakConnection(QtAbstractPropertyManager *manager)
    {
        QSetIterator<PropertyManager *> it(m_managers);
        while (it.hasNext()) {
            PropertyManager *m = it.next();
            if (m == manager) {
                removePropertyManager(m);
                return;
            }
        }
    }
private:
    QSet<PropertyManager *> m_managers;
    friend class QtAbstractPropertyEditor;
};


/*!
    \class QtBrowserItem

    \brief The QtBrowserItem class represents a property in
    a property browser instance.

    Browser items are created whenever a QtProperty is inserted to the
    property browser. A QtBrowserItem uniquely identifies a
    browser's item. Thus, if the same QtProperty is inserted multiple
    times, each occurrence gets its own unique QtBrowserItem. The
    items are owned by QtAbstractPropertyBrowser and automatically
    deleted when they are removed from the browser.

    You can traverse a browser's properties by calling parent() and
    children(). The property and the browser associated with an item
    are available as property() and browser().

    \sa QtAbstractPropertyBrowser, QtProperty
*/
class QtAbstractPropertyBrowser;
class QtBrowserItemPrivate;

class QT_QTPROPERTYBROWSER_EXPORT QtBrowserItem
{
public:
    QtProperty *property() const;
    QtBrowserItem *parent() const;
    QList<QtBrowserItem *> children() const;
    QtAbstractPropertyBrowser *browser() const;
private:
    explicit QtBrowserItem(QtAbstractPropertyBrowser *browser, QtProperty *property, QtBrowserItem *parent);
    ~QtBrowserItem();
    QtBrowserItemPrivate *d_ptr;
    friend class QtAbstractPropertyBrowserPrivate;
};

class QtAbstractPropertyBrowserPrivate;

class QT_QTPROPERTYBROWSER_EXPORT QtAbstractPropertyBrowser : public QWidget
{
    Q_OBJECT
public:

    explicit QtAbstractPropertyBrowser(QWidget *parent = 0);
    ~QtAbstractPropertyBrowser();

    QList<QtProperty *> properties() const;
    QList<QtBrowserItem *> items(QtProperty *property) const;
    QtBrowserItem *topLevelItem(QtProperty *property) const;
    QList<QtBrowserItem *> topLevelItems() const;
    void clear();

    template <class PropertyManager>
    void setFactoryForManager(PropertyManager *manager,
                    QtAbstractEditorFactory<PropertyManager> *factory) {
        QtAbstractPropertyManager *abstractManager = manager;
        QtAbstractEditorFactoryBase *abstractFactory = factory;

        if (addFactory(abstractManager, abstractFactory))
            factory->addPropertyManager(manager);
    }

    void unsetFactoryForManager(QtAbstractPropertyManager *manager);

    QtBrowserItem *currentItem() const;
    void setCurrentItem(QtBrowserItem *);

Q_SIGNALS:
    void currentItemChanged(QtBrowserItem *);

public Q_SLOTS:

    QtBrowserItem *addProperty(QtProperty *property);
    QtBrowserItem *insertProperty(QtProperty *property, QtProperty *afterProperty);
    void removeProperty(QtProperty *property);

protected:

    virtual void itemInserted(QtBrowserItem *item, QtBrowserItem *afterItem) = 0;
    virtual void itemRemoved(QtBrowserItem *item) = 0;
    // can be tooltip, statustip, whatsthis, name, icon, text.
    virtual void itemChanged(QtBrowserItem *item) = 0;

    virtual QWidget *createEditor(QtProperty *property, QWidget *parent);
private:

    bool addFactory(QtAbstractPropertyManager *abstractManager,
                QtAbstractEditorFactoryBase *abstractFactory);

    QtAbstractPropertyBrowserPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtAbstractPropertyBrowser)
    Q_DISABLE_COPY(QtAbstractPropertyBrowser)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyInserted(QtProperty *,
                            QtProperty *, QtProperty *))
    Q_PRIVATE_SLOT(d_func(), void slotPropertyRemoved(QtProperty *,
                            QtProperty *))
    Q_PRIVATE_SLOT(d_func(), void slotPropertyDestroyed(QtProperty *))
    Q_PRIVATE_SLOT(d_func(), void slotPropertyDataChanged(QtProperty *))

};

#if QT_VERSION >= 0x040400
QT_END_NAMESPACE
#endif

#endif // QTPROPERTYBROWSER_H
