#include "RuntimeSourceDialog.h"
#include "ui_RuntimeSourceDialog.h"
#include "NewDataSourceDialog.h"
#include "NewDataBaseDialog.h"
#include "SourceTreeDialog.h"
#include <QMessageBox>
#include <QDebug>

RuntimeSourceDialog::RuntimeSourceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RuntimeSourceDialog),
    m_pDBSourcePopupMenu(NULL),
    m_pDBSource_DataArchives(NULL),
    m_pDBSource_NewAction(NULL),
    m_pDBSource_DeleteAction(NULL),
    m_pDBSource_EditAction(NULL),
    m_pDBSource_DefaultDataSource(NULL),
    m_pDataBasePopupMenu(NULL),
    m_pDataBase_NewAction(NULL),
    m_pDataBase_DeleteAction(NULL),
    m_pDataBase_ModifyAction(NULL),
    m_pDataBase_DefaultDataBase(NULL),
    m_DBSourcePath("")
{
    ui->setupUi(this);
    sigAndSlots();
    QWidget::setTabOrder(ui->listWidgetDS,ui->listWidgetDB);
    QWidget::setTabOrder(ui->listWidgetDB, ui->pbt_Confirm);
    QWidget::setTabOrder(ui->pbt_Confirm, ui->pbt_Cancel);
    QWidget::setTabOrder(ui->pbt_Cancel, ui->listWidgetDS);
}

RuntimeSourceDialog::~RuntimeSourceDialog()
{
    delete ui;

    if(m_pDBSourcePopupMenu)
    {
        delete m_pDBSourcePopupMenu;
        m_pDBSourcePopupMenu = NULL;
    }
    if(m_pDBSource_DataArchives)
    {
        delete m_pDBSource_DataArchives;
        m_pDBSource_DataArchives = NULL;
    }
    if(m_pDBSource_NewAction)
    {
        delete m_pDBSource_NewAction;
        m_pDBSource_NewAction = NULL;
    }
    if(m_pDBSource_DeleteAction)
    {
        delete m_pDBSource_DeleteAction;
        m_pDBSource_DeleteAction = NULL;
    }
    if(m_pDBSource_EditAction)
    {
        delete m_pDBSource_EditAction;
        m_pDBSource_EditAction = NULL;
    }
    if(m_pDBSource_DefaultDataSource)
    {
        delete m_pDBSource_DefaultDataSource;
        m_pDBSource_DefaultDataSource = NULL;
    }
    if(m_pDataBasePopupMenu)
    {
        delete m_pDataBasePopupMenu;
        m_pDataBasePopupMenu = NULL;
    }
    if(m_pDataBase_NewAction)
    {
        delete m_pDataBase_NewAction;
        m_pDataBase_NewAction = NULL;
    }
    if(m_pDataBase_DeleteAction)
    {
        delete m_pDataBase_DeleteAction;
        m_pDataBase_DeleteAction = NULL;
    }
    if(m_pDataBase_ModifyAction)
    {
        delete m_pDataBase_ModifyAction;
        m_pDataBase_ModifyAction = NULL;
    }
    if(m_pDataBase_DefaultDataBase)
    {
        delete m_pDataBase_DefaultDataBase;
        m_pDataBase_DefaultDataBase = NULL;
    }
}

