#include "HMIFramework.h"
#include "HMIStudio.h"
#include "LaunchConfigWidget.h"
#include "ui_LaunchConfigWidget.h"

CLaunchConfigWidget::CLaunchConfigWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLaunchConfigWidget)
{
    m_fullScreen = false;//!全屏
    m_canEvent = false;//!事件触发是否有效
    m_framework = NULL;
    m_file = NULL;
    m_isSelect = false;

    ui->setupUi(this);

    QIntValidator* v = new QIntValidator(1, 9999, this);
    ui->lineEditWidth->setValidator(v);
    ui->lineEditHeight->setValidator(v);
}
CLaunchConfigWidget::~CLaunchConfigWidget()
{
    delete ui;
}
void CLaunchConfigWidget::on_listWidgetSelected_itemChanged(QListWidgetItem *item)
{
    if (!m_canEvent)
        return;

    m_canEvent = false;

    QTreeWidgetItem* treeItem = reinterpret_cast<QTreeWidgetItem*>(item->data(Qt::UserRole).toULongLong());
    delFromList(treeItem);

    m_canEvent = true;
}
void CLaunchConfigWidget::on_treeWidgetForm_itemChanged(QTreeWidgetItem *item, int column)
{
    if (!m_canEvent)
        return;
    if (column != 0)
        return;

    m_canEvent = false;

    bool isCheck = (item->checkState(0) == Qt::Checked);
    if (isCheck)
    {
        QString path = m_file->getRelativeFilePath(item);
        addToList(item, path);
    }
    else
        delFromList(item);

    m_canEvent = true;
}
//////////////////////////////////////////////////////////////////////////
void CLaunchConfigWidget::saveData()
{
    m_fullScreen = (ui->checkBoxFullScreen->checkState() == Qt::Checked);

    m_selectForms.clear();
    QListWidget* list = ui->listWidgetSelected;
    for (int i = 0; i < list->count(); i++)
        m_selectForms.append(list->item(i)->text());

    if (!m_isSelect)
    {
        CLaunchConfig* config = m_framework->launchConfig();

        config->setLaunchFullScreen(m_fullScreen);
        config->setLaunchForms(m_selectForms);

        config->setScaleEnable(ui->checkBoxScale->isChecked());
        config->setDesignWidth(ui->lineEditWidth->text().toInt());
        config->setDesignHeight(ui->lineEditHeight->text().toInt());

        config->saveLaunchConfig();
    }
}
void CLaunchConfigWidget::initSelect(IHMIFramework *framework, const QStringList &selectForms)
{
    init(framework, selectForms, true);
}
void CLaunchConfigWidget::init(IHMIFramework *framework)
{
    QStringList list;
    init(framework, list, false);
}
//////////////////////////////////////////////////////////////////////////
void CLaunchConfigWidget::addToList(QTreeWidgetItem* node, const QString& path)
{
    QListWidget* list = ui->listWidgetSelected;

    QListWidgetItem* item = new QListWidgetItem(path);
    item->setFlags(node->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Checked);
    item->setData(Qt::UserRole, (qulonglong)node);

    list->addItem(item);
}
void CLaunchConfigWidget::delFromList(QTreeWidgetItem *node)
{
    QListWidget* list = ui->listWidgetSelected;

    for (int i = 0; i < list->count(); i++)
    {
        if (list->item(i)->data(Qt::UserRole).toULongLong() == (qulonglong)node)
        {
            delete list->takeItem(i);
            break;
        }
    }

    node->setCheckState(0, Qt::Unchecked);
}
void CLaunchConfigWidget::initTree(CFileNode *file)
{
    QTreeWidget* tree = ui->treeWidgetForm;
    file->getNodes(m_framework->hmiPath(), "*.gdi", NULL,tree);
    tree->header()->hide();
    tree->expandAll();

    int count = tree->topLevelItemCount();
    for (int i = 0; i < count; i++)
        resetNode(tree->topLevelItem(i));
}
void CLaunchConfigWidget::resetNode(QTreeWidgetItem *node)
{
    CFileNode::FileType type = (CFileNode::FileType)node->data(0, Qt::UserRole+1).toULongLong();
    if (type == CFileNode::FileType::Dir)
    {
        for (int i = 0; i < node->childCount(); i++)
            resetNode(node->child(i));
    }
    else
    {
        node->setFlags(node->flags() | Qt::ItemIsUserCheckable);
        QString path = m_file->getRelativeFilePath(node);
        if (m_selectForms.indexOf(path) != -1)
        {
            node->setCheckState(0, Qt::Checked);
            addToList(node, path);
        }
        else
            node->setCheckState(0, Qt::Unchecked);
    }
}
void CLaunchConfigWidget::init(IHMIFramework *framework, const QStringList &selectForms, bool isSelect)
{
    Q_ASSERT(framework != NULL);

    m_isSelect = isSelect;
    m_framework = dynamic_cast<CHMIFramework*>(framework);
    CHMIStudio* studio = dynamic_cast<CHMIStudio*>(m_framework->manager());
    m_file = studio->fileNode();
    m_fullScreen = m_framework->launchConfig()->launchFullScreen();

    if (m_isSelect)
    {
        m_selectForms = selectForms;
        ui->checkBoxFullScreen->setVisible(false);
    }
    else
    {
        m_selectForms = m_framework->launchConfig()->launchForms();
        ui->pushButtonOk->setVisible(false);
        ui->pushButtonCancel->setVisible(false);
    }

    ui->checkBoxFullScreen->setChecked(m_fullScreen);
    initTree(m_file);

    m_canEvent = true;

    CLaunchConfig* config = m_framework->launchConfig();
    ui->checkBoxScale->setChecked(config->scaleEnable());
    ui->lineEditWidth->setText(QString::number(config->designWidth()));
    ui->lineEditHeight->setText(QString::number(config->designHeight()));
}
//////////////////////////////////////////////////////////////////////////
void CLaunchConfigWidget::on_pushButtonOk_clicked()
{
    saveData();
    accept();
}
void CLaunchConfigWidget::on_pushButtonCancel_clicked()
{
    reject();
}
bool CLaunchConfigWidget::onOk()
{
    saveData();
    accept();
    return true;
}
