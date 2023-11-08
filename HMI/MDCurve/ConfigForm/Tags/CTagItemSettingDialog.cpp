#include "CTagItemSettingDialog.h"
#include "ui_CTagItemSettingDialog.h"
#include "CFeatureTagItem.h"
#include "CVariantDialog.h"

#include "IMDRTDBManagerUI.h"
#include "CTranslator.h"
#include "CFormToolFunc.h"
#include <QFontDialog>
#include <QDebug>

using namespace Translator;
CTagItemSettingDialog::CTagItemSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CTagItemSettingDialog),
    m_item(0),
    m_variantForm(0),
    m_transTool(0)
{
    ui->setupUi(this);
    ui->variantLineEdit->setEnabled(false);
    installDialogTranslator(m_transTool,this);
}

CTagItemSettingDialog::~CTagItemSettingDialog()
{
    delete ui;
}

CFeatureTagItem *CTagItemSettingDialog::getItem(bool *ok, CFeatureTagItem *initial, QWidget *parent)
{
    if(initial)
    {
        CTagItemSettingDialog dlg(parent);
        dlg.setData(initial);
        dlg.setWindowTitle(QString("%1-%2").arg(initial->itemName(),initial->comment()));
        int ret = dlg.exec();
        if(ok)
        {
            *ok = !!ret;
            return initial;
        }
    }
    return NULL;
}

void CTagItemSettingDialog::setData(CFeatureTagItem *item)
{
    if(item)
    {
        m_item = item;
        initialize();
    }
}

void CTagItemSettingDialog::setVariantForm(IMDRTDBManagerUI *variantForm)
{
    m_variantForm = variantForm;
}

void CTagItemSettingDialog::setFields(const QStringList &fields)
{
    m_fields = fields;
}

void CTagItemSettingDialog::initialize()
{
    initializeData();
    initializeUI();
}

void CTagItemSettingDialog::initializeData()
{
    if(m_item)
    {
        m_font = m_item->font();
    }
}

void CTagItemSettingDialog::initializeUI()
{
    if(m_item)
    {
        breakConnections();
        ui->commentLineEdit->setMaxLength(100);
        switch (m_item->type())
        {
        case CFeatureTagItem::ItemType::Comment:
            ui->exampleLabel->setVisible(false);
            ui->formatLabel->setVisible(false);
            ui->formatLineEdit->setVisible(false);
            break;
        case CFeatureTagItem::ItemType::Time:
            ui->exampleLabel->setText(tr("例如  yyyy-MM-dd hh:mm:ss\n      (年-月-日 时:分:秒)"));
            break;
        case CFeatureTagItem::ItemType::Value:
            ui->formatLabel->setText(tr("数据精度"));
            ui->exampleLabel->setVisible(false);
            break;
        default:
            break;
        }

        ui->visibleCheckBox->setChecked(m_item->visible());
        ui->variantLineEdit->setText(m_item->itemName());
        ui->commentLineEdit->setText(m_item->comment());
        ui->formatLineEdit->setText(m_item->format());

        createConnections();
    }
}

void CTagItemSettingDialog::applyChanges()
{
    if(m_item)
    {
        m_item->setItemName(ui->variantLineEdit->text());
        m_item->setVisible(ui->visibleCheckBox->isChecked());
        m_item->setComment(ui->commentLineEdit->text());
        m_item->setFormat(ui->formatLineEdit->text());
        m_item->setFont(m_font);
    }
}

void CTagItemSettingDialog::createConnections()
{
    connect(ui->okButton,SIGNAL(clicked()),this,SLOT(slotOkClicked()));
    connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(slotCancelClicked()));
    connect(ui->fontButton,SIGNAL(clicked()),this,SLOT(slotSetFont()));
}

void CTagItemSettingDialog::breakConnections()
{
    disconnect(ui->okButton,SIGNAL(clicked()),this,SLOT(slotOkClicked()));
    disconnect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(slotCancelClicked()));
    disconnect(ui->fontButton,SIGNAL(clicked()),this,SLOT(slotSetFont()));
}

void CTagItemSettingDialog::slotOkClicked()
{
    applyChanges();
    accept();
}

void CTagItemSettingDialog::slotCancelClicked()
{
    reject();
}

void CTagItemSettingDialog::slotSetFont()
{
    FormToolFunc::selectFont(m_font,this);
}