void RuntimeSourceDialog::initUI()
{
    m_DBsourcesList.clear();
    QString errStr;
    ui->listWidgetDS->setContextMenuPolicy( Qt::CustomContextMenu );
    ui->listWidgetDB->setContextMenuPolicy( Qt::CustomContextMenu );
    connect(ui->listWidgetDS,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slot_EditDataSourceMenu(QPoint)));
    connect(ui->listWidgetDB,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slot_EditDataBaseMenu(QPoint)));
    disconnect(ui->listWidgetDS,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(slot_DoubleClickedModifyDBSource(QModelIndex)));
    connect(ui->listWidgetDS,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(slot_DoubleClickedModifyDBSource(QModelIndex)));
    disconnect(ui->listWidgetDB,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(slot_DoubleClickedModifyDataBase(QModelIndex)));
    connect(ui->listWidgetDB,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(slot_DoubleClickedModifyDataBase(QModelIndex)));


    bool ok = DBQ::DataSource_Load(m_DBSourcePath+"/"+"global_dbsources.json", DBQ::FILE_TYPE_JSON, m_DBsourcesList, errStr);
    if(ok)
    {
        int size = m_DBsourcesList.size();
        if(size == 1)
        {
            m_DBsourcesList.at(0)->bDefault = true;
        }
        for(int i = 0 ; i < size; i++)
        {
            QListWidgetItem *pSrcItem =  new QListWidgetItem;
            QVariant var;
            var.setValue(m_DBsourcesList.at(i));
            pSrcItem->setData(Qt::UserRole,var);
            pSrcItem->setText(m_DBsourcesList.at(i)->dbSrcName);
            ui->listWidgetDS->addItem(pSrcItem);
            if(m_DBsourcesList.at(i)->bDefault)
            {
                pSrcItem->setTextColor(QColor(Qt::blue));
                ui->listWidgetDS->setCurrentItem(pSrcItem);
                emit ui->listWidgetDS->itemClicked(pSrcItem);
            }
            else
            {
                pSrcItem->setTextColor(QColor(Qt::black));
            }
        }

        ui->listWidgetDS->sortItems(Qt::AscendingOrder);
    }
    int size = ui->listWidgetDS->count();
    for(int i = 0; i < size; i++)
    {
        QListWidgetItem *item = ui->listWidgetDS->item(i);
        setListWidgetItemStyle(item);
    }
}

void RuntimeSourceDialog::sigAndSlots()
{
    connect(ui->listWidgetDS, SIGNAL(itemClicked(QListWidgetItem*)),this, SLOT(slot_itemChanged(QListWidgetItem*)));
}

int RuntimeSourceDialog::getDBsourcesListCount()
{
    return m_DBsourcesList.count();
}

QList<DBQ::DBSource *> RuntimeSourceDialog::DBsourcesList() const
{
    return m_DBsourcesList;
}

void RuntimeSourceDialog::setDBsourcesList(const QList<DBQ::DBSource *> &DBsourcesList)
{
    m_DBsourcesList = DBsourcesList;
}

void RuntimeSourceDialog::showDataSourceMenu(const QPoint &pos)
{
    if(!m_pDBSourcePopupMenu)
    {
        m_pDBSourcePopupMenu = new QMenu(tr("新建"),ui->listWidgetDS);

        if(!m_pDBSource_DataArchives)
        {
            m_pDBSource_DataArchives = new QAction(tr("从数据归档添加数据源"),ui->listWidgetDS);
            connect(m_pDBSource_DataArchives, SIGNAL(triggered()),this,SLOT(slot_addDataArchivesDataSource()));
            m_pDBSourcePopupMenu->addAction(m_pDBSource_DataArchives);
        }
        if(!m_pDBSource_NewAction)
        {
            m_pDBSource_NewAction = new QAction(tr("新建数据源"),ui->listWidgetDS);
            connect(m_pDBSource_NewAction,SIGNAL(triggered()),this,SLOT(slot_NewDBSource()));
            m_pDBSourcePopupMenu->addAction(m_pDBSource_NewAction);
        }
        if(!m_pDBSource_DeleteAction)
        {
            m_pDBSource_DeleteAction = new QAction(tr("删除数据源"),ui->listWidgetDS);
            connect(m_pDBSource_DeleteAction,SIGNAL(triggered()),this,SLOT(slot_DeleteDBSource()));
            m_pDBSourcePopupMenu->addAction(m_pDBSource_DeleteAction);
        }
        if(!m_pDBSource_EditAction)
        {
            m_pDBSource_EditAction = new QAction(tr("修改数据源"),ui->listWidgetDS);
            connect(m_pDBSource_EditAction,SIGNAL(triggered()),this,SLOT(slot_ModifyDBSource()));
            m_pDBSourcePopupMenu->addAction(m_pDBSource_EditAction);
        }
        if(!m_pDBSource_DefaultDataSource)
        {
            m_pDBSource_DefaultDataSource = new QAction(tr("设为当前数据源"),ui->listWidgetDS);
            connect(m_pDBSource_DefaultDataSource,SIGNAL(triggered()),this,SLOT(slot_setDefaultDataSource()));
            m_pDBSourcePopupMenu->addAction(m_pDBSource_DefaultDataSource);
        }
    }
    m_pDBSourcePopupMenu->exec(ui->listWidgetDS->mapToGlobal(pos));

}

