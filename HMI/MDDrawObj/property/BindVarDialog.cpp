#include "BindVarDialog.h"
#include "ui_BindVarDialog.h"
//#include "IMDRTDBManager.h"
#include "IStudioInterface.h"
#include "MDSelectVariableWidget.h"
#include "MDProjectConfiger.h"
//////////////////////////////////////////////////////////////
CBindVarDialog::CBindVarDialog(QList<CMDExpInfo *> *data, IDrawEnvrionment* envir,
    const QString &selectName, QWidget *parent)
    :QDialog(parent),
    ui(new Ui::CBindVarDialog)
{
    ui->setupUi(this);

    m_varWidget = NULL;
    m_envir = envir;
    init(data);
}
CBindVarDialog::~CBindVarDialog()
{
    delete m_varWidget;
    delete ui;
}
//////////////////////////////////////////////////////////////
void CBindVarDialog::init(QList<CMDExpInfo*>* data)
{
    QString style = R"(
QTreeWidget::item {
    border: 1px solid #d9d9d9;
    border-top-color:transparent;
    border-left-color:transparent;
}
QTreeWidget::item:selected {
    background-color:#2FC3E8;
}
QTreeWidget::item:has-children{
    border-right-color:transparent;
}
)";

    CMDSelectVariableWidget* w = new CMDSelectVariableWidget;
    w->setRTDBManager(m_envir->studioProject()->projectConfiger()->nsRTDBManager());
    layout()->addWidget(w);
    connect(w, &CMDSelectVariableWidget::selectVariableName,
        this, &CBindVarDialog::addVar);
    m_varWidget = w;

    QTreeWidget* tree = ui->treeWidget;
    tree->setStyleSheet(style);
    tree->setColumnWidth(0, 150);
    connect(tree, &QTreeWidget::itemDoubleClicked, this, &CBindVarDialog::setItemEditable);

    for(int i = 0; i < data->count(); i++)
        createItem(tree, data->at(i));
    tree->expandAll();
    if(tree->topLevelItemCount() > 0 && tree->topLevelItem(0)->childCount() > 0)
        tree->setCurrentItem(tree->topLevelItem(0)->child(0));

    on_pushButtonShowVar_clicked();
    on_pushButtonShowVar_clicked();
}
QTreeWidgetItem *CBindVarDialog::createItem(QTreeWidget* tree, CMDExpInfo* exp)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(QStringList({exp->showText(), exp->expression()}));
    item->setData(0, Qt::UserRole, (qulonglong)exp);
    if(exp->group() != NULL)
    {
        QTreeWidgetItem* group = createGroup(tree, exp->group());
        group->addChild(item);
    }
    else
        tree->addTopLevelItem(item);
    return item;
}
QTreeWidgetItem* CBindVarDialog::createGroup(QTreeWidget* tree, CMDExpGroup* group)
{
    QString n = group->name().toLower();
    if(m_groups.contains(n))
        return m_groups.value(n);

    QTreeWidgetItem* item = new QTreeWidgetItem(QStringList(group->showText()));
    m_groups.insert(n, item);
    tree->addTopLevelItem(item);
    return item;
}
void CBindVarDialog::setItemEditable(QTreeWidgetItem *item, int column)
{
    if(column == 0 || item->childCount() > 0)
        item->setFlags(item->flags() &~(Qt::ItemIsEditable));
    else
        item->setFlags(item->flags() | Qt::ItemIsEditable);
}
void CBindVarDialog::save()
{
    QTreeWidget* tree = ui->treeWidget;
    for(int i = 0; i < tree->topLevelItemCount(); i++)
        saveExpInfo(tree->topLevelItem(i));
}
void CBindVarDialog::saveExpInfo(QTreeWidgetItem *item)
{
    qulonglong pointer = item->data(0, Qt::UserRole).toULongLong();
    if(pointer != 0)
    {
        CMDExpInfo* exp = reinterpret_cast<CMDExpInfo*>(pointer);
        exp->setExpression(item->text(1).simplified());
    }

    for(int i = 0; i < item->childCount(); i++)
        saveExpInfo(item->child(i));
}
void CBindVarDialog::addVar(const QString &var)
{
    QTreeWidgetItem* current = ui->treeWidget->currentItem();
    if(current == NULL || current->childCount() > 0)
        return;

    QString text = current->text(1);
    current->setText(1, text + var);
}
void CBindVarDialog::setVarVisible(bool value)
{
    QString text = (value) ? ">>" : "<<";
    ui->pushButtonShowVar->setText(text);
    m_varWidget->setVisible(value);
}
//////////////////////////////////////////////////////////////
void CBindVarDialog::on_pushButtonOK_clicked()
{
    save();
    accept();
}
void CBindVarDialog::on_pushButtonCancel_clicked()
{
    reject();
}
void CBindVarDialog::on_pushButtonShowVar_clicked()
{
    static bool varVisible = false;
    varVisible = !varVisible;
    setVarVisible(varVisible);
}
//////////////////////////////////////////////////////////////

