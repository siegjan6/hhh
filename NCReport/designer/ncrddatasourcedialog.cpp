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
#include "ncrddatasourcedialog.h"
#include "ncrddatasourcetypedialog.h"
#include "ncrddocument.h"
#include "ncrddef.h"
#include "ncrdcustomdatasource.h"
#include "ncreportsqldatasource.h"
#include "ncreporttextdatasource.h"
#include "ncreportxmldatasource.h"
#include "ncreportstringlistdatasource.h"
#include "ncreportitemmodeldatasource.h"
#include "ncreportdataviewdatasource.h"//me:扩展
#include "ncrdformgeometry.h"
#include "ncreportevaluator.h"
#include "ncrdmainwindow.h"
#include "ncreportdatafieldinfo.h"
#include "ncrdsqleditorhighlighter.h"
#include "ncrdsqltextedit.h"

#include <QStandardItemModel>
#include <QDataWidgetMapper>
#include <QHash>
#include <QMessageBox>
#include <QSqlError>
#include <QCompleter>
#include <QSqlQueryModel>
#include <QClipboard>
#include "../../include/MDDBQueryConfigure/MDDBQueryConfigure.h"
#include <QDebug>

//#define TEST_DS

#ifdef TEST_DS
#include <QFile>
#include <QTextStream>

//<test>
static void WriteLog(const QString & msg)
{
    QFile file("C://report.log");
    file.open(QIODevice::ReadWrite|QIODevice::Append);
    QTextStream stream(&file);
    stream<<msg<<"\r\n";
    file.close();
}
#endif

//</test>

NCRDDataSourceDialog::NCRDDataSourceDialog(NCRDDocument *doc, QWidget* parent)
: QDialog( parent ), Ui::NCRDDataSourceDialogUI(), m_document(doc)
{
    setupUi(this);
    //gbStringList->setVisible( false );
    //gbModel->setVisible( false );
    //gbCustom->setVisible( false );
    comboDsType->setDisabled(true); //禁止Data source type下拉框.因为在新建数据源时,已经指定了数据源类型.

    m_mapper = new QDataWidgetMapper(this);
    setupModel();
    listDS->setModel(m_model);//?显示model的第一列
    // completer
    //<提示数据源ID>
    QCompleter *completer = new QCompleter(this);
    completer->setModel(m_model);
    completer->setCompletionColumn( ColID );
    wParentDataSource->setCompleter(completer); //parent datasource
    //</提示数据源ID>

    //MAPPER
    NCRDDataSourceItemDelegate *deleg = new NCRDDataSourceItemDelegate( this );

    m_mapper->setModel(m_model);
    m_mapper->setItemDelegate( deleg );
    m_mapper->addMapping(dsID, ColID);
    m_mapper->addMapping(comboDsType, ColDSTYPE);
    m_mapper->addMapping(comboLocation, ColLOCTYPE);
    m_mapper->addMapping(filenameurl, ColFILENAME);
    //mapper->addMapping(cbFilter, ColHAVEFILTER);
    m_mapper->addMapping(cbEmpty,ColAllowEmpty);
    m_mapper->addMapping(comboRole, ColOPENROLE);

    m_mapper->addMapping(checkBox,ColEnableSampling);
    m_mapper->addMapping(samplingField,ColSamplingField);
    m_mapper->addMapping(samplingPeriod,ColSamplingPeriodPara);
    m_mapper->addMapping(samplingNullGap,ColSamplingNullGap);

    m_mapper->addMapping(cbPrivateConn, ColISEXTERNAL);
    m_mapper->addMapping(cHost, ColHOST); //SQL 连接中的主机名
    m_mapper->addMapping(cDatabase, ColDB);
    m_mapper->addMapping(cUsername, ColUSER);
    m_mapper->addMapping(cPassword, ColPASSW);
    m_mapper->addMapping(cPort, ColPORT);
    m_mapper->addMapping(cDriver, ColDRIVER);
    m_mapper->addMapping(connID, ColCONNID);
    m_mapper->addMapping(wParentDataSource, ColPARENTID);

    m_mapper->addMapping(stringListID, ColLISTID);
    m_mapper->addMapping(modelID, ColMODELID);
    m_mapper->addMapping(customID, ColCUSTOMID);
    m_mapper->addMapping(customDescr, ColCUSTOMDESCR);

    m_mapper->addMapping(comboTxtColumnSep, ColTXTCOLDELIM);
    m_mapper->addMapping(delimCharTxt, ColTXTCDCHAR);
    m_mapper->addMapping(comboSlColumnSep, ColSLCOLDELIM);
    m_mapper->addMapping(delimCharSl, ColSLCDCHAR);
    m_mapper->addMapping(cbColumnHeader, ColTXTCOLHEADER);
    m_mapper->addMapping(cbFwOnly, ColFWDONLY);
    m_mapper->addMapping(connectOpt, ColCONNECTOPTIONS);
    m_mapper->addMapping(spPKColIndex, ColPKCOLINDEX);
    m_mapper->addMapping(wItemModelSpec, ColMODELRELATIONSPEC);
    m_mapper->addMapping(wEncoding, ColTXTENCODING);
    m_mapper->addMapping(cbUseModelAppearance, ColUseModelAppearance);
    m_mapper->addMapping(cbForceUpdateStepBackRel, ColForceUpdateOnStepBackRel);

    m_mapper->addMapping(queryEditor, ColQRY);
    m_mapper->addMapping(wStaticText, ColSPECTEXT);
    m_mapper->addMapping(staticXML, ColSPECXML);

    m_mapper->addMapping(dataViewNameComboBox, ColDataViewName);//me:扩展
    // SQL query model for testing SQL commands
    m_sqlTestModel = new QSqlQueryModel;
    sqlTestView->setModel(m_sqlTestModel); //在Query test中
    sqlTestView->setAlternatingRowColors( true );
    sqlTestView->horizontalHeader()->setHighlightSections( false );
    sqlTestView->verticalHeader()->setDefaultSectionSize( 20 );

    m_sqlTestDataSource = new NCReportSQLDataSource(this);
    tabWidget->setTabEnabled(2,false); //将Query test页面禁止掉
    //sqlTestError = new NCReportError();

    connect( listDS->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
             this, SLOT(slotItemChanged(const QModelIndex&,const QModelIndex&)));

    connect(btnAdd, SIGNAL(clicked()), this, SLOT(add())); //绿色添加数据源按钮
    connect(btnRemove, SIGNAL(clicked()), this, SLOT(remove()));//红色删除数据源按钮
    connect(btnTestConn, SIGNAL(clicked()), this, SLOT(testExternalConnection())); //Test Query 按钮
    connect(comboLocation, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEnable()) );//Location type下拉框
    connect(comboRole, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEnable()) );//opening/running role
    connect(comboTxtColumnSep, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEnable()) );
    connect(comboSlColumnSep, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEnable()) );
    connect(cbPrivateConn, SIGNAL(clicked(bool)), this, SLOT(updateEnable()) );
    connect(btnTestQuery, SIGNAL(clicked(bool)), this, SLOT(runSqlTest()) );
    connect(dataViewNameComboBox, SIGNAL(activated(int)), this, SLOT(activatedInDataViewNameComboBox(int)));//me:扩展
    QFont courierFont = QFont("Courier New",10);


    new NCRDSqlEditorHighlighter(queryEditor->document());
    queryEditor->setFont( courierFont );
    queryEditor->installEventFilter(new NCRDTextEditKeyPressEater(queryEditor, "    "));

    new NCRDSqlEditorHighlighter(staticXML->document());
    staticXML->setFont(courierFont);
    staticXML->installEventFilter(new NCRDTextEditKeyPressEater(staticXML,"    "));

    wStaticText->setFont( courierFont );
    wStaticText->installEventFilter(new NCRDTextEditKeyPressEater(wStaticText, "\t"));

