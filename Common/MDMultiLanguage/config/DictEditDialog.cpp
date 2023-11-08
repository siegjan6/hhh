#include <QMessageBox>
#include <QFileDialog>
#include "DictEditDialog.h"
#include "ui_DictEditDialog.h"
#include "DictDialog.h"
#include "MDMultiLanguage.h"
/////////////////////////////////////////////////////////////
CDictEditDialog::CDictEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDictEditDialog)
{
    ui->setupUi(this);

    m_isSystem = true;
    m_manager = NULL;

    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setModel(&m_model);
}
CDictEditDialog::~CDictEditDialog()
{
    delete ui;
}
/////////////////////////////////////////////////////////////
void CDictEditDialog::on_pushButtonAdd_clicked()
{
    QString key = ui->lineEditAdd->text();
    if(key.length() == 0)
        return;

    bool result = m_manager->addWord(key, m_isSystem);
    if(result)
    {
        m_manager->appendModel(m_model, key, m_isSystem);
        ui->tableView->setCurrentIndex(QModelIndex());
    }
}
void CDictEditDialog::on_pushButtonDel_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid())
        return;

    QString key = m_model.item(index.row(), 1)->text();
    QMessageBox box(this);
    box.setText(CMDMultiLanguage::instance()->value(tr("确认删除[%1]?")).arg(key));
    box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    box.setDefaultButton(QMessageBox::Cancel);
    CMDMultiLanguage::instance()->translateWidget(&box);
    if(box.exec() == QMessageBox::Ok)
    {
        m_model.removeRow(index.row());
        m_manager->deleteWord(key, m_isSystem);
    }
}
void CDictEditDialog::on_pushButtonFind_clicked()
{
    QString str = ui->lineEditAdd->text();
    if(str.isEmpty())
        return;

    int currentRow = ui->tableView->currentIndex().row();
    QList<QStandardItem*> list = m_model.findItems(str, Qt::MatchContains, 1);
    if(list.isEmpty())
        return;

    int findRow = -1;
    for(int i = 0; i < list.count(); i++)
    {
        int row = list.at(i)->row();
        if(row > currentRow)
        {
            findRow = row;
            break;
        }
    }
    if(findRow == -1)
        findRow = list.at(0)->row();

    ui->tableView->setCurrentIndex(m_model.index(findRow, 1));
}
void CDictEditDialog::on_pushButtonExport_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.length() == 0)
        return;

    m_manager->exportToFile(m_model, fileName);
}
void CDictEditDialog::on_pushButtonImport_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (fileName.length() == 0)
        return;

    save();
    m_manager->importFromFile(fileName, m_isSystem);
    refresh();
}
/////////////////////////////////////////////////////////////
void CDictEditDialog::init(CDictionaryManager *manager, bool isSytem)
{
    m_manager = manager;
    m_isSystem = isSytem;

    refresh();

    connect(&m_model, &QStandardItemModel::itemChanged, this, &CDictEditDialog::itemChanged);
    connect(ui->tableView, &QTableView::clicked, this, &CDictEditDialog::clicked);
}
void CDictEditDialog::save()
{
    m_manager->saveDataFromModel(m_model, m_isSystem);
}
void CDictEditDialog::refresh()
{
    m_manager->fillModel(m_model, m_isSystem);

    QTableView* view = ui->tableView;
    view->setItemDelegateForColumn(0, &m_readOnlyDelegate);
    view->setItemDelegateForColumn(1, &m_readOnlyDelegate);
    view->sortByColumn(0);
    view->setCurrentIndex(m_model.index(0, 0));
    view->setColumnWidth(0, 200);
    view->setColumnWidth(1, 200);
}
/////////////////////////////////////////////////////////////
void CDictEditDialog::itemChanged(QStandardItem *item)
{
    int col = item->index().column();
    if(col == 0 || col == 1)
        return;
}
void CDictEditDialog::clicked(const QModelIndex &index)
{
    if(index.column() == 1)
        ui->lineEditAdd->setText(m_model.item(index.row(), index.column())->text());
}
/////////////////////////////////////////////////////////////



