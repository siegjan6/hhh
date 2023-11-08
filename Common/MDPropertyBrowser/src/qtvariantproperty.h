#ifndef QTVARIANTPROPERTY_H
#define QTVARIANTPROPERTY_H

#include "qtpropertybrowser.h"
#include <QVariant>
#include <QIcon>

#if QT_VERSION >= 0x040400
QT_BEGIN_NAMESPACE
#endif

typedef QMap<int, QIcon> QtIconMap;

class QtVariantPropertyManager;
class QtVariantPropertyPrivate;

/*!
    \class QtVariantProperty

    \brief The QtVariantProperty class is a convenience class handling
    QVariant based properties.
*/
class QT_QTPROPERTYBROWSER_EXPORT QtVariantProperty : public QtProperty
{
public:
    ~QtVariantProperty();
    QVariant value() const;
    QVariant attributeValue(const QString &attribute) const;
    int valueType() const;
    int propertyType() const;

    void setValue(const QVariant &value);
    void setAttribute(const QString &attribute, const QVariant &value);
protected:
    QtVariantProperty(QtVariantPropertyManager *manager);
private:
    friend class QtVariantPropertyManager;
    QtVariantPropertyPrivate *d_ptr;
};

/*!
    \class QtVariantPropertyManager

    \brief The QtVariantPropertyManager class provides and manages QVariant based properties.

    QtVariantPropertyManager provides the addProperty() function which
    creates QtVariantProperty objects. The QtVariantProperty class is
    a convenience class handling QVariant based properties inheriting
    QtProperty. A QtProperty object created by a
    QtVariantPropertyManager instance can be converted into a
    QtVariantProperty object using the variantProperty() function.
*/
class QtVariantPropertyManagerPrivate;
class QT_QTPROPERTYBROWSER_EXPORT QtVariantPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtVariantPropertyManager(QObject *parent = 0);
    ~QtVariantPropertyManager();

    virtual QtVariantProperty *addProperty(int propertyType, const QString &name = QString(),const QString &markText = QString());//! mark

    int propertyType(const QtProperty *property) const;
    int valueType(const QtProperty *property) const;
    QtVariantProperty *variantProperty(const QtProperty *property) const;

    virtual bool isPropertyTypeSupported(int propertyType) const;
    virtual int valueType(int propertyType) const;
    virtual QStringList attributes(int propertyType) const;
    virtual int attributeType(int propertyType, const QString &attribute) const;

    virtual QVariant value(const QtProperty *property) const;
    virtual QVariant attributeValue(const QtProperty *property, const QString &attribute) const;

    static int enumTypeId();
    static int flagTypeId();
    static int groupTypeId();
    static int iconMapTypeId();

public Q_SLOTS:
    virtual void setValue(QtProperty *property, const QVariant &val);
    virtual void setAttribute(QtProperty *property,
                const QString &attribute, const QVariant &value);
Q_SIGNALS:
    void valueChanged(QtProperty *property, const QVariant &val);
    void updatePropertyOutside(QtProperty *property, const QVariant &val);
    void attributeChanged(QtProperty *property,
                const QString &attribute, const QVariant &val);
protected:
    virtual bool hasValue(const QtProperty *property) const;
    QString valueText(const QtProperty *property) const;
    QIcon valueIcon(const QtProperty *property) const;
    virtual void initializeProperty(QtProperty *property);
    virtual void uninitializeProperty(QtProperty *property);
    virtual QtProperty *createProperty();