//    connect(queryEditor, SIGNAL(outFocus()), this, SLOT(textEditorContentChanged()) );
//    connect(staticText, SIGNAL(textChanged()), this, SLOT(textEditorContentChanged()) );
//    connect(staticXML, SIGNAL(outFocus()), this, SLOT(textEditorContentChanged()) );
//    connect(wItemModelSpec, SIGNAL(outFocus()), this, SLOT(textEditorContentChanged()) );

    //connect(listSpec, SIGNAL(outFocus()), this, SLOT(textEditorContentChanged()) );
    //connect(modelSpec, SIGNAL(outFocus()), this, SLOT(textEditorContentChanged()) );
    //connect(customSpec, SIGNAL(outFocus()), this, SLOT(textEditorContentChanged()) );


    NCRDFormGeometry *fg = new NCRDFormGeometry(this,splitter); //记录窗体的位置 下次打开时 在上次打开的位置处打开窗体
    connect(this,SIGNAL(finished(int)),fg,SLOT(save()));
    fg->restore();


    listDS->setCurrentIndex( m_model->index(0,0) ); //设置左侧数据源列表

    //mapper->toFirst();
    updateEnable();
}

NCRDDataSourceDialog::~NCRDDataSourceDialog()
{
}

/*$SPECIALIZATION$*/

//初始化m_model
//从document中获得定义的报表reportDef.从reportDef中获得所有数据源
//将所有数据源添加到Model中.
void NCRDDataSourceDialog::setupModel()
{
    int cnt = m_document->reportDef()->dataSources().count();
    m_model = new QStandardItemModel( cnt, ColCount, this);

    m_columnViewModel = new QStandardItemModel( this);
    columnView->setModel( m_columnViewModel);

    //me:扩展
    m_dataViewFieldModel = new QStandardItemModel(this);
    dataViewFieldTableView->setModel(m_dataViewFieldModel);

    int row=0;
    QHash<QString, NCReportDataSource*>::const_iterator i = m_document->reportDef()->dataSources().constBegin();
    while (i != m_document->reportDef()->dataSources().constEnd()) {
        NCReportDataSource* ds = i.value();
        appendModel( ds, row++, false );
        ++i;
    }
    m_model->sort(ColID);//Model按ID排序
    updateParentDataSourceWidget();
}

