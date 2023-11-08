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
#include "ncrdmainwindow.h"
#include "ncrddocument.h"
#include "ncrdruler.h"
#include "ncrddesignsection.h"
#include "ncrddesignarea.h"
#include "ncrd_global.h"
#include "ncrdsectionscene.h"
#include "ncrddef.h"
#include "ncrdpropertyeditor.h"
#include "ncreportdatasource.h"
#include "ncreportgraphitem.h"
#include "ncrdlanguage.h"

#include "ncrdlabelpropdia.h"
#include "ncrdfieldpropdialog.h"
#include "ncrdshapepropdialog.h"
#include "ncrdimagepropdialog.h"
#include "ncrdbarcodepropdialog.h"
#include "ncrdgraphpropdialog.h"
#include "ncrdtextpropdialog.h"
#include "ncrdsectiontitle.h"
#include "ncrdsectiondialog.h"
#include "ncrdcommands.h"
#include "ncrdcrosstabdialog.h"
#include "ncrdhighchartsitemdialog.h"
#include "ncrdtableitemdialog.h"
#include "ncreportgroup.h"
#include "ncrdxmlcopyreader.h"
#include "ncrdxmldefreader.h"
#include "ncrdxmldefwriter.h"
#include "ncrdxmlcopywriter.h"
#include "ncrdcustomdatasource.h"
#include "ncrddatasourcetree.h"
#include "ncrdgraphitemdialogfactory.h"
#include "ncreportevaluator.h"

#include <QApplication>
#include <QCloseEvent>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QSplitter>
#include <QFrame>
#include <QUndoStack>
#include <QUndoGroup>
#include <QMdiSubWindow>
#include <QScrollBar>
#include <QBuffer>
#include "../../include/MDDBQueryConfigure/MDDBQueryConfigure.h"
//#include "D:\checkout\NetSCADA_CPP\include\MDDBQueryConfigure\MDDBQueryConfigure.h"

#define RULERWEIGHT	28
#define DEFAULT_PAGE_WIDTH	210.0

NCRDDocument::NCRDDocument( NCRDMainWindow *parent ) : QScrollArea( parent ),
    m_isUntitled(true),
    m_modified(false),
    m_counterOfDetails(0),
    m_zoomScale(1.0f),
    m_mainWindow(parent),
    m_rdef(0),
    m_splittter(0),
    m_currentDesignSection(0),
    m_rulertop(0),
    m_sectionContainer(0),
    m_laySC(0),
    m_currentScene(0),
    m_undostack(new QUndoStack( this )),
    m_firstSelectedItem(0),
    m_subWindow(0)
{
    //DBQ::SetGlobalDirectory("D:/svn/HaiDe/ncreport/ConfigureTestCase/case1");//temp test
    //--------------------------------------------
    // Attributes
    //--------------------------------------------
    setAttribute(Qt::WA_DeleteOnClose);
    setBackgroundRole(QPalette::Dark);

    //--------------------------------------------
    // REPORT definition
    //--------------------------------------------
    m_rdef = new NCRDDef( parent, this, this );
    m_rdef->setZintPath(m_mainWindow->zintPath());

    //--------------------------------------------
    // Add top ruler
    //--------------------------------------------
    m_rulertop = new NCRDRuler( Qt::Horizontal, this );
    m_rulertop->setRulerMargin( RULERWEIGHT );
    m_rulertop->move(0,0);
    setViewportMargins( 0, m_rulertop->height()-2, 0, 0 );

    //--------------------------------------------
    // Section container
    //--------------------------------------------
    m_sectionContainer = new QWidget( this );
    m_sectionContainer->setBackgroundRole( QPalette::Window );

    m_laySC = new QVBoxLayout( m_sectionContainer );
    m_laySC->setMargin(0);
    m_laySC->setSpacing(0);
    m_sectionContainer->setLayout( m_laySC );

    setWidget( m_sectionContainer );

    connect( horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(slotHorizontalScroll(int)) );

#ifdef PACOS_INTEGRATION
    NCRDCustomDataSource *ds = new NCRDCustomDataSource( rdef );
    ds->setID("PACOS");
    rdef->addDataSource( ds );
#endif
}

NCRDDocument::~ NCRDDocument()
{
    if (m_mainWindow)
    {
//        m_mainWindow->resetManagers( 0 );
        if ( m_mainWindow->isPropEditor() ) {
            m_mainWindow->propertyEditor()->setDocument( 0 );
            m_mainWindow->propertyEditor()->setSection( 0 );
            m_mainWindow->propertyEditor()->setItem( 0 );
        }
    }
}

NCRDMainWindow * NCRDDocument::mainWindow() const
{
    return m_mainWindow;
}

void NCRDDocument::newFile( const QString& filename )
{
    initReportDef();

    static int sequenceNumber = 1;
    m_isUntitled = true;
    m_source.setSourceType(NCReportSource::File);
    m_source.setFileName(filename.arg(sequenceNumber++));
    setWindowTitle(m_source.fileName() + "[*]");
    if ( m_subWindow )
        m_subWindow->setWindowIcon( QIcon(":/designer/images/page.png") );

    //--------------------------------------------
    // Sections test
    //--------------------------------------------
    m_rdef->addPageHeader();
    m_rdef->addDetail();	// default detail
    m_rdef->addPageFooter();
    //updateGrid();

    setDefaultFont(m_mainWindow->defaultFont());
    updateLanguageMenu( m_mainWindow->languageMenu(), m_mainWindow->languageActionGroup(), m_mainWindow );

    m_mainWindow->resetManagers( this );
    setModified(false);

    updateSectionsByReportDef();
}