private:
    QtVariantPropertyManagerPrivate *d_ptr;
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, int, int))
    Q_PRIVATE_SLOT(d_func(), void slotSingleStepChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, double))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, double, double))
    Q_PRIVATE_SLOT(d_func(), void slotSingleStepChanged(QtProperty *, double))
    Q_PRIVATE_SLOT(d_func(), void slotDecimalsChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, bool))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QString &))
    Q_PRIVATE_SLOT(d_func(), void slotRegExpChanged(QtProperty *, const QRegExp &))
    Q_PRIVATE_SLOT(d_func(), void slotEchoModeChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QDate &))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, const QDate &, const QDate &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QTime &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QDateTime &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QKeySequence &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QChar &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QLocale &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QPoint &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QPointF &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QSize &))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, const QSize &, const QSize &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QSizeF &))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, const QSizeF &, const QSizeF &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QRect &))
    Q_PRIVATE_SLOT(d_func(), void slotConstraintChanged(QtProperty *, const QRect &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QRectF &))
    Q_PRIVATE_SLOT(d_func(), void slotConstraintChanged(QtProperty *, const QRectF &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QColor &))
    Q_PRIVATE_SLOT(d_func(), void slotEnumNamesChanged(QtProperty *, const QStringList &))
    Q_PRIVATE_SLOT(d_func(), void slotEnumIconsChanged(QtProperty *, const QMap<int, QIcon> &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QSizePolicy &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QFont &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QCursor &))
    Q_PRIVATE_SLOT(d_func(), void slotFlagNamesChanged(QtProperty *, const QStringList &))
    Q_PRIVATE_SLOT(d_func(), void slotReadOnlyChanged(QtProperty *, bool))
    Q_PRIVATE_SLOT(d_func(), void slotTextVisibleChanged(QtProperty *, bool))
    Q_PRIVATE_SLOT(d_func(), void slotPropertyInserted(QtProperty *, QtProperty *, QtProperty *))
    Q_PRIVATE_SLOT(d_func(), void slotPropertyRemoved(QtProperty *, QtProperty *))

    Q_PRIVATE_SLOT(d_func(), void slotUpdateValue(QtProperty *property, int val))
    Q_PRIVATE_SLOT(d_func(), void slotUpdateValue(QtProperty *property, double val))
    Q_PRIVATE_SLOT(d_func(), void slotUpdateValue(QtProperty *property, bool val))
    Q_PRIVATE_SLOT(d_func(), void slotUpdateValue(QtProperty *property,const QString &val))
    Q_PRIVATE_SLOT(d_func(), void slotUpdateValue(QtProperty *property,const QDate &val))
    Q_PRIVATE_SLOT(d_func(), void slotUpdateValue(QtProperty *property,const QTime &val))
    Q_PRIVATE_SLOT(d_func(), void slotUpdateValue(QtProperty *property,const QDateTime &val))
    Q_PRIVATE_SLOT(d_func(), void slotUpdateValue(QtProperty *property,const QKeySequence &val))
    Q_PRIVATE_SLOT(d_func(), void slotUpdateValue(QtProperty *property,const QChar &val))
    Q_PRIVATE_SLOT(d_func(), void slotUpdateValue(QtProperty *property,const QLocale &val))
    Q_PRIVATE_SLOT(d_func(), void slotUpdateValue(QtProperty *property,const QPoint &val))
    Q_PRIVATE_SLOT(d_func(), void slotUpdateValue(QtProperty *property,const QPointF &val))
    Q_PRIVATE_SLOT(d_func(), void slotUpdateValue(QtProperty *property,const QSize &val))
    Q_PRIVATE_SLOT(d_func(), void slotUpdateValue(QtProperty *property,const QSizeF &val))
    Q_PRIVATE_SLOT(d_func(), void slotUpdateValue(QtProperty *property,const QRect &val))
    Q_PRIVATE_SLOT(d_func(), void slotUpdateValue(QtProperty *property,const QRectF &val))
    Q_PRIVATE_SLOT(d_func(), void slotUpdateValue(QtProperty *property,const QColor &val))
    Q_PRIVATE_SLOT(d_func(), void slotUpdateValue(QtProperty *property,const QSizePolicy &val))
    Q_PRIVATE_SLOT(d_func(), void slotUpdateValue(QtProperty *property,const QFont &val))
    Q_PRIVATE_SLOT(d_func(), void slotUpdateValue(QtProperty *property,const QCursor &val))
    Q_DECLARE_PRIVATE(QtVariantPropertyManager)
    Q_DISABLE_COPY(QtVariantPropertyManager)
};


/*!
    \class QtVariantEditorFactory

    \brief The QtVariantEditorFactory class provides widgets for properties
    created by QtVariantPropertyManager objects.

    The variant factory provides the following widgets for the
    specified property types:

    \table
    \header
        \o Property Type
        \o Widget
    \row
        \o \c int
        \o QSpinBox
    \row
        \o \c double
        \o QDoubleSpinBox
    \row
        \o \c bool
        \o QCheckBox
    \row
        \o QString
        \o QLineEdit
    \row
        \o QDate
        \o QDateEdit
    \row
        \o QTime
        \o QTimeEdit
    \row
        \o QDateTime
        \o QDateTimeEdit
    \row
        \o QKeySequence
        \o customized editor
    \row
        \o QChar
        \o customized editor
    \row
        \o \c enum
        \o QComboBox
    \row
        \o QCursor
        \o QComboBox
    \endtable

    Note that QtVariantPropertyManager supports several additional property
    types for which the QtVariantEditorFactory class does not provide
    editing widgets, e.g. QPoint and QSize. To provide widgets for other
    types using the variant approach, derive from the QtVariantEditorFactory
    class.

    \sa QtAbstractEditorFactory, QtVariantPropertyManager
*/
class QtVariantEditorFactoryPrivate;
class QT_QTPROPERTYBROWSER_EXPORT QtVariantEditorFactory : public QtAbstractEditorFactory<QtVariantPropertyManager>
{
    Q_OBJECT
public:
    QtVariantEditorFactory(QObject *parent = 0);
    ~QtVariantEditorFactory();
protected:
    void connectPropertyManager(QtVariantPropertyManager *manager);
    QWidget *createEditor(QtVariantPropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtVariantPropertyManager *manager);
private:
    QtVariantEditorFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtVariantEditorFactory)
    Q_DISABLE_COPY(QtVariantEditorFactory)
};

#if QT_VERSION >= 0x040400
QT_END_NAMESPACE
#endif

Q_DECLARE_METATYPE(QIcon)
Q_DECLARE_METATYPE(QtIconMap)
#endif