//将一个数据源添加到Model中
void NCRDDataSourceDialog::appendModel(NCReportDataSource *ds, int row, bool flagNew)
{
    QStandardItem *item;
    // ID
    item = new QStandardItem( ds->id() );
    m_model->setItem( row, ColID, item);
    // DS.type
    item = new QStandardItem( QString::number((int)(ds->dataSourceType())) );
    //virtual void setData(const QVariant &value, int role = Qt::UserRole + 1);
    item->setData( (int)(ds->dataSourceType()) );
    m_model->setItem( row, ColDSTYPE, item);
    // DS.open role
    item = new QStandardItem( QString::number((int)(ds->openRole())) );
    item->setData( (int)(ds->openRole()) );
    m_model->setItem( row, ColOPENROLE, item);
    // DS.location
    item = new QStandardItem( QString::number((int)(ds->locationType())) );
    item->setData( (int)(ds->locationType()) );
    m_model->setItem( row, ColLOCTYPE, item);
    // Filename
    item = new QStandardItem( ds->rawFileName() );
    m_model->setItem( row, ColFILENAME, item);
    // Have Filter
    item = new QStandardItem( ds->recordFilter().isEmpty() ? "false" : "true" );
    m_model->setItem( row, ColHAVEFILTER, item);
    // Filter
    item = new QStandardItem( ds->recordFilter() );
    m_model->setItem( row, ColFILTER, item);
    //
    // Empty allow?
    item = new QStandardItem( ds->allowEmpty() ? "true" : "false" );
    m_model->setItem( row, ColAllowEmpty, item);

    // enable sampling
    item = new QStandardItem( ds->EnableSampling() ? "true" : "false" );
    m_model->setItem( row, ColEnableSampling, item);

    // samplingField
    item = new QStandardItem( ds->samplingField() );
    m_model->setItem( row, ColSamplingField, item);

    // samplingPeriodPara
    item = new QStandardItem( ds->samplingPeriodPara() );
    m_model->setItem( row, ColSamplingPeriodPara, item);

    // samplingNullGap
    item = new QStandardItem( ds->samplingNullGap() );
    m_model->setItem( row, ColSamplingNullGap, item);

    item = new QStandardItem( ds->parentId() );
    m_model->setItem( row, ColPARENTID, item);

    item = new QStandardItem();
    item->setData( ds->primaryKeyColumn(), Qt::EditRole );
    m_model->setItem( row, ColPKCOLINDEX, item);

    switch (ds->dataSourceType()) {
    //--------------- SQL ------------------------
    case NCReportDataSource::Sql:
    {
        NCReportSQLDataSource *sqlDS = qobject_cast<NCReportSQLDataSource*>(ds);
        if ( sqlDS ) {
            item = new QStandardItem( sqlDS->connectionID() );
            m_model->setItem( row, ColCONNID, item);
            item = new QStandardItem( ds->connectionType() == NCReportDataSource::Internal ? "false" : "true" );
            m_model->setItem( row, ColISEXTERNAL, item);
            item = new QStandardItem( sqlDS->dbHost() );
            m_model->setItem( row, ColHOST, item);
            item = new QStandardItem( sqlDS->dbDatabase() );
            m_model->setItem( row, ColDB, item);
            item = new QStandardItem( sqlDS->dbUser() );
            m_model->setItem( row, ColUSER, item);
            item = new QStandardItem( sqlDS->dbPassword() );
            m_model->setItem( row, ColPASSW, item);
            item = new QStandardItem( sqlDS->dbPort() );
            m_model->setItem( row, ColPORT, item);
            item = new QStandardItem( sqlDS->isForwardOnly() ? "true" : "false" );
            m_model->setItem( row, ColFWDONLY, item);
            item = new QStandardItem( sqlDS->connectOptions() );
            m_model->setItem( row, ColCONNECTOPTIONS, item);
            item = new QStandardItem( sqlDS->forceUpdateOnStepBackwordInRelation() ? "true" : "false" );
            m_model->setItem( row, ColForceUpdateOnStepBackRel, item);

            // DRIVER COMBO
            item = new QStandardItem();
            item->setData( cDriver->findText(sqlDS->dbDriver()), Qt::EditRole );
            //item->setData( sqlDS->dbDriver(), Qt::DisplayRole );
            //qDebug( "Find %s driver combo value: %i",qPrintable(sqlDS->dbDriver()), cDriver->findText(sqlDS->dbDriver()));
            m_model->setItem( row, ColDRIVER, item);

            item = new QStandardItem( sqlDS->specText() );
            m_model->setItem( row, ColQRY, item);
        }
        break;
    }
    case NCReportDataSource::Text:
    {
        NCReportTextDataSource *txtDS = qobject_cast<NCReportTextDataSource*>(ds);
        if (txtDS) {
            item = new QStandardItem( ds->specText() );	// static text
            m_model->setItem( row, ColSPECTEXT, item);

            item = new QStandardItem( QString::number((int)(txtDS->columnDelimiter())) );
            item->setData( (int)(txtDS->columnDelimiter()), Qt::EditRole );
            m_model->setItem( row, ColTXTCOLDELIM, item);

            //delimiter
            char dc = txtDS->delimiterCharacter();
            item = new QStandardItem();
            if ( txtDS->columnDelimiter() == NCReportTextDataSource::Custom )
                item->setText(QString(dc));
            else
                item->setText("");
            m_model->setItem( row, ColTXTCDCHAR, item);

            item = new QStandardItem( txtDS->encoding() );
            m_model->setItem( row, ColTXTENCODING, item);

            item = new QStandardItem( txtDS->hasColumnHeader() ? "true" : "false" );
            m_model->setItem( row, ColTXTCOLHEADER, item);
        }

        break;
    }
    case NCReportDataSource::Xml:
    {
        NCReportXMLDataSource *xmlDS = qobject_cast<NCReportXMLDataSource*>(ds);
        if (xmlDS) {
            item = new QStandardItem( ds->specText() );	// static text
            m_model->setItem( row, ColSPECXML, item);
        }

        break;
    }
    case NCReportDataSource::StringList:
    {
        NCReportStringListDataSource *slDS = qobject_cast<NCReportStringListDataSource*>(ds);
        if (slDS) {
            item = new QStandardItem( slDS->listID() );	// static text
            m_model->setItem( row, ColLISTID, item);

            item = new QStandardItem();
            item->setData( (int)(slDS->columnDelimiter()), Qt::EditRole );
            m_model->setItem( row, ColSLCOLDELIM, item);

            char dc = slDS->delimiterCharacter();
            item = new QStandardItem();
            if ( slDS->columnDelimiter() == NCReportStringListDataSource::Custom )
                item->setText(QString(dc));
            else
                item->setText("");
            m_model->setItem( row, ColSLCDCHAR, item);
        }
        break;
    }
    case NCReportDataSource::ItemModel:
    {
        NCReportItemModelDataSource *modelDS = qobject_cast<NCReportItemModelDataSource*>(ds);
        if (modelDS) {
            item = new QStandardItem( modelDS->modelID() );	// static text
            m_model->setItem( row, ColMODELID, item);

            item = new QStandardItem( ds->specText() );
            m_model->setItem( row, ColMODELRELATIONSPEC, item);

            item = new QStandardItem( ds->useAppearanceRoles() ? "true" : "false" );
            m_model->setItem( row, ColUseModelAppearance, item);
        }
        break;
    }
    case NCReportDataSource::Custom:
    {
        item = new QStandardItem( ds->specText() );	// static text
        m_model->setItem( row, ColCUSTOMID, item);
        item = new QStandardItem( ds->briefDescription());
        m_model->setItem( row, ColCUSTOMDESCR, item);
        updateColumnView();
        break;
    }
    case NCReportDataSource::DataView:
    {
        NCReportDataViewDataSource * viewDS = qobject_cast<NCReportDataViewDataSource * >(ds);
        if (viewDS) {
            updateDataViewNames();//先初化视图下拉框 不然index为-1
            int index = dataViewNameComboBox->findText(viewDS->getViewName());//视图名在下拉框中的索引
            //item = new QStandardItem( viewDS->getViewName() );
            qDebug()<<"appendModel index = "<<index;
            item = new QStandardItem(QString::number(index));//构造参数须是字符串 代表ComoBox下拉选项的索引
            item->setData(index);
            m_model->setItem( row, ColDataViewName, item);
        }

        break;
    }
    } //END CASE

    item = new NCRDDataSourceItem( ds, flagNew );
    m_model->setItem( row, ColPOINTER, item);
}

