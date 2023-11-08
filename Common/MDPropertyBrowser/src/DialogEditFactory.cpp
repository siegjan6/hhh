#include "DialogEdit.h"
#include "DialogEditFactory.h"
#include <QDebug>
class CDialogEditFactoryPrivate
{
    Q_DECLARE_PUBLIC(CDialogEditFactory)

    CDialogEditFactory *q_ptr;
public:
    QMap<QtProperty *, QList<CDialogEdit *> > m_CreatedEditors;
    QMap<CDialogEdit *, QtProperty *>         m_EditorToProperty;
};

/**
 * @brief CDialogEditFactory::CDialogEditFactory
 * @param parent
 */
CDialogEditFactory::CDialogEditFactory(QObject *parent)
    : QtAbstractEditorFactory<CDialogManager>(parent)
{
    d_ptr = new CDialogEditFactoryPrivate;
    d_ptr->q_ptr = this;
}

CDialogEditFactory::~CDialogEditFactory()
{

    QList<CDialogEdit *> editors = d_ptr->m_EditorToProperty.keys();
    QListIterator<CDialogEdit *> it(editors);
    while (it.hasNext())
        delete it.next();
     delete d_ptr;
}

QWidget *CDialogEditFactory::createEditor(CDialogManager *manager,
        QtProperty *property, QWidget *parent)
{
    CDialogEdit *editor = new CDialogEdit(parent);

    editor->setValue(manager->value(property));
    editor->setIcon(manager->pixmap(property));

    d_ptr->m_CreatedEditors[property].append(editor);
    d_ptr->m_EditorToProperty[editor] = property;

    connect(editor, SIGNAL(valueChanged(const QVariant &)),
                this, SLOT(slotSetValue(const QVariant &)));
    connect(editor, SIGNAL(destroyed(QObject *)),
                this, SLOT(slotEditorDestroyed(QObject *)));
    return editor;
}

void CDialogEditFactory::connectPropertyManager(CDialogManager *manager)
{
    connect(manager, SIGNAL(valueChanged(QtProperty *, const QString &)),
                this, SLOT(slotPropertyChanged(QtProperty *, const QString &)));
}

void CDialogEditFactory::disconnectPropertyManager(CDialogManager *manager)
{
    disconnect(manager, SIGNAL(valueChanged(QtProperty *, const QString &)),
                this, SLOT(slotPropertyChanged(QtProperty *, const QString &)));
}

void CDialogEditFactory::slotPropertyChanged(QtProperty *property,
                const QString &value)
{
    if (!d_ptr->m_CreatedEditors.contains(property))
        return;

    QList<CDialogEdit *> editors = d_ptr->m_CreatedEditors[property];
    QListIterator<CDialogEdit *> itEditor(editors);
    while (itEditor.hasNext())
        itEditor.next()->setValue(value);
}

void CDialogEditFactory::slotSetValue(const QString &value)
{
    QObject *object = sender();
    QMap<CDialogEdit *, QtProperty *>::ConstIterator itEditor =
                d_ptr->m_EditorToProperty.constBegin();
    while (itEditor != d_ptr->m_EditorToProperty.constEnd()) {
        if (itEditor.key() == object) {
            QtProperty *property = itEditor.value();
            CDialogManager *manager = propertyManager(property);
            if (!manager)
                return;
            manager->slotSetValue(property, value);
            return;
        }
        itEditor++;
    }
}

void CDialogEditFactory::slotSetIcon(const QPixmap &img)
{
    QObject *object = sender();
    QMap<CDialogEdit *, QtProperty *>::ConstIterator itEditor =
                d_ptr->m_EditorToProperty.constBegin();
    while (itEditor != d_ptr->m_EditorToProperty.constEnd()) {
        if (itEditor.key() == object) {
            QtProperty *property = itEditor.value();
            CDialogManager *manager = propertyManager(property);
            if (!manager)
                return;
            manager->slotSetIcon(property,img);
            return;
        }
        itEditor++;
    }
}

void CDialogEditFactory::slotEditorDestroyed(QObject *object)
{
    QMap<CDialogEdit *, QtProperty *>::ConstIterator itEditor =
                 d_ptr->m_EditorToProperty.constBegin();
    while (itEditor != d_ptr->m_EditorToProperty.constEnd()) {
        if (itEditor.key() == object) {
            CDialogEdit *editor = itEditor.key();
            QtProperty *property = itEditor.value();
            d_ptr->m_EditorToProperty.remove(editor);
            d_ptr->m_CreatedEditors[property].removeAll(editor);
            if (d_ptr->m_CreatedEditors[property].isEmpty())
                d_ptr->m_CreatedEditors.remove(property);
            return;
        }
        itEditor++;
    }
}





#include "moc_DialogEditFactory.cpp"