bool NCRDDocument::load(const NCReportSource & rs )
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    bool ok = true;

    initReportDef();

    NCReportSource reportSource = rs;
    reportSource.setReportDef( m_rdef );
    NCRDXMLDefReader reader( m_rdef );
    ok = reportSource.load( reader );

    //<为DataViewSource设置view路径>
    //修复Bug:报表中已设置DataSource，但是字段未列出。
    if(ok)
    {
        QHash<QString,NCReportDataSource*>::const_iterator ite = m_rdef->dataSources().constBegin();
        while ( ite!= m_rdef->dataSources().constEnd() ) {
            NCReportDataSource* source = ite.value();
            source->setViewPath(m_ViewPath);
            ++ite;
        }
    }
    //</为DataViewSource设置view路径>

    if (ok) {
        updateSectionsByReportDef();
        //updateGrid();
        m_mainWindow->resetManagers( this );
        setModified(false);
    }
    QApplication::restoreOverrideCursor();

    if (ok) {
        setReportSource( reportSource );
        updateLanguageMenu( m_mainWindow->languageMenu(), m_mainWindow->languageActionGroup(), m_mainWindow );
    }
    else
        QMessageBox::warning(this, REPORTDESIGNER_NAME, m_rdef->error()->errorMsg() );

    return ok;
}

bool NCRDDocument::save()
{
    if (m_isUntitled) {
        return saveAs(m_source.fileName());
    } else {
        return save(m_source);
    }
}

bool NCRDDocument::saveAs(const QString& fname )
{
    NCReportSource rs;
    rs.setFileName( QFileDialog::getSaveFileName(this, tr("Save As"), (fname.isEmpty() ? m_source.fileName() : fname), NCREPORTAPP_DIALOG_EXTENSIONS) );
    if (rs.fileName().isEmpty())
        return false;
    if ( !(rs.fileName().endsWith(".XML",Qt::CaseInsensitive) || rs.fileName().endsWith(".NCR",Qt::CaseInsensitive)) )
        rs.setFileName( rs.fileName() += ".ncr" );
    return save( rs );
}

bool NCRDDocument::StudiosaveAs(const QString& fname )
{
    NCReportSource rs;
//    rs.setFileName( QFileDialog::getSaveFileName(this, tr("Save As"), (fname.isEmpty() ? m_source.fileName() : fname), NCREPORTAPP_DIALOG_EXTENSIONS) );
    rs.setFileName(fname);
    if (rs.fileName().isEmpty())
        return false;
    if ( !(rs.fileName().endsWith(".XML",Qt::CaseInsensitive) || rs.fileName().endsWith(".NCR",Qt::CaseInsensitive)) )
        rs.setFileName( rs.fileName() += ".ncr" );
    return save( rs );
}

bool NCRDDocument::save(const NCReportSource &rs )
{
    QApplication::setOverrideCursor(Qt::BusyCursor);

    //HRaba
    //m_source.setSourceType( rs.sourceType() );
    //m_source.setFileName( rs.fileName() );
    //m_source.setSaveQuery( rs.saveQuery() );
    //m_source.setConnectionID( rs.connectionID() );
    NCReportSource reportSource = rs;
    reportSource.setReportDef( m_rdef );

    NCRDXMLDefWriter writer;
    writer.setReportDef( m_rdef );
    bool ok = reportSource.save( writer );

    QApplication::restoreOverrideCursor();

    if (ok)
        setReportSource( reportSource );
    else
        QMessageBox::warning(this, REPORTDESIGNER_NAME, m_rdef->error()->errorMsg() );

    return true;
}
/*!
 * \brief NCRDDocument::saveToByteArray
 * \return report definition XML bytearray
 * Saves the current report document to a QByteArray
 */

QString NCRDDocument::saveToString() const
{
    QString xml;
    NCRDXMLDefWriter writer(&xml);
    writer.setReportDef( m_rdef );
    writer.write(0);
    return xml;
}

QString NCRDDocument::userFriendlyCurrentFile() const
{
    //return strippedName( curSourceInfo.fileName );
    return m_source.fileName();
}

QString NCRDDocument::fileName() const
{
    return m_source.fileName();
}

void NCRDDocument::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        m_mainWindow->undoGroup()->removeStack(m_undostack);
        m_mainWindow->dataSourceTree()->clearDataSources();
        m_mainWindow->dataSourceTree()->clearVariables();

        event->accept();
    } else {
        event->ignore();
    }
}

void NCRDDocument::setDocumentModified()
{
    setModified( true );
}