//新添加一个数据源 就在 m_model中添加一行
void NCRDDataSourceDialog::add()
{
    updateParentDataSourceWidget();

    NCReportDataSource *ds = 0;
    NCRDDataSourceTypeDialog dialog(this); //调用选择数据源类型对话框
    if ( dialog.exec() == QDialog::Rejected )
        return;

    switch ( dialog.selectedType() ) {
        case NCReportDataSource::Sql:
            ds = new NCReportSQLDataSource( m_document->reportDef() );
            break;
        case NCReportDataSource::Text:
            ds = new NCReportTextDataSource( m_document->reportDef() );
            break;
        case NCReportDataSource::Xml:
            ds = new NCReportXMLDataSource( m_document->reportDef() );
            break;
        case NCReportDataSource::StringList:
            ds = new NCReportStringListDataSource( m_document->reportDef() );
            break;
        case NCReportDataSource::ItemModel:
            ds = new NCReportItemModelDataSource( m_document->reportDef() );
            break;
        case NCReportDataSource::Custom:
            ds = new NCRDCustomDataSource( m_document->reportDef() );
            break;
        case NCReportDataSource::DataView:
            ds = new NCReportDataViewDataSource(m_document->reportDef());//me:扩展
            ((NCReportDataViewDataSource *)ds)->setViewPath(m_document->getViewPath());
            break;
    }

    if (!ds)
        return;

    ds->setDataSourceType( dialog.selectedType() );
    //stackW->setCurrentIndex( (int)ds->dataSourceType() );

    int nextID = m_model->rowCount();
    ds->setID( tr("DataSource%1").arg(nextID+1) );

    appendModel( ds, nextID, true );
    listDS->setCurrentIndex( m_model->index(nextID,0) );

    updateEnable();
    dsID->setFocus();
    dsID->selectAll();
}

void NCRDDataSourceDialog::remove()
{
    if ( m_model->rowCount() == 0 )
        return;

    int row = m_mapper->currentIndex();
    //在appendModel函数中,将(row,ColPOINTER)处的item设置为NCRDDataSourceItem.
    NCRDDataSourceItem *dsItem = (NCRDDataSourceItem*)(m_model->item( row, ColPOINTER ));
    NCReportDataSource *ds = dsItem->dataSource();

    if ( dsItem->flagNew() ) {
        if (m_model->rowCount()==1)
            m_model->clear();
        else
            m_model->removeRow( row );

        if (ds) {
            qDebug("Remove datasource %p",ds);
            delete ds; //因为是新建的 还没有保存到m_document0>reportDeft 所以不用从reportDef中删除此ds
        }
    } else {

        switch ( QMessageBox::warning( this, tr("Remove warning"), tr("Are you sure to remove datasource %1?").arg( ds->id() ),
                QMessageBox::Yes | QMessageBox::No ) ) {

                case QMessageBox::Yes: {
                    qDebug("Remove datasource %p",ds);
                    m_document->reportDef()->removeDataSource( ds );

                    if ( m_model->rowCount() == 1 )
                        m_model->clear();
                    else
                        m_model->removeRow( row );

                    break;
                }
                case QMessageBox::No:
                default:
                    return;
            }

    }

    updateEnable();
}

void NCRDDataSourceDialog::slotItemChanged(const QModelIndex & current, const QModelIndex & )
{
    m_mapper->setCurrentModelIndex( current ); //切换数据源
    int row = current.row();
    int index = m_model->item( row, ColDSTYPE )->data().toInt();
    stackW->setCurrentIndex( index);//显示数据源类型对应的stack页
        // stackW->setCurrentIndex( model->item( current.row(),
        // ColDSTYPE )->data().toInt() );

    NCRDDataSourceItem *dsItem = (NCRDDataSourceItem*)(m_model->item( current.row(), ColPOINTER ));
    NCReportDataSource *ds = dsItem->dataSource();

    switch ( ds->dataSourceType() ) {
        case NCReportDataSource::Sql:
            //queryEditor->setAdjustedText( m_model->item( current.row(), ColQRY )->text() ); break;
        case NCReportDataSource::Text:
            /*staticText->setPlainText( m_model->item( current.row(), ColSPECTEXT )->text() );*/ break;
        case NCReportDataSource::Xml:
            /*staticXML->setPlainText( m_model->item( current.row(), ColSPECXML )->text() );*/ break;
        case NCReportDataSource::Custom:
            updateColumnView(); break;
         case NCReportDataSource::DataView:
            updateDataViewNames(); updateFieldsInDataView(); break;
        default:
            break;
    }

    updateEnable();
}

