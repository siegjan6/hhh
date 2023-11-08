/****************************************************************************
*
*  Copyright (C) 2002-2008 Helta Kft. / NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: nszabo@helta.hu, info@nocisoft.com
*  Web: www.nocisoft.com
*
*  This file is part of the NCReport reporting software
*
*  Licensees holding a valid NCReport License Agreement may use this
*  file in accordance with the rights, responsibilities, and obligations
*  contained therein. Please consult your licensing agreement or contact
*  nszabo@helta.hu if any conditions of this licensing are not clear
*  to you.
*
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/
#include "ncrdconnectiondialog.h"
#include "ncrdmainwindow.h"
#include "ncrddef.h"
#include "ncrdformgeometry.h"

#include <QStandardItemModel>
#include <QDataWidgetMapper>
#include <QHash>
#include <QHeaderView>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>

#define CONNDIA_COL_NAME		0
#define CONNDIA_COL_DRIVER		1
#define CONNDIA_COL_HOST		2
#define CONNDIA_COL_USER		3
#define CONNDIA_COL_PASSWORD	4
#define CONNDIA_COL_DATABASE	5
#define CONNDIA_COL_PORT		6
#define CONNDIA_COL_CONNECTED	7
#define CONNDIA_COL_ISNEW		8
#define CONNDIA_COL_OPTIONS		9

#define CONNDIA_COLS			10

//! 多语言
#include "MDMultiLanguage.h"


static const char* const _drivers[] =
{ "QDB2","QIBASE","QMYSQL","QOCI","QODBC","QPSQL","QSQLITE2","QSQLITE","QTDS" ,"QSYMYSQL" ,"QKINGBASE" };


NCRDConnectionDialog::NCRDConnectionDialog(NCRDMainWindow* mw, QWidget* parent)
    : QDialog( parent ), Ui::NCRDSqlConnectionDialogUI(), mainwindow( mw )
{
    setupUi(this);
    lblStatus->setText( disconnectedText() );

    setupModel();
    listConns->setModel(model);

    NCRDConnectionItemDelegate *deleg = new NCRDConnectionItemDelegate( this );
    //deleg->setComboBoxes( comboFunc, comboScope );
    //MAPPER
    mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->setItemDelegate( deleg );

    mapper->addMapping(connName, CONNDIA_COL_NAME);
    mapper->addMapping(comboDriver, CONNDIA_COL_DRIVER);
    mapper->addMapping(host, CONNDIA_COL_HOST);
    mapper->addMapping(username, CONNDIA_COL_USER);
    mapper->addMapping(password, CONNDIA_COL_PASSWORD);
    mapper->addMapping(dbname, CONNDIA_COL_DATABASE);
    mapper->addMapping(wPort, CONNDIA_COL_PORT);
    mapper->addMapping(btnConnect, CONNDIA_COL_CONNECTED);
    mapper->addMapping(options, CONNDIA_COL_OPTIONS);

    connect( listConns->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
             this, SLOT(slotItemChanged(const QModelIndex&,const QModelIndex&)));

    connect(btnAdd, SIGNAL(clicked()), this, SLOT(add()));
    connect(btnRemove, SIGNAL(clicked()), this, SLOT(remove()));
    connect(btnConnect, SIGNAL(clicked(bool)), this, SLOT(connectDB(bool)));

    mapper->toFirst();
    updateEnable();
    NCRDFormGeometry *fg = new NCRDFormGeometry(this,splitter);
    connect(this,SIGNAL(finished(int)),fg,SLOT(save()));
    fg->restore();

    //! 多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}


NCRDConnectionDialog::~NCRDConnectionDialog()
{
}

/*$SPECIALIZATION$*/

void NCRDConnectionDialog::add()
{
    NCRDConnection con;
    int nextID = model->rowCount();
    con.name = tr("db%1").arg(nextID);
    con.driver = 0;
    con.connected = false;

    appendModel( con, nextID, true );

    updateEnable();
    listConns->setCurrentIndex( model->index(nextID,0) );
    comboDriver->setFocus();
    //->selectAll();
}

