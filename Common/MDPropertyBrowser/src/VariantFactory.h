/*!

\author eHVr

\section 功能

    变体型数据工程。为CVariantManager创建的属性提供widget，
    继承QtVariantEditorFactory，增加了对扩展属性的处理，覆盖了基类的接口
    其他类型的处理依然使用基类函数。
*/
#ifndef VARIANTFACTORY_H
#define VARIANTFACTORY_H

#include "qtvariantproperty.h"
#include "DialogManager.h"

class CDialogEdit;
class CVariantFactoryPrivate;


/**
 * @brief CVariantFactory

 */
class QT_QTPROPERTYBROWSER_EXPORT CVariantFactory : public QtVariantEditorFactory
{
    Q_OBJECT
public:
    CVariantFactory(QObject *parent = 0);
    virtual ~CVariantFactory();
protected:
    virtual QWidget *createEditor(QtVariantPropertyManager *manager, QtProperty *property,
                QWidget *parent);

    virtual void connectPropertyManager(QtVariantPropertyManager *manager);
    virtual void disconnectPropertyManager(QtVariantPropertyManager *manager);

public:
    void setTranslate(QMap<QString,QVariant>translateName);
private:
    QMap<QString,QVariant> m_translateName;

public Q_SLOTS:
    void slotSetIcon(const QPixmap &image);

private Q_SLOTS:
    void slotPropertyChanged(QtProperty *property, const QVariant &value);
    void slotSetValue(const QString &value);
    void slotEditorDestroyed(QObject *object);
Q_SIGNALS:
    void valueChanged(QVariant);
private:
    CVariantFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(CVariantFactory)
};
#endif
