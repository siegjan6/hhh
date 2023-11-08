#include "ViewDBSourceDialog.h"
#include "ui_ViewDBSourceDialog.h"
#include <QDebug>

ViewDBSourceDialog::ViewDBSourceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewDBSourceDialog)
{
    ui->setupUi(this);
    connect(ui->listWidgetDS, SIGNAL(itemClicked(QListWidgetItem*)),this, SLOT(slot_itemDSChanged(QListWidgetItem*)));
    connect(ui->listWidgetDB, SIGNAL(itemClicked(QListWidgetItem*)),this, SLOT(slot_itemDBChanged(QListWidgetItem*)));
}

ViewDBSourceDialog::~ViewDBSourceDialog()
{
    delete ui;
}

void ViewDBSourceDialog::setDBQueryPath(const QString &DBQueryPath)
{
    m_DBQueryPath = DBQueryPath;
}

QList<DBQ::ViewDBSource> ViewDBSourceDialog::ViewDBsourceList() const
{
    return m_ViewDBsourceList;
}

void ViewDBSourceDialog::setViewDBsourceList(const QList<DBQ::ViewDBSource> &ViewDBsourceList)
{
    m_ViewDBsourceList = ViewDBsourceList;
}

void ViewDBSourceDialog::initViewDBSource()
{
    m_DBsourcesList.clear();
    QString errStr;
    bool ok = DBQ::DataSource_Load(m_DBQueryPath+"/global_dbsources.json", DBQ::FILE_TYPE_JSON, m_DBsourcesList, errStr);
    if(ok)
    {
        int size = m_DBsourcesList.size();
        for(int i = 0 ; i < size; i++)
        {
            QListWidgetItem *pSrcItem =  new QListWidgetItem;
            QVariant var;
            var.setValue(m_DBsourcesList.at(i));
            pSrcItem->setData(Qt::UserRole,var);
            pSrcItem->setText(m_DBsourcesList.at(i)->dbSrcName);
            pSrcItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
            if(isDSChecked(m_DBsourcesList.at(i)))
            {
                pSrcItem->setCheckState(Qt::Checked);
            }
            else
            {
                pSrcItem->setCheckState(Qt::Unchecked);
            }
            ui->listWidgetDS->addItem(pSrcItem);
        }

        ui->listWidgetDS->sortItems(Qt::AscendingOrder);
    }
    int count = ui->listWidgetDS->count();
    for(int i = 0; i < count; i++)
    {
        QListWidgetItem *item = ui->listWidgetDS->item(i);
        setListWidgetItemStyle(item);
    }

    for(int i = 0; i < count; i++)
    {
        QListWidgetItem *item = ui->listWidgetDS->item(i);
        if(item->checkState() == Qt::Checked)
        {
            emit ui->listWidgetDS->itemClicked(item);
            return;
        }
    }
}

void ViewDBSourceDialog::setListWidgetItemStyle(QListWidgetItem *item)
{
    item->setSizeHint(QSize(item->sizeHint().width(),30));
    QFont font;
    font.setPointSize(12);
    item->setFont(font);
}

DBQ::DBSource *ViewDBSourceDialog::getDataSource(const QString &name)
{
    DBQ::DBSource *pDataSrc = NULL;
    int size = m_DBsourcesList.size();
    for (int i = 0; i < size; ++i)
    {
        pDataSrc = m_DBsourcesList.at(i);
        if (pDataSrc && (pDataSrc->dbSrcName == name))
        {
            return pDataSrc;
        }
    }

    return NULL;
}

QString ViewDBSourceDialog::getDataBase(DBQ::ViewDBSource viewdbsource,const QString &name)
{
    int size = viewdbsource.dbNames.size();
    for (int i = 0; i < size; ++i)
    {
        if (viewdbsource.dbNames.at(i) == name)
        {
            return name;
        }
    }

    return "";
}

bool ViewDBSourceDialog::isDSChecked(DBQ::DBSource *dbsource)
{
    bool ok = false;
    int size = m_ViewDBsourceList.size();
    for(int i = 0; i < size; ++i)
    {
        if(dbsource == getDataSource(m_ViewDBsourceList.at(i).dbSrcName))
        {
            ok = true;
            return ok;
        }
    }
    return ok;
}

bool ViewDBSourceDialog::isDBChecked(DBQ::ViewDBSource viewdbsource, QString name)
{
    bool ok = false;
    if( name == getDataBase(viewdbsource,name))
    {
        ok = true;
    }
    return ok;
}

bool ViewDBSourceDialog::isExistDBSrcName(QString dbsrcname)
{
    bool ok = false;
    int size = m_ViewDBsourceList.size();
    for(int i = 0 ; i < size; i++)
    {
        if(m_ViewDBsourceList.at(i).dbSrcName == dbsrcname)
        {
            ok = true;
            return ok;
        }
    }
    return ok;
}

DBQ::ViewDBSource ViewDBSourceDialog::getViewDBSource(QString dbsrcname)
{
    DBQ::ViewDBSource viewDBsource;
    viewDBsource.dbSrcName = "";
    viewDBsource.dbNames.clear();
    int size = m_ViewDBsourceList.size();
    for(int i = 0 ; i < size; i++)
    {
        if(m_ViewDBsourceList.at(i).dbSrcName == dbsrcname)
        {
            viewDBsource = m_ViewDBsourceList.at(i);
        }
    }
    return viewDBsource;
}

