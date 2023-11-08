#include "PropertyBrowser_p.h"
#include "MDMultiLanguage.h"
#include <QTextEdit>
#include <QDebug>
#include <QTreeWidget>

void CPropertyBrowserPrivate::setProperty(const QString &name, const QVariant &value)
{
    if(name.isEmpty() || !m_object || value.isNull())
        return;

    QtProperty * _property = 0;
    for(int idx = 0; idx < m_object->metaObject()->propertyCount();idx++)
    {
        QMetaProperty metaProperty =  m_object->metaObject()->property(idx);
        QString propertyName = metaProperty.name();
        if(name == propertyName)
        {
           _property = m_indexToProperty.value(idx);
           m_manager->setValue(_property,value);
           return;
        }
    }
}

void CPropertyBrowserPrivate::setPropertyHide(const QStringList &list, bool isHide)
{
    if(list.isEmpty())
        return;
    for(int nameIndex = 0; nameIndex< list.count(); nameIndex++)
    {
        QString groupName = list.at(nameIndex);
        if(groupName.startsWith("GUID_"))
        {
            groupName = groupName.remove(0,5);
        }
        QString preString = findTranslate(groupName);
        QtTreePropertyBrowser *browser = qobject_cast<QtTreePropertyBrowser*>(m_browser);
        QList<QtBrowserItem*> items = m_browser->topLevelItems();
        for(int i = 0; i<items.count();i++)
        {
            QString itemName = items.at(i)->property()->propertyName();
            if(itemName == preString)
            {
                 browser->setItemVisible(items.at(i),!isHide);
                 for(int j = 0; j<items.at(i)->children().count(); j++)
                 {
                     browser->setItemVisible(items.at(i)->children().at(j),!isHide);
                 }
            }
        }
        preString = groupName;
        for(int idx = 0; idx < m_object->metaObject()->propertyCount();idx++)
        {
            QMetaProperty metaProperty =  m_object->metaObject()->property(idx);
            QtProperty * property = 0;
            QString propertyName = metaProperty.name();
            if(preString == propertyName)
            {
               property = m_indexToProperty.value(idx);
               QList<QtBrowserItem*> browserItem = m_browser->items(property);
               for(int j = 0;j< browserItem.count();j++)
               {
                  browser->setItemVisible(browserItem.at(j),!isHide);
               }
            }
        }
    }
}

void CPropertyBrowserPrivate::setSinglePropertyHide(const QString &str, bool isHide)
{
    QString groupName = str;
    if(groupName.startsWith("GUID_"))
    {
        groupName = groupName.remove(0,5);
    }
    QString preString = findTranslate(groupName);
    QtTreePropertyBrowser *browser = qobject_cast<QtTreePropertyBrowser*>(m_browser);
    QList<QtBrowserItem*> items = m_browser->topLevelItems();
    for(int i = 0; i<items.count();i++)
    {
        QString itemName = items.at(i)->property()->propertyName();
        if(itemName == preString)
        {
             browser->setItemVisible(items.at(i),!isHide);
             for(int j = 0; j<items.at(i)->children().count(); j++)
             {
                 browser->setItemVisible(items.at(i)->children().at(j),!isHide);
             }
        }
    }

    for(int idx = 0; idx < m_object->metaObject()->propertyCount();idx++)
    {
        QMetaProperty metaProperty =  m_object->metaObject()->property(idx);
        QtProperty * property = 0;
        QString propertyName = metaProperty.name();
        QString translateName = findTranslate(propertyName);
        if(preString == translateName)
        {
           property = m_indexToProperty.value(idx);
           QList<QtBrowserItem*> browserItem = m_browser->items(property);
           for(int j = 0;j< browserItem.count();j++)
           {
              browser->setItemVisible(browserItem.at(j),!isHide);
           }
        }
    }
}

void CPropertyBrowserPrivate::setReadOnlyBackColor(const QColor &color)
{
    QtTreePropertyBrowser *browser = qobject_cast<QtTreePropertyBrowser*>(m_browser);
    for(int idx = 0; idx < m_object->metaObject()->propertyCount();idx++)
    {
        QMetaProperty metaProperty =  m_object->metaObject()->property(idx);
        QtProperty * property = m_indexToProperty.value(idx);
        if(!metaProperty.isWritable())
        {
          QList<QtBrowserItem*> browserItem = m_browser->items(property);
          for(int j = 0;j< browserItem.count();j++)
          {
             browser->setBackgroundColor(browserItem.at(j),color);
          }
        }
    }
}

