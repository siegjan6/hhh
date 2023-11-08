/*!

\author eHVr

\section 功能
    提供并管理变体类型属性。
    继承自QtVariantPropertyManager，添加了对扩展类型的处理，
    其他类的处理，仍采用基类函数
*/
#ifndef VARIANTMANAGER_H
#define VARIANTMANAGER_H

#include "qtvariantproperty.h"

class CVariantManagerPrivate;

class QT_QTPROPERTYBROWSER_EXPORT CVariantManager : public QtVariantPropertyManager
{
    Q_OBJECT
public:
    CVariantManager(QObject *parent = 0);
    virtual ~CVariantManager();

public:
    virtual QVariant value(const QtProperty *property) const;
    virtual int valueType(int propertyType) const;
    virtual bool isPropertyTypeSupported(int propertyType) const;

    QIcon valueIcon(const QtProperty *property) const;
    static int dialogTypeId();
public Q_SLOTS:
    virtual void setValue(QtProperty *property, const QVariant &val);

protected:
    QString valueText(const QtProperty *property) const;

    virtual void initializeProperty(QtProperty *property);
    virtual void uninitializeProperty(QtProperty *property);
private:
    CVariantManagerPrivate * d_ptr;
    Q_DECLARE_PRIVATE(CVariantManager)
};



#endif
