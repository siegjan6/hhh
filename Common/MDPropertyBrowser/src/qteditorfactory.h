#ifndef QTEDITORFACTORY_H
#define QTEDITORFACTORY_H

#include "qtpropertymanager.h"

#if QT_VERSION >= 0x040400
QT_BEGIN_NAMESPACE
#endif

/**
 * @brief 此文件包含针对各种类型的工厂模型
 *
 */
/*!
    \class QtSpinBoxFactory

    \brief The QtSpinBoxFactory class provides QSpinBox widgets for
    properties created by QtIntPropertyManager objects.

    \sa QtAbstractEditorFactory, QtIntPropertyManager
*/
class QtSpinBoxFactoryPrivate;
class QT_QTPROPERTYBROWSER_EXPORT QtSpinBoxFactory : public QtAbstractEditorFactory<QtIntPropertyManager>
{
    Q_OBJECT
public:
    QtSpinBoxFactory(QObject *parent = 0);
    ~QtSpinBoxFactory();
protected:
    void connectPropertyManager(QtIntPropertyManager *manager);
    QWidget *createEditor(QtIntPropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtIntPropertyManager *manager);
private:
    QtSpinBoxFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtSpinBoxFactory)
    Q_DISABLE_COPY(QtSpinBoxFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, int, int))
    Q_PRIVATE_SLOT(d_func(), void slotSingleStepChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotReadOnlyChanged(QtProperty *, bool))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(int))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};


/*!
    \class QtSliderFactory

    \brief The QtSliderFactory class provides QSlider widgets for
    properties created by QtIntPropertyManager objects.

    \sa QtAbstractEditorFactory, QtIntPropertyManager
*/
class QtSliderFactoryPrivate;
class QT_QTPROPERTYBROWSER_EXPORT QtSliderFactory : public QtAbstractEditorFactory<QtIntPropertyManager>
{
    Q_OBJECT
public:
    QtSliderFactory(QObject *parent = 0);
    ~QtSliderFactory();
protected:
    void connectPropertyManager(QtIntPropertyManager *manager);
    QWidget *createEditor(QtIntPropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtIntPropertyManager *manager);
private:
    QtSliderFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtSliderFactory)
    Q_DISABLE_COPY(QtSliderFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, int, int))
    Q_PRIVATE_SLOT(d_func(), void slotSingleStepChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(int))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};


/*!
    \class QtScrollBarFactory

    \brief The QtScrollBarFactory class provides QScrollBar widgets for
    properties created by QtIntPropertyManager objects.

    \sa QtAbstractEditorFactory, QtIntPropertyManager
*/
class QtScrollBarFactoryPrivate;
class QT_QTPROPERTYBROWSER_EXPORT QtScrollBarFactory : public QtAbstractEditorFactory<QtIntPropertyManager>
{
    Q_OBJECT
public:
    QtScrollBarFactory(QObject *parent = 0);
    ~QtScrollBarFactory();
protected:
    void connectPropertyManager(QtIntPropertyManager *manager);
    QWidget *createEditor(QtIntPropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtIntPropertyManager *manager);
private:
    QtScrollBarFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtScrollBarFactory)
    Q_DISABLE_COPY(QtScrollBarFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, int, int))
    Q_PRIVATE_SLOT(d_func(), void slotSingleStepChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(int))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};


/*!
    \class QtCheckBoxFactory

    \brief The QtCheckBoxFactory class provides QCheckBox widgets for
    properties created by QtBoolPropertyManager objects.

    \sa QtAbstractEditorFactory, QtBoolPropertyManager
*/
class QtCheckBoxFactoryPrivate;
class QT_QTPROPERTYBROWSER_EXPORT QtCheckBoxFactory : public QtAbstractEditorFactory<QtBoolPropertyManager>
{
    Q_OBJECT
public:
    QtCheckBoxFactory(QObject *parent = 0);
    ~QtCheckBoxFactory();
protected:
    void connectPropertyManager(QtBoolPropertyManager *manager);
    QWidget *createEditor(QtBoolPropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtBoolPropertyManager *manager);
private:
    QtCheckBoxFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtCheckBoxFactory)
    Q_DISABLE_COPY(QtCheckBoxFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, bool))
    Q_PRIVATE_SLOT(d_func(), void slotTextVisibleChanged(QtProperty *, bool))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(bool))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
    Q_PRIVATE_SLOT(d_func(), void slotSetEditorClick(QString))

};


