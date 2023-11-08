#include <QFontDialog>
#include <QColorDialog>
#include "PropertyDialog.h"
#include "ui_PropertyDialog.h"
#include "DataGroupWidget.h"
#include "CommonForm.h"
#include "AppearanceForm.h"
#include "MDMultiLanguage.h"
#include "IMDDataArchives.h"
#include "IMDDataSourceManager.h"
#include "IMDDataSource.h"
#include "IMDDataTable.h"
#include "IMDDataField.h"

CPropertyDialog::CPropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CPropertyDialog),
    m_commonForm(new CCommonForm(this)),
    m_apperanceForm(new CAppearanceForm(this))
{
    ui->setupUi(this);
    QStringList types;
    types << "QLineEdit" << "QComboBox";
    CMDMultiLanguage::instance()->translateWidget(this, &types);
}

CPropertyDialog::~CPropertyDialog()
{
    delete ui;
}

void CPropertyDialog::initialize()
{
    addItems();
    addWidgets();
    creatConnections();
    m_commonForm->initialize();
    m_apperanceForm->initialize();
    CMDMultiLanguage::instance()->translateWidget(this);
}

void CPropertyDialog::setTitleName(const QString& str)
{
    if(propertyData()->m_gridType == GridType::Alarm)
        setWindowTitle(tr("报警配置框") + " - " + str);
    else if(propertyData()->m_gridType == GridType::Log)
        setWindowTitle(tr("日志配置框") + " - " + str);
    else
        setWindowTitle(tr("数据归档配置框") + " - " + str);
}

void CPropertyDialog::addItems()
{
    insertItem(tr("字段"), (int)WidgetName::Common);
    insertItem(tr("外观"), (int)WidgetName::Appearance);
    ui->listWidget->setCurrentRow((int)WidgetName::Common);
}

void CPropertyDialog::addWidgets()
{
    ui->stackedWidget->addWidget(m_commonForm);
    ui->stackedWidget->addWidget(m_apperanceForm);
}

void CPropertyDialog::insertItem(const QString &name, int userRole, bool visible)
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setData(Qt::UserRole,userRole);
    item->setText(name);
    ui->listWidget->addItem(item);

    item->setHidden(!visible); // should be called after the item added into the widget;
}

void CPropertyDialog::creatConnections()
{
    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slotShowWidget(QListWidgetItem*)));
    connect(ui->okButton,&QPushButton::clicked,this,&CPropertyDialog::slotConfirmButtonClicked);
    connect(ui->cancelButton,&QPushButton::clicked,this,&CPropertyDialog::slotCancelButtonClicked);
}

void CPropertyDialog::slotShowWidget(QListWidgetItem *item)
{
    ui->stackedWidget->setCurrentIndex(item->data (Qt::UserRole).toInt());
}

void CPropertyDialog::slotConfirmButtonClicked()
{
    accept();
    m_commonForm->saveFields();
}

void CPropertyDialog::slotCancelButtonClicked()
{

    reject();
}

void CPropertyDialog::setPropertyData (CDataGroupBasicData *value)
{
    m_apperanceForm->setPropertyData(value);
}

CDataGroupBasicData *CPropertyDialog::propertyData()
{
    return m_apperanceForm->propertyData();
}

void CPropertyDialog::setFields(QList<CDataGroupField> *value)
{
    m_commonForm->setFields(value);
    m_apperanceForm->setFields(value);
}

QList<CDataGroupField> *CPropertyDialog::fields()
{
    return m_commonForm->fields();
}

QString CPropertyDialog::getViewName()
{
    return m_commonForm->getViewName();
}
