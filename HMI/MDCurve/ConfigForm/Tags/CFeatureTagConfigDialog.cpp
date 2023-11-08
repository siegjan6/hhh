#include "CFeatureTagConfigDialog.h"
#include "ui_CFeatureTagConfigDialog.h"

#include "CFeatureTagPropertySettingDialog.h"
#include "CRealTagSettingDialog.h"
#include "CTranslator.h"

CFeatureTagConfigDialog::CFeatureTagConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CFeatureTagConfigDialog),
    m_tagSetttingDialog(new CFeatureTagPropertySettingDialog(this)),
    m_itemPropertyDialog(new CRealTagSettingDialog(this))
{
    ui->setupUi(this);
    initialize();
}

CFeatureTagConfigDialog::~CFeatureTagConfigDialog()
{
    delete ui;
}

void CFeatureTagConfigDialog::setTitle(const QString &title)
{
    setWindowTitle(title);
}

void CFeatureTagConfigDialog::setData(CFeatureTag *featureTag, const CTagPos &tagPos)
{
    m_tagSetttingDialog->setData(featureTag,tagPos);
}

void CFeatureTagConfigDialog::setData(CRealFTag *realTag, const CTagPos &tagPos)
{
    m_tagSetttingDialog->setData(realTag,tagPos);
}

CTagPos CFeatureTagConfigDialog::tagPos() const
{
    return m_tagSetttingDialog->tagPos();
}

void CFeatureTagConfigDialog::tagSettingInitialize()
{
    m_tagSetttingDialog->initialize();
}

void CFeatureTagConfigDialog::applyChanges()
{
    m_tagSetttingDialog->applyChanges();
    m_itemPropertyDialog->applyChanges();
}

void CFeatureTagConfigDialog::setData(CFeatureTag *tag, const QList<CXAxis *> &xList)
{
   m_itemPropertyDialog->setData(tag,xList);
}

void CFeatureTagConfigDialog::setData(CRealFTag *tag, const QList<CXAxis *> &xList)
{
    m_itemPropertyDialog->setData(tag,xList);
}

void CFeatureTagConfigDialog::setVariantForm(IMDRTDBManagerUI *variantForm)
{
    m_itemPropertyDialog->setVariantForm(variantForm);
}

void CFeatureTagConfigDialog::setFields(QStringList fields)
{
    m_itemPropertyDialog->setFields(fields);
}

void CFeatureTagConfigDialog::initialize()
{
    breakConnections();
    addItems();
    setItemsData();
    createConnections();
}

void CFeatureTagConfigDialog::addItems()
{
    QListWidgetItem *item1 = new QListWidgetItem;
    item1->setData(Qt::UserRole,int(dialogType::itemPropertyDialog));
    item1->setText(CMDMultiLanguage::instance()->value("子标签配置"));
    ui->listWidget->addItem(item1);

    QListWidgetItem *item2 = new QListWidgetItem;
    item2->setData(Qt::UserRole,int(dialogType::tagSetttingDialog));
    item2->setText(CMDMultiLanguage::instance()->value("属性配置"));
    ui->listWidget->addItem(item2);

    ui->listWidget->setCurrentRow((int)dialogType::itemPropertyDialog);

}

void CFeatureTagConfigDialog::setItemsData()
{
    m_tagSetttingDialog->close();
    m_itemPropertyDialog->show();
}

void CFeatureTagConfigDialog::createConnections()
{
    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slotShowWidget(QListWidgetItem*)));
    connect(ui->okButton,SIGNAL(clicked()),this,SLOT(slotConfirmButtonClicked()));
    connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(slotCancelButtonClicked()));
}

void CFeatureTagConfigDialog::breakConnections()
{
    disconnect(ui->okButton,SIGNAL(clicked()),this,SLOT(slotConfirmButtonClicked()));
    disconnect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(slotCancelButtonClicked()));
    disconnect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slotShowWidget(QListWidgetItem*)));
}

void CFeatureTagConfigDialog::slotShowWidget(QListWidgetItem *item)
{
    switch (item->data(Qt::UserRole).toInt())
    {
    case (int)dialogType::itemPropertyDialog:
    {
        m_tagSetttingDialog->close();
        m_itemPropertyDialog->show();
    }

        break;
    case  (int)dialogType::tagSetttingDialog:
    {
        m_tagSetttingDialog->show();
        m_itemPropertyDialog->close();
    }
        break;
    default:
        break;
    }
}

void CFeatureTagConfigDialog::slotConfirmButtonClicked()
{
    if(m_tagSetttingDialog->applyChanges())
    {
        accept();
    }
}

void CFeatureTagConfigDialog::slotCancelButtonClicked()
{
    reject();
}

