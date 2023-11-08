/*!

\author eHVr

\section 功能

 用于创建和管理扩展类型的属性。
*/
#ifndef DIALOGMANAGER_H
#define DIALOGMANAGER_H

#include "qtpropertybrowser.h"
#include <QMap>
#include <QIcon>

class CDialogManagerPrivate;
class QT_QTPROPERTYBROWSER_EXPORT CDialogManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    CDialogManager(QObject *parent = 0);
    virtual ~CDialogManager();

    QString value(const QtProperty *property) const;
    QPixmap pixmap(const QtProperty *property) const;

public Q_SLOTS:
    void slotSetValue(QtProperty *property, const QString &val);
    void slotSetIcon(QtProperty *property, const QPixmap &img);
Q_SIGNALS:
    void valueChanged(QtProperty *property, const QString &val);
protected:
    virtual QString valueText(const QtProperty *property) const {return value(property);}
    virtual QIcon valueIcon(const QtProperty *property) const {return QIcon(pixmap(property));}
    virtual void initializeProperty(QtProperty *property);
    virtual void uninitializeProperty(QtProperty *property);
private:
    CDialogManagerPrivate *d_ptr;
    Q_DECLARE_PRIVATE(CDialogManager)
};
#endif