void RuntimeSourceDialog::showDataBaseMenu(const QPoint &pos)
{
    if(!m_pDataBasePopupMenu)
    {
        m_pDataBasePopupMenu = new QMenu(tr("新建"),ui->listWidgetDB);

        if(!m_pDataBase_NewAction)
        {
            m_pDataBase_NewAction = new QAction(tr("新建数据库"),ui->listWidgetDB);
            connect(m_pDataBase_NewAction,SIGNAL(triggered()),this,SLOT(slot_NewDataBase()));
            m_pDataBasePopupMenu->addAction(m_pDataBase_NewAction);
        }
        if(!m_pDataBase_DeleteAction)
        {
            m_pDataBase_DeleteAction = new QAction(tr("删除数据库"),ui->listWidgetDB);
            connect(m_pDataBase_DeleteAction,SIGNAL(triggered()),this,SLOT(slot_DeleteDataBase()));
            m_pDataBasePopupMenu->addAction(m_pDataBase_DeleteAction);
        }
        if(!m_pDataBase_ModifyAction)
        {
            m_pDataBase_ModifyAction = new QAction(tr("修改数据库"),ui->listWidgetDB);
            connect(m_pDataBase_ModifyAction,SIGNAL(triggered()),this,SLOT(slot_ModifyDataBase()));
            m_pDataBasePopupMenu->addAction(m_pDataBase_ModifyAction);
        }
        if(!m_pDataBase_DefaultDataBase)
        {
            m_pDataBase_DefaultDataBase = new QAction(tr("设为当前数据库"),ui->listWidgetDB);
            connect(m_pDataBase_DefaultDataBase,SIGNAL(triggered()),this,SLOT(slot_setDefaultDataBase()));
            m_pDataBasePopupMenu->addAction(m_pDataBase_DefaultDataBase);
        }
    }
    m_pDataBasePopupMenu->exec(ui->listWidgetDB->mapToGlobal(pos));
}

bool RuntimeSourceDialog::removeDataSource(DBQ::DBSource *dbsource)
{
    bool success = false;
    if (dbsource)
    {
        int index = m_DBsourcesList.indexOf(dbsource);
        if (-1 != index)
        {
            m_DBsourcesList.removeAt(index);
            delete dbsource;
            dbsource = NULL;
            success = true;
        }
    }

    return success;
}

bool RuntimeSourceDialog::removeDB(DBQ::DBSource *dbsource, QListWidgetItem* item)
{
    bool ok = false;
    int num = -1;
    for(int i = 0 ; i < dbsource->dbs.size(); i++)
    {
        if(dbsource->dbs.at(i).dbName == item->text())
        {
            ok = true;
            num = i;
        }
    }
    if(ok)
    {
        dbsource->dbs.removeAt(num);
        QListWidgetItem * item = ui->listWidgetDB->takeItem(ui->listWidgetDB->currentRow());
        delete item;
        qDebug()<<"成功删除当前数据库节点！";
        return true;
    }
    else
    {
        QMessageBox::information(NULL,"警告",tr("删除数据库%1失败！").arg(dbsource->dbSrcName));
        return false;

    }
}

DBQ::DBSource *RuntimeSourceDialog::getDataSource(const QString &name)
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

DBQ::DBSource *RuntimeSourceDialog::getDataSource(int index)
{
    DBQ::DBSource *pDataSrc = NULL;

    if (0 <= index && index < m_DBsourcesList.size())
    {
        pDataSrc = m_DBsourcesList.at(index);
    }

    return pDataSrc;
}

DBQ::DB *RuntimeSourceDialog::getDataBase(DBQ::DBSource *dbsource, const QString &name)
{
    DBQ::DB *db = new DBQ::DB;
    int size = dbsource->dbs.size();
    for(int i = 0; i< size; i++)
    {
        *db = dbsource->dbs[i];
        if(db && (db->dbName == name))
        {
            return db;
        }
    }
    return NULL;
}

void RuntimeSourceDialog::setListWidgetItemStyle(QListWidgetItem *item)
{
    item->setSizeHint(QSize(item->sizeHint().width(),30));
    QFont font;
    font.setPointSize(12);
    item->setFont(font);
}

