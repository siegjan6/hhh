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
#include "ncrdtestdatawindow.h"
#include "ncrdtestdatareader.h"
#include "ncrdtestdatawriter.h"
#include "ncreportdef.h"
#include "info.h"
#include "ncrd_global.h"

#include <QHeaderView>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QStandardItemModel>
#include <QDataWidgetMapper>
#include <QSettings>
#include <QCloseEvent>
#include <QMessageBox>


#define DEFAULTTESTDATAFILE	"defaulttestdata.xml"

NCRDTestDataWindow::NCRDTestDataWindow(QWidget* parent)
: QMainWindow( parent ), Ui::NCRDTestDataWindowUI()
{
    m_changed = false;
    m_internalChange = false;

    setupUi(this);
    tableData->setAlternatingRowColors( true );
    tableData->verticalHeader()->setDefaultSectionSize( 21 );

    //-------------- MODEL ---------------------------------------------------
    model = new QStandardItemModel( 0, TESTDATAMODEL_COLUMNCOUNT, this);

    connect( actNew, SIGNAL(triggered()), this, SLOT(newData()) );
    connect( actOpen, SIGNAL(triggered()), this, SLOT(openData()) );
    connect( actSave, SIGNAL(triggered()), this, SLOT(saveData()) );
    connect( actSaveAs, SIGNAL(triggered()), this, SLOT(saveAsData()) );
    connect( actClose, SIGNAL(triggered()), this, SLOT(close()) );

    connect( actAddData, SIGNAL(triggered()), this, SLOT(addData()) );
    connect( actRemoveData, SIGNAL(triggered()), this, SLOT(removeData()) );
    connect( actAddRow, SIGNAL(triggered()), this, SLOT(addRow()) );
    connect( actInsertRow, SIGNAL(triggered()), this, SLOT(insertRow()) );
    connect( actRemoveRow, SIGNAL(triggered()), this, SLOT(removeRow()) );
    connect( actAddColumn, SIGNAL(triggered()), this, SLOT(addCol()) );
    connect( actInsertColumn, SIGNAL(triggered()), this, SLOT(insertCol()) );
    connect( actRemoveColumn, SIGNAL(triggered()), this, SLOT(removeCol()) );
    connect( actLoadContent, SIGNAL(triggered()), this, SLOT(loadTableFromTextFile()) );

    connect( tableData, SIGNAL(cellChanged(int,int)), this, SLOT(slotDataChanged()) );

    connect( id, SIGNAL(textEdited(const QString &)), this, SLOT(slotDataChanged()) );
    connect( comboType, SIGNAL(activated(int)), this, SLOT(slotDataChanged()) );
    connect( comboSeparator, SIGNAL(activated(int)), this, SLOT(slotDataChanged()) );
    connect( cbEnabled, SIGNAL(clicked(bool)), this, SLOT(slotDataChanged()) );

    //connect( model, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(slotDataChanged()) );

    listID->setModel(model);

    NCRDTestDataItemDelegate *deleg = new NCRDTestDataItemDelegate( this );
    //deleg->setComboBoxes( comboFunc, comboScope );
    //MAPPER
    mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->setItemDelegate( deleg );

    mapper->addMapping(id, TESTDATAMODEL_COL_ID);
    mapper->addMapping(comboType, TESTDATAMODEL_COL_DSTYPE);
    mapper->addMapping(cbEnabled, TESTDATAMODEL_COL_ENABLED);

    connect( listID->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
             this, SLOT(slotItemChanged(const QModelIndex&,const QModelIndex&)));


    readSettings();
    load(DEFAULTTESTDATAFILE);
    slotDataChanged( false );
}

NCRDTestDataWindow::~NCRDTestDataWindow()
{
}

/*$SPECIALIZATION$*/

void NCRDTestDataWindow::newData()
{
    //saveData();
    model->clear();
    updateEnable();
}

void NCRDTestDataWindow::addData()
{
    NCRDTestData tdata;

    int nextID = model->rowCount();
    tdata.id = tr("testdata%1").arg(nextID);
    tdata.type = 0;
    tdata.enabled = true;

    NCRDTestDataReader::appendModel( model, tdata );
    updateEnable();
    listID->setCurrentIndex( model->index(nextID,0) );
    id->setFocus();
    id->selectAll();

    slotDataChanged(true);
}