void CPropertyBrowserPrivate::setPropertyBackColor(const QString &name, const QColor &color)
{
    QString groupName = name;
    if(groupName.startsWith("GUID_"))
    {
        groupName = groupName.remove(0,5);
    }
    QtTreePropertyBrowser *browser = qobject_cast<QtTreePropertyBrowser*>(m_browser);
    QList<QtBrowserItem*> items = m_browser->topLevelItems();
    for(int i = 0; i<items.count();i++)
    {
        QString itemName = items.at(i)->property()->propertyName();
        if(itemName == groupName)
        {
             browser->setBackgroundColor(items.at(i),color);
//             for(int j = 0; j<items.at(i)->children().count(); j++)
//             {
//                 browser->setBackgroundColor(items.at(i)->children().at(j),color);
//             }
        }
    }

    for(int idx = 0; idx < m_object->metaObject()->propertyCount();idx++)
    {
        QMetaProperty metaProperty =  m_object->metaObject()->property(idx);
        QtProperty * property = 0;
        QString propertyName = metaProperty.name();
        if(groupName == propertyName)
        {
           property = m_indexToProperty.value(idx);
           QList<QtBrowserItem*> browserItem = m_browser->items(property);
           for(int j = 0;j< browserItem.count();j++)
           {
              browser->setBackgroundColor(browserItem.at(j),color);
           }
        }
    }
}

void CPropertyBrowserPrivate::setPropertyReadOnly(bool isReadOnly,const QStringList &strList)
{
    if(!m_object)
        return;
    if(strList.isEmpty())
    {
        QtTreePropertyBrowser *browser = qobject_cast<QtTreePropertyBrowser*>(m_browser);
        QList<QtBrowserItem*> items = browser->topLevelItems();
        for(int i = 0; i < items.count(); i++)
        {
            QtBrowserItem* item = items.at(i);
            item->property()->setEnabled(!isReadOnly); //! 设置整体只读
        }
    }
    else
    {
        QtProperty * property = 0;
        for(int i = 0; i<strList.count();i++)
        {
            QString name = strList.at(i);
            if(name.startsWith("GUID_"))
            {
                name = name.remove(0,5);
            }
            for(int idx = 0; idx < m_object->metaObject()->propertyCount();idx++)
            {
                QMetaProperty metaProperty =  m_object->metaObject()->property(idx);
                QString propertyName = metaProperty.name();
                if(name == propertyName)
                {
                   property = m_indexToProperty.value(idx);
                   property->setEnabled(!isReadOnly);
                   break;
                }
            }
        }
    }
}

void CPropertyBrowserPrivate::resetEnums(const QString &name, const QStringList &list)
{
    for(int idx = 0; idx < m_object->metaObject()->propertyCount();idx++)
    {
        QMetaProperty metaProperty =  m_object->metaObject()->property(idx);
        QString propertyName = metaProperty.name();
        if(name == propertyName)
        {
           if(!metaProperty.isEnumType())
               return;
           QtProperty * property = 0;
           property = m_indexToProperty.value(idx);
           QtVariantProperty *variant = m_manager->variantProperty(property);
           QMetaEnum metaEnum = metaProperty.enumerator();

            //setAttribute会把值清零，所以要先读取值
            int value = metaProperty.read(m_object).toInt();
           variant->setAttribute(QLatin1String("enumNames"), list);
           variant->setValue(enumToInt(metaEnum, value));

           break;
        }
    }
}

QString CPropertyBrowserPrivate::getCurrentEnumText(const QString &name)
{
    for(int idx = 0; idx < m_object->metaObject()->propertyCount();idx++)
    {
        QMetaProperty metaProperty =  m_object->metaObject()->property(idx);
        QString propertyName = metaProperty.name();
        if(name == propertyName)
        {
            if(metaProperty.isEnumType())
            {
                QtProperty * property = 0;
                property = m_indexToProperty.value(idx);
                QtVariantProperty *variant = m_manager->variantProperty(property);
                return variant->valueText();
            }
        }
    }
    return QString();
}