void RuntimeSourceDialog::setDSItemBackgroundColor()
{
    for(int i = 0 ; i < ui->listWidgetDS->count(); i ++)
    {
        if(m_DBsourcesList.at(i)->bDefault)
        {
            ui->listWidgetDS->item(i)->setTextColor(QColor(Qt::blue));
        }
        else
        {
            ui->listWidgetDS->item(i)->setTextColor(QColor(Qt::black));
        }
    }
}

bool RuntimeSourceDialog::existDBSource(const QString &dbSrcName)
{
    bool ok = false;
    if(dbSrcName.isEmpty())
    {
        return ok;
    }
    int size = ui->listWidgetDS->count();
    for(int i = 0; i < size; i++)
    {
        if(ui->listWidgetDS->item(i)->text() == dbSrcName)
        {
            ok = true;
            return ok;
        }
    }
    return ok;
}

void RuntimeSourceDialog::setUNDefaultDBSouce()
{
    for(int i = 0; i < m_DBsourcesList.size(); i++)
    {
        m_DBsourcesList.at(i)->bDefault = false;
        int dbsize = m_DBsourcesList.at(i)->dbs.size();
        if(dbsize > 0)
        {
            for(int j = 0; j < dbsize; j++)
            {
                m_DBsourcesList.at(i)->dbs[j].bDefault = false;
            }
        }
    }
}

void RuntimeSourceDialog::modifyDBSource(QListWidgetItem *item,DBQ::DBSource *dbsource)
{
    Q_UNUSED(item);
    NewDataSourceDialog dlg(tr("修改数据源"),this);
    dlg.setOriginalDSParameter(dbsource);

    int ret = dlg.exec();

    if(ret == QDialog::Rejected)
        return ; //用户取消修改

    if(ret == QDialog::Accepted)
    {
        dlg.getDataSourceParameter();
        DBQ::DBSource *dbSrc = new DBQ::DBSource;
        *dbSrc = *dlg.getDBSource();
        int size = dbsource->dbs.size();
        for(int i = 0; i < size; i++)
        {
            dbSrc->dbs.append(dbsource->dbs.at(i));
        }
        dbSrc->bDefault = dbsource->bDefault;;
        if(!removeDataSource(dbsource))
        {
            QMessageBox::information(NULL,"警告",tr("修改数据源%1失败！").arg(dbsource->dbSrcName));
            return;
        }

        if (dbSrc && (NULL == getDataSource(dbSrc->dbSrcName)))
        {
            QVariant var;
            var.setValue(dbSrc);
            ui->listWidgetDS->currentItem()->setData(Qt::UserRole, var);
            ui->listWidgetDS->currentItem()->setText(dbSrc->dbSrcName);
            m_DBsourcesList.append(dbSrc);
        }
    }
}

void RuntimeSourceDialog::modifyDataBase(QListWidgetItem *item)
{
    DBQ::DB db;
    DBQ::DBSource *dsdbsource = new DBQ::DBSource;
    DBQ::DBSource *dbsource = new DBQ::DBSource;

    dsdbsource = ui->listWidgetDS->currentItem()->data(Qt::UserRole).value<DBQ::DBSource*>();
    if(dsdbsource)
    {
        QVariant var;
        var.setValue(dsdbsource);
        item->setData(Qt::UserRole, var);
    }

    dbsource = item->data(Qt::UserRole).value<DBQ::DBSource*>();

    if(dbsource)
    {
        NewDataBaseDialog dlg(tr("修改数据库"),this);
        QString olddbname = ui->listWidgetDB->currentItem()->data(Qt::UserRole+1).toString();
        bool defaultDB = ui->listWidgetDB->currentItem()->data(Qt::UserRole+2).toBool();
        dlg.setDBName(olddbname);
        dlg.setDatabaseParameter(dbsource);

        int ret = dlg.exec();

        if(ret == QDialog::Rejected)
            return ; //用户取消修改

        if(ret == QDialog::Accepted)
        {
            QString dbname = dlg.getDBName();
            if(dbname == olddbname)
            {
                return;
            }

            if(NULL == getDataBase(dbsource,dbname))
            {
                if(dbname.isEmpty())
                {
                    QMessageBox::information(NULL,"警告",tr("数据库名为空！"));
                    return;
                }
                else
                {
                    if(removeDB(dbsource,item))
                    {
                        db.dbName = dbname;
                        db.bDefault = defaultDB;
                        dbsource->dbs.append(db);
                        emit ui->listWidgetDS->itemClicked(ui->listWidgetDS->currentItem());
                        ui->listWidgetDB->update();
                    }
                }
            }
            else
            {
                QMessageBox::information(NULL,"警告",tr("数据库名%1已经存在！").arg(dbname));
                return;
            }
        }
    }
}

