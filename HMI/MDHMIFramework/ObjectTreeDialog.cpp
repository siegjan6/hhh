#include "ObjectTreeDialog.h"
#include "ui_ObjectTreeDialog.h"
#include "MDDrawGroup.h"
#include "MDDrawCombine.h"
#include "MDDrawObjectConfig.h"
//////////////////////////////////////////////////////////////////////////
CObjectTreeDialog::CObjectTreeDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrawObjTreeDialog)
{
    m_form = NULL;
    m_operationType = OperationType::Other;

    ui->setupUi(this);
    ui->treeWidget->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
    connect(ui->treeWidget, &QTreeWidget::itemChanged, this, &CObjectTreeDialog::itemChanged);
    connect(ui->treeWidget, &QTreeWidget::itemSelectionChanged, this, &CObjectTreeDialog::itemSelectionChanged);

    m_transTool = CMDMultiLanguage::createWidgetTool();
    m_transTool->init(this);

    m_icons = CMDDrawObjectConfig::createDrawIcons();
}
CObjectTreeDialog::~CObjectTreeDialog()
{
    delete ui;
    delete m_transTool;
    qDeleteAll(*m_icons);
    delete m_icons;
}
//////////////////////////////////////////////////////////////////////////
void CObjectTreeDialog::setHMIForm(IHMIForm *value)
{
    QTreeWidget* t = tree();
    if (value == NULL || m_form != value)
        clear();
    if (m_form == value || value == NULL)
    {
        m_form = value;
        return;
    }
    m_form = value;

    QList<QTreeWidgetItem*> items;
    QList<CMDDrawObj*>* objs = m_form->common()->objs();
    for(int i = 0; i < objs->count(); i++)
        items.insert(0, createItem(NULL, objs->at(i)));
    t->addTopLevelItems(items);
    t->expandAll();
    selectDefault();
}
//////////////////////////////////////////////////////////////////////////
QTreeWidgetItem *CObjectTreeDialog::createItem(QTreeWidgetItem *parent, CMDDrawObj *obj)
{
    QTreeWidgetItem* item = new QTreeWidgetItem();
    if (parent != NULL)
        parent->insertChild(0, item);
    item->setText(0, obj->name());
    Qt::CheckState visible = obj->visible() ? Qt::Checked : Qt::Unchecked;
    item->setCheckState(1, visible);
    Qt::CheckState locked = obj->locked() ? Qt::Checked : Qt::Unchecked;
    item->setCheckState(2, locked);
    item->setData(0, Qt::UserRole, (qulonglong)obj);
    item->setIcon(0, *m_icons->value((int)obj->type()));

    if (obj->type() == DrawType::Group)
    {
        CMDDrawGroup* group = static_cast<CMDDrawGroup*>(obj);
        for(int i = 0; i < group->objList()->count(); i++)
            createItem(item, group->objList()->at(i));
    }

    return item;
}
void CObjectTreeDialog::itemChanged(QTreeWidgetItem *item, int column)
{
    if (column == 0)
        return;

    bool checked = item->checkState(column);
    CMDDrawObj* obj = reinterpret_cast<CMDDrawObj*>(item->data(0, Qt::UserRole).toULongLong());
    if (column == 1)//visible
        obj->setVisible(checked);
    else if (column == 2)//locked
        obj->setLocked(checked);
    m_form->setModify(true);
}
QTreeWidget *CObjectTreeDialog::tree()
{
    return ui->treeWidget;
}
void CObjectTreeDialog::deleteItems(QList<CMDDrawObj *>* objs)
{
    QTreeWidget* t = tree();
    QTreeWidgetItem* item = NULL;
    for (int j = 0; j < objs->count(); j++)
    {
        for(int i = 0; i < t->topLevelItemCount(); i++)
        {
            item = t->topLevelItem(i);
            if ((qulonglong)objs->at(j) == item->data(0, Qt::UserRole).toULongLong())
            {
                delete t->takeTopLevelItem(i);
                break;
            }
        }
    }
}
QTreeWidgetItem *CObjectTreeDialog::findItem(CMDDrawObj *obj)
{
    QTreeWidget* t = tree();
    QTreeWidgetItem* item = NULL;
    for(int i = 0; i < t->topLevelItemCount(); i++)
    {
        item = findChildItem(t->topLevelItem(i), obj);
        if (item != NULL)
            return item;
    }

    return NULL;
}
QTreeWidgetItem *CObjectTreeDialog::findChildItem(QTreeWidgetItem *parent, CMDDrawObj *obj)
{
    if ((qulonglong)obj == parent->data(0, Qt::UserRole).toULongLong())
        return parent;

    QTreeWidgetItem* item = NULL;
    for(int i = 0; i < parent->childCount(); i++)
    {
        item = findChildItem(parent->child(i), obj);
        if (item != NULL)
            return item;
    }

    return NULL;
}