void CPropertyBrowserPrivate::setExpandAll()
{
    QtTreePropertyBrowser *browser = qobject_cast<QtTreePropertyBrowser*>(m_browser);
    QList<QtBrowserItem*> items = m_browser->topLevelItems();

    for(int i = 0; i < items.count(); i++)
    {
        QtBrowserItem* item = items.at(i);
        browser->setExpanded(item, true);
    }
}

void CPropertyBrowserPrivate::setPropertyTipMap(const QMap<QString, QVariant> &propertyTip)
{
    m_propertyToTip = propertyTip;
}

void CPropertyBrowserPrivate::setWhatsThis(const QString &name)
{
    for(int idx = 0; idx < m_object->metaObject()->propertyCount();idx++)
    {
        QMetaProperty metaProperty =  m_object->metaObject()->property(idx);
        QString propertyName = metaProperty.name();
        if(name == propertyName)
        {
            QtProperty * property = 0;
            property = m_indexToProperty.value(idx);
            property->setSource(2);
        }
    }
}

void CPropertyBrowserPrivate::setCurrentPropertyName(const QString &str)
{
    m_currentPropertyName = str;
}

QString CPropertyBrowserPrivate::currentPropertyName() const
{
    return m_currentPropertyName;
}

int CPropertyBrowserPrivate::enumToInt(const QMetaEnum &metaEnum, int enumValue) const
{
    QMap<int, int> valueMap; // dont show multiple enum values which have the same values
    int pos = 0;
    for (int i = 0; i < metaEnum.keyCount(); i++)
    {
        int value = metaEnum.value(i);
        if (!valueMap.contains(value)) {
            if (value == enumValue)
                return pos;
            valueMap[value] = pos++;
        }
    }

    //support dynamic enum  by aGRw
    return enumValue;
    //return -1;
}

int CPropertyBrowserPrivate::intToEnum(const QMetaEnum &metaEnum, int intValue) const
{
    QMap<int, bool> valueMap; // dont show multiple enum values which have the same values
    QList<int> values;
    for (int i = 0; i < metaEnum.keyCount(); i++) {
        int value = metaEnum.value(i);
        if (!valueMap.contains(value)) {
            valueMap[value] = true;
            values.append(value);
        }
    }

    if (intValue >= values.count())
        //support dynamic enum  by aGRw
        return intValue;
        //return -1;

    return values.at(intValue);
}

bool CPropertyBrowserPrivate::isSubValue(int value, int subValue) const
{
    if (value == subValue)
        return true;
    int i = 0;
    while (subValue) {
        if (!(value & (1 << i))) {
            if (subValue & 1)
                return false;
        }
        i++;
        subValue = subValue >> 1;
    }
    return true;
}

bool CPropertyBrowserPrivate::isPowerOf2(int value) const
{
    while (value) {
        if (value & 1) {
            return value == 1;
        }
        value = value >> 1;
    }
    return false;
}

QString CPropertyBrowserPrivate::findTranslate(const QString &propertyName) const
{
    if(m_translateName.contains(propertyName)){
        return translateByMutli(m_translateName.value(propertyName).toString());
    }
    return propertyName;
}

QString CPropertyBrowserPrivate::translateByMutli(const QString &propertyName) const
{
    return CMDMultiLanguage::instance()->value(propertyName).isEmpty() ? propertyName : CMDMultiLanguage::instance()->value(propertyName);
}

void CPropertyBrowserPrivate::ExpandFunction(QtBrowserItem *item, bool isExpand)
{
    if(!m_needSaveExpandState)
        return;
    if(item->parent() != NULL)
        return;

    m_expandDict.insert(item->property()->propertyName(), isExpand);
}
void CPropertyBrowserPrivate::initExpand()
{
    QtTreePropertyBrowser *browser = qobject_cast<QtTreePropertyBrowser*>(m_browser);
    QList<QtBrowserItem*> items = m_browser->topLevelItems();

    for(int i = 0; i < items.count(); i++)
    {
//        browser->setBackgroundColor(items.at(i)->children().at(0),Qt::red);//! 属性设置背景色
//        browser->setItemVisible(items.at(i)->children().at(0),false); //! 设置属性隐藏

        QtBrowserItem* item = items.at(i);
//        item->property()->setEnabled(false); //! 设置整体只读

        //! test (只读属性设置背景色)
//        for(int idx = 0; idx < m_object->metaObject()->propertyCount();idx++){
//            QMetaProperty metaProperty =  m_object->metaObject()->property(idx);
//            QtProperty * property = m_indexToProperty.value(idx);
//            if(!metaProperty.isWritable()){
//              QList<QtBrowserItem*> browserItem = m_browser->items(property);
//              for(int j = 0;j< browserItem.count();j++)
//              {
//                 browser->setBackgroundColor(browserItem.at(j),Qt::darkGray);
//                 browserItem.at(j)->property()->setToolTip("该属性只读");
//              }
//            }
//        }

        QString name = item->property()->propertyName();
        bool expand = false;
        if(m_expandDict.contains(name))
            expand = m_expandDict.value(name);
        browser->setExpanded(item, expand);
    }
}