void NCRDDataSourceDialog::applyChanges()
{
    //移除所有数据源 但不释放数据源指针(NCReportDataSource*).
    //m_model中已经保存了所有数据源的信息.此处将reportDef 中的数据源清空,是为了
    //从m_model中更新reportDef中的数据源信息
    m_document->reportDef()->removeDataSourcesNoDelete();

    //界面操作修改了m_model.将m_model中的数据源信息保存到对应的数据源对象(NCReportDataSource*)中
    //m_model中的一行对应一个数据源对象
    for (int row = 0; row <  m_model->rowCount(); ++row) {
        NCRDDataSourceItem *dsItem = (NCRDDataSourceItem*)(m_model->item( row, ColPOINTER ));
        NCReportDataSource *ds = dsItem->dataSource();

        if ( !m_model->item(row,ColID)->text().isEmpty() && m_model->item(row,ColID)->text() == m_model->item(row,ColPARENTID)->text()	) {
            QMessageBox::warning( this, tr("Error"), tr("Invalid parent ID: %1").arg(m_model->item(row,ColID)->text() ) );
            return;
        }
        // ID
        ds->setID( m_model->item(row,ColID)->text() );
        //ds->setDataSourceType( model->item(row,ColDSTYPE)->data().toInt() );
        qDebug("Apply: DSTYPE: %i", m_model->item(row,ColDSTYPE)->data().toInt() );

        ds->setLocationType( (NCReportDataSource::LocationType)(m_model->item(row,ColLOCTYPE)->data(Qt::EditRole).toInt()) );
        qDebug("Apply: LOCTYPE: %i", (NCReportDataSource::LocationType)(m_model->item(row,ColLOCTYPE)->data(Qt::EditRole).toInt()) );
        ds->setOpenRole( (NCReportDataSource::OpenRoles)(m_model->item(row,ColOPENROLE)->data(Qt::EditRole).toInt()) );

        ds->setFileName( m_model->item(row,ColFILENAME)->text() );
        ds->setRecordFilter( m_model->item(row,ColFILTER)->text() );
        ds->setAllowEmpty( m_model->item(row,ColAllowEmpty)->text() == "true" );
        ds->setParentId( m_model->item(row,ColPARENTID)->text() );
        ds->setPrimaryKeyColumn( m_model->item(row,ColPKCOLINDEX)->data(Qt::EditRole).toInt() );

        ds->setEnableSampling( m_model->item(row,ColEnableSampling)->text() == "true" );
        ds->setSamplingField( m_model->item(row,ColSamplingField)->text() );
        ds->setSamplingPeriodPara( m_model->item(row,ColSamplingPeriodPara)->text() );
        ds->setSamplingNullGap( m_model->item(row,ColSamplingNullGap)->text() );

        switch (ds->dataSourceType()) {
        case NCReportDataSource::Sql:
        {
            NCReportSQLDataSource *sqlds = qobject_cast<NCReportSQLDataSource*>(ds);
            if ( sqlds ) {
                setSqlDataSourceData(sqlds,row);
            }
            break;
        }
        case NCReportDataSource::Text:
        {
            NCReportTextDataSource *txtds = qobject_cast<NCReportTextDataSource*>(ds);

            txtds->setSpecText( m_model->item(row,ColSPECTEXT)->text() );
            txtds->setColumnDelimiter( (NCReportTextDataSource::Delimiter)(m_model->item(row,ColTXTCOLDELIM)->data(Qt::EditRole).toInt()) );
            if ( txtds->columnDelimiter() == NCReportTextDataSource::Custom )
                txtds->setCustomDelimiterCharacter( m_model->item(row,ColTXTCDCHAR)->text()[0].toLatin1() );
            txtds->setHasColumnHeader( m_model->item(row,ColTXTCOLHEADER)->text() == "true" );
            txtds->setEncoding( m_model->item(row, ColTXTENCODING)->text() );
            break;
        }
        case NCReportDataSource::Xml:
        {
            ds->setSpecText( m_model->item(row,ColSPECXML)->text() );
            break;
        }
        case NCReportDataSource::StringList:
        {
            NCReportStringListDataSource *slds = qobject_cast<NCReportStringListDataSource*>(ds);

            slds->setListID( m_model->item(row,ColLISTID)->text() );
            slds->setColumnDelimiter( (NCReportStringListDataSource::Delimiter)(m_model->item(row,ColSLCOLDELIM)->data(Qt::EditRole).toInt()) );
            if ( slds->columnDelimiter() == NCReportStringListDataSource::Custom ) {
                QString dchar = m_model->item(row,ColSLCDCHAR)->text();
                char dc = dchar[0].toLatin1();
                slds->setCustomDelimiterCharacter( dc );
            }
            if (!m_document->reportDef()->stringLists().isEmpty())
                slds->setStringList( m_document->reportDef()->stringLists().value(slds->listID()) );
            break;
        }
        case NCReportDataSource::ItemModel:
        {
            NCReportItemModelDataSource *modelDS = qobject_cast<NCReportItemModelDataSource*>(ds);
            modelDS->setModelID( m_model->item(row,ColMODELID)->text() );
            modelDS->setSpecText( m_model->item(row,ColMODELRELATIONSPEC)->text() );
            modelDS->setUseAppearanceRoles(m_model->item(row,ColUseModelAppearance)->text() == "true" );

            modelDS->setItemModel( m_document->reportDef()->itemModels().value(modelDS->modelID()));
            break;
        }
        case NCReportDataSource::Custom:
        {
            ds->setSpecText( m_model->item(row,ColCUSTOMID)->text() );
            ds->setBriefDescription( m_model->item(row,ColCUSTOMDESCR)->text() );
            break;
        }
        case NCReportDataSource::DataView:
        {
            NCReportDataViewDataSource * viewDs = qobject_cast<NCReportDataViewDataSource*>(ds);
            //TODO:assert(viewDs)
            int index = m_model->item(row, ColDataViewName)->data(Qt::EditRole).toInt();
            qDebug()<<"applyChanges:index = "<<index;
            if(index>-1)
            {
                QString viewName = dataViewNameComboBox->itemText(index);
                qDebug()<<"applyChanges:viewName = "<<viewName;
                viewDs->setViewName(viewName);
            }
            break;
        }
        }

        //之前移出了数据源.现在更新好了,在添加回去.
        m_document->reportDef()->addDataSource( ds );
    }

    //执行删除数据源操作
    for (int i=0; i<m_removedDatasources.count(); ++i) {
        NCReportDataSource *ds = m_removedDatasources.at(i);
        switch ( QMessageBox::warning( this, tr("Remove warning"), tr("Are you sure to remove datasource %1?").arg( ds->id() ),
            QMessageBox::Yes | QMessageBox::No ) ) {

            case QMessageBox::Yes: {
                qDebug("Remove datasource %p",ds);
                m_document->reportDef()->removeDataSource( ds );
                break;
            }
            case QMessageBox::No:
            default:
                break;
        }
    }
    m_document->setDocumentModified();
    //更新主窗体中右下角的树形控件
    m_document->updateDataSourceTree(m_document->mainWindow()->dataSourceTree(), true,false,false,true);
}

void NCRDDataSourceDialog::discardChanges()
{
    for (int row = 0; row <  m_model->rowCount(); ++row) {
        NCRDDataSourceItem *dsItem = (NCRDDataSourceItem*)(m_model->item( row, ColPOINTER ));
        NCReportDataSource *ds = dsItem->dataSource();
        if ( dsItem->flagNew() ) {
            qDebug("(discard) Remove datasource %p",ds);
            delete ds;
        }
    }
}

