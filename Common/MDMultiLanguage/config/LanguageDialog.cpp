#include "LanguageDialog.h"
#include "ui_LanguageDialog.h"
#include "DictionaryManager.h"
#include "MDMultiLanguage.h"
/////////////////////////////////////////////////////////
CLanguageDialog::CLanguageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLanguageDialog)
{
    ui->setupUi(this);

    m_manager = NULL;
    ui->tableWidget->setColumnWidth(0, 200);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
CLanguageDialog::~CLanguageDialog()
{
    delete ui;
}
/////////////////////////////////////////////////////////
void CLanguageDialog::init(CDictionaryManager *manager)
{
    m_manager = manager;
    int systemIndex = m_manager->defaultSystemIndex();
    int projectIndex = m_manager->defaultProjectIndex();
    int count = m_manager->languages()->count();
    if (systemIndex < 0 && systemIndex >= count)
        systemIndex = -1;
    if (projectIndex < 0 && projectIndex >= count)
        projectIndex = -1;

    ui->checkBoxEnable->setChecked(m_manager->enable());

    ui->tableWidget->setRowCount(count);
    for(int i = 0; i < count; i++)
    {
        QString name = CMDMultiLanguage::instance()->value(m_manager->languages()->at(i)->name());
        bool isSelect = m_manager->languages()->at(i)->isSelect();

        QTableWidgetItem* item = new QTableWidgetItem(name);
        if (isSelect)
            item->setCheckState(Qt::Checked);
        else
            item->setCheckState(Qt::Unchecked);
        ui->tableWidget->setItem(i, 0, item);
    }
    resetSelectIndexs();
    resetComboBoxItem();
    resetComboBoxIndex(systemIndex, true);
    resetComboBoxIndex(projectIndex, false);

    connect(ui->tableWidget, &QTableWidget::cellChanged, this, &CLanguageDialog::cellChanged);
}
void CLanguageDialog::save()
{
    m_manager->setEnable(ui->checkBoxEnable->checkState() == Qt::Checked);

    m_manager->setDefaultSystemIndex(defaultIndex(true));
    m_manager->setDefaultProjectIndex(defaultIndex(false));

    int count = ui->tableWidget->rowCount();
    for(int i = 0; i < count; i++)
    {
        QTableWidgetItem* item = ui->tableWidget->item(i, 0);
        m_manager->languages()->at(i)->setIsSelect(item->checkState() == Qt::Checked);
    }
}
/////////////////////////////////////////////////////////
void CLanguageDialog::resetSelectIndexs()
{
    m_selectIndexs.clear();

    int count = ui->tableWidget->rowCount();
    for(int i = 0; i < count; i++)
    {
        QTableWidgetItem* item = ui->tableWidget->item(i, 0);
        if (item->checkState() == Qt::Checked)
            m_selectIndexs.append(i);
    }
}
void CLanguageDialog::cellChanged(int row, int column)
{
    int systemIndex = defaultIndex(true);
    int projectIndex = defaultIndex(false);

    resetSelectIndexs();
    resetComboBoxItem();

    resetComboBoxIndex(systemIndex, true);
    resetComboBoxIndex(projectIndex, false);
}
int CLanguageDialog::defaultIndex(bool isSystem)
{
    int index = 0;
    if (isSystem)
        index = ui->comboBoxSystem->currentIndex();
    else
        index = ui->comboBoxProject->currentIndex();

    if (index != -1)
        index = m_selectIndexs.at(index);

    return index;
}
void CLanguageDialog::resetComboBoxIndex(int oldIndex, bool isSytem)
{
    int index = m_selectIndexs.indexOf(oldIndex);
    if (index == -1)
        index = 0;

    if (isSytem)
        ui->comboBoxSystem->setCurrentIndex(index);
    else
        ui->comboBoxProject->setCurrentIndex(index);
}
void CLanguageDialog::resetComboBoxItem()
{
    ui->comboBoxSystem->clear();
    ui->comboBoxProject->clear();

    int count = ui->tableWidget->rowCount();
    for(int i = 0; i < count; i++)
    {
        QTableWidgetItem* item = ui->tableWidget->item(i, 0);
        if (item->checkState() == Qt::Checked)
        {
            ui->comboBoxSystem->addItem(item->text());
            ui->comboBoxProject->addItem(item->text());
        }
    }
}
/////////////////////////////////////////////////////////