void NCRDConnectionDialog::remove()
{
    if ( model->rowCount() == 0 )
        return;

    int row = mapper->currentIndex();
    QString id = model->item( row, CONNDIA_COL_NAME )->text();

    bool isnew = model->item( row, CONNDIA_COL_ISNEW )->data(Qt::EditRole).toBool();

    if ( isnew ) {
        // nothing to do
    } else {
        removeConns.append( id );
    }
    model->removeRow( row );
    updateEnable();
}

void NCRDConnectionDialog::slotItemChanged(const QModelIndex & current, const QModelIndex & )
{
    mapper->setCurrentModelIndex( current );
    updateConnectText();
}

void NCRDConnectionDialog::setupModel()
{
    int cnt = mainwindow->sqlConnections().count();
    model = new QStandardItemModel( cnt, CONNDIA_COLS, this);

    int row=0;
    QHash<QString, NCRDConnection>::const_iterator i = mainwindow->sqlConnections().constBegin();
    while (i != mainwindow->sqlConnections().constEnd()) {
        appendModel( i.value(), row++, false );
        ++i;
    }
    model->sort(0);
}

void NCRDConnectionDialog::appendModel( const NCRDConnection& con, int row, bool flagNew)
{
    QStandardItem *item =0;
    // ID
    item = new QStandardItem( con.name );
    model->setItem( row, CONNDIA_COL_NAME, item);

    item = new QStandardItem();
    item->setData( con.driver, Qt::EditRole );
    model->setItem( row, CONNDIA_COL_DRIVER, item);

    item = new QStandardItem();
    item->setData( con.port, Qt::EditRole );
    model->setItem( row, CONNDIA_COL_PORT, item);

    item = new QStandardItem();
    item->setData( flagNew, Qt::EditRole );
    model->setItem( row, CONNDIA_COL_ISNEW, item);


    item = new QStandardItem( con.host );
    model->setItem( row, CONNDIA_COL_HOST, item);

    item = new QStandardItem( con.database );
    model->setItem( row, CONNDIA_COL_DATABASE, item);

    item = new QStandardItem( con.user );
    model->setItem( row, CONNDIA_COL_USER, item);

    item = new QStandardItem( con.password );
    model->setItem( row, CONNDIA_COL_PASSWORD, item);

    item = new QStandardItem();
    item->setData( con.connected, Qt::EditRole );
    model->setItem( row, CONNDIA_COL_CONNECTED, item);


    item = new QStandardItem( con.options );
    model->setItem( row, CONNDIA_COL_OPTIONS, item);

    //mainwindow->sqlConnections().insert( con.name, con );
}

void NCRDConnectionDialog::updateEnable()
{
    bool enable = (model->rowCount() > 0);
    gbConn->setEnabled( enable );
}

void NCRDConnectionDialog::applyChanges()
{
    for (int row = 0; row <  model->rowCount(); ++row) {

        NCRDConnection con;
        con.name =  model->item(row,CONNDIA_COL_NAME)->text();
        con.driver = model->item(row,CONNDIA_COL_DRIVER)->data(Qt::EditRole).toInt();
        con.host =  model->item(row,CONNDIA_COL_HOST)->text();
        con.database =  model->item(row,CONNDIA_COL_DATABASE)->text();
        con.user =  model->item(row,CONNDIA_COL_USER)->text();
        con.password =  model->item(row,CONNDIA_COL_PASSWORD)->text();
        con.port =  model->item(row,CONNDIA_COL_PORT)->data(Qt::EditRole).toInt();
        con.connected =  model->item(row,CONNDIA_COL_CONNECTED)->data(Qt::EditRole).toBool();
        con.options = model->item(row,CONNDIA_COL_OPTIONS)->text();

        if ( model->item(row,CONNDIA_COL_ISNEW)->data(Qt::EditRole).toBool() )	// new
            mainwindow->sqlConnections().insert( con.name, con );
        else // modify
            mainwindow->sqlConnections()[con.name] = con;
    }

    for (int i=0; i<removeConns.count(); ++i) {
        QString connID = removeConns.at(i);
        switch ( QMessageBox::warning( this, tr("Remove warning"),
                 tr("Are you sure to remove SQL connection %1?").arg( connID ),
            QMessageBox::Yes | QMessageBox::No ) ) {

                case QMessageBox::Yes: {
                    mainwindow->sqlConnections().remove( connID );
                    break;
                }
                case QMessageBox::No:
                default:
                    break;
            }
    }

    mainwindow->updateConnectionMenu();

}