void CPropertyBrowserPrivate::setCurrentPropertyTip(QtBrowserItem *item)
{
    QtTreePropertyBrowser *browser = qobject_cast<QtTreePropertyBrowser*>(m_browser);
    if(!item->property())
        return;
    QString name = item->property()->propertyName();
    if(m_translateName.values().contains(name))
    {
        name = m_translateName.key(name);
    }
    if(browser->topLevelItems().contains(item))
    {
        name = "GUID_"+ name;
    }
    setCurrentPropertyName(name);
    if(m_propertyToTip.contains(name))
    {
        QString tip= m_propertyToTip.value(name).toString();
        m_edit->setPlainText(tip);
    }
    else
        m_edit->setPlainText("");

}
int CPropertyBrowserPrivate::flagToInt(const QMetaEnum &metaEnum, int flagValue) const
{
    if (!flagValue)
        return 0;
    int intValue = 0;
    QMap<int, int> valueMap; // dont show multiple enum values which have the same values
    int pos = 0;
    for (int i = 0; i < metaEnum.keyCount(); i++) {
        int value = metaEnum.value(i);
        if (!valueMap.contains(value) && isPowerOf2(value)) {
            if (isSubValue(flagValue, value))
                intValue |= (1 << pos);
            valueMap[value] = pos++;
        }
    }
    return intValue;
}

int CPropertyBrowserPrivate::intToFlag(const QMetaEnum &metaEnum, int intValue) const
{
    QMap<int, bool> valueMap; // dont show multiple enum values which have the same values
    QList<int> values;
    for (int i = 0; i < metaEnum.keyCount(); i++) {
        int value = metaEnum.value(i);
        if (!valueMap.contains(value) && isPowerOf2(value)) {
            valueMap[value] = true;
            values.append(value);
        }
    }
    int flagValue = 0;
    int temp = intValue;
    int i = 0;
    while (temp) {
        if (i >= values.count())
            return -1;
        if (temp & 1)
            flagValue |= values.at(i);
        i++;
        temp = temp >> 1;
    }
    return flagValue;
}

void CPropertyBrowserPrivate::updateClassProperties(const QMetaObject *metaObject, bool recursive)
{
    if (!metaObject)
        return;

    if (recursive)
        updateClassProperties(metaObject->superClass(), recursive);

    QString classProperty = m_classToProperty.value(metaObject);
    if(classProperty.isEmpty())
        return;

    for (int idx = metaObject->propertyOffset(); idx < metaObject->propertyCount(); idx++)
    {
        QMetaProperty metaProperty = metaObject->property(idx);
        if (metaProperty.isReadable()) {
            if (m_classToIndexToProperty.contains(metaObject) && m_classToIndexToProperty[metaObject].contains(idx)) {
                QtVariantProperty *subProperty = m_classToIndexToProperty[metaObject][idx];
                subProperty->setSource(OUTSIDECHANGE);  // the source of property value.
                if (metaProperty.isEnumType())
                {
                    if (metaProperty.isFlagType())
                        subProperty->setValue(flagToInt(metaProperty.enumerator(), metaProperty.read(m_object).toInt()));
                    else
                        subProperty->setValue(enumToInt(metaProperty.enumerator(), metaProperty.read(m_object).toInt()));
                }
                else
                {
                     subProperty->setValue(metaProperty.read(m_object));

                }
            }
        }
    }
}

