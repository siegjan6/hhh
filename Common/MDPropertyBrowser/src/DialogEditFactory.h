/*!

    \author eHVr

    \class CDialogEditFactory

    \brief The CDialogEditFactory class provides CDialogEdit widgets for
    properties created by CDialogManager objects.

    \sa QtAbstractEditorFactory, CDialogManager
*/
#ifndef DIALOGEDITFACTORY_H
#define DIALOGEDITFACTORY_H

#include "qtpropertybrowser.h"
#include "DialogManager.h"

class CDialogEdit;
class CDialogEditFactoryPrivate;
class QT_QTPROPERTYBROWSER_EXPORT CDialogEditFactory : public QtAbstractEditorFactory<CDialogManager>
{
    Q_OBJECT
public:
    CDialogEditFactory(QObject *parent = 0);
    virtual ~CDialogEditFactory();
protected:
    virtual QWidget *createEditor(CDialogManager *manager, QtProperty *property,
                QWidget *parent);

    virtual void connectPropertyManager(CDialogManager *manager);
    virtual void disconnectPropertyManager(CDialogManager *manager);
private Q_SLOTS:
    void slotPropertyChanged(QtProperty *property, const QString &value);
    void slotSetValue(const QString &value);
    void slotSetIcon(const QPixmap &img);
    void slotEditorDestroyed(QObject *object);
Q_SIGNALS:
    void valueChanged(QString value);
private:
    CDialogEditFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(CDialogEditFactory)
};

#endif