void RuntimeSourceDialog::slot_EditDataSourceMenu(const QPoint &pos)
{
    showDataSourceMenu(pos);
}

void RuntimeSourceDialog::slot_EditDataBaseMenu(const QPoint &pos)
{
    showDataBaseMenu(pos);
}

void RuntimeSourceDialog::slot_DoubleClickedModifyDBSource(QModelIndex index)
{
    if(ui->listWidgetDS->item(index.row()))
    {
        DBQ::DBSource *dbsource = ui->listWidgetDS->item(index.row())->data(Qt::UserRole).value<DBQ::DBSource*>();
        if(dbsource)
        {
            modifyDBSource(ui->listWidgetDS->item(index.row()),dbsource);
        }
    }
}

void RuntimeSourceDialog::slot_DoubleClickedModifyDataBase(QModelIndex index)
{
    if(ui->listWidgetDB->item(index.row()))
    {
        modifyDataBase(ui->listWidgetDB->item(index.row()));
    }
}

void RuntimeSourceDialog::slot_itemChanged(QListWidgetItem *item)
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
            bool defaultdb = false;
            dbname = dbsource->dbs.at(i).dbName;
            if(dbname.isEmpty())
            {
                continue;
            }
            defaultdb = dbsource->dbs.at(i).bDefault;
            QListWidgetItem *pDBItem = new QListWidgetItem;
            vardbsource.setValue(dbsource);
            pDBItem->setData(Qt::UserRole, vardbsource);
            pDBItem->setData(Qt::UserRole+1, dbname);
            pDBItem->setData(Qt::UserRole+2, defaultdb);
            pDBItem->setText(dbsource->dbs.at(i).dbName);
            ui->listWidgetDB->addItem(pDBItem);
            setListWidgetItemStyle(pDBItem);
            if(pDBItem->data(Qt::UserRole+2).toBool())
            {
                pDBItem->setTextColor(QColor(Qt::blue));
                ui->listWidgetDB->setCurrentItem(pDBItem);
            }
            else
            {
                pDBItem->setTextColor(QColor(Qt::black));
            }
        }
        ui->listWidgetDB->sortItems(Qt::AscendingOrder);
    }
}