void CPropertyBrowserPrivate::addNormalProperty(int type,QMetaProperty &metaProperty, QtVariantProperty *&subProperty,const QString &propertyName,const QString &markText)
{
    // changed the QLatin1String(metaProperty.name()) to propertyName
    if (!metaProperty.isReadable())
    {  //read only property
        subProperty = m_readOnlyManager->addProperty(QVariant::String,propertyName,markText);
        subProperty->setValue(QLatin1String("< Non Readable >"));
    }
    else if (metaProperty.isEnumType())
    {
        if (metaProperty.isFlagType())
        {  //flag type
            subProperty = m_manager->addProperty(QtVariantPropertyManager::flagTypeId(),propertyName,markText);
            QMetaEnum metaEnum = metaProperty.enumerator();
            QMap<int, bool> valueMap;
            QStringList flagNames;
            for (int i = 0; i < metaEnum.keyCount(); i++)
            {
                int value = metaEnum.value(i);
                if (!valueMap.contains(value) && isPowerOf2(value))
                {
                    valueMap[value] = true;
                    flagNames.append(QLatin1String(metaEnum.key(i)));
                }
                subProperty->setAttribute(QLatin1String("flagNames"), flagNames);
                subProperty->setValue(flagToInt(metaEnum, metaProperty.read(m_object).toInt()));
            }
        } else
        {   // enum type
            subProperty = m_manager->addProperty(QtVariantPropertyManager::enumTypeId(),propertyName,markText);
            QMetaEnum metaEnum = metaProperty.enumerator();
            QMap<int, bool> valueMap; // dont show multiple enum values which have the same values
            QStringList enumNames;
            for (int i = 0; i < metaEnum.keyCount(); i++)
            {
                int value = metaEnum.value(i);
                if (!valueMap.contains(value))
                {
                    valueMap[value] = true;
                    enumNames.append(QLatin1String(metaEnum.key(i)));//! multi language
                }
            }
            // to show the translated enumname
            QStringList enumVisibleNames;
            for(int i = 0; i < enumNames.count(); i++)
            {
                enumVisibleNames.append(findTranslate(enumNames.at(i)));
            }
            subProperty->setAttribute(QLatin1String("enumNames"), enumVisibleNames);
            subProperty->setValue(enumToInt(metaEnum, metaProperty.read(m_object).toInt()));
        }
    } else if (m_manager->isPropertyTypeSupported(type))
    {
        if (!metaProperty.isWritable())
            subProperty = m_readOnlyManager->addProperty(type,propertyName,markText);
        else if (!metaProperty.isDesignable())
            subProperty = m_readOnlyManager->addProperty(type,propertyName + QLatin1String(" (Non Designable)"),markText);
        else
            subProperty = m_manager->addProperty(type,propertyName,markText);
        subProperty->setValue(metaProperty.read(m_object));
    } else
    { // the type is not supported
        subProperty = m_readOnlyManager->addProperty(QVariant::String,propertyName,markText);
        subProperty->setValue(QLatin1String("< Unknown Type >"));
        subProperty->setEnabled(false);
    }
}

