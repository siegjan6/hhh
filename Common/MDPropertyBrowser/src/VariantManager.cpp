#include "VariantManager.h"
#include "qtpropertybrowserutils_p.h"

class CVariantManagerPrivate
{
    CVariantManager *q_ptr;
    Q_DECLARE_PUBLIC(CVariantManager)
public:
    struct Data {
        QString     value;
        QPixmap     pixmap;
    };
    typedef  QMap<const QtProperty *, Data> PropertyValueMap;
    PropertyValueMap m_values;
};

CVariantManager::CVariantManager(QObject *parent)
    : QtVariantPropertyManager(parent)
{
    d_ptr = new CVariantManagerPrivate;
    d_ptr->q_ptr = this;
}

CVariantManager::~CVariantManager()
{
    delete d_ptr;
}

class CDialogPropertyType
{
};

Q_DECLARE_METATYPE(CDialogPropertyType)

int CVariantManager::dialogTypeId()
{
    return qMetaTypeId<CDialogPropertyType>();
}

QVariant CVariantManager::value(const QtProperty *property) const
{
    if (d_ptr->m_values.contains(property))
        return d_ptr->m_values[property].value;
    return QtVariantPropertyManager::value(property);
}

int CVariantManager::valueType(int propertyType) const
{
    if (propertyType == dialogTypeId())
        return QVariant::String;
    return QtVariantPropertyManager::valueType(propertyType);
}

bool CVariantManager::isPropertyTypeSupported(int propertyType) const
{
    if (propertyType == dialogTypeId())
        return true;
    return QtVariantPropertyManager::isPropertyTypeSupported(propertyType);
}

QString CVariantManager::valueText(const QtProperty *property) const
{
    if (d_ptr->m_values.contains(property))
        return d_ptr->m_values[property].value;
    return QtVariantPropertyManager::valueText(property);
}

QIcon CVariantManager::valueIcon(const QtProperty *property) const
{
    if(d_ptr->m_values.contains(property))
        return QIcon(d_ptr->m_values.value(property).pixmap);
    return  QtVariantPropertyManager::valueIcon(property);
}

void CVariantManager::setValue(QtProperty *property, const QVariant &val)
{
    if(d_ptr->m_values.contains(property))
    {
        CVariantManagerPrivate::Data d = d_ptr->m_values[property];
        if(val.canConvert(QVariant::String))
        {
            QString str = val.toString();

            d.value = str;
            d_ptr->m_values[property] = d;
            emit propertyChanged(property);
            emit valueChanged(property, str);
            property->setSource(IMDIDECHANGE);  // to set the Extension property source inside
                                                 // without this statement,the dialog would not show next time
            return;
        }
        if(val.canConvert(QVariant::Pixmap))
        {
            QPixmap pixmap = val.value<QPixmap>();
            if(pixmap.isNull())
                return;
            d.pixmap = pixmap;
            d_ptr->m_values[property] = d;
            emit propertyChanged(property);
            emit valueChanged(property,QVariant::fromValue(pixmap));
            return;
        }
    }

    QtVariantPropertyManager::setValue(property, val);
}

void CVariantManager::initializeProperty(QtProperty *property)
{

    if (propertyType(property) == dialogTypeId()){
        d_ptr->m_values[property] = CVariantManagerPrivate::Data();

        // initializing the propoerty with icon.
//        d_ptr->m_values[property].pixmap =  QtPropertyBrowserUtils::brushValuePixmap(QBrush(QColor()));
    }
    QtVariantPropertyManager::initializeProperty(property);
}

void CVariantManager::uninitializeProperty(QtProperty *property)
{
    d_ptr->m_values.remove(property);
    QtVariantPropertyManager::uninitializeProperty(property);
}



#include "moc_VariantManager.cpp"