void RuntimeSourceDialog::slot_addDataArchivesDataSource()
{
    QListWidgetItem *pSrcItem = NULL;
    QVariant var;
    DBQ::DBSource *dbsource = new DBQ::DBSource;
    QString dataSrcFullPath = m_DBSourcePath.left(m_DBSourcePath.length()-7) + "DataArchives/" + QString("dataSource.dat");
    SourceTreeDialog datasourceDialog(QString(tr("数据归档数据源")),dataSrcFullPath);
    datasourceDialog.initDataSourceTree();
    datasourceDialog.setReturnMode(0);
    datasourceDialog.setStackWidgetShowMode(0);
    if(datasourceDialog.exec() == QDialog::Accepted)
    {
        QList<DBQ::DBSource *> m_addDBSourcesList;
        if(!datasourceDialog.getDBSourcesSelected().isEmpty())
        {
            foreach (DBQ::DBSource * pDBSource, datasourceDialog.getDBSourcesSelected())
            {
                DBQ::DBSource * pDBSourceCp = new DBQ::DBSource;
                *pDBSourceCp = *pDBSource;
                m_addDBSourcesList.append(pDBSourceCp);
            }

            setUNDefaultDBSouce();

            int size = m_addDBSourcesList.size();
            for(int i = 0; i < size; i++)
            {
                QString datasrcName = m_addDBSourcesList.at(i)->dbSrcName;
                var.setValue(m_addDBSourcesList.at(i));
                pSrcItem = new QListWidgetItem;
                pSrcItem->setData(Qt::UserRole,var);
                pSrcItem->setText(datasrcName);
                setListWidgetItemStyle(pSrcItem);
                ui->listWidgetDS->addItem(pSrcItem);
                ui->listWidgetDS->setCurrentItem(pSrcItem);

                if(i == 0)
                {
                    m_addDBSourcesList.at(i)->bDefault = true;
                    int dbsize = m_addDBSourcesList.at(i)->dbs.size();
                    if(dbsize > 0)
                    {
                        for(int j = 0 ; j < dbsize; j++)
                        {
                            m_addDBSourcesList.at(i)->dbs[0].bDefault = true;
                            if(j > 0)
                            {
                                m_addDBSourcesList.at(i)->dbs[j].bDefault = false;
                            }
                        }
                    }
                    emit ui->listWidgetDS->itemClicked(pSrcItem);
                }
                else
                {
                    m_addDBSourcesList.at(i)->bDefault = false;
                    int dbsize = m_addDBSourcesList.at(i)->dbs.size();
                    if(dbsize > 0)
                    {
                        for(int j = 0 ; j < dbsize; j++)
                        {
                            m_addDBSourcesList.at(i)->dbs[j].bDefault = false;
                        }
                    }
                }

                m_DBsourcesList.append(m_addDBSourcesList.at(i));
            }
            setDSItemBackgroundColor();
        }
        else
        {
            *dbsource = *datasourceDialog.getDBSouce();
            QString datasrcName = dbsource->dbSrcName;

            if(existDBSource(datasrcName))
            {
                QMessageBox::information(NULL,"警告",tr("%1数据源已经添加!").arg(datasrcName));
                return;
            }

            setUNDefaultDBSouce();

            dbsource->bDefault = true;
            int dbsize = dbsource->dbs.size();
            if(dbsize > 0)
            {
                for(int j = 0 ; j < dbsize; j++)
                {
                    dbsource->dbs[0].bDefault = true;
                    if(j > 0)
                    {
                        dbsource->dbs[j].bDefault = false;
                    }
                }
            }

            var.setValue(dbsource);
            pSrcItem = new QListWidgetItem;
            pSrcItem->setData(Qt::UserRole,var);
            pSrcItem->setText(datasrcName);
            setListWidgetItemStyle(pSrcItem);
            ui->listWidgetDS->addItem(pSrcItem);
            ui->listWidgetDS->setCurrentItem(pSrcItem);
            m_DBsourcesList.append(dbsource);
            emit ui->listWidgetDS->itemClicked(pSrcItem);
            setDSItemBackgroundColor();
        }
    }
}

void RuntimeSourceDialog::slot_NewDBSource()
{
    NewDataSourceDialog dlg(tr("新建数据源"),this);
    if(dlg.exec() == QDialog::Accepted)
    {
        dlg.getDataSourceParameter();
        DBQ::DBSource *dbsource = new DBQ::DBSource;
        *dbsource = *dlg.getDBSource();
        int size = ui->listWidgetDS->count();
        for(int i = 0; i< size; i++)
        {
            if(ui->listWidgetDS->item(i)->text() == dbsource->dbSrcName)
            {
                QMessageBox::information(NULL,"警告",tr("%1数据源已经添加!").arg(dbsource->dbSrcName));
                return;
            }
            if(dbsource->dbSrcName.isEmpty())
            {
                QMessageBox::information(NULL,"警告",tr("%1数据源名称为空!").arg(dbsource->dbSrcName));
                return;
            }
        }
        QListWidgetItem *pItem = new QListWidgetItem;
        QVariant var;
        setUNDefaultDBSouce();
        dbsource->bDefault = true;
        var.setValue(dbsource);
        pItem->setData(Qt::UserRole, var);
        pItem->setText(dbsource->dbSrcName);
        ui->listWidgetDS->addItem(pItem);
        ui->listWidgetDS->setCurrentItem(pItem);
        setListWidgetItemStyle(pItem);
        m_DBsourcesList.append(dbsource);
        setDSItemBackgroundColor();
        emit ui->listWidgetDS->itemClicked(pItem);
    }
}

