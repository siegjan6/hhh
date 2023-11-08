#include "CustomPropertyDialog.h"
#include "ui_CustomPropertyDialog.h"
#include "InputPropertyNameDialog.h"
#include "MDMultiLanguage.h"
//////////////////////////////////////////////////////////////////////////
CCustomPropertyDialog::CCustomPropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CCustomPropertyDialog)
{
    ui->setupUi(this);

    m_object = NULL;
    m_maxIndex = 0;
    sBaseName = "customProperty";

    CMDMultiLanguage::instance()->translateWidget(this);
}
CCustomPropertyDialog::~CCustomPropertyDialog()
{
    delete ui;
}
//////////////////////////////////////////////////////////////////////////
void CCustomPropertyDialog::init(CMDDrawGroup *obj)
{
    Q_ASSERT(obj != NULL);
    m_object = obj;

    QList<CMDCustomProperty>* list = m_object->customPropertys();
    QTreeWidget* w = ui->treeWidget;
    if (w->topLevelItemCount() > 0)
        return;

    int count = list->count();
    for (int i = 0; i < count; i++)
    {
        QStringList strs;
        strs << (*list)[i].name() << (*list)[i].expression();
        new QTreeWidgetItem(w, strs);
    }

    m_maxIndex = getMaxIndex();

    connect(ui->treeWidget, &QTreeWidget::itemDoubleClicked, this, &CCustomPropertyDialog::itemDoubleClicked);
}
void CCustomPropertyDialog::saveData()
{
    QList<CMDCustomProperty>* list = m_object->customPropertys();
    QTreeWidget* w = ui->treeWidget;

    list->clear();
    int count = w->topLevelItemCount();
    for (int i = 0; i < count; i++)
    {
        QString name = w->topLevelItem(i)->text(0);
        QString expression = w->topLevelItem(i)->text(1);
        list->append(CMDCustomProperty(name, expression));
    }
}
int CCustomPropertyDialog::getMaxIndex()
{
    CMDDrawGroup* group = m_object;
    while (group->groupParent() != NULL)
        group = group->groupParent();

    int index = 0;
    getMaxIndex(group, index);
    return index;
}
void CCustomPropertyDialog::getMaxIndex(CMDDrawGroup *group, int& index)
{
    QList<CMDCustomProperty>* list = group->customPropertys();
    for(int j = 0; j < list->count(); j++)
    {
        CMDCustomProperty value = list->at(j);
        int i = 0;
        if (isNameIndex(value.name(), i))
        {
            if (i > index)
                index = i;
        }
    }

    for(int i = 0; i < group->objList()->count(); i++)
    {
        CMDDrawObj* obj = group->objList()->at(i);
        if (obj->type() == DrawType::Group)
            getMaxIndex(static_cast<CMDDrawGroup*>(obj), index);
    }
}
bool CCustomPropertyDialog::isNameIndex(const QString& name, int &index)
{
    QString str = name;
    if (str.indexOf(sBaseName) == 0)
    {
        str.remove(0, sBaseName.length());
        bool result = false;
        index = str.toInt(&result);
        return result;
    }

    return false;
}
void CCustomPropertyDialog::itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    CInputPropertyNameDialog dlg(this);
    dlg.setDrawObject(m_object);
    QStringList names = treeNames();
    names.removeOne(item->text(0));
    dlg.setCurrentNames(names);
    dlg.setName(item->text(0));
    dlg.setExpression(item->text(1));

    if (dlg.exec() == QDialog::Accepted)
    {
        item->setText(0, dlg.name());
        item->setText(1, dlg.expression());
        checkMaxIndex(dlg.name(), m_maxIndex);
    }
}
void CCustomPropertyDialog::checkMaxIndex(const QString &name, int &maxIndex)
{
    int i = 0;
    if (isNameIndex(name, i))
    {
        if (i > maxIndex)
            maxIndex = i;
    }
}
QStringList CCustomPropertyDialog::treeNames()
{
    QStringList names;
    QTreeWidget* w = ui->treeWidget;
    int count = w->topLevelItemCount();
    for (int i = 0; i < count; i++)
        names << w->topLevelItem(i)->text(0);
    return names;
}
//////////////////////////////////////////////////////////////////////////
void CCustomPropertyDialog::on_pushButtonAdd_clicked()
{
    QString name;
    name.setNum(m_maxIndex + 1);
    name = sBaseName + name;

    CInputPropertyNameDialog dlg(this);
    dlg.setDrawObject(m_object);
    dlg.setCurrentNames(treeNames());
    dlg.setName(name);

    if (dlg.exec() == QDialog::Accepted)
    {
        QStringList strs;
        strs << dlg.name() << dlg.expression();
        new QTreeWidgetItem(ui->treeWidget, strs);
        checkMaxIndex(dlg.name(), m_maxIndex);
    }
}
void CCustomPropertyDialog::on_pushButtonDelete_clicked()
{
    QTreeWidget* w = ui->treeWidget;
    delete w->takeTopLevelItem(w->indexOfTopLevelItem(w->currentItem()));
}
void CCustomPropertyDialog::on_pushButtonOK_clicked()
{
    saveData();
    accept();
}
void CCustomPropertyDialog::on_pushButtonCancel_clicked()
{
    reject();
}
//////////////////////////////////////////////////////////////////////////