bool NCRDDocument::maybeSave()
{
    if (isModified()) {
    QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, REPORTDESIGNER_NAME,
                     tr("'%1' has been modified.\n"
                        "Do you want to save your changes?")
                     .arg(userFriendlyCurrentFile()),
                     QMessageBox::Save | QMessageBox::Discard
             | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void NCRDDocument::setReportSource(const NCReportSource & rs )
{
    m_source = rs;
    if ( rs.sourceType() == NCReportSource::File ) {
        m_source.setFileName(QFileInfo(rs.fileName()).canonicalFilePath());
        setWindowTitle(userFriendlyCurrentFile() + "[*]");
    } else {
        setWindowTitle( rs.fileName() + "[*]");
    }
    m_isUntitled = false;
    setModified(false);
}

QString NCRDDocument::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void NCRDDocument::initReportDef()
{
    m_rdef->setItemModels( m_mainWindow->itemModels() );
    m_rdef->addGraphItemRenderers( m_mainWindow->graphItemRenderers() );
    m_rdef->addDataSources( m_mainWindow->customDataSources() );
    //m_mainWindow->addCustomDataSourcesToDocument(this);
}

void NCRDDocument::setModified(bool set)
{
    m_modified = set;
    setWindowModified(set);
    if ( m_subWindow )
        m_subWindow->setWindowIcon( set ? QIcon(":/designer/images/page_save.png") : QIcon(":/designer/images/page.png") );

    m_mainWindow->updateMenus( this );
}

bool NCRDDocument::isModified()
{
    return m_modified;
}

//void NCRDDocument::addSection( NCReportSection::SectionType st )
//{
//	NCReportSection *sect = 0;
//	switch ( st ) {
//		case NCReportSection::PageHeader:
//			sect = rdef->pageHeader();
//			break;
//		case NCReportSection::PageFooter:
//			sect = rdef->pageFooter();
//			break;
//		case NCReportSection::ReportHeader:
//			sect = rdef->reportHeader();
//			break;
//		case NCReportSection::ReportFooter:
//			sect = rdef->reportFooter();
//			break;
//		case NCReportSection::Detail:
//			sect = rdef->details().at(0);
//			break;
//		default: return;
//	}
//
//	addSection( sect );
//}

/*
    NCRDDesignSection *ds = new NCRDDesignSection( m_mainWindow, this, m_sectionContainer );
    ......
    connect( ds, SIGNAL(sectionResized(const QSize&)), this, SLOT(slotSectionResized(const QSize&)) );
 */
void NCRDDocument::slotSectionResized( const QSize& )
{
    //sender为ds.ds为NCRDDesignSection
    NCRDDesignSection *obj = qobject_cast<NCRDDesignSection*>(sender());
    //laySC->update();
    if ( obj && m_mainWindow->isPropEditor() )
        m_mainWindow->propertyEditor()->setSection( obj );
}

QUndoStack * NCRDDocument::undoStack() const
{
    return m_undostack;
}

//设置鼠标形状
//普通情况下为箭头
//当在工具栏上点击表格等按钮后，鼠标进入各个section时鼠标形状显示成十字交叉线
void NCRDDocument::setToolMode()
{
    //return;
    for ( int i=0; i<m_sections.count(); ++i ) {
        NCRDDesignSection *section = m_sections.at(i);//会有多个section 所以要遍历
        switch ( m_mainWindow->currentToolState() ) {
            case NCRDMainWindow::Tool:
            {
                section->designArea()->setDragMode( QGraphicsView::NoDrag );//禁止拖拽
                section->designArea()->setCursor( QCursor(Qt::CrossCursor) );
                break;
            }
            case NCRDMainWindow::Arrow:
            {
                section->designArea()->setDragMode( QGraphicsView::RubberBandDrag );//设置可显示拖拽框
                section->designArea()->setCursor( QCursor(Qt::ArrowCursor) );
                break;
            }
        }
    }
}

///*!
//  Returns the index of layout for insert
//  */
//int NCRDDocument::sectionIndex( NCReportSection* sect )
//{
//	if (!sect)
//		return -1;
//	int idx=0;
//	for ( int i=0; i<sectionIndexes.count(); ++i ) {
//		int si = sectionIndexes.at(i);
//		qDebug("sectionIndex( sect=%i < dsect=%i )", sect->layoutIndex(), si );
//		if ( sect->layoutIndex() < si ) {
//			qDebug("Return %i",idx);
//			return idx;
//		}
//
//		idx++;
//	}
//	qDebug("End.Return %i",idx);
//	return idx;
//}

/*!
Updates the entire document by the content of NCReportDef
*/
void NCRDDocument::updateSectionsByReportDef()
{
    //<获得所有NCReportSection的ID>
    QList<int> layoutIDX;
    layoutIDX << updateSection( m_rdef->pageHeader() );
    layoutIDX << updateSection( m_rdef->reportHeader() );

    for ( int i=0; i<m_rdef->details().count(); ++i ) {
        NCReportSection *detail = m_rdef->details().at(i);
        for ( int j=0; j<detail->groups().count(); ++j ) {
            NCReportGroup *group = detail->groups().at(j);
            layoutIDX << updateSection(group->header());
            layoutIDX << updateSection(group->footer());
        }
        layoutIDX << updateSection( detail );
    }
    layoutIDX << updateSection( m_rdef->reportFooter() );
    layoutIDX << updateSection( m_rdef->pageFooter() );
    //</获得所有NCReportSection的ID>


    //先将NCReportSection ID的List排序按NCReportSection ID号找到对应的NCRDDesignSection，并添加到section container中。
    updateSectionsLayoutOrder( layoutIDX );
    adjustContent();
    updateGrid();
    m_rulertop->resize( qMax(width(),m_sectionContainer->width()), RULERWEIGHT );
}

/*
void NCRDDocument::updateGroupSections( NCReportSection *detail )
{
    rdef->sortGroups( detail );
    for ( int j=0; j<detail->groups().count(); ++j ) {
        NCReportGroup *group = detail->groups().at(j);
        updateSection(group->header());
        updateSection(group->footer());
    }
    adjustContent();
    updateSectionOrder();
}


void NCRDDocument::updateSectionOrder()
{
    sectionIndexes.clear();

    QLayoutItem *child;
    while ((child = laySC->takeAt(0)) != 0) {
        delete child;
    }
    for ( int i=0; i<sections.count(); ++i ) {
        NCRDDesignSection *dsect = sections.at(i);

        laySC->insertWidget( sectionIndex(dsect->sectionScene()), dsect );
        sectionIndexes.append(dsect->sectionScene()->layoutIndex());
        qSort(sectionIndexes.begin(), sectionIndexes.end());
    }
}
*/

void NCRDDocument::updateSectionsLayoutOrder( QList<int>& layoutIndexList )
{
    QLayoutItem *child;
    while ((child = m_laySC->takeAt(0)) != 0) {	// removes all items from layout 移除所有QLayoutItem
        delete child;
    }
    qSort(layoutIndexList.begin(), layoutIndexList.end());
    for ( int i=0; i<layoutIndexList.count(); ++i ) {
        //以ID找到对应的NCRDDesignSection
        NCRDDesignSection *dsect = designSectionByLayoutIndex(layoutIndexList.at(i));
        if ( dsect )
            m_laySC->addWidget( dsect );
    }
}

/*!
Find designer section by NCReportSection* (scene). Returns the appropriate design section if exists, 0 if does not.
通过NCReportSection *找到对应的NCRDDesignSection.
*/
NCRDDesignSection* NCRDDocument::designSection( NCReportSection *s )
{
    if (!s)
        return 0;
    for ( int i=0; i<m_sections.count(); ++i ) {
        NCRDDesignSection *dsect = m_sections.at(i);
        if ( dsect->sectionScene() == s )
            return dsect;
    }
    return 0;
}

//通过NCReportSection的ID找到对应的NCRDDesignSection.
NCRDDesignSection* NCRDDocument::designSectionByLayoutIndex( int idx )
{
    for ( int i=0; i<m_sections.count(); ++i ) {
        NCRDDesignSection *dsect = m_sections.at(i);
        if ( dsect->sectionScene()->layoutIndex() == idx )
            return dsect;
    }
    return 0;
}

/*!
  Adds a new section as design section into the document.
  */
void NCRDDocument::addSection( NCReportSection *sect )
{
    if (!sect)
        return;

    NCRDDesignSection *ds = new NCRDDesignSection( m_mainWindow, this, m_sectionContainer );
    ds->setSectionScene( (NCRDSectionScene*)sect );
    ds->setTitle( sect->caption() );
    ds->setHeightMM( sect->metricHeight() );
    ds->setWidthMM( purePageWidth() );
    connect( ds, SIGNAL(sectionResized(const QSize&)), this, SLOT(slotSectionResized(const QSize&)) );

    // INSERT INTO LAYOUT
    //laySC->insertWidget( sectionIndex(sect), ds );
    ds->show();
    m_sections.append(ds);
    //sectionIndexes.append( sect->layoutIndex() );
    //qSort(sectionIndexes.begin(), sectionIndexes.end());

    qDebug("Section added.");
}

/*!
  Updates the section's geometry and title if exists. If not, added as new sections.
  */
int NCRDDocument::updateSection( NCReportSection *sect )
{
    if (!sect)
        return -1;
    NCRDDesignSection *ds = designSection(sect);

    if ( ds ) {
        // ok, section is already exist, we just update geometry
        ds->setTitle( sect->caption() );
        ds->setHeightMM( sect->metricHeight() );
        ds->setWidthMM(purePageWidth() );
    } else {
        // we must add the section into the document
        addSection( sect );
    }
    return sect->layoutIndex();
}

void NCRDDocument::removeSection( NCReportSection *sect )
{
    if (!sect)
        return;
    NCRDDesignSection *dsect = designSection(sect);
    if ( !dsect )
        return;

    m_sections.removeAll( dsect );
    delete dsect;
    //rdef->removeSection(sect);
}

void NCRDDocument::testFunction()
{
//	NCRDDesignSection *ds = new NCRDDesignSection( mainwindow, this, sectioncontainer, "Test" );
//	ds->setHeight( 15.0 );
//	laySC->addWidget( ds );
//	ds->show();
//	sectioncontainer->adjustSize();
}

NCRDDef * NCRDDocument::reportDef()
{
    return m_rdef;
}

void NCRDDocument::removeGroupSections( NCReportSection *detail )
{
    for ( int i=0; i<detail->groups().count(); ++i ) {
        NCReportGroup *group = detail->groups().at(i);
        if ( group->header() )
            removeSection( group->header() );
        if ( group->footer() )
            removeSection( group->footer() );
    }
}

/*!
Updates the page settings
*/
void NCRDDocument::updatePageSettings()
{
    for ( int i=0; i<m_sections.count(); ++i ) {
        NCRDDesignSection *dsect = m_sections.at(i);
        dsect->setWidthMM( purePageWidth() );
        dsect->sectionScene()->clearSelection();
        //qDebug("Set section width: %f mm", netWidth );
    }
    m_mainWindow->resetManagers( this );
    updateLanguageMenu( m_mainWindow->languageMenu(), m_mainWindow->languageActionGroup(), m_mainWindow );
}

void NCRDDocument::sceneContentModified(const QList< QRectF > & )
{
    //qDebug("QList< QRectF > %i", rList.count() );
}

qreal NCRDDocument::purePageWidth() const
{
    return m_rdef->option().pageWidthMM() - m_rdef->option().leftMarginMM() - m_rdef->option().rightMarginMM();
}

void NCRDDocument::updateGrid()
{
    for ( int i=0; i<m_sections.count(); ++i ) {
        NCRDDesignSection *dsect = m_sections.at(i);
        dsect->sectionScene()->setGridVisible( m_mainWindow->isShowGrid() );
        dsect->sectionScene()->setSnapX( m_mainWindow->isSnapGrid() );
        dsect->sectionScene()->setSnapY( m_mainWindow->isSnapGrid() );
        dsect->sectionScene()->setDeltaX( m_mainWindow->grid().x() );
        dsect->sectionScene()->setDeltaY( m_mainWindow->grid().y() );
        dsect->sectionScene()->invalidate();
    }
}

void NCRDDocument::setCurrentScene(NCRDSectionScene *scene )
{
    m_currentScene = scene;
}

void NCRDDocument::setCurrentDesignSection(NCRDDesignSection *sect)
{
    m_currentDesignSection = sect;
    if ( m_mainWindow->isPropEditor() ) {
        m_mainWindow->propertyEditor()->setDocument( this );
        m_mainWindow->propertyEditor()->setSection( sect );
    }
}

NCRDSectionScene * NCRDDocument::currentScene()
{
    return m_currentScene;
}

void NCRDDocument::bringToFront()
{
    itemAlignmentOperation( BringFront );
}

void NCRDDocument::moveToBack()
{
    itemAlignmentOperation( MoveBack );
}

void NCRDDocument::alignLeft()
{
    itemAlignmentOperation( AlignLeft );
}

void NCRDDocument::alignRight()
{
    itemAlignmentOperation( AlignRight );
}

void NCRDDocument::alignHCenter()
{
    itemAlignmentOperation( AlignHCenter );
}

void NCRDDocument::alignVCenter()
{
    itemAlignmentOperation( AlignVCenter );
}

void NCRDDocument::alignTop()
{
    itemAlignmentOperation( AlignTop );
}

void NCRDDocument::alignBottom()
{
    itemAlignmentOperation( AlignBottom );
}

void NCRDDocument::alignSameWidth()
{
    itemAlignmentOperation( AlignSameWidth );
}

void NCRDDocument::alignSameHeight()
{
    itemAlignmentOperation( AlignSameHeight );
}

void NCRDDocument::itemAlignmentOperation(const ItemOperation op )
{
    NCReportItem *firstItem =0;
    NCRDAlignItemCommand *command = 0;

    for ( int i=0; i<m_sections.count(); ++i ) {
        NCRDDesignSection *dsect = m_sections.at(i);
        qDebug( "section: %s", qPrintable(dsect->title()->text()) );

        NCRDSectionScene *scene = dsect->sectionScene();

        QList<QGraphicsItem*> list = scene->selectedItems();
        for (int j = 0; j < list.size(); ++j) {
            NCReportItem *item = (NCReportItem*)list.at(j);
            qDebug( "-> selected item: %s", qPrintable(item->tagname()) );
            if ( !command )
                command = new NCRDAlignItemCommand( scene );

            if (!firstItem)
                firstItem = firstSelectedItem(); // previous: scene->firstSelectedItem();
            if (!firstItem)
                firstItem = item;

            switch (op) {
                case BringFront: {
                    item->setZValue( scene->nextZValue() );
                    break;
                }
                case MoveBack: {
                    item->setZValue( -scene->nextZValue() );
                    break;
                }
                case AlignLeft: {
                    if ( item != firstItem ) {
                        command->addItem( item, item->scenePos(), QPointF( firstItem->scenePos().x(), item->scenePos().y() ) );
                    }
                    break;
                }
                case AlignRight: {
                    if ( item != firstItem ) {
                        qreal wf = firstItem->boundingRect().width();
                        qreal w = item->boundingRect().width();
                        command->addItem( item, item->scenePos(), QPointF( firstItem->scenePos().x() + wf - w, item->scenePos().y() ) );
                    }
                    break;
                }
                case AlignTop: {
                    if ( item != firstItem ) {
                        command->addItem( item, item->scenePos(), QPointF( item->scenePos().x(), firstItem->scenePos().y() ) );
                    }
                    break;
                }
                case AlignBottom: {
                    if ( item != firstItem ) {
                        qreal hf = firstItem->boundingRect().height();
                        qreal h = item->boundingRect().height();
                        command->addItem( item, item->scenePos(), QPointF( item->scenePos().x(), firstItem->scenePos().y() + hf - h ) );
                    }
                    break;
                }
                case AlignVCenter: {
                    if ( item != firstItem ) {
                        qreal wf = firstItem->boundingRect().width();
                        qreal w = item->boundingRect().width();
                        command->addItem( item, item->scenePos(), QPointF( firstItem->scenePos().x() + wf/2 - w/2, item->scenePos().y() ) );
                    }
                    break;
                }
                case AlignHCenter: {
                    if ( item != firstItem ) {
                        qreal hf = firstItem->boundingRect().height();
                        qreal h = item->boundingRect().height();
                        command->addItem( item, item->scenePos(), QPointF( item->scenePos().x(), firstItem->scenePos().y() + hf/2 - h/2 ) );
                    }
                    break;
                }
                case AlignSameWidth: {
                    if ( item != firstItem ) {
                        qreal wf = firstItem->currentSize().width();
                        qreal h = item->currentSize().height();
                        command->addItem( item, item->currentSize(), QSizeF(wf,h) );
                    }
                    break;
                }
                case AlignSameHeight: {
                    if ( item != firstItem ) {
                        qreal w = item->currentSize().width();
                        qreal hf = firstItem->currentSize().height();
                        command->addItem( item, item->currentSize(), QSizeF(w,hf) );
                    }
                    break;
                }
                default:
                    break;

            }
        }
    }

    if (command)	// action - undoable move
        undoStack()->push( command );
}

void NCRDDocument::itemEdit(NCReportItem *item)
{
    if (!item)
        return;

    QDialog* dia = 0;

    switch ( item->objectType() ) {
    case NCReportItem::Label:
        dia = new NCRDLabelPropDialog( item, this );
        break;
    case NCReportItem::Field:
        dia = new NCRDFieldPropDialog( item, this );
        break;
    case NCReportItem::Text:
        dia = new NCRDTextPropDialog( item, this );
        break;
    case NCReportItem::Line:
    case NCReportItem::Rectangle:
    case NCReportItem::Ellipse:
        dia = new NCRDShapePropDialog( item, this );
        break;
    case NCReportItem::Image:
        dia = new NCRDImagePropDialog( item, this );
        break;
    case NCReportItem::Barcode:
        dia = new NCRDBarcodePropDialog( item, this );
        break;
    case NCReportItem::Graph:
    {
        if (mainWindow()->graphItemDialogFactory()) {
            dia = mainWindow()->graphItemDialogFactory()->createDialog(item, this);
            mainWindow()->graphItemDialogFactory()->updateWidget(dia,item);
        } else {
            dia = new NCRDGraphPropDialog( item, this );
        }
        break;
    }
    case NCReportItem::CrossTab:
        dia = new NCRDCrossTabDialog( item, this );
        break;
    case NCReportItem::Chart:
#ifdef HIGHCHARTS_INTEGRATION
        dia = new NCRDHighChartsItemDialog( item, this );
#endif
        break;
    case NCReportItem::Table:
        dia = new NCRDTableItemDialog( item, this );
        break;
    case NCReportItem::NoObject:
        break;
    }

    if (!dia)
        return;

    if (dia->exec() == QDialog::Accepted) {

        if (item->objectType()==NCReportItem::Graph) {
            if (mainWindow()->graphItemDialogFactory()) {
                mainWindow()->graphItemDialogFactory()->updateData(dia,item);
                mainWindow()->graphItemDialogFactory()->afterUpdateData(item);
                setModified(true);
            }
        } else if (item->objectType()==NCReportItem::Field) {
            if (mainWindow()->fieldItemDialogWidgetFactory()) {
                dia->resize(dia->width(),0);    // shrink
                mainWindow()->fieldItemDialogWidgetFactory()->updateData(item);    // update all widget data
                mainWindow()->fieldItemDialogWidgetFactory()->afterUpdateData(item);
            }
        }
    }

    delete dia;
}

void NCRDDocument::copy()
{
    NCRDSectionScene *scene = currentScene();
    if ( !scene )
        return;
    if ( scene->selectedItems().count() == 0 )
        return;

    QString copy;
    NCRDXMLCopyWriter writer( scene, &copy );
    writer.write(0);
    m_mainWindow->setCopyXML( copy );
    qDebug( "Clipboard: %s",qPrintable(copy) );
}

void NCRDDocument::paste()
{
    if ( !currentScene() )
        return;
    if ( m_mainWindow->copyXML().isEmpty() )
        return;

    NCRDXMLCopyReader reader( currentScene() );
    reader.setReportDef( reportDef() );
    if ( !reader.read( 0, m_mainWindow->copyXML() ) ) {
        qDebug( "Error: %s",qPrintable(reader.errorString()) );
    }
}

void NCRDDocument::cut()
{
    if ( !currentScene() )
        return;

    copy();
    currentScene()->removeItemCommand( 0, true );
}

void NCRDDocument::lock_unlock( bool setlock )
{
    for ( int i=0; i<m_sections.count(); ++i ) {
        NCRDDesignSection *dsect = m_sections.at(i);
        NCRDSectionScene *scene = dsect->sectionScene();

        QList<QGraphicsItem*> list = scene->selectedItems();
        for (int j = 0; j < list.size(); ++j) {
            NCReportItem *item = (NCReportItem*)list.at(j);
            item->setLocked(setlock);
            item->update();
        }
    }
}

void NCRDDocument::setZoneValue( int value )
{
    if ( !currentScene() )
        return;

    QList<QGraphicsItem*> list = currentScene()->selectedItems();
    for (int i = 0; i < list.size(); ++i) {
        NCReportItem *item = (NCReportItem*)list.at(i);
        item->setZoneID(value);
    }

    setModified(true);
}

void NCRDDocument::scrollContentsBy(int dx, int dy)
{
    QScrollArea::scrollContentsBy( dx, dy );
}

void NCRDDocument::resizeEvent(QResizeEvent *event)
{
    m_rulertop->resize( qMax(width(),m_sectionContainer->width()), RULERWEIGHT );
    QScrollArea::resizeEvent(event);
}

void NCRDDocument::focusInEvent(QFocusEvent *event)
{
    QScrollArea::focusInEvent(event);
    mainWindow()->dataSourceTree()->setErrorHandler(m_rdef->error());
    updateDataSourceTree(mainWindow()->dataSourceTree(),true,true,false,false);
}

void NCRDDocument::adjustContent()
{
    m_sectionContainer->adjustSize();
}

void NCRDDocument::initPropertyEditor()
{
    if ( m_mainWindow->isPropEditor() ) {
        m_mainWindow->propertyEditor()->setDocument( this );
        m_mainWindow->propertyEditor()->setSection( m_currentDesignSection );
        m_mainWindow->propertyEditor()->setItem( 0 );
    }
}

void NCRDDocument::slotSectionClickFocus()
{
    // Ctrl key pressed?
    if ( QApplication::keyboardModifiers() == Qt::ControlModifier )
        return;

    NCRDDesignArea *obj = qobject_cast<NCRDDesignArea*>(sender());
    if (obj) {
        for ( int i=0; i<m_sections.count(); ++i ) {
            NCRDDesignSection *section = m_sections.at(i);

            if ( section->designArea() != obj ) // not the clicked (focus scene)
                section->sectionScene()->clearSelection();
        }
    }
}

void NCRDDocument::slotSectionFirstItemSelected(NCReportItem* item )
{
    if ( !item	) {
        m_firstSelectedItem = 0;
        return;
    }

    // 1st item was selected in a section scene
    int scount=0;
    for ( int i=0; i<m_sections.count(); ++i ) {
        NCRDDesignSection *section = m_sections.at(i);
        scount += section->sectionScene()->selectedItems().count();
    }

    if ( scount == 1 ) {
        qDebug("NCRDDocument::slotSectionFirstItemSelected() ... scount=1");
        m_firstSelectedItem = item;
    }

}

NCReportItem * NCRDDocument::firstSelectedItem()
{
    return m_firstSelectedItem;
}

void NCRDDocument::setSubWindow(QMdiSubWindow *sw )
{
    m_subWindow = sw;
}

void NCRDDocument::setDefaultFont(const QFont & font )
{
    reportDef()->option().setDefaultFont(font);
}

void NCRDDocument::updateManagers()
{
    if ( currentScene() ) {
        currentScene()->slotSelectionChanged();
    }
}

void NCRDDocument::keyboardAction(QKeyEvent *e)
{
    // sends keypress actions to all sections
    for ( int i=0; i<m_sections.count(); ++i ) {
        NCRDDesignSection *dsect = m_sections.at(i);
        dsect->sectionScene()->keyboardAction( e );
    }
}

void NCRDDocument::zoom(ZoomMode zm )
{
    //qreal zScale = 1.0f;
    bool modif = isModified();
    for ( int i=0; i<m_sections.count(); ++i ) {
        NCRDDesignSection *dsect = m_sections.at(i);
        switch (zm) {
            case ZoomIn: dsect->designArea()->zoomIn(); break;
            case ZoomOut: dsect->designArea()->zoomOut(); break;
            case ZoomOne: dsect->designArea()->zoomOne(); break;
            case ZoomFull: dsect->designArea()->zoomFull(); break;
            case ZoomFit: dsect->designArea()->zoomFit(); break;
        }

        m_zoomScale = dsect->designArea()->zoomScale();
        dsect->leftRuler()->setZoomScale( m_zoomScale );
        dsect->setHeightMM( dsect->sectionScene()->metricHeight() );
        dsect->setWidthMM( purePageWidth() );
        dsect->designArea()->ensureVisible( 0.0, 0.0, 0.0, 0.0, 0, 0 );

    }
    adjustContent();
    m_rulertop->setZoomScale( m_zoomScale );
    m_rulertop->resize( qMax(width(),m_sectionContainer->width()), RULERWEIGHT );
    if ( !modif )
        setModified( false );
}

qreal NCRDDocument::zoomScale() const
{
    return m_zoomScale;
}

void NCRDDocument::slotHorizontalScroll(int value )
{
    m_rulertop->setScaleOffset( -value, true );
}

void NCRDDocument::actionManagerChanged( int property )
{
    for ( int i=0; i<m_sections.count(); ++i ) {
        NCRDDesignSection *dsect = m_sections.at(i);
        dsect->sectionScene()->actionManagerChanged(property);
    }
}

void NCRDDocument::sectionPropDialog(NCReportSection *section)
{
    NCRDDesignSection *dsect = m_currentDesignSection;
    if ( section ) {
        dsect = designSection(section);
    } else
        dsect = m_currentDesignSection;

    if (!dsect)
        return;

    NCRDSectionDialog dialog( this, dsect, this );
    switch (dialog.exec()) {
        case QDialog::Accepted: dialog.applyChanges(); break;
        case QDialog::Rejected: dialog.discardChanges(); break;
    }
}

NCRDDesignSection * NCRDDocument::currentDesignSection()
{
    return m_currentDesignSection;
}

void NCRDDocument::loadDataSourceColumnNames( QStringList& keywords, NCReportDataSource* ds ) const
{
    if (ds) {
        keywords << ds->keywordList();
    } else {
        QHash<QString, NCReportDataSource*>::const_iterator i = m_rdef->dataSources().constBegin();
        while (i != m_rdef->dataSources().constEnd()) {
            loadDataSourceColumnNames( keywords, i.value() );	// recursively call
            ++i;
        }
    }
}

QStringList NCRDDocument::dataSourceColumnNames( const QString& dataSourceID )
{
    if (m_rdef->dataSources().contains(dataSourceID))
        return m_rdef->dataSources().value(dataSourceID)->keywordList();
    else
        return QStringList();
}

QStringList NCRDDocument::allKeywords( NCReportItem::SourceType type )
{
    QStringList keywords;

    switch ((NCReportItem::SourceType)type) {
    case NCReportItem::DataSource:
        loadDataSourceColumnNames( keywords, 0 );
        break;
    case NCReportItem::Parameter:
        keywords = mainWindow()->parameterIDList();
        break;
    case NCReportItem::Variable:
        keywords = reportDef()->variableIdList();
        break;
    case NCReportItem::SystemVariable:
        keywords = NCReportEvaluator::availableSystemVariables();
        break;
    case NCReportItem::ScriptExpression:
    case NCReportItem::Template:
    {
        loadDataSourceColumnNames( keywords, 0 );
        NCReportEvaluator::tokenizeKeyWordList(keywords, NCReportItem::DataSource );
        keywords << reportDef()->parameterIdList();
        NCReportEvaluator::tokenizeKeyWordList(keywords, NCReportItem::Parameter );
        keywords << reportDef()->variableIdList();
        keywords << NCReportEvaluator::availableSystemVariables();
        NCReportEvaluator::tokenizeKeyWordList(keywords, NCReportItem::Variable );
        break;
    }
    default:
        break;
    }

    return keywords;
}

void NCRDDocument::updateLanguageMenu( QMenu* menu, QActionGroup* actionGroup, QWidget* parent )
{
    NCRDLanguage lm;
    lm.setLanguages(reportDef()->option().languages());
    lm.setCurrentLanguage( reportDef()->option().currentLanguage() );
    lm.buildLanguageMenu(menu,actionGroup,parent);
}

void NCRDDocument::setCurrentLanguage(const QString &langcode)
{
    reportDef()->option().setCurrentLanguage(langcode);
    updateLanguage();
}

void NCRDDocument::updateLanguage()
{
    for ( int i=0; i<m_sections.count(); ++i ) {
        NCRDDesignSection *dsect = m_sections.at(i);
        NCRDSectionScene *scene = dsect->sectionScene();
        QString tmp;
        QList<QGraphicsItem*> list = scene->items();
        for (int j = 0; j < list.size(); ++j) {
            NCReportItem *item = (NCReportItem*)list.at(j);
            if ( item->objectType() == NCReportItem::Label || item->objectType() == NCReportItem::Field ) {
                item->update();
                //NCReportLabelData *ld = (NCReportLabelData*)item->data();
                //tmp = ld->text;
                //ld->translations[reportDef()->option().currentLanguage];
            }
        }
    }
}

void NCRDDocument::updateDataSourceTree( NCRDDataSourceTree* tree, bool updateDS, bool updateVar, bool updatePar, bool forceUpdate)
{
    if (!forceUpdate && tree->currentDocument() == this)
        return;     // avoid unnecessary repeated update

    if (!tree->isVisible())
        return;

    QApplication::setOverrideCursor(Qt::WaitCursor);

    if (updateDS)
        tree->updateDataSources(m_rdef);
    if (updateVar)
        tree->updateVariables(m_rdef);
    if (updatePar)
        tree->updateParameters(m_rdef);

    tree->setCurrentDocument(this);

    QApplication::restoreOverrideCursor();
}

QDialog* NCRDDocument::createGraphItemDialog(NCReportGraphItem *item)
{
    QDialog *dialog = 0;
    if (mainWindow()->graphItemDialogFactory()) {
        dialog = mainWindow()->graphItemDialogFactory()->createDialog(item, this);
        mainWindow()->graphItemDialogFactory()->updateWidget(dialog,item);
    } else {
        dialog = new NCRDGraphPropDialog( item, this );
    }
    return dialog;
}

QString NCRDDocument::getViewPath()
{
	return m_ViewPath;
}
void NCRDDocument::setViewPath(const QString & viewPath)
{
	m_ViewPath = viewPath;
}
	
QString NCRDDocument::currentLanguage()
{
    return reportDef()->option().currentLanguage();
}


