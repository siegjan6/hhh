/*!
\author aGRw
\section 控件选择管理,属性封装类
*/
#ifndef PROPERTYSELECTOBJECT_H
#define PROPERTYSELECTOBJECT_H

#include "SelectObjectManager.h"

class CPropertySelectObject : public QObject
{
public:
    Q_OBJECT

    Q_PROPERTY(QMap translateName READ translateName)

    Q_PROPERTY(QString GUID_Layout)
    Q_PROPERTY(QRectF rect READ rect WRITE setRect)

public:
    CPropertySelectObject(CSelectObjectManager* parent);
    virtual ~CPropertySelectObject(){}
/*!
\section property
*/
public:
    QRectF rect();
    void setRect(const QRectF& value);
/*!
\section field
*/
private:
    CSelectObjectManager* m_object;
/*!
\section translate
*/
public:
    QMap<QString,QVariant> translateName(){return m_translateMap;}
private:
    QMap<QString,QVariant> m_translateMap;
private:
    void initTranslateMap();
};

#endif // PROPERTYSELECTOBJECT_H