void NCRDTestDataWindow::openData()
{
    QString fname = QFileDialog::getOpenFileName( this, tr("Open test data file"), "", tr("Test data files (*.xml)") );
    if ( fname.isEmpty() )
        return;

    load( fname );

}

void NCRDTestDataWindow::saveAsData()
{
    QString fname = QFileDialog::getSaveFileName( this, tr("Save As"), "", tr("Test data files (*.xml)") );
    if ( fname.isEmpty())
        return;

    if ( !fname.toUpper().endsWith(".XML") )
        fname += ".xml";

    if ( save( fname ) ) {
        currentFileName = fname;
        slotDataChanged( false );
    }
}

void NCRDTestDataWindow::saveData()
{
    save( currentFileName );
}


void NCRDTestDataWindow::addRow()
{
    addRow( false );
}

void NCRDTestDataWindow::insertRow()
{
    addRow( true );
}

void NCRDTestDataWindow::addRow( bool insert )
{
    //if ( tableData->columnCount() == 0 )
        //columnChange(spinCols->value());

    if ( tableData->rowCount() == 0 ) {
        tableData->insertRow( 0 );
        return;
    }

    if ( insert )
        tableData->insertRow( tableData->currentRow() );
    else
        tableData->setRowCount( tableData->rowCount()+1 );
}


void NCRDTestDataWindow::removeRow()
{
    if ( tableData->rowCount() == 0 )
        return;
    tableData->removeRow( tableData->currentRow() );
}

void NCRDTestDataWindow::addCol()
{
    tableData->setColumnCount( tableData->columnCount()+1 );
    updateLabels();
}

void NCRDTestDataWindow::insertCol()
{
    tableData->insertColumn( tableData->currentColumn() );
    updateLabels();
}

void NCRDTestDataWindow::removeCol()
{
    if ( tableData->columnCount() == 0 )
        return;
    tableData->removeColumn( tableData->currentColumn() );
    updateLabels();
}

void NCRDTestDataWindow::columnChange(int columnno)
{
    if ( columnno< tableData->columnCount() ) {

    } else {
        tableData->setColumnCount( columnno );
        updateLabels();
    }
}


void NCRDTestDataWindow::loadTableFromTextFile()
{
    QString s = QFileDialog::getOpenFileName( this, tr("Select text file"), "", "Text files (*.txt)" );
    if ( s.isEmpty() )
        return;

    QFile file(s);
    if ( file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            addTableRow( in.readLine() );
        }
        slotDataChanged(true);
    }
}

void NCRDTestDataWindow::updateLabels()
{
    QStringList labels;

    for (int i=0; i<tableData->columnCount(); ++i)
        labels << tr("col%1").arg(i);

    tableData->setHorizontalHeaderLabels( labels );
    //spinCols->setValue( tableWidget->columnCount() );
}

void NCRDTestDataWindow::refreshWidgets()
{
}


void NCRDTestDataWindow::slotItemChanged(const QModelIndex & current, const QModelIndex & previous)
{
    if ( current.isValid() )
        mapper->setCurrentModelIndex( current );
    //save
    if ( previous.isValid() )
        model->item( previous.row(), TESTDATAMODEL_COL_TEXTDATA )->setText( tableDataToString() );
    // load
    if ( current.isValid() )
        loadTable( model->item( current.row(), TESTDATAMODEL_COL_TEXTDATA )->text() );
    else
        loadTable( QString() );
}

bool NCRDTestDataWindow::save(const QString & filename)
{
    mapper->submit();
    model->item( mapper->currentIndex(), TESTDATAMODEL_COL_TEXTDATA )->setText( tableDataToString() );

    bool ok = true;
    NCRDTestDataWriter w;
    w.setModel( model );
    ok = w.write( filename );
    if ( ok )
        slotDataChanged(false);

    return ok;
}