void NCRDDataSourceDialog::updateEnable()
{
    bool enable = (m_model->rowCount() > 0);
    gbDS->setEnabled( enable );
    stackW->setEnabled( enable );
    if (!enable) {
        filenameurl->setVisible( false );
        lblFilename->setVisible( false );
        return;
    }

    int row = m_mapper->currentIndex();
    NCRDDataSourceItem *dsItem = (NCRDDataSourceItem*)(m_model->item( row, ColPOINTER ));
    NCReportDataSource *ds = dsItem->dataSource();
    NCReportDataSource::LocationType loc = (NCReportDataSource::LocationType)comboLocation->currentIndex();
    NCReportDataSource::OpenRoles role = (NCReportDataSource::OpenRoles)comboRole->currentIndex();
    QString caption;
    enable = (loc != NCReportDataSource::Static);
    switch (loc) {
        case NCReportDataSource::Static: caption = ""; break;
        case NCReportDataSource::File: caption = tr("File name"); break;
        case NCReportDataSource::Http: caption = tr("Http URL"); break;
        case NCReportDataSource::Ftp: caption = tr("Ftp URL"); break;
        case NCReportDataSource::Parameter: caption = tr("Parameter ID"); break;
    }
    lblFilename->setText( caption );
    switch (role) {
        case NCReportDataSource::BeginReport:
        case NCReportDataSource::ChildDataSource:
        case NCReportDataSource::SubReport: caption = tr("Parent datasource ID"); break;
        case NCReportDataSource::GroupChange: caption = tr("Group ID"); break;
    }
    lblParentID->setText( caption );

    switch ( ds->dataSourceType() ) {
        case NCReportDataSource::StringList: enable = false; break;
        case NCReportDataSource::ItemModel: enable = false; break;
        case NCReportDataSource::Custom: enable = false; break;
        default: break;
    }

    filenameurl->setVisible( enable );
    lblFilename->setVisible( enable );

    queryEditor->setEnabled( !enable );
    wStaticText->setEnabled( !enable );
    staticXML->setEnabled( !enable );

    //------------------------------------------
    enable = (cbPrivateConn->isChecked());
    gbConnection->setEnabled(enable);

    //------------------------------------------
    enable = (comboTxtColumnSep->currentIndex() == 5);
    delimCharTxt->setEnabled( enable );
    enable = (comboSlColumnSep->currentIndex() == 5);
    delimCharSl->setEnabled( enable );

    wParentDataSource->setEnabled( role != NCReportDataSource::BeginReport );
}

QComboBox * NCRDDataSourceDialog::comboBoxDSType()
{
    return comboDsType;
}

QComboBox * NCRDDataSourceDialog::comboBoxLocType()
{
    return comboLocation;
}

QComboBox * NCRDDataSourceDialog::comboTxtColumnDelimiter()
{
    return comboTxtColumnSep;
}

QComboBox * NCRDDataSourceDialog::comboSlColumnDelimiter()
{
    return comboSlColumnSep;
}

QComboBox * NCRDDataSourceDialog::comboOpenRole()
{
    return comboRole;
}

//me:扩展
QComboBox * NCRDDataSourceDialog::comboDataViewName()
{
    return dataViewNameComboBox;
}

void NCRDDataSourceDialog::setCurrentDataSource(const QString &dsID)
{
    if (dsID.isEmpty())
        return;

    QModelIndexList indexList = m_model->match(m_model->index(0,0), Qt::EditRole, dsID, 1, Qt::MatchExactly);
    if (indexList.count()>=1) {
        listDS->setCurrentIndex(indexList.at(0));
        //slotItemChanged(indexList.at(0), QModelIndex());
    }
}


void NCRDDataSourceDialog::textEditorContentChanged()
{
//    int row = m_mapper->currentIndex();
//    if ( sender() == queryEditor ) {
//        m_model->item( row, ColQRY )->setText( queryEditor->toPlainText() );
//    } else if ( sender() == staticText ) {
//        m_model->item( row, ColSPECTEXT )->setText( staticText->toPlainText() );
//    }  else if ( sender() == staticXML ) {
//        m_model->item( row, ColSPECXML )->setText( staticXML->toPlainText() );
//    }  else if ( sender() == wItemModelSpec ) {
//        m_model->item( row, ColMODELRELATIONSPEC )->setText( wItemModelSpec->toPlainText() );
//    }
}


//------------------------------------------------------------------------------
// NCRDDataSourceItemDelegate
//------------------------------------------------------------------------------
NCRDDataSourceItemDelegate::NCRDDataSourceItemDelegate( NCRDDataSourceDialog *parent )
    : QItemDelegate( parent ), dialog(parent)
{
}

//如果索引对应的控件为comboBox, 则返回对应comboBox组件.
QWidget * NCRDDataSourceItemDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    switch ( index.column() ) {
        case NCRDDataSourceDialog::ColDSTYPE: return dialog->comboBoxDSType();
        case NCRDDataSourceDialog::ColLOCTYPE: return dialog->comboBoxLocType();
        case NCRDDataSourceDialog::ColTXTCOLDELIM: return dialog->comboTxtColumnDelimiter();
        case NCRDDataSourceDialog::ColSLCOLDELIM: return dialog->comboSlColumnDelimiter();
        case NCRDDataSourceDialog::ColOPENROLE: return dialog->comboOpenRole();
        case NCRDDataSourceDialog::ColDataViewName: return dialog->comboDataViewName();//me:扩展
    default: break;
    }
    return QItemDelegate::createEditor( parent, option, index);
}

//如果是组合框 则将组合框的当前选项添加到model中.
void NCRDDataSourceItemDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
    if ( ((NCRDDataSourceItemDelegate*)this)->isComboBox(index) ) {
        QComboBox *combo = qobject_cast<QComboBox*>(editor);
        if ( combo ) {
            combo->setCurrentIndex( index.data(Qt::EditRole).toInt() );
            //qDebug("Model index row:%i col:%i, data:%i", index.row(), index.column(), index.data( Qt::EditRole ).toInt() );
            //qDebug("%p combo->setCurrentIndex(%i)",combo ,index.data(Qt::DisplayRole).toInt() );
            //<test>
            qDebug()<<"--------setEditor---------";
            for(int i=0; i<combo->count();i++)
            {
                qDebug()<<combo->itemText(i)<<",";
            }
            //</test>
            qDebug("Model index row:%i col:%i, data:%i", index.row(), index.column(), index.data( Qt::EditRole ).toInt() );
            qDebug()<<"---------/setEditor---------";
        }
    } else {
        QItemDelegate::setEditorData( editor, index);
    }
}

void NCRDDataSourceItemDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
    //通过索引判断是否为组合框.如果是组合框,则将editor强转成组合框.
    //将组合框的当前索引设置到model中.
    if ( ((NCRDDataSourceItemDelegate*)this)->isComboBox(index) ) {
        QComboBox *combo = qobject_cast<QComboBox*>(editor);
        if ( combo ) {
            //bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
            model->setData( index, combo->currentIndex() );
            //<test>
            qDebug()<<"--------model----------";
            for(int i=0; i<combo->count();i++)
            {
                qDebug()<<combo->itemText(i)<<",";
            }
            //</test>
            qDebug("%p model->setData(%i)", combo, combo->currentIndex() );
            qDebug()<<"---------/model---------";
        }
    } else {
        QItemDelegate::setModelData( editor, model, index);
    }
}

void NCRDDataSourceItemDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QItemDelegate::updateEditorGeometry( editor, option, index );
}

bool NCRDDataSourceItemDelegate::isComboBox(const QModelIndex & index)
{
    switch ( index.column() ) {
        case NCRDDataSourceDialog::ColDSTYPE:
        case NCRDDataSourceDialog::ColLOCTYPE:
        case NCRDDataSourceDialog::ColTXTCOLDELIM:
        case NCRDDataSourceDialog::ColSLCOLDELIM:
        case NCRDDataSourceDialog::ColDRIVER:
        case NCRDDataSourceDialog::ColOPENROLE:
        case NCRDDataSourceDialog::ColDataViewName://me:扩展
            return true;
        default:
            break;
    }
    return false;
}

void NCRDDataSourceDialog::testExternalConnection()
{
    bool ok = true;
    QString msg;

    QSqlDatabase database = QSqlDatabase::addDatabase( cDriver->currentText(), "external_test" );
    ok = database.isValid();
    if ( !ok )
        msg = tr("Could not load %1 database driver.").arg(cDriver->currentText());

    if (ok) {
        database.setHostName( cHost->text() );
        database.setDatabaseName( cDatabase->text() );
        database.setUserName( cUsername->text() );
        database.setPassword( cPassword->text() );
        if ( cPort->value() != 0 )
            database.setPort( cPort->value() );

        ok = database.open();

        if ( ok )
            database.close();
        else
            msg = database.lastError().databaseText();
    }

    QSqlDatabase::removeDatabase("external_test");

    if ( ok )
        QMessageBox::information( this, tr("Connection OK"), tr("Connection succeded!") );
    else
        QMessageBox::warning( this, tr("Connection error"), msg );

}

void NCRDDataSourceDialog::runSqlTest()
{
    if (queryEditor->toPlainText().isEmpty() || m_model->rowCount() == 0 )
        return;

    if (m_sqlTestDataSource->isOpened() )
        m_sqlTestDataSource->close();

    setSqlDataSourceData(m_sqlTestDataSource,m_mapper->currentIndex());
    m_sqlTestDataSource->setErrorHandler( m_document->reportDef()->error() );
    m_sqlTestDataSource->setSpecText(queryEditor->toPlainText());
    // add parameters
    QHash<QString,QVariant>::const_iterator i = m_document->reportDef()->mainWindow()->reportParameters().constBegin();
    while (i != m_document->reportDef()->mainWindow()->reportParameters().constEnd()) {
        m_document->reportDef()->addParameter(i.key(),i.value());
        ++i;
    }
    NCReportEvaluator evaluator;
    evaluator.setReportDef(m_document->reportDef());
    m_sqlTestDataSource->evaluate( &evaluator );

    if (m_sqlTestDataSource->open(NCReportDataSource::OPEN_MODEL_QUERY)) {
        QSqlQuery *query = m_sqlTestDataSource->queryObject();
        m_sqlTestModel->setQuery(*query);
        sqlTestView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
        //sqlTestView->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
        tabWidget->setTabEnabled(2,true);
        tabWidget->setCurrentWidget(tabTester);
    } else {
#if defined(Q_WS_MAC) || defined(Q_OS_MAC)
        QMessageBox msgBox;
        msgBox.setWindowTitle( tr("Error") );
        msgBox.setText(tr("SQL Error %1").arg(m_document->reportDef()->error()->number()) );
        msgBox.setInformativeText( m_document->reportDef()->error()->errorMsg() );
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
#else
        QMessageBox::warning(this,tr("SQL Error %1").arg(m_document->reportDef()->error()->number()),
                            m_document->reportDef()->error()->errorMsg() );
#endif
        qApp->clipboard()->setText(m_document->reportDef()->error()->errorMsg());
    }
}

void NCRDDataSourceDialog::setSqlDataSourceData( NCReportSQLDataSource *ds, int row)
{
    ds->setID( m_model->item(row,ColID)->text() );
    ds->setConnectionID( m_model->item(row,ColCONNID)->text() );
    ds->setConnectionType( m_model->item(row,ColISEXTERNAL)->text() == "true" ? NCReportDataSource::External : NCReportDataSource::Internal );
    ds->setHost( m_model->item(row,ColHOST)->text() );
    ds->setDB( m_model->item(row,ColDB)->text() );
    ds->setUser( m_model->item(row,ColUSER)->text() );
    ds->setPassword( m_model->item(row,ColPASSW)->text() );
    ds->setPort( m_model->item(row,ColPORT)->text() );
    //ds->setDriver( model->item(row,ColDRIVER)->text() );
    ds->setDriver( cDriver->itemText(m_model->item(row,ColDRIVER)->data( Qt::EditRole ).toInt()) );
    ds->setSpecText( m_model->item(row,ColQRY)->text() );
    ds->setParentId( m_model->item(row,ColPARENTID)->text() );
    ds->setForwardOnly( m_model->item(row,ColFWDONLY)->text() == "true" );
    ds->setConnectOptions( m_model->item(row,ColCONNECTOPTIONS)->text());
    ds->setForceUpdateOnStepBackwordInRelation( m_model->item(row,ColForceUpdateOnStepBackRel)->text() == "true" );
}