int CObjectTreeDialog::objectIndex(CMDDrawObj *obj)
{
    QList<CMDDrawObj*>* objs = obj->parent()->common()->objs();
    return objs->count() - objs->indexOf(obj) - 1;
}
void CObjectTreeDialog::itemSelectObject()
{
    QTreeWidget* t = tree();
    QList<QTreeWidgetItem *> list = t->selectedItems();
}
void CObjectTreeDialog::itemSelectionChanged()
{
    if (m_operationType == OperationType::Event)
        return;
    m_operationType = OperationType::Mouse;

    QTreeWidget* t = tree();
    QList<QTreeWidgetItem *> items = t->selectedItems();
    QList<CMDDrawObj*> objs;
    int count = t->topLevelItemCount();
    QVector<bool> itemVector(count, false);

    //预防成组控件添加多次
    for(int i = 0; i < items.count(); i++)
    {
        QTreeWidgetItem* item = items.at(i);
        while (item->parent() != NULL)
            item = item->parent();
        itemVector[t->indexOfTopLevelItem(item)] = true;
    }
    for(int i = 0; i < count; i++)
    {
        if (itemVector.at(i))
            objs.append(reinterpret_cast<CMDDrawObj*>(t->topLevelItem(i)->data(0, Qt::UserRole).toULongLong()));
    }
    m_form->studio()->itemSelectObject(&objs);

    //设置属性框对象,仅限于单个对象
    if (items.count() == 1)
    {
        QTreeWidgetItem* item = items.at(0);

        CMDDrawObj* obj = reinterpret_cast<CMDDrawObj*>(item->data(0, Qt::UserRole).toULongLong());
        //child object
        if (item->parent() != NULL)
        {
            m_form->studio()->setLastSelectedObject(obj);
            CMDDrawGroup* group = obj->groupParent();
            while (group != NULL)
            {
                if (group->groupParent() == NULL)
                    group->invalidate();
                group = group->groupParent();
            }
        }
        else
        {
            m_form->studio()->setLastSelectedObject(NULL);
            obj->invalidate();
        }

        m_form->studio()->selectPropetyObject(obj);
    }

    m_operationType = OperationType::Other;
}
void CObjectTreeDialog::selectDefault()
{
    QList<CMDDrawObj*>* objs = m_form->studio()->selectObjs();
    select(objs);
}
void CObjectTreeDialog::clear()
{
    m_operationType = OperationType::Event;

    QTreeWidget* t = tree();
    QList<QTreeWidgetItem*> items;
    for(int i=0,count=t->topLevelItemCount(); i < count; i++)
        items << t->topLevelItem(i);
    qDeleteAll(items);

    m_operationType = OperationType::Other;
}
//////////////////////////////////////////////////////////////////////////
void CObjectTreeDialog::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    QTreeWidget* t = tree();
    int width = t->width();
    t->setColumnWidth(0, width - 110);
    t->setColumnWidth(1, 50);
    t->setColumnWidth(2, 50);
}
QSize CObjectTreeDialog::sizeHint() const
{
    return QSize(300, 300);
}
//////////////////////////////////////////////////////////////////////////
void CObjectTreeDialog::add(QList<CMDDrawObj *>* list)
{
    m_operationType = OperationType::Event;

    for(int i = 0; i < list->count(); i++)
        tree()->insertTopLevelItem(0, createItem(NULL, list->at(i)));

    m_operationType = OperationType::Other;
}
void CObjectTreeDialog::dele(QList<CMDDrawObj *>* list)
{
    deleteItems(list);
}
void CObjectTreeDialog::group(CMDDrawObj *obj)
{
    if (obj->type() != DrawType::Group)
        return;

    CMDDrawGroup* group = static_cast<CMDDrawGroup*>(obj);
    deleteItems(group->objList());

    int index = objectIndex(obj);
    tree()->insertTopLevelItem(index, createItem(NULL, obj));

    selectDefault();
}
void CObjectTreeDialog::unGroup(CMDDrawObj *obj)
{
    QTreeWidget* t = tree();
    QTreeWidgetItem* item = findItem(obj);
    if (item == NULL)
        return;

    m_operationType = OperationType::Event;

    int index = t->indexOfTopLevelItem(item);
    t->takeTopLevelItem(index);
    QList<QTreeWidgetItem*> list = item->takeChildren();
    t->insertTopLevelItems(index, list);
    delete item;

    selectDefault();
}
void CObjectTreeDialog::combine(CMDDrawObj *obj)
{
    if (obj->type() != DrawType::Combine)
        return;

    CMDDrawCombine* combine = static_cast<CMDDrawCombine*>(obj);
    deleteItems(combine->objList());

    int index = objectIndex(obj);
    tree()->insertTopLevelItem(index, createItem(NULL, obj));

    selectDefault();
}
void CObjectTreeDialog::unCombine(CMDDrawObj *obj)
{
    if (obj->type() != DrawType::Combine)
        return;
    QTreeWidget* t = tree();
    QTreeWidgetItem* item = findItem(obj);
    if (item == NULL)
        return;

    m_operationType = OperationType::Event;

    CMDDrawCombine* combine = dynamic_cast<CMDDrawCombine*>(obj);
    int index = t->indexOfTopLevelItem(item);
    t->takeTopLevelItem(index);
    QList<QTreeWidgetItem*> list;
    foreach(CMDDrawObj* o, *combine->objList())
        list.append(createItem(NULL, o));
    t->insertTopLevelItems(index, list);
    delete item;

    selectDefault();
}
void CObjectTreeDialog::top(CMDDrawObj *obj)
{
    QTreeWidgetItem* item = findItem(obj);
    if (item == NULL)
        return;

    m_operationType = OperationType::Event;

    QTreeWidget* t = tree();
    t->takeTopLevelItem(t->indexOfTopLevelItem(item));
    int index = objectIndex(obj);
    t->insertTopLevelItem(index, item);

    selectDefault();
}
void CObjectTreeDialog::last(CMDDrawObj *obj)
{
    top(obj);
}
void CObjectTreeDialog::front(CMDDrawObj *obj)
{
    top(obj);
}
void CObjectTreeDialog::back(CMDDrawObj *obj)
{
    top(obj);
}
void CObjectTreeDialog::select(QList<CMDDrawObj *> *list)
{
    if (m_operationType == OperationType::Mouse)
        return;
    m_operationType = OperationType::Event;

    QTreeWidget* t = tree();
    t->setCurrentItem(NULL);
    for(int i = 0; i < list->count(); i++)
    {
        QTreeWidgetItem* item = findItem(list->at(i));
        if (item == NULL)
            continue;
        item->setSelected(true);
    }

    m_operationType = OperationType::Other;
}
void CObjectTreeDialog::nameChanged(CMDDrawObj *obj)
{
    QTreeWidgetItem* item = findItem(obj);
    if (item == NULL)
        return;

    item->setText(0, obj->name());
}
void CObjectTreeDialog::reset()
{
    clear();
    if(m_form == NULL)
        return;

    QTreeWidget* t = tree();
    QList<QTreeWidgetItem*> items;
    QList<CMDDrawObj*>* objs = m_form->common()->objs();
    for(int i = 0; i < objs->count(); i++)
        items.insert(0, createItem(NULL, objs->at(i)));
    t->addTopLevelItems(items);
}
void CObjectTreeDialog::changeLanguage()
{
    m_transTool->changeLanguage();
}
//////////////////////////////////////////////////////////////////////////