bool NCRDTestDataWindow::load(const QString & filename)
{
    if ( !maybeSave() )
        return false;

    model->clear();

    bool ok = true;
    NCRDTestDataReader reader;
    reader.setModel( model );
    ok = reader.read( filename );

    if ( ok ) {
        model->sort(0);
        mapper->toFirst();
        //loadTable( model->item( mapper->currentIndex(), TESTDATAMODEL_COL_TEXTDATA )->text() );

        updateEnable();

        currentFileName = filename;
        slotDataChanged( false );
    } else {
        QMessageBox::warning( this, tr("Open error"), tr("An error occured while opening %1.").arg( filename ) + "\n"+
                              reader.errorString()+ "\n"+
                              tr("Line: %1 Column: %2").arg(reader.lineNumber()).arg(reader.columnNumber()) );
    }

    return ok;
}

void NCRDTestDataWindow::loadTable(const QString &data)
{
    tableData->clear();
    tableData->setRowCount(0);
    tableData->setColumnCount(0);

    QString d = data;
    QTextStream in(&d);
    while (!in.atEnd()) {
        addTableRow( in.readLine() );
    }
}

void NCRDTestDataWindow::addTableRow(const QString & line)
{
    //qDebug("addTableRow: %s",qPrintable(line) );

    QStringList data = line.split("\t");
    QStringList::const_iterator it;

    if ( data.count() == 0 )
        return;

    m_internalChange = true;

    if ( tableData->columnCount() != data.count() ) {
        //spinCols->setValue( data.count() );
        columnChange( data.count() );
    }

    addRow( false );
    int row = tableData->rowCount() - 1;
    int col=0;

    for (it = data.constBegin(); it != data.constEnd(); ++it) {
        QTableWidgetItem *item = new QTableWidgetItem( *it );
        tableData->setItem( row, col, item );
        col++;
    }

    m_internalChange = false;
}

void NCRDTestDataWindow::updateEnable()
{
    bool enable = (model->rowCount() > 0);
    id->setEnabled( enable );
    comboType->setEnabled( enable );
    cbEnabled->setEnabled( enable );
    tableData->setEnabled( enable );
}

QComboBox * NCRDTestDataWindow::comboBoxType()
{
    return comboType;
}

void NCRDTestDataWindow::writeSettings()
{
    QSettings settings;
    settings.setValue("testdatawindow_state", saveState() );
    settings.setValue("twpos", pos());
    settings.setValue("twsize", size());
}

void NCRDTestDataWindow::readSettings()
{
    QSettings settings;
    QPoint pos = settings.value("twpos", QPoint(0, 0)).toPoint();
    QSize size = settings.value("twsize", QSize(400, 400)).toSize();
    move(pos);
    resize(size);

    QByteArray state = settings.value("testdatawindow_state", QByteArray() ).toByteArray();
    if ( !state.isEmpty() )
        restoreState( state );
}

void NCRDTestDataWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

bool NCRDTestDataWindow::maybeSave()
{
    if (m_changed) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Save warning"),
                                        tr("'%1' has been modified.\n"
                                           "Do you want to save your changes?").arg(currentFileName),
                                           QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );
        if (ret == QMessageBox::Save)
            return save(currentFileName);
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

QString NCRDTestDataWindow::tableDataToString() const
{
    QString d;
    QTextStream out(&d);
    for (int i=0; i<tableData->rowCount(); ++i) {
        for (int j=0; j<tableData->columnCount(); ++j) {
            if ( tableData->item(i,j) ) {
                out << tableData->item(i,j)->text();
                if ( j<tableData->columnCount()-1 )
                    out << "\t";
            }
        }
        out << "\n";
    }
    return d;
}

