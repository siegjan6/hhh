/*!
\author aGRw 2015.06.03
\section 成组控件属性类
成组控件的属性类需要分为普通属性类和矢量属性类
*/
#ifndef MDPROPERTYDRAWGROUP_H
#define MDPROPERTYDRAWGROUP_H

#include "MDPropertyDrawVector.h"

/*!
\section 普通属性类
*/
class MDDRAWOBJSHARED_EXPORT CMDPropertyDrawGroup : public CMDPropertyDrawObj
{
    Q_OBJECT

/*!
\section 分组
*/
    Q_PROPERTY(QString GUID_Variable)
    Q_PROPERTY(QString DILcustomProperty READ customProperty WRITE setCustomProperty)

//////////////////////////////////////////////////////////////////////////
public:
    CMDPropertyDrawGroup(CMDDrawObj* parent);
    virtual ~CMDPropertyDrawGroup(){}
//////////////////////////////////////////////////////////////////////////
public:
    void setCustomProperty(const QString&);
    QString customProperty();
//////////////////////////////////////////////////////////////////////////
protected:
    CMDDrawGroup *group();
};

/*!
\section 矢量属性类
*/
class MDDRAWOBJSHARED_EXPORT CMDPropertyDrawGroupVector : public CMDPropertyDrawVector
{
    Q_OBJECT
/*!
\section 分组
*/
    Q_PROPERTY(QString GUID_Variable)
    Q_PROPERTY(QString DILcustomProperty READ customProperty WRITE setCustomProperty)

    Q_PROPERTY(QString $DILbrush)
    Q_PROPERTY(QString $DILline)
    Q_PROPERTY(QString $DILdynamicBrush)
    Q_PROPERTY(QString $DILdynamicLine)
    Q_PROPERTY(QString $DILfill)
//////////////////////////////////////////////////////////////////////////
public:
    CMDPropertyDrawGroupVector(CMDDrawObj* parent);
    virtual ~CMDPropertyDrawGroupVector(){}
//////////////////////////////////////////////////////////////////////////
public:
    void setCustomProperty(const QString&);
    QString customProperty();
//////////////////////////////////////////////////////////////////////////
protected:
    CMDDrawGroup *group();
};

#endif // MDPROPERTYDRAWGROUP_H












