#include "selectfromlistdlg.h"
#include "ui_selectfromlistdlg.h"

#include <QList>
#include <QVariant>
#include <QListWidget>

SelectFromListDlg::SelectFromListDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectFromListDlg)
{
    ui->setupUi(this);
}

SelectFromListDlg::~SelectFromListDlg()
{
    delete ui;
}

bool SelectFromListDlg::SelectItem( const QString & s )
{
    QList<QListWidgetItem *> lst = ui->m_pList->findItems(s,Qt::MatchExactly);
    if( lst.size()>0 )
    {
        ui->m_pList->setCurrentItem( lst[0] );
        return true;
    }
    return false;
}

void SelectFromListDlg::AddItem( const QString & s, int iIndex )
{
    QListWidgetItem * pItem = new QListWidgetItem( s );
    pItem->setData(Qt::UserRole,iIndex);
    ui->m_pList->addItem(pItem);
}

QString SelectFromListDlg::GetSelectedItem() const
{
    QList<QListWidgetItem *> lst = ui->m_pList->selectedItems();
    if( lst.size()>0 )
    {
        return lst[0]->text();
    }
    return "";
}

int SelectFromListDlg::GetSelectedItemIndex() const
{
    QList<QListWidgetItem *> lst = ui->m_pList->selectedItems();
    if( lst.size()>0 )
    {
        return lst[0]->data(Qt::UserRole).toInt();
    }
    return -1;
}

bool SelectFromListDlg::IsInList( const QString & s ) const
{
    return ui->m_pList->findItems(s,Qt::MatchFixedString).size()>0;
}