bool NCRDTestDataWindow::generateTestData( const QString& fname, NCReportDef* reportDef )
{
    //-----------------------------
    //GEN TEST DATA
    //-----------------------------
    QStandardItemModel *model = 0;
    QStandardItem *item =0;
    QString param;
    QStandardItemModel *model_temp = new QStandardItemModel( 0, 0 );

    NCRDTestDataReader reader;
    reader.setModel( model_temp );
    bool ok = reader.read( fname );
    if ( ok ) {
        for (int row = 0; row <  model_temp->rowCount(); ++row) {
            QStringList list;

            NCRDTestData td;
            td.id = model_temp->item(row, TESTDATAMODEL_COL_ID )->text();
            td.type = model_temp->item(row, TESTDATAMODEL_COL_DSTYPE )->data(Qt::EditRole).toInt();
            td.enabled = (model_temp->item(row, TESTDATAMODEL_COL_ENABLED )->text()=="true");
            td.data = model_temp->item(row, TESTDATAMODEL_COL_TEXTDATA )->text();

            if ( !td.enabled )
                continue;

            QTextStream in(&td.data);
            int lineRow =0;
            while (!in.atEnd()) {
                QString line = in.readLine();

                if (line.isEmpty())
                    continue;

                if ( td.type == 1 ) {	//model
                    QStringList fields = line.split("\t");

                    if ( !model )
                        model = new QStandardItemModel( 0, fields.count() );

                    QStringList::const_iterator it;
                    if ( fields.count() > 0 ) {
                        int col=0;
                        for (it = fields.constBegin(); it != fields.constEnd(); ++it) {
                            item = new QStandardItem();
                            //qDebug("data: %s",qPrintable(*it) );
                            item->setData( *it, Qt::EditRole );
                            model->setItem( lineRow, col, item);
                            col++;
                        }
                    }
                }

                if ( td.type == 2 ) // parameter
                    param += line +"\n";
                if ( td.type == 0 )	// stringlist
                    list << line;

                lineRow++;
            }
            // ADD TO REPORT
            if (td.type==1)
                reportDef->addItemModel( td.id, model );
            if (td.type==2)
                reportDef->addParameter( td.id, param );
            if (td.type==0)
                reportDef->addStringList( td.id, list );
        }
    }

    delete model_temp;
    return ok;
}

const QString NCRDTestDataWindow::defaultTestDataFile()
{
    return DEFAULTTESTDATAFILE;
}

void NCRDTestDataWindow::removeData()
{
    if ( model->rowCount() == 0 )
        return;

    int row = mapper->currentIndex();
    QString id = model->item( row, TESTDATAMODEL_COL_ID )->text();


    switch ( QMessageBox::warning( this, tr("Remove warning"),
             tr("Are you sure to remove test data %1?").arg( id ),
            QMessageBox::Yes | QMessageBox::No ) ) {

        case QMessageBox::Yes:
        {
            model->removeRow( row );
            updateEnable();
            slotDataChanged(true);
            break;
        }
        case QMessageBox::No:
        default:
            break;
    }

}

void NCRDTestDataWindow::updateWindowTitle()
{
    setWindowTitle( tr("Test data editor")+QString(" [%1%2]").arg( currentFileName ).arg( m_changed ? "*" : "") );
}

void NCRDTestDataWindow::slotDataChanged( bool changed )
{
    if (m_internalChange)
        return;

    m_changed = changed;
    updateWindowTitle();
}




//------------------------------------------------------------------------------
// NCRDTestDataItemDelegate
//------------------------------------------------------------------------------
NCRDTestDataItemDelegate::NCRDTestDataItemDelegate(NCRDTestDataWindow * parent ) : window(parent)
{
}

QWidget * NCRDTestDataItemDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    if ( index.column() == TESTDATAMODEL_COL_DSTYPE ) {
        return window->comboBoxType();
    } else {
        return QItemDelegate::createEditor( parent, option, index);
    }
}

void NCRDTestDataItemDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
    if ( index.column() == TESTDATAMODEL_COL_DSTYPE ) {
        QComboBox *combo = qobject_cast<QComboBox*>(editor);
        if ( combo ) {
            combo->setCurrentIndex( index.data(Qt::EditRole).toInt() );
        }
    } else {
        QItemDelegate::setEditorData( editor, index);
    }
}

void NCRDTestDataItemDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
    if ( index.column() == TESTDATAMODEL_COL_DSTYPE ) {
        QComboBox *combo = qobject_cast<QComboBox*>(editor);
        if ( combo ) {
            model->setData( index, combo->currentIndex() );
        }
    } else {
        QItemDelegate::setModelData( editor, model, index);
    }
}
