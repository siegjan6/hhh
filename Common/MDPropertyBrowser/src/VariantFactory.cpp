#include "VariantFactory.h"
#include "VariantManager.h"
#include "DialogEdit.h"
#include <QDebug>

class CVariantFactoryPrivate
{
    CVariantFactory *q_ptr;
    Q_DECLARE_PUBLIC(CVariantFactory)
public:

    QMap<QtProperty *, QList<CDialogEdit *> > m_createdEditors;
    QMap<CDialogEdit *, QtProperty *>         m_editorToProperty;
};

/**
 * @brief CVariantFactory::CVariantFactory
 * @param parent
 */
CVariantFactory::CVariantFactory(QObject *parent)
    : QtVariantEditorFactory(parent)
{
    d_ptr = new CVariantFactoryPrivate;
    d_ptr->q_ptr = this;
}

CVariantFactory::~CVariantFactory()
{
    qDeleteAll(d_ptr->m_editorToProperty.keys());
    delete d_ptr;
}


QWidget *CVariantFactory::createEditor(QtVariantPropertyManager *manager,
        QtProperty *property, QWidget *parent)
{
    if (manager->propertyType(property) == CVariantManager::dialogTypeId()) {
        CDialogEdit *editor = new CDialogEdit(parent);
//        QString name = manager->variantProperty(property)->propertyName();
//        if(m_translateName.values().contains(name))
//        {
//            name = m_translateName.key(name);
//        }
//        if(name.count()>5 && name.left(3) == "DIL" && name.left(5) != "DIL_W")
//        {
//            editor->lineEdit()->setVisible(false);
//        }
//        else
//            editor->label()->setVisible(false);
        editor->lineEdit()->setVisible(false);

        editor->setValue(manager->value(property).toString());
        editor->setIcon(((CVariantManager*)manager)->valueIcon(property).pixmap(16,16));
        d_ptr->m_createdEditors[property].append(editor);
        d_ptr->m_editorToProperty[editor] = property;

        connect(editor, SIGNAL(valueChanged(const QString &)),
                this, SLOT(slotSetValue(const QString &)));
        connect(editor, SIGNAL(destroyed(QObject *)),
                this, SLOT(slotEditorDestroyed(QObject *)));
        return editor;
    }
    return QtVariantEditorFactory::createEditor(manager, property, parent);
}

void CVariantFactory::connectPropertyManager(QtVariantPropertyManager *manager)
{
    connect(manager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                this, SLOT(slotPropertyChanged(QtProperty *, const QVariant &)));

    QtVariantEditorFactory::connectPropertyManager(manager);
}

void CVariantFactory::disconnectPropertyManager(QtVariantPropertyManager *manager)
{
    disconnect(manager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                this, SLOT(slotPropertyChanged(QtProperty *, const QVariant &)));

    QtVariantEditorFactory::disconnectPropertyManager(manager);
}

void CVariantFactory::setTranslate(QMap<QString, QVariant> translateName)
{
    m_translateName = translateName;
}

void CVariantFactory::slotPropertyChanged(QtProperty *property,
                const QVariant &value)
{
    if (!d_ptr->m_createdEditors.contains(property))
        return;

    QList<CDialogEdit *> editors = d_ptr->m_createdEditors[property];
    QListIterator<CDialogEdit *> itEditor(editors);
    while (itEditor.hasNext()){
            if(value.canConvert(QVariant::Pixmap))
                itEditor.next()->setIcon(value.value<QPixmap>());
            else
                itEditor.next()->setValue(value.toString());
    }
}


void CVariantFactory::slotSetValue(const QString &value)
{
    QObject *object = sender();
    QMap<CDialogEdit *, QtProperty *>::ConstIterator itEditor =
                d_ptr->m_editorToProperty.constBegin();
    while (itEditor != d_ptr->m_editorToProperty.constEnd()) {
        if (itEditor.key() == object) {
            QtProperty *property = itEditor.value();
            QtVariantPropertyManager *manager = propertyManager(property);
            if (!manager)
                return;
            manager->setValue(property, value);
            return;
        }
        itEditor++;
    }
}

void CVariantFactory::slotSetIcon(const QPixmap &image)
{
    QObject *object = sender();
    QMap<CDialogEdit *, QtProperty *>::ConstIterator itEditor =
                d_ptr->m_editorToProperty.constBegin();
    while (itEditor != d_ptr->m_editorToProperty.constEnd()) {
        if (itEditor.key() == object) {
            QtProperty *property = itEditor.value();
            QtVariantPropertyManager *manager = propertyManager(property);
            if (!manager)
                return;
            manager->setValue(property,QVariant::fromValue(image));
            return;
        }
        itEditor++;
    }
}

void CVariantFactory::slotEditorDestroyed(QObject *object)
{
    QMap<CDialogEdit *, QtProperty *>::ConstIterator itEditor =
                d_ptr->m_editorToProperty.constBegin();
    while (itEditor != d_ptr->m_editorToProperty.constEnd()) {
        if (itEditor.key() == object) {
            CDialogEdit *editor = itEditor.key();
            QtProperty *property = itEditor.value();
            d_ptr->m_editorToProperty.remove(editor);
            d_ptr->m_createdEditors[property].removeAll(editor);
            if (d_ptr->m_createdEditors[property].isEmpty())
                d_ptr->m_createdEditors.remove(property);
            return;
        }
        itEditor++;
    }
}



#include "moc_VariantFactory.cpp"