bool ViewDBSourceDialog::removeViewDBSource(QString dbsrcname)
{
    bool success = false;
    int size = m_ViewDBsourceList.size();
    for(int i = 0; i < size; i++)
    {
        if(m_ViewDBsourceList.at(i).dbSrcName == dbsrcname)
        {
            m_ViewDBsourceList.removeAt(i);
            success = true;
            return success;
        }
    }
    return success;
}

void ViewDBSourceDialog::AddDBToViewDBs(QString dbSrcName, QString currentDBName)
{
    if(!isExistDBSrcName(dbSrcName))
        return;

    DBQ::ViewDBSource viewDBsource = getViewDBSource(dbSrcName);
    if(!viewDBsource.dbNames.contains(currentDBName,Qt::CaseSensitive))
    {
        viewDBsource.dbNames.append(currentDBName);
        if(removeViewDBSource(dbSrcName))
        {
            m_ViewDBsourceList.append(viewDBsource);
        }
    }
}

void ViewDBSourceDialog::RemoveDBFromViewDBs(QString dbSrcName, QString currentDBName)
{
    if(!isExistDBSrcName(dbSrcName))
        return;

    DBQ::ViewDBSource viewDBsource = getViewDBSource(dbSrcName);
    if(viewDBsource.dbNames.contains(currentDBName,Qt::CaseSensitive))
    {
        int size = viewDBsource.dbNames.size();
        for(int i = 0; i < size; i++)
        {
            if(viewDBsource.dbNames.at(i) == currentDBName)
            {
                viewDBsource.dbNames.removeAt(i);
            }
        }
        viewDBsource.dbNames.removeOne(currentDBName);
        if(removeViewDBSource(dbSrcName))
        {
            m_ViewDBsourceList.append(viewDBsource);
        }
    }
}

void ViewDBSourceDialog::slot_itemDSChanged(QListWidgetItem *item)
{
    DBQ::DBSource *dbsource = item->data(Qt::UserRole).value<DBQ::DBSource*>();

    if(dbsource)
    {
        ui->listWidgetDB->clear();
        int size = dbsource->dbs.size();
        for(int i = 0 ; i < size; i++)
        {
            QVariant vardbsource;
            QString dbname = "";
            dbname = dbsource->dbs.at(i).dbName;
            bool defaultdb = false;
            defaultdb = dbsource->dbs.at(i).bDefault;
            QListWidgetItem *pDBItem = new QListWidgetItem;
            vardbsource.setValue(dbsource);
            pDBItem->setData(Qt::UserRole, vardbsource);
            pDBItem->setData(Qt::UserRole+1, dbname);
            pDBItem->setData(Qt::UserRole+2, defaultdb);
            pDBItem->setText(dbsource->dbs.at(i).dbName);
            pDBItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
            if(isDSChecked(dbsource))
            {
                if(isDBChecked(getViewDBSource(dbsource->dbSrcName),dbsource->dbs.at(i).dbName))
                {
                    pDBItem->setCheckState(Qt::Checked);
                }
                else
                {
                    pDBItem->setCheckState(Qt::Unchecked);
                }
            }
            else
            {
                pDBItem->setCheckState(Qt::Unchecked);
            }
            ui->listWidgetDB->addItem(pDBItem);
            setListWidgetItemStyle(pDBItem);
        }

        ui->listWidgetDB->sortItems(Qt::AscendingOrder);
    }

    QString currentDBSrcName = dbsource->dbSrcName;

    if(item->checkState() == Qt::Checked)
    {
        if(isExistDBSrcName(currentDBSrcName))
            return;

        DBQ::ViewDBSource viewDBsource;
        viewDBsource.dbSrcName = currentDBSrcName;
        viewDBsource.dbNames.clear();
        m_ViewDBsourceList.append(viewDBsource);
    }
    else if(item->checkState() == Qt::Unchecked)
    {
        if(!isExistDBSrcName(currentDBSrcName))
            return;

        if(removeViewDBSource(currentDBSrcName))
        {
            int size = ui->listWidgetDB->count();
            for(int i = 0 ; i < size; i++)
            {
                ui->listWidgetDB->item(i)->setCheckState(Qt::Unchecked);
            }
            qDebug()<<"从视图数据源中删除"<<currentDBSrcName<<"成功！";
        }
    }
    else
    {
        return;
    }
}

void ViewDBSourceDialog::slot_itemDBChanged(QListWidgetItem *item)
{
    DBQ::DBSource *dbsource = item->data(Qt::UserRole).value<DBQ::DBSource*>();
    QString currentDBSrcName = dbsource->dbSrcName;
    QString currentDBName = item->text();
    if(item->checkState() == Qt::Checked)
    {
        AddDBToViewDBs(currentDBSrcName,currentDBName);
    }
    else if(item->checkState() == Qt::Unchecked)
    {
        RemoveDBFromViewDBs(currentDBSrcName,currentDBName);
    }
    else
    {
        return;
    }
}

void ViewDBSourceDialog::on_pbt_Confirm_clicked()
{
    this->accept();
}

void ViewDBSourceDialog::on_pbt_Cancel_clicked()
{
    this->reject();
}