void NCRDDataSourceDialog::updateColumnView()
{
    if ( m_mapper) {
        int row = m_mapper->currentIndex();
        NCRDDataSourceItem *dsItem = (NCRDDataSourceItem*)(m_model->item( row, ColPOINTER ));
        if (!dsItem)
            return;

        m_columnViewModel->clear();
        m_columnViewModel->setColumnCount(2);
        m_columnViewModel->setHorizontalHeaderItem( 0, new QStandardItem( tr( "Column id")));
        m_columnViewModel->setHorizontalHeaderItem( 1, new QStandardItem( tr( "Description")));

        NCReportDataSource *ds = dsItem->dataSource();

        if (!ds)
            return;

        QVector<NCReportDataFieldInfo> info = ds->fieldInfo();
        for( int n=0; n<info.size(); n++) {
            NCReportDataFieldInfo field = info[n];
            QList<QStandardItem*> rowItems;
            QStandardItem *item;

            item = new QStandardItem( field.name());
            item->setEditable( false);
            rowItems.push_back( item);

            item = new QStandardItem( field.briefDescription());
            item->setEditable( false);
            rowItems.push_back( item);

            m_columnViewModel->appendRow( rowItems);
        }
    }
}

//更新视图名下拉列表框
void NCRDDataSourceDialog::updateDataViewNames()
{
    QStringList items;
    DBQ::View_GetAllViewNameFromDir(m_document->getViewPath() ,items);

#ifdef TEST_DS
    QString viewNames;
    foreach(const QString & itemStr, items)
    {
        viewNames.append(itemStr).append(",");
    }

    WriteLog("views:"+viewNames);
    //qDebug()<<"views:"<<items;
#endif

    dataViewNameComboBox->clear();
    dataViewNameComboBox->addItems(items);
    dataViewNameComboBox->setCurrentIndex(-1);//先将Edit框设置成空

    NCReportDataViewDataSource * dvSource = getDataViewDataSource();
    if(!dvSource)
        return;  //TODO:log

    int nameIndex = dataViewNameComboBox->findText(dvSource->getViewName(), Qt::MatchExactly);
    dataViewNameComboBox->setCurrentIndex(nameIndex);//设置Edit框为dvSource的视图名
}
void NCRDDataSourceDialog::updateFieldsInDataView()
{
    m_dataViewFieldModel->clear();
    m_dataViewFieldModel->setColumnCount(4);
    m_dataViewFieldModel->setHorizontalHeaderItem( 0, new QStandardItem( tr( "Name")));
    m_dataViewFieldModel->setHorizontalHeaderItem( 1, new QStandardItem( tr( "Type")));
    m_dataViewFieldModel->setHorizontalHeaderItem( 2, new QStandardItem( tr( "Alias")));
    m_dataViewFieldModel->setHorizontalHeaderItem( 3, new QStandardItem( tr( "Table Name")));

    if(!m_mapper)
        return ;
    int row = m_mapper->currentIndex();

    int index = m_model->item( row, ColDataViewName )->data(Qt::EditRole).toInt();
    QString viewName = dataViewNameComboBox->itemText(index);
    qDebug()<<"View:index = "<<index<<", name = "<<viewName<<", row = "<<row;
    if(index<0)
        return ;

    DBQ::View * pView = 0;//TODO:删除
    QString errStr;
    if(!DBQ::View_LoadFromDir(m_document->getViewPath(), viewName, &pView, errStr))
            return ; //TODO:log

    foreach(DBQ::ViewField * pField, pView->fields)
    {
        QList<QStandardItem*> rowItems;
        QStandardItem *item;

        item = new QStandardItem(pField->fieldName);
        rowItems.push_back(item);

        item = new QStandardItem(DBQ::View_GetFieldTypeStringFromType(pField->type));
        rowItems.push_back(item);

        item = new QStandardItem(pField->alias);
        rowItems.push_back(item);

        item = new QStandardItem(pField->tableName);
        rowItems.push_back(item);

        m_dataViewFieldModel->appendRow( rowItems);
    }
    delete pView;
}

//视图名称Comobox控件 当前索引改变事件
//人为通过下拉框改变当前索引,才会触发此槽函数.不然会程序会自动调用此函数,导致在此Dialog显示的时候 就已在下拉框中选中一项.
//addItems函数就会发送currentIndexChanged信号.
void NCRDDataSourceDialog::activatedInDataViewNameComboBox(int index)
{
    /*
    if(index<0)
        return ;

    QString viewName = dataViewNameComboBox->itemText(index);//TODO:index非法会怎样
    //设置DataSource的视图名称
    NCReportDataViewDataSource * ds = getDataViewDataSource();
    if(ds)
        ds->setViewName(viewName);
    //ds == 0 TODO:log
    */

    if(index<0)
        return ;
    if(!m_mapper)
        return ;
    int row = m_mapper->currentIndex();

    //bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    bool b = m_model->setData(m_model->index(row, ColDataViewName), index);
    //<test>
    qDebug()<<"--------activated----------";

    qDebug()<<b<<","<<index;
    qDebug()<<"---------/activated---------";
     //</test>

    updateFieldsInDataView();
}

NCReportDataViewDataSource * NCRDDataSourceDialog::getDataViewDataSource()
{
    if(!m_mapper)
        return 0;

    int row = m_mapper->currentIndex();
    NCRDDataSourceItem *dsItem = (NCRDDataSourceItem*)(m_model->item( row, ColPOINTER ));
    if (!dsItem)
        return 0;  //TODO:log
    NCReportDataSource *ds = dsItem->dataSource();
    if (!ds)
        return 0;
    NCReportDataViewDataSource * dvSource = qobject_cast<NCReportDataViewDataSource *>(ds);
    if(!dvSource)
        return 0;

    return dvSource;
}

void NCRDDataSourceDialog::updateParentDataSourceWidget()
{
//    comboParentDS->clear();
//    if (m_model->rowCount()==0)
//        comboParentDS->addItems( m_document->reportDef()->dataSourceIdList() );
//    else {
//        for (int i=0;i<m_model->rowCount();i++)
//            comboParentDS->addItem( m_model->data( m_model->index(i,0)).toString() );
//    }
//    comboParentDS->addItem("");
}
