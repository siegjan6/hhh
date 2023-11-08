#include "DialogManager.h"
#include "qtpropertybrowserutils_p.h"

class CDialogManagerPrivate
{
    Q_DECLARE_PUBLIC(CDialogManager)
    CDialogManager *q_ptr;
public:

    struct Data
    {
        QString     value;
        QPixmap     pixmap;
    };

    QMap<const QtProperty *, Data> m_values;
};

CDialogManager::CDialogManager(QObject *parent):
    QtAbstractPropertyManager(parent)
{
    d_ptr = new CDialogManagerPrivate;
    d_ptr->q_ptr = this;
}

CDialogManager::~CDialogManager()
{
    clear();
    delete d_ptr;
}


QString CDialogManager::value(const QtProperty *property) const
{
    if (!d_ptr->m_values.contains(property))
        return QString();
    return d_ptr->m_values[property].value;
}

QPixmap CDialogManager::pixmap(const QtProperty *property) const
{
    if(!d_ptr->m_values.contains(property))
        return QPixmap();
    return d_ptr->m_values[property].pixmap;
}

void CDialogManager::slotSetValue(QtProperty *property, const QString &val)
{
    if (!d_ptr->m_values.contains(property))
        return;

    CDialogManagerPrivate::Data data = d_ptr->m_values[property];

    if (data.value == val)
        return;

    data.value = val;

    d_ptr->m_values[property] = data;

    emit propertyChanged(property);
    emit valueChanged(property, data.value);
}


void CDialogManager::slotSetIcon(QtProperty *property, const QPixmap &img)
{
    if(!d_ptr->m_values.contains(property))
        return;

    CDialogManagerPrivate::Data data = d_ptr->m_values[property];
    data.pixmap = img;
    d_ptr->m_values[property] = data;

    emit propertyChanged(property);
}




void CDialogManager::initializeProperty(QtProperty *property)
{
    d_ptr->m_values[property].pixmap =  QtPropertyBrowserUtils::brushValuePixmap(QBrush(QColor()));
}

void CDialogManager::uninitializeProperty(QtProperty *property)
{
    d_ptr->m_values.remove(property);
}
#include "moc_DialogManager.cpp"