/*! \class QtDoubleSpinBoxFactory

    \brief The QtDoubleSpinBoxFactory class provides QDoubleSpinBox
    widgets for properties created by QtDoublePropertyManager objects.

    \sa QtAbstractEditorFactory, QtDoublePropertyManager
*/
class QtDoubleSpinBoxFactoryPrivate;
class QT_QTPROPERTYBROWSER_EXPORT QtDoubleSpinBoxFactory : public QtAbstractEditorFactory<QtDoublePropertyManager>
{
    Q_OBJECT
public:
    QtDoubleSpinBoxFactory(QObject *parent = 0);
    ~QtDoubleSpinBoxFactory();
protected:
    void connectPropertyManager(QtDoublePropertyManager *manager);
    QWidget *createEditor(QtDoublePropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtDoublePropertyManager *manager);
private:
    QtDoubleSpinBoxFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtDoubleSpinBoxFactory)
    Q_DISABLE_COPY(QtDoubleSpinBoxFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, double))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, double, double))
    Q_PRIVATE_SLOT(d_func(), void slotSingleStepChanged(QtProperty *, double))
    Q_PRIVATE_SLOT(d_func(), void slotDecimalsChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotReadOnlyChanged(QtProperty *, bool))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(double))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

/*!
    \class QtLineEditFactory

    \brief The QtLineEditFactory class provides QLineEdit widgets for
    properties created by QtStringPropertyManager objects.

    \sa QtAbstractEditorFactory, QtStringPropertyManager
*/
class QtLineEditFactoryPrivate;
class QT_QTPROPERTYBROWSER_EXPORT QtLineEditFactory : public QtAbstractEditorFactory<QtStringPropertyManager>
{
    Q_OBJECT
public:
    QtLineEditFactory(QObject *parent = 0);
    ~QtLineEditFactory();
protected:
    void connectPropertyManager(QtStringPropertyManager *manager);
    QWidget *createEditor(QtStringPropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtStringPropertyManager *manager);
private:
    QtLineEditFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtLineEditFactory)
    Q_DISABLE_COPY(QtLineEditFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QString &))
    Q_PRIVATE_SLOT(d_func(), void slotRegExpChanged(QtProperty *, const QRegExp &))
    Q_PRIVATE_SLOT(d_func(), void slotEchoModeChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotReadOnlyChanged(QtProperty *, bool))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QString &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

/*!
    \class QtDateEditFactory

    \brief The QtDateEditFactory class provides QDateEdit widgets for
    properties created by QtDatePropertyManager objects.

    \sa QtAbstractEditorFactory, QtDatePropertyManager
*/
class QtDateEditFactoryPrivate;
class QT_QTPROPERTYBROWSER_EXPORT QtDateEditFactory : public QtAbstractEditorFactory<QtDatePropertyManager>
{
    Q_OBJECT
public:
    QtDateEditFactory(QObject *parent = 0);
    ~QtDateEditFactory();
protected:
    void connectPropertyManager(QtDatePropertyManager *manager);
    QWidget *createEditor(QtDatePropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtDatePropertyManager *manager);
private:
    QtDateEditFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtDateEditFactory)
    Q_DISABLE_COPY(QtDateEditFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QDate &))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *,
                        const QDate &, const QDate &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QDate &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