void NCRDConnectionDialog::discardChanges()
{
}

QComboBox * NCRDConnectionDialog::comboBoxDriver()
{
    return comboDriver;
}

bool NCRDConnectionDialog::connectDB( bool on )
{
    bool ok=true;
    QString msg;
    QString txtOn = connectedText();
    QString txtOff = disconnectedText();
    QApplication::setOverrideCursor(QCursor(Qt::BusyCursor));

    NCRDConnection con;
    con.name = connName->text();
    con.driver = comboDriver->currentIndex();
    con.host = host->text();
    con.database = dbname->text();
    con.user = username->text();
    con.password = password->text();
    con.port = wPort->value();
    con.options = options->text();

    if (on) {
        // CONNECT
        ok = connectDatabase( con, msg );
        if ( ok ) {
            lblStatus->setText( txtOn );
        } else {
            btnConnect->setChecked( false );
            lblStatus->setText( txtOff );
        }
    } else {
        // DISCONNECT
        disconnectDatabase( con.name );
        lblStatus->setText( txtOff );
    }

    QApplication::restoreOverrideCursor();
    if ( !ok )
        QMessageBox::warning( this, tr("Connection error"), msg );

    return ok;
}

bool NCRDConnectionDialog::connectDatabase( const NCRDConnection& con, QString& msg )
{
    bool ok=true;
    //_drivers[con.driver]
    if ( QSqlDatabase::contains(con.name) )
        QSqlDatabase::removeDatabase( con.name );

    QSqlDatabase database = QSqlDatabase::addDatabase( _drivers[con.driver], con.name );
    if ( !database.isValid() ) {
        //fprintf( stderr, "Error: Could not load database driver. \n" );
        msg = tr("Could not load %1 database driver.").arg(_drivers[con.driver]);
        ok = false;
    }
    if (ok) {
        database.setHostName( con.host );
        database.setDatabaseName( con.database );
        database.setUserName( con.user );
        database.setPassword( con.password );
        if (con.port != 0 )
            database.setPort(con.port);

        if ( !con.options.isEmpty() )
            database.setConnectOptions(con.options);

        if ( database.open() ) {
            qDebug("Database connected (%s)", qPrintable(con.name) );
            //con.connected = true;
        } else {
            msg = database.lastError().databaseText();
            ok = false;
        }
    }

    return ok;
}

bool NCRDConnectionDialog::disconnectDatabase( const QString& cname )
{
    QSqlDatabase::removeDatabase( cname );
    return true;
}

void NCRDConnectionDialog::updateConnectText()
{
    if ( btnConnect->isChecked() )
        lblStatus->setText( connectedText() );
    else
        lblStatus->setText( disconnectedText() );
}

QString NCRDConnectionDialog::connectedText() const
{
    return "<font color=\"#00FF00\">" + tr("Connected") +"</font>";
}

QString NCRDConnectionDialog::disconnectedText() const
{
    return "<font color=\"#FF0000\">" + tr("Disconnected") + "</font>";
}


//------------------------------------------------------------------------------
// NCRDConnectionItemDelegate
//------------------------------------------------------------------------------
NCRDConnectionItemDelegate::NCRDConnectionItemDelegate(NCRDConnectionDialog * parent ) : dialog(parent)
{
}

QWidget * NCRDConnectionItemDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    if ( index.column() == CONNDIA_COL_DRIVER ) {
        return dialog->comboBoxDriver();
    } else {
        return QItemDelegate::createEditor( parent, option, index);
    }
}

void NCRDConnectionItemDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
    if ( index.column() == CONNDIA_COL_DRIVER ) {
        QComboBox *combo = qobject_cast<QComboBox*>(editor);
        if ( combo ) {
            combo->setCurrentIndex( index.data(Qt::EditRole).toInt() );
        }
    } else {
        QItemDelegate::setEditorData( editor, index);
    }
}

void NCRDConnectionItemDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
    if ( index.column() == CONNDIA_COL_DRIVER ) {
        QComboBox *combo = qobject_cast<QComboBox*>(editor);
        if ( combo ) {
            model->setData( index, combo->currentIndex() );
        }
    } else {
        QItemDelegate::setModelData( editor, model, index);
    }
}
