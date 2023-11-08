#include "MDNewDataGroup.h"

#include "IMDDataGroup.h"
#include "IMDDataGroupManager.h"
#include "MDMultiLanguage.h"

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

CMDNewDataGroup::CMDNewDataGroup(QWidget *parent) :
    QDialog(parent)
{
    initData();
    initUI();
    resize(240, 150);
    setWindowTitle(tr("历史组"));
    initLanguage();
}

CMDNewDataGroup::~CMDNewDataGroup()
{
    if (m_pMulLnaguageTool)
    {
        delete m_pMulLnaguageTool;
        m_pMulLnaguageTool = NULL;
    }

    if (m_pHFirstLayout)
    {
        m_pHFirstLayout->removeWidget(m_pLbGrpName);
        m_pHFirstLayout->removeWidget(m_pLineGrpName);
    }

    if (m_pHSecondLayout)
    {
        m_pHSecondLayout->removeWidget(m_pLbDescri);
        m_pHSecondLayout->removeWidget(m_pLineDescri);
    }

    if (m_pHThirdLayout)
    {
        m_pHThirdLayout->removeWidget(m_pBtnOk);
        m_pHThirdLayout->removeWidget(m_pBtnCancel);
    }

    if (m_pVMainLayout)
    {
        QLayoutItem *child = NULL;
        while(child = m_pVMainLayout->takeAt(0))
        {
            delete child;
            child = NULL;
        }
        delete m_pVMainLayout;
        m_pVMainLayout = NULL;
    }

    if (m_pBtnOk)
    {
        delete m_pBtnOk;
        m_pBtnOk = NULL;
    }

    if (m_pBtnCancel)
    {
        delete m_pBtnCancel;
        m_pBtnCancel = NULL;
    }

    if (m_pLbGrpName)
    {
        delete m_pLbGrpName;
        m_pLbGrpName = NULL;
    }

    if (m_pLbDescri)
    {
        delete m_pLbDescri;
        m_pLbDescri = NULL;
    }

    if (m_pLineGrpName)
    {
        delete m_pLineGrpName;
        m_pLineGrpName = NULL;
    }

    if (m_pLineDescri)
    {
        delete m_pLineDescri;
        m_pLineDescri = NULL;
    }

}

int CMDNewDataGroup::calculateLabelWidth(QLabel *pLb)
{
    QFontMetrics met(pLb->font());
    return met.width(pLb->text());
}

void CMDNewDataGroup::initUI()
{
    m_pBtnOk = new QPushButton(tr("确定"), this);
    m_pBtnOk->setAttribute(Qt::WA_DeleteOnClose);
    connect(m_pBtnOk, SIGNAL(clicked()), this, SLOT(on_OkButton_clicked()));

    m_pBtnCancel = new QPushButton(tr("取消"), this);
    m_pBtnCancel->setAttribute(Qt::WA_DeleteOnClose);
    connect(m_pBtnCancel, SIGNAL(clicked()), this, SLOT(on_CancelButton_clicked()));

    m_pLbGrpName = new QLabel(tr("数据组名"), this);
    m_pLbGrpName->setAttribute(Qt::WA_DeleteOnClose);

    m_pLbDescri = new QLabel(tr("描述"), this);
    m_pLbDescri->setAttribute(Qt::WA_DeleteOnClose);

    m_pLineGrpName = new QLineEdit(this);
    m_pLineGrpName->setAttribute(Qt::WA_DeleteOnClose);
    m_pLineGrpName->setFixedWidth(GROUP_FIXED_W);

    m_pLineDescri = new QLineEdit(this);
    m_pLineDescri->setAttribute(Qt::WA_DeleteOnClose);
    m_pLineDescri->setFixedWidth(GROUP_FIXED_W);

    m_pHFirstLayout = new QHBoxLayout;
    m_pHSecondLayout = new QHBoxLayout;
    m_pHThirdLayout = new QHBoxLayout;
    m_pVMainLayout = new QVBoxLayout;

    int maxWidth = calculateLabelWidth(m_pLbGrpName);
    m_pHFirstLayout->addWidget(m_pLbGrpName);
    m_pHFirstLayout->addSpacing(maxWidth - calculateLabelWidth(m_pLbGrpName) + GROUP_LAYOUT_SPACE);
    m_pHFirstLayout->addWidget(m_pLineGrpName);

    m_pHSecondLayout->addWidget(m_pLbDescri);
    m_pHSecondLayout->addSpacing(maxWidth - calculateLabelWidth(m_pLbDescri) + GROUP_LAYOUT_SPACE);
    m_pHSecondLayout->addWidget(m_pLineDescri);

    m_pHThirdLayout->addStretch(0);
    m_pHThirdLayout->addWidget(m_pBtnOk);
    m_pHThirdLayout->addSpacing(maxWidth - calculateLabelWidth(m_pLbDescri) + GROUP_LAYOUT_SPACE);
    m_pHThirdLayout->addWidget(m_pBtnCancel);

    m_pVMainLayout->addLayout(m_pHFirstLayout);
    m_pVMainLayout->addLayout(m_pHSecondLayout);
    m_pVMainLayout->addLayout(m_pHThirdLayout);
    m_pVMainLayout->setContentsMargins(GROUP_LAYOUT_SPACE, 0, GROUP_LAYOUT_SPACE, 0);

    setLayout(m_pVMainLayout);
}

