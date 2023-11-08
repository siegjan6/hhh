#include "ViewListDialog.h"
#include "ui_ViewListDialog.h"
#include <QDir>
#include <QDebug>

ViewListDialog::ViewListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewListDialog)
{
    ui->setupUi(this);
    m_pModel = new QStandardItemModel(1,1);
    m_pModel->setHeaderData(0, Qt::Horizontal, QObject::tr("视图名称"));
    ui->tableView->setModel(m_pModel);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);  //! 禁止编辑
    ui->tableView->verticalHeader()->hide();                            //! 隐藏计数列
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    QWidget::setTabOrder(ui->tableView, ui->okButton);
    QWidget::setTabOrder(ui->okButton, ui->cancelButton);
    QWidget::setTabOrder(ui->cancelButton, ui->tableView);
}

ViewListDialog::~ViewListDialog()
{
    delete ui;
}

bool ViewListDialog::LoadViews(const QString & dirStr)
{
    QDir dir(dirStr);
    if (!dir.exists())
        return false; //TODO:log

    //遍历文件名
    QStringList filters;
    filters << QString("*.%1").arg("json") ;
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files); // | QDir::NoDotAndDotDot

    QStringList m_CaseNameList = dir.entryList();

    //从文件名中读取视图名称 添加到表格中
    int size = m_CaseNameList.size();
    QString viewName;
    for(int i=0, j=0; i<size;i++)
    {
        if(GetViewNameFromFileName(m_CaseNameList[i], viewName))
        {
            m_pModel->setItem(j, 0, new QStandardItem(viewName));
            j++;
        }
    }

    return true;
}

QStringList ViewListDialog::GetViewNameSelected()
{
    return m_ViewNamesSelected;
}

void ViewListDialog::selectedCurrentViewName(QString viewName)
{
    m_ViewNamesSelected.clear();

    for(int i = 0 ; i < m_pModel->rowCount();i++)
    {
        if(m_pModel->data(m_pModel->index(i,0), Qt::UserRole).toString() == viewName)
        {
            qDebug()<<"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
        }
    }

//    QItemSelectionModel *selections = ui->tableView->selectionModel();
//    QModelIndexList selected = selections->selectedIndexes();
//    QModelIndex indexSelected;
//    foreach (QModelIndex index, selected)
//    {
//        //qDebug()<<index;
//        //设置了按行选中的模式 所以会返回所有选中行的cell
//        if(index.column() != 0)
//            continue;

//        indexSelected = m_pModel->index(index.row(), 0);

//        variant = m_pModel->data(indexSelected, Qt::UserRole);

//        QItemSelectionModel *selections = ui->tableView->selectionModel();
//        QModelIndexList selected = selections->selectedIndexes();
//        foreach (QModelIndex index, selected)
//        {
//            QModelIndex indexV = m_pModel->index(index.row(), 0);//获得视图名
//            if(!m_pModel->data(indexV).toString().isEmpty())
//                m_ViewNamesSelected<<m_pModel->data(indexV).toString();
//        }
//    }
}

bool ViewListDialog::GetViewNameFromFileName(const QString & strIn, QString & viewNameOut)
{
    //前提假设:文件名strIn以.xml结尾.
    QString str = strIn;
    int index = str.indexOf("view_", 0, Qt::CaseInsensitive); //文件名必须以view_t开始
    if(index != 0)
        return false;

    if(str.indexOf("run_view_vars")>=0)
        return false;

    //Test:str.clear(); 不会崩溃
    str = str.right(str.size()-5);//6==view_ 去掉view_
    str = str.left(str.size()-5); //4=.xml 去掉.xml
    if(str.isEmpty())
        return false;

    viewNameOut = str;
    return true;
}

//返回选中的多行
void ViewListDialog::on_okButton_clicked()
{
    m_ViewNamesSelected.clear();

    QItemSelectionModel *selections = ui->tableView->selectionModel();
    QModelIndexList selected = selections->selectedIndexes();
    foreach (QModelIndex index, selected)
    {
        QModelIndex indexV = m_pModel->index(index.row(), 0);//获得视图名
        if(!m_pModel->data(indexV).toString().isEmpty())
            m_ViewNamesSelected<<m_pModel->data(indexV).toString();
    }
    this->accept();
}

void ViewListDialog::on_cancelButton_clicked()
{
    this->reject();
}