void RuntimeSourceDialog::slot_DeleteDBSource()
{
    QListWidgetItem *item = ui->listWidgetDS->currentItem();
    QString strMsg = QString("确定删除数据源'%1'吗？").arg(item->text());
    QMessageBox message(QMessageBox::Information,"提示", strMsg, QMessageBox::Yes | QMessageBox::No, NULL);

    if(item != NULL)
    {
        if (QMessageBox::Yes == message.exec())
        {
            DBQ::DBSource *dbsource = item->data(Qt::UserRole).value<DBQ::DBSource*>();
            bool bdefalut = dbsource->bDefault;

            if(removeDataSource(dbsource))
            {
                QListWidgetItem * item = ui->listWidgetDS->takeItem(ui->listWidgetDS->currentRow());
                delete item;
                if(ui->listWidgetDB->currentItem())
                {
                    ui->listWidgetDB->clear();
                }
                setDSItemBackgroundColor();
            }
            else
            {
                QMessageBox::information(NULL,"警告",tr("删除数据源%1失败！").arg(dbsource->dbSrcName));
                return;
            }

            if(bdefalut)
            {
                if(ui->listWidgetDS->count() > 0)
                {
                    QListWidgetItem *item = ui->listWidgetDS->item(0);
                    ui->listWidgetDS->setCurrentItem(item);
                    slot_setDefaultDataSource();
                    emit ui->listWidgetDS->itemClicked(item);
                }
            }
        }
    }
}

void RuntimeSourceDialog::slot_ModifyDBSource()
{
    if(ui->listWidgetDS->currentItem())
    {
        DBQ::DBSource *dbsource = ui->listWidgetDS->currentItem()->data(Qt::UserRole).value<DBQ::DBSource*>();
        if(dbsource)
        {
            modifyDBSource(ui->listWidgetDS->currentItem(),dbsource);
        }
    }
}

void RuntimeSourceDialog::slot_setDefaultDataSource()
{
    DBQ::DBSource *dbsource = new DBQ::DBSource;
    dbsource = ui->listWidgetDS->currentItem()->data(Qt::UserRole).value<DBQ::DBSource*>();
    int size = m_DBsourcesList.size();
    for(int i = 0 ; i < size; i++)
    {
        if(getDataSource(i) == dbsource)
        {
            m_DBsourcesList.at(i)->bDefault = true;
            ui->listWidgetDS->item(i)->setTextColor(Qt::blue);
            int dbsize = m_DBsourcesList.at(i)->dbs.size();
            if(dbsize > 0)
            {
                for(int j = 0; j < dbsize; j++)
                {
                    m_DBsourcesList.at(i)->dbs[0].bDefault = true;
                    if(j > 0)
                    {
                        m_DBsourcesList.at(i)->dbs[j].bDefault = false;
                    }
                }
            }
        }
        else
        {
            m_DBsourcesList.at(i)->bDefault = false;
            ui->listWidgetDS->item(i)->setTextColor(Qt::black);
            int dbsize =  m_DBsourcesList.at(i)->dbs.size();
            if(dbsize > 0)
            {
                for(int j = 0; j < dbsize; j++)
                {
                    m_DBsourcesList.at(i)->dbs[j].bDefault = false;
                }
            }
        }
    }
    emit ui->listWidgetDS->itemClicked(ui->listWidgetDS->currentItem());

}

void RuntimeSourceDialog::slot_NewDataBase()
{
    DBQ::DBSource *dbsource = new DBQ::DBSource;
    DBQ::DB db;
    QString dbname;
    if(ui->listWidgetDS->currentItem())
    {
        dbsource = ui->listWidgetDS->currentItem()->data(Qt::UserRole).value<DBQ::DBSource*>();
        if(dbsource)
        {
            NewDataBaseDialog dlg(tr("新建数据库"),this);;
            dlg.setDatabaseParameter(dbsource);
            if(dlg.exec() == QDialog::Accepted)
            {
                dbname = dlg.getDBName();

                if(NULL == getDataBase(dbsource,dbname))
                {
                    if(dbname.isEmpty())
                    {
                        QMessageBox::information(NULL,"警告",tr("数据库名为空！"));
                        return;
                    }
                    else
                    {
                        db.dbName = dbname;

                        if(dbsource->bDefault)
                        {
                            if(dbsource->dbs.size() == 0)
                            {
                                db.bDefault = true;
                            }
                            else
                            {
                                db.bDefault = false;
                            }
                        }
                        else
                        {
                            db.bDefault = false;
                        }

                        dbsource->dbs.append(db);
                        emit ui->listWidgetDS->itemClicked(ui->listWidgetDS->currentItem());
                    }
                }
                else
                {
                    QMessageBox::information(NULL,"警告",tr("数据库名%1已经存在！").arg(dbname));
                    return;
                }
            }
        }
    }
}