void CMDNewDataGroup::initLanguage()
{
    if (NULL == m_pMulLnaguageTool)
    {
        m_pMulLnaguageTool = CMDMultiLanguage::createWidgetTool();
        m_pMulLnaguageTool->init(this);
    }
    m_pMulLnaguageTool->changeLanguage();
}

void CMDNewDataGroup::initData(IMDDataGroup *pDataGrp, IMDDataGroupManager *pDataGrpMana)
{
    if (NULL == pDataGrp || NULL == pDataGrpMana)
    {
        return;
    }

    m_pDataGrp = pDataGrp;
    m_pDataGrpMana = pDataGrpMana;
}

void CMDNewDataGroup::changeLanguage()
{
    initLanguage();
}

void CMDNewDataGroup::on_OkButton_clicked()
{
    if (NULL == m_pDataGrp || NULL == m_pDataGrpMana)
    {
        return;
    }

    QMessageBox message(this);
    message.setWindowTitle(CMDMultiLanguage::instance()->value(tr("提示")));
    message.addButton(CMDMultiLanguage::instance()->value(tr("确认")), QMessageBox::YesRole);
    if (m_pLineGrpName->text().isEmpty())
    {
        message.setText(CMDMultiLanguage::instance()->value(tr("组名不能为空！")));
        CMDMultiLanguage::instance()->translateWidget(&message);
        message.exec();
        return;
    }

    QString strName = m_pLineGrpName->text();
    QString strFront = CMDMultiLanguage::instance()->value(tr("组名 "));
    QString strBack = CMDMultiLanguage::instance()->value(tr("已存在报警组中！"));

    if (m_pDataGrpMana->getAlarmDataGroup()->name() == strName)
    {
        strBack = CMDMultiLanguage::instance()->value(tr("已存在报警组中！"));
        message.setText(strFront + "\'" + strName + "\'" + strBack);
        CMDMultiLanguage::instance()->translateWidget(&message);
        message.exec();

        return;
    }
    else if (m_pDataGrpMana->getLogDataGroup()->name() == strName)
    {
        strBack = CMDMultiLanguage::instance()->value(tr("已存在日志组中！"));
        message.setText(strFront + "\'" + strName + "\'" + strBack);
        CMDMultiLanguage::instance()->translateWidget(&message);
        message.exec();

        return;
    }
    else if (m_pDataGrpMana->getHisDataGroup(strName))
    {
        strBack = CMDMultiLanguage::instance()->value(tr("已存在历史组中！"));
        message.setText(strFront + "\'" + strName + "\'" + strBack);
        CMDMultiLanguage::instance()->translateWidget(&message);
        message.exec();

        return;
    }

    m_pDataGrp->setName(strName);
    m_pDataGrp->setDescription(m_pLineDescri->text());

    QDialog::accept();
}

void CMDNewDataGroup::on_CancelButton_clicked()
{
    reject();
}

void CMDNewDataGroup::initData()
{
    m_pBtnOk            = NULL; //! 确定按钮
    m_pBtnCancel        = NULL; //! 取消按钮
    m_pLbGrpName        = NULL; //! 数据组名标签
    m_pLbDescri         = NULL; //! 数据组描述标签
    m_pLineGrpName      = NULL; //! 数据组名编辑框
    m_pLineDescri       = NULL; //! 数据组描述编辑框

    m_pHFirstLayout     = NULL; //! 数据组名水平布局
    m_pHSecondLayout    = NULL; //! 描述水平布局
    m_pHThirdLayout     = NULL; //! 操作水平布局
    m_pVMainLayout      = NULL; //! 整体垂直布局

    m_pDataGrp          = NULL; //! 数据组
    m_pDataGrpMana      = NULL; //! 数据组管理器

    m_pMulLnaguageTool = NULL; //! 多语言翻译工具
}