void CPropertyBrowserPrivate::classPropertyNotExist(const QMetaObject *metaObject, QString className)
{
    className = metaObject->className();
    m_classToProperty[metaObject] = className;

    QtProperty * groupProperty = 0;

    for (int idx = metaObject->propertyOffset(); idx < metaObject->propertyCount(); idx++)
    {
        QMetaProperty metaProperty = metaObject->property(idx);
        int type = metaProperty.userType();
        QtVariantProperty *subProperty = 0;
        QString propertyName = metaProperty.name();

        if(propertyName == QString("translateName")){
            m_translateName = metaProperty.read(m_object).toMap();
            for(int i = 0; i<m_translateName.keys().count(); i++)
            {
                if(m_translateName.keys().at(i).startsWith("GUID_"))
                {
                    QString str =  m_translateName.keys().at(i);
                    str = str.remove(0,5);
                    QVariant value = m_translateName.value(m_translateName.keys().at(i));
                    m_translateName.insert(str,value);
                }
            }
            continue;
        }
        if(propertyName == QString("propertyTip"))
        {
            m_propertyToTip = metaProperty.read(m_object).toMap();
        }
        // add group property
        if(propertyName.startsWith("GUID_"))
        {  //不展开的分组
            propertyName = propertyName.remove(0,5);
            groupProperty = m_propertyMap.value(propertyName);
            m_propertyToExpanded[groupProperty] = false;
            m_propertyToVisible[groupProperty] = true;

            // to show the translated group name
            QString visibleName = propertyName;
            visibleName = findTranslate(visibleName);// translate
            if(!groupProperty){
                subProperty  = m_manager->addProperty(QtVariantPropertyManager::groupTypeId(),visibleName);
                groupProperty = subProperty;
                m_propertyMap[propertyName] = groupProperty;
                m_propertyToExpanded[groupProperty] = false;
                m_propertyToVisible[groupProperty] = true;
            }
            continue;
        }

        if(propertyName.startsWith("EX_GUID_"))
        {  //展开的分组
            propertyName = propertyName.remove(0,8);
            groupProperty = m_propertyMap.value(propertyName);
            m_propertyToExpanded[groupProperty] = true;
            m_propertyToVisible[groupProperty] = true;

            // to show the translated group name
            QString visibleName = propertyName;
            visibleName = findTranslate(visibleName);// translate
            if(!groupProperty){
                subProperty  = m_manager->addProperty(QtVariantPropertyManager::groupTypeId(),visibleName);
                groupProperty = subProperty;
                m_propertyMap[propertyName] = groupProperty;
                m_propertyToExpanded[groupProperty] = true;
                m_propertyToVisible[groupProperty] = true;
            }
            continue;
        }

        if(propertyName.startsWith("N_GUID_"))
        {  //不显示的分组
            propertyName = propertyName.remove(0,7);
            groupProperty = m_propertyMap.value(propertyName);
            m_propertyToVisible[groupProperty] = false;
            m_propertyToExpanded[groupProperty] = false;
            // to show the translated group name
            QString visibleName = propertyName;
            visibleName = findTranslate(visibleName);// translate
            if(!groupProperty){
                subProperty = m_manager->addProperty(QtVariantPropertyManager::groupTypeId(),visibleName);
                groupProperty = subProperty;
                m_propertyMap[propertyName] = groupProperty;
                m_propertyToVisible[groupProperty] = false;
                m_propertyToExpanded[groupProperty] = false;
            }
            continue;
        }

        //不添加隐藏属性
        if(propertyName.startsWith('$'))
            continue;
        if(m_hideDict.contains(propertyName.toLower()))
            continue;

        if(propertyName.left(3) == "DIL")
        { // add extenion property
            propertyName = propertyName.remove(0,3);
            propertyName = findTranslate(propertyName);// translate
            subProperty = m_manager->addProperty(CVariantManager::dialogTypeId(),propertyName);
            subProperty->setValue(metaProperty.read(m_object));
        }
        else
        {
            QString markText = propertyName;
            propertyName = findTranslate(propertyName);// translat
            addNormalProperty(type,metaProperty,subProperty,propertyName,markText);
        }

        if(groupProperty)
        {
            groupProperty->addSubProperty(subProperty);
        }

        m_propertyToIndex[subProperty] = idx;
        m_indexToProperty[idx] = subProperty;
        m_classToIndexToProperty[metaObject][idx] = subProperty;
    }
}

void CPropertyBrowserPrivate::previewClassProperties(const QMetaObject *metaObject)
{
    if (!metaObject)
        return;
    if(QLatin1String(metaObject->className()) == QString("QObject"))
        return;

    previewClassProperties(metaObject->superClass());
    getHideClassProperties(metaObject);
}
void CPropertyBrowserPrivate::getHideClassProperties(const QMetaObject *metaObject)
{
    for (int i=metaObject->propertyOffset();i<metaObject->propertyCount();i++)
    {
        QString name = metaObject->property(i).name();
        if(name.startsWith('$'))
            m_hideDict.insert(name.remove(0, 1).toLower(), 0);
        else
            continue;
    }
}
void CPropertyBrowserPrivate::addClassProperties(const QMetaObject *metaObject)
{
    if (!metaObject)
        return;
    if(QLatin1String(metaObject->className()) == QString("QObject"))
        return;

    addClassProperties(metaObject->superClass());

    QString className = m_classToProperty.value(metaObject);
    if (className.isEmpty()) {
        classPropertyNotExist(metaObject,className);
    } else {
        updateClassProperties(metaObject, false);
    }
}



