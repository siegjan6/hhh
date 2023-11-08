#include "SelectPathDialog.h"
#include "ui_SelectPathDialog.h"

CSelectPathDialog::CSelectPathDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSelectPathDialog)
{
    ui->setupUi(this);

    m_transTool = CMDMultiLanguage::createWidgetTool();
}
CSelectPathDialog::~CSelectPathDialog()
{
    delete ui;
    delete m_transTool;
}
void CSelectPathDialog::on_pushButtonOk_clicked()
{
    accept();
}
void CSelectPathDialog::on_pushButtonCancel_clicked()
{
    reject();
}
//////////////////////////////////////////////////////////////////////////
void CSelectPathDialog::init(const QString &path)
{
    m_selectPath = path;
    connect(&m_node, &CStorageFileNode::currentPathChanged, this, &CSelectPathDialog::setSelectPath);
}
void CSelectPathDialog::reset()
{
    m_node.init(ui->treeWidget, m_selectPath);
    ui->treeWidget->setCurrentItem(ui->treeWidget->topLevelItem(0));
}
//////////////////////////////////////////////////////////////////////////
void CSelectPathDialog::changeLanguage()
{
    m_transTool->changeLanguage();
    m_node.changeLanguage();
}
//////////////////////////////////////////////////////////////////////////