void RuntimeSourceDialog::slot_DeleteDataBase()
{
    QListWidgetItem * item = ui->listWidgetDB->currentItem();
    if(item != NULL)
    {
        QString strMsg = QString("确定删除数据库'%1'吗？").arg(item->text());
        QMessageBox message(QMessageBox::Information,"提示", strMsg, QMessageBox::Yes | QMessageBox::No, NULL);
        if (QMessageBox::Yes == message.exec())
        {
            DBQ::DBSource *dbsource = item->data(Qt::UserRole).value<DBQ::DBSource*>();
            removeDB(dbsource,item);
        }
    }
}

void RuntimeSourceDialog::slot_ModifyDataBase()
{
    if(ui->listWidgetDB->currentItem())
    {
        modifyDataBase(ui->listWidgetDB->currentItem());
    }
}

void RuntimeSourceDialog::slot_setDefaultDataBase()
{
    DBQ::DBSource *dbsource = new DBQ::DBSource;
    dbsource = ui->listWidgetDB->currentItem()->data(Qt::UserRole).value<DBQ::DBSource*>();
    if(!dbsource->bDefault)
    {
        QMessageBox::information(NULL,"警告",tr("当前数据源不是默认数据源，请先设置默认数据源，再设置默认数据库！"));
        return;
    }

    int size = dbsource->dbs.size();
    for(int i = 0 ; i < size; i++)
    {
        if(dbsource->dbs.at(i).dbName == ui->listWidgetDB->currentItem()->data(Qt::UserRole+1).toString())
        {
            dbsource->dbs[i].bDefault = true;
        }
        else
        {
            dbsource->dbs[i].bDefault = false;
        }
    }
    int itemcount = ui->listWidgetDB->count();
    for(int i = 0 ;i < itemcount; i++)
    {
        if(ui->listWidgetDB->item(i)->data(Qt::UserRole+1).toString() == ui->listWidgetDB->currentItem()->text())
        {
            if(ui->listWidgetDB->item(i)->data(Qt::UserRole+2).toBool())
            {
                ui->listWidgetDB->item(i)->setTextColor(QColor(Qt::blue));
            }
        }
        else
        {
            ui->listWidgetDB->item(i)->setTextColor(QColor(Qt::black));
        }
    }

    emit ui->listWidgetDS->itemClicked(ui->listWidgetDS->currentItem());
    ui->listWidgetDB->update();
}

void RuntimeSourceDialog::on_pbt_Confirm_clicked()
{
    QString errStr;
    bool ok = DBQ::DataSource_Write(m_DBSourcePath + "/global_dbsources.json", DBQ::FILE_TYPE_JSON, m_DBsourcesList, errStr);
    if(ok)
    {
        qDebug()<<"save DBSources to json ok!";
    }

    this->accept();
}

void RuntimeSourceDialog::on_pbt_Cancel_clicked()
{
    this->reject();
}
QString RuntimeSourceDialog::DBSourcePath() const
{
    return m_DBSourcePath;
}

void RuntimeSourceDialog::setDBSourcePath(const QString &DBSourcePath)
{
    m_DBSourcePath = DBSourcePath;
}

void RuntimeSourceDialog::on_btn_DSAdd_clicked()
{
    slot_addDataArchivesDataSource();
}

void RuntimeSourceDialog::on_btn_DSRemove_clicked()
{
    slot_DeleteDBSource();
}

void RuntimeSourceDialog::on_btn_DBAdd_clicked()
{
    slot_NewDataBase();
}

void RuntimeSourceDialog::on_btn_DBRemove_clicked()
{
    slot_DeleteDataBase();
}
