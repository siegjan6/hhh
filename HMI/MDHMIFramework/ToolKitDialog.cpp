#include "ToolKitDialog.h"
#include "ui_ToolKitDialog.h"
#include "DrawObjDefine.h"
#include "HMIStudio.h"
#include "MDDrawObjectConfig.h"
//////////////////////////////////////////////////////////////////
CToolKitDialog::CToolKitDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CToolKitDialog)
{
    ui->setupUi(this);

    m_container = NULL;
    //!是否是鼠标操作状态
    m_isMouseOperate = true;
    //!不要置位到指针
    m_notReset = false;

    m_transTool = CMDMultiLanguage::createWidgetTool();
    m_transTool->init(this);
}
CToolKitDialog::~CToolKitDialog()
{
    delete ui;
    qDeleteAll(m_itemTexts.begin(), m_itemTexts.end());
    m_itemTexts.clear();
    delete m_transTool;
}
//////////////////////////////////////////////////////////////////
void CToolKitDialog::init(CHMIStudio *container)
{
    m_container = container;

    QTreeWidget* tree = ui->treeWidget;
    tree->header()->setVisible(false);
    tree->setIconSize(QSize(24, 24));
    tree->setIndentation(8);
    tree->setHeaderHidden(true);

    CMDDrawObjectConfig::createToolkit(tree);

    tree->expandAll();
    tree->setCurrentItem(tree->topLevelItem(0));
    setEnabled(false);
    connect(tree, &QTreeWidget::currentItemChanged, this, &CToolKitDialog::currentItemChanged);

    //init language
    qDeleteAll(m_itemTexts);
    m_itemTexts.clear();
    for(int i = 0; i < tree->topLevelItemCount()+1; i++)
        m_itemTexts.append(new QStringList());
    for(int i = 0; i < tree->topLevelItemCount(); i++)
    {
        QTreeWidgetItem* group = tree->topLevelItem(i);
        m_itemTexts.at(0)->append(group->text(0));
        for(int j = 0; j < group->childCount(); j++)
            m_itemTexts.at(i+1)->append(group->child(j)->text(0));
    }
}
void CToolKitDialog::currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if (current->parent() == NULL)
    {
        if (m_isMouseOperate)
            m_container->endCreate();
    }
    else
    {
        m_notReset = true;
        DrawType type = (DrawType)current->data(0, Qt::UserRole).toInt();
        m_container->setCreateState(type);
        m_notReset = false;
    }
}
void CToolKitDialog::resetToolKit()
{
    if (m_notReset)
        return;

    m_isMouseOperate = false;

    QTreeWidget* tree = ui->treeWidget;
    //当前项在分类上，则不需要置位到指针
    QTreeWidgetItem* current = tree->currentItem();
    if (current == NULL)
        return;
    if (current->parent() == NULL)
        return;

    tree->setCurrentItem(tree->topLevelItem(0));

    m_isMouseOperate = true;
}
//////////////////////////////////////////////////////////////////
void CToolKitDialog::changeLanguage()
{
    CMDMultiLanguage* l = CMDMultiLanguage::instance();
    for(int i = 0; i < ui->treeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem* group = ui->treeWidget->topLevelItem(i);
        group->setText(0, l->value(m_itemTexts.at(0)->at(i)));
        for(int j = 0; j < group->childCount(); j++)
            group->child(j)->setText(0, l->value(m_itemTexts.at(i+1)->at(j)));
    }

    m_transTool->changeLanguage();
}