//! 重新加载之前的扩展状态，切换对象时，保持焦点在相同属性上。
void CPropertyBrowserPrivate::restoreExpandedState()
{
    initExpand();
}

void CPropertyBrowserPrivate::slotValueChanged(QtProperty *property, const QVariant &value)
{
    if(property->source() == OUTSIDECHANGE)
        return;

    if (!m_propertyToIndex.contains(property))
        return;

    int idx = m_propertyToIndex.value(property);

    const QMetaObject *metaObject = m_object->metaObject();
    QMetaProperty metaProperty = metaObject->property(idx);
    if (metaProperty.isEnumType()) {
        if (metaProperty.isFlagType())
            metaProperty.write(m_object, intToFlag(metaProperty.enumerator(), value.toInt()));
        else
            metaProperty.write(m_object, intToEnum(metaProperty.enumerator(), value.toInt()));
    } else {
        metaProperty.write(m_object, value);
    }

    updateClassProperties(metaObject, true);
}

void CPropertyBrowserPrivate::addPropertiesToBrowser()
{
    if(m_propertyMap.isEmpty())
        return;
    QMap<QString,QtProperty *>::ConstIterator iterator = m_propertyMap.constBegin();
    while(iterator != m_propertyMap.constEnd()){
        m_browser->addProperty(iterator.value());
        iterator++;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CPropertyBrowser::CPropertyBrowser(QWidget *parent)
    : QWidget(parent)
{
    d_ptr = new CPropertyBrowserPrivate;
    d_ptr->q_ptr = this;

    d_ptr->m_object = 0;

/*
    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);

    d_ptr->m_browser = new QtGroupBoxPropertyBrowser(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(scroll);
    scroll->setWidget(d_ptr->m_browser);
*/
    QtTreePropertyBrowser *browser = new QtTreePropertyBrowser(this);
    browser->setRootIsDecorated(true);

    browser->setAlternatingRowColors(true);
    d_ptr->m_browser = browser;
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(d_ptr->m_browser);
    QTextEdit* textEdit = new QTextEdit(this);
    d_ptr->m_edit = textEdit;
    d_ptr->m_edit->setReadOnly(true);
    d_ptr->m_edit->setFixedHeight(50);
    layout->addWidget(d_ptr->m_edit);

    d_ptr->m_readOnlyManager = new CVariantManager(this);
    d_ptr->m_manager = new CVariantManager(this);
    d_ptr->m_factory = new CVariantFactory(this);
    d_ptr->m_browser->setFactoryForManager(d_ptr->m_manager, d_ptr->m_factory);
    connect(d_ptr->m_manager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                this, SLOT(slotValueChanged(QtProperty *, const QVariant &)));

    d_ptr->m_needSaveExpandState = false;
    connect(browser, &QtTreePropertyBrowser::collapsed, this,
            &CPropertyBrowser::collaps);
    connect(browser, &QtTreePropertyBrowser::expanded, this,
            &CPropertyBrowser::expand);
    connect(browser,&QtTreePropertyBrowser::currentItemChanged,this,&CPropertyBrowser::setCurrentPropertyTip);

}

CPropertyBrowser::~CPropertyBrowser()
{
    delete d_ptr;
}

void CPropertyBrowser::setObject(QObject *object)
{
    if (d_ptr->m_object == object)
        return;

    // clear the pre-object information
    if (d_ptr->m_object)
    {
        QMap<QString,QtProperty *>::ConstIterator iterator = d_ptr->m_propertyMap.constBegin();
        while(iterator != d_ptr->m_propertyMap.constEnd())
        {
            d_ptr->m_browser->removeProperty(iterator.value());
            iterator++;
        }
        d_ptr->m_propertyMap.clear();
        d_ptr->m_translateName.clear();
        d_ptr->m_classToProperty.clear();
        d_ptr->m_hideDict.clear();
        d_ptr->m_propertyToTip.clear();
    }

    d_ptr->m_object = object;
    if (!d_ptr->m_object)
        return;

    d_ptr->m_needSaveExpandState = false;
    d_ptr->previewClassProperties(d_ptr->m_object->metaObject());

    d_ptr->addClassProperties(d_ptr->m_object->metaObject());
    d_ptr->addPropertiesToBrowser();

    d_ptr->m_factory->setTranslate(d_ptr->m_translateName);//!mark

    d_ptr->restoreExpandedState();
    d_ptr->m_needSaveExpandState = true;
    emit propertyFinish();
}

QObject *CPropertyBrowser::object() const
{
    return d_ptr->m_object;
}

void CPropertyBrowser::setPropertyAttribute( const QString &propertyName, const QString &attribute, const QVariant &value)
{
    if(propertyName.isEmpty() || attribute.isEmpty() || value.isNull() || !d_ptr->m_object)
        return;

    QtProperty * _property = 0;
    for(int idx = 0; idx < d_ptr->m_object->metaObject()->propertyCount();idx++){
        QMetaProperty metaProperty =  d_ptr->m_object->metaObject()->property(idx);
        QString name = metaProperty.name();
        if(name == propertyName){
           _property = d_ptr->m_indexToProperty.value(idx);
           d_ptr->m_manager->setAttribute(_property,attribute,value);
           emit d_ptr->m_manager->propertyChanged(_property);
           return;
        }
    }
}

void CPropertyBrowser::setPropertyFont(const QString &propertyName, const QFont &propertyValue)
{
    d_ptr->setProperty(propertyName,/*propertyValue*/QVariant::fromValue(propertyValue));
}

void CPropertyBrowser::setPropertyPixmap(const QString &propertyName, const QPixmap &propertyPixmap)
{
    d_ptr->setProperty(propertyName,QVariant::fromValue(propertyPixmap));
}

void CPropertyBrowser::setPropertyHide(const QStringList &list, bool isHide)
{
    if(!d_ptr->m_object)
        return;
    d_ptr->setPropertyHide(list,isHide);
}

void CPropertyBrowser::setSinglePropertyHide(const QString &str, bool isHide)
{
    if(!d_ptr->m_object)
        return;
    d_ptr->setSinglePropertyHide(str,isHide);
}

void CPropertyBrowser::setReadOnlyBackColor(const QColor &color)
{
    if(!d_ptr->m_object)
        return;
    d_ptr->setReadOnlyBackColor(color);
}

void CPropertyBrowser::setPropertyTipMap(const QMap<QString, QVariant> &propertyTip)
{
    if(!d_ptr->m_object)
        return;
    d_ptr->setPropertyTipMap(propertyTip);
}

void CPropertyBrowser::setPropertyBackColor(const QString &name, const QColor &color)
{
    if(!d_ptr->m_object)
        return;
    d_ptr->setPropertyBackColor(name,color);
}

void CPropertyBrowser::setPropertyReadOnly(bool isReadOnly,const QStringList &strList)
{
    if(!d_ptr->m_object)
        return;
    d_ptr->setPropertyReadOnly(isReadOnly,strList);
}

void CPropertyBrowser::resetEnums(const QString &name, const QStringList &list)
{
    if(!d_ptr->m_object)
        return;
    d_ptr->resetEnums(name,list);
}

QString CPropertyBrowser::getCurrentPropertyName() const
{
    if(!d_ptr->m_object)
        return QString();
    return d_ptr->currentPropertyName();
}

//QString CPropertyBrowser::getCurrentEnumText(const QString &name)
//{
//    if(d_ptr->m_object)
//        return d_ptr->getCurrentEnumText(name);
//    return QString();
//}

void CPropertyBrowser::setExpandAll()
{
    if(!d_ptr->m_object)
        return;
    d_ptr->setExpandAll();
}

void CPropertyBrowser::updateProperty()
{
    if(!d_ptr->m_object)
        return;
    d_ptr->updateClassProperties(d_ptr->m_object->metaObject(),true);
}
void CPropertyBrowser::collaps(QtBrowserItem *item)
{
    expandFunction(item, false);
}

void CPropertyBrowser::expand(QtBrowserItem *item)
{
    expandFunction(item, true);
}
void CPropertyBrowser::expandFunction(QtBrowserItem *item, bool isExpand)
{
    d_ptr->ExpandFunction(item, isExpand);
}

void CPropertyBrowser::setCurrentPropertyTip(QtBrowserItem *item)
{
    if(!d_ptr->m_object)
        return;
    d_ptr->setCurrentPropertyTip(item);

}


#include "moc_PropertyBrowser.cpp"