/*!
    \class QtTimeEditFactory

    \brief The QtTimeEditFactory class provides QTimeEdit widgets for
    properties created by QtTimePropertyManager objects.

    \sa QtAbstractEditorFactory, QtTimePropertyManager
*/
class QtTimeEditFactoryPrivate;
class QT_QTPROPERTYBROWSER_EXPORT QtTimeEditFactory : public QtAbstractEditorFactory<QtTimePropertyManager>
{
    Q_OBJECT
public:
    QtTimeEditFactory(QObject *parent = 0);
    ~QtTimeEditFactory();
protected:
    void connectPropertyManager(QtTimePropertyManager *manager);
    QWidget *createEditor(QtTimePropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtTimePropertyManager *manager);
private:
    QtTimeEditFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtTimeEditFactory)
    Q_DISABLE_COPY(QtTimeEditFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QTime &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QTime &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

/*!
    \class QtDateTimeEditFactory

    \brief The QtDateTimeEditFactory class provides QDateTimeEdit
    widgets for properties created by QtDateTimePropertyManager objects.

    \sa QtAbstractEditorFactory, QtDateTimePropertyManager
*/

class QtDateTimeEditFactoryPrivate;
class QT_QTPROPERTYBROWSER_EXPORT QtDateTimeEditFactory : public QtAbstractEditorFactory<QtDateTimePropertyManager>
{
    Q_OBJECT
public:
    QtDateTimeEditFactory(QObject *parent = 0);
    ~QtDateTimeEditFactory();
protected:
    void connectPropertyManager(QtDateTimePropertyManager *manager);
    QWidget *createEditor(QtDateTimePropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtDateTimePropertyManager *manager);
private:
    QtDateTimeEditFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtDateTimeEditFactory)
    Q_DISABLE_COPY(QtDateTimeEditFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QDateTime &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QDateTime &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

/*!
    \class QtKeySequenceEditorFactory

    \brief The QtKeySequenceEditorFactory class provides editor
    widgets for properties created by QtKeySequencePropertyManager objects.

    \sa QtAbstractEditorFactory
*/
class QtKeySequenceEditorFactoryPrivate;
class QT_QTPROPERTYBROWSER_EXPORT QtKeySequenceEditorFactory : public QtAbstractEditorFactory<QtKeySequencePropertyManager>
{
    Q_OBJECT
public:
    QtKeySequenceEditorFactory(QObject *parent = 0);
    ~QtKeySequenceEditorFactory();
protected:
    void connectPropertyManager(QtKeySequencePropertyManager *manager);
    QWidget *createEditor(QtKeySequencePropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtKeySequencePropertyManager *manager);
private:
    QtKeySequenceEditorFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtKeySequenceEditorFactory)
    Q_DISABLE_COPY(QtKeySequenceEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QKeySequence &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QKeySequence &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

/*!
    \class QtCharEditorFactory

    \brief The QtCharEditorFactory class provides editor
    widgets for properties created by QtCharPropertyManager objects.

    \sa QtAbstractEditorFactory
*/
class QtCharEditorFactoryPrivate;
class QT_QTPROPERTYBROWSER_EXPORT QtCharEditorFactory : public QtAbstractEditorFactory<QtCharPropertyManager>
{
    Q_OBJECT
public:
    QtCharEditorFactory(QObject *parent = 0);
    ~QtCharEditorFactory();
protected:
    void connectPropertyManager(QtCharPropertyManager *manager);
    QWidget *createEditor(QtCharPropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtCharPropertyManager *manager);
private:
    QtCharEditorFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtCharEditorFactory)
    Q_DISABLE_COPY(QtCharEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QChar &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QChar &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

/*!
    \class QtEnumEditorFactory

    \brief The QtEnumEditorFactory class provides QComboBox widgets for
    properties created by QtEnumPropertyManager objects.

    \sa QtAbstractEditorFactory, QtEnumPropertyManager
*/
class QtEnumEditorFactoryPrivate;
class QT_QTPROPERTYBROWSER_EXPORT QtEnumEditorFactory : public QtAbstractEditorFactory<QtEnumPropertyManager>
{
    Q_OBJECT
public:
    QtEnumEditorFactory(QObject *parent = 0);
    ~QtEnumEditorFactory();
protected:
    void connectPropertyManager(QtEnumPropertyManager *manager);
    QWidget *createEditor(QtEnumPropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtEnumPropertyManager *manager);
private:
    QtEnumEditorFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtEnumEditorFactory)
    Q_DISABLE_COPY(QtEnumEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotTextChanged(QtProperty *, QString))
    Q_PRIVATE_SLOT(d_func(), void slotEnumNamesChanged(QtProperty *,
                        const QStringList &))
    Q_PRIVATE_SLOT(d_func(), void slotEnumIconsChanged(QtProperty *,
                        const QMap<int, QIcon> &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(int))
    Q_PRIVATE_SLOT(d_func(), void slotSetText(QtProperty*,QString)) //! mark
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))

    QLineEdit *m_lineEdit;
    QtProperty *m_property;

private slots:
    void editFinsh();
signals:
    void sendText(QtProperty *property,const QString &text);

};

/*!
    \class QtCursorEditorFactory

    \brief The QtCursorEditorFactory class provides QComboBox widgets for
    properties created by QtCursorPropertyManager objects.

    \sa QtAbstractEditorFactory, QtCursorPropertyManager
*/
class QtCursorEditorFactoryPrivate;
class QT_QTPROPERTYBROWSER_EXPORT QtCursorEditorFactory : public QtAbstractEditorFactory<QtCursorPropertyManager>
{
    Q_OBJECT
public:
    QtCursorEditorFactory(QObject *parent = 0);
    ~QtCursorEditorFactory();
protected:
    void connectPropertyManager(QtCursorPropertyManager *manager);
    QWidget *createEditor(QtCursorPropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtCursorPropertyManager *manager);
private:
    QtCursorEditorFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtCursorEditorFactory)
    Q_DISABLE_COPY(QtCursorEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QCursor &))
    Q_PRIVATE_SLOT(d_func(), void slotEnumChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

/*!
    \class QtColorEditorFactory

    \brief The QtColorEditorFactory class provides color editing  for
    properties created by QtColorPropertyManager objects.

    \sa QtAbstractEditorFactory, QtColorPropertyManager
*/
class QtColorEditorFactoryPrivate;
class QT_QTPROPERTYBROWSER_EXPORT QtColorEditorFactory : public QtAbstractEditorFactory<QtColorPropertyManager>
{
    Q_OBJECT
public:
    QtColorEditorFactory(QObject *parent = 0);
    ~QtColorEditorFactory();
protected:
    void connectPropertyManager(QtColorPropertyManager *manager);
    QWidget *createEditor(QtColorPropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtColorPropertyManager *manager);
private:
    QtColorEditorFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtColorEditorFactory)
    Q_DISABLE_COPY(QtColorEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QColor &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QColor &))
};

/*!
    \class QtFontEditorFactory

    \brief The QtFontEditorFactory class provides font editing for
    properties created by QtFontPropertyManager objects.

    \sa QtAbstractEditorFactory, QtFontPropertyManager
*/
class QtFontEditorFactoryPrivate;
class QT_QTPROPERTYBROWSER_EXPORT QtFontEditorFactory : public QtAbstractEditorFactory<QtFontPropertyManager>
{
    Q_OBJECT
public:
    QtFontEditorFactory(QObject *parent = 0);
    ~QtFontEditorFactory();
protected:
    void connectPropertyManager(QtFontPropertyManager *manager);
    QWidget *createEditor(QtFontPropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtFontPropertyManager *manager);
private:
    QtFontEditorFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtFontEditorFactory)
    Q_DISABLE_COPY(QtFontEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QFont &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QFont &))
};

#if QT_VERSION >= 0x040400
QT_END_NAMESPACE
#endif

#endif
