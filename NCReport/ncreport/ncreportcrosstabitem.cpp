/****************************************************************************
*
*  Copyright (C) 2002-2011 Helta Kft. / NociSoft Software Solutions
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
#include "ncreportcrosstabitem.h"
#include "ncreportoutput.h"
#include "ncreportdatasource.h"
#include "ncreportxmlreader.h"
#include "ncreportxmlwriter.h"
#include "ncreportdef.h"
#include "ncreportdirector.h"
#include "ncreportitemmodeldatasource.h"
#include "ncreportpivottablemodel.h"

#include <QPainter>

NCReportCrossTabItem::NCReportCrossTabItem( NCReportDef* rdef, QGraphicsItem * parent ) : NCReportItem(parent)
{
    d = new NCReportCrossTabItemData;
    m_d = (NCReportCrossTabItemData*)d;
    d->objectType = CrossTab;
    d->tagname = "crosstab";
    d->reportDef = rdef;

    for ( int celltype =0; celltype < NCREPORTCROSSTABITEMCELL_COUNT; ++celltype )
        m_cells[celltype].setType((NCReportCrossTabItemCell::CellType)celltype);

    for ( int region=0; region < NCREPORTCROSSTABREGION_COUNT; ++region )
        m_regionSizes[region]=-1;
}

NCReportCrossTabItem::~NCReportCrossTabItem()
{
}

bool NCReportCrossTabItem::read(NCReportXMLReader *r )
{
    NCReportItem::read( r );

    if ( r->isAttrib("datasource") )
        setDataSourceByID(r->attribToString("datasource"));

    if ( r->isAttrib("tableSpacing") )
        m_d->tableSpacingMM = r->attribToReal("tableSpacing");
    if ( r->isAttrib("cellPadding") )
        m_d->cellPaddingMM = r->attribToReal("cellPadding");
    if ( r->isAttrib("cellSpacing") )
        m_d->cellSpacingMM = r->attribToReal("cellSpacing");

    if ( r->isAttrib("leftMargin") )
        m_d->leftMarginMM = r->attribToReal("leftMargin");
    if ( r->isAttrib("rightMargin") )
        m_d->rightMarginMM = r->attribToReal("rightMargin");

    if ( r->isAttrib("topMargin") )
        m_d->topMarginMM = r->attribToReal("topMargin");
    if ( r->isAttrib("bottomMargin") )
        m_d->bottomMarginMM = r->attribToReal("bottomMargin");

    if ( r->isAttrib("colWidth") ) {
        m_d->colWidthMM = r->attribToReal("colWidth");
        if (m_d->colWidthMM==0)
            m_d->colWidthMM=NCREPORTCROSSTAB_DEFAULT_COLWIDTH;	//auto width
    }
    if ( r->isAttrib("rowHeight") ) {
        m_d->rowHeightMM = r->attribToReal("rowHeight");
        if (m_d->rowHeightMM==0) {
            m_d->rowHeightMM=NCREPORTCROSSTAB_DEFAULT_ROWHEIGHT;	//auto height
        }
    }

    /*
    if ( r->isAttrib("autoWidth") )
        m_d->autoWidth = r->attribToBool("autoWidth");
    if ( r->isAttrib("autoHeight") )
        m_d->autoHeight = r->attribToBool("autoHeight");
    */
    if ( r->isAttrib("showColHeader") )
        m_d->showColHeader = r->attribToBool("showColHeader");
    if ( r->isAttrib("showRowHeader") )
        m_d->showRowHeader = r->attribToBool("showRowHeader");
    if ( r->isAttrib("showBottomSummary") )
        m_d->showBottomSummary = r->attribToBool("showBottomSummary");
    if ( r->isAttrib("showSideSummary") )
        m_d->showSideSummary = r->attribToBool("showSideSummary");
    if ( r->isAttrib("breakTable") )
        m_d->breakTable = r->attribToBool("breakTable");
    if ( r->isAttrib("columnTitleSource") ) {
        m_d->columnTitleSource = r->attribToString("columnTitleSource");
        //m_reservedDSColumns << m_d->columnTitleSource;
    }

    if ( r->isAttrib("hiddenColumns") )
        setHiddenDataSourceColumns(r->attribToString("hiddenColumns"));

    if ( r->isAttrib("isPivot") )
        m_d->isPivotTable = r->attribToBool("isPivot");
    if ( r->isAttrib("ptSort") )
        m_d->ptAutoSort = r->attribToBool("ptSort");
    if ( r->isAttrib("ptAgg") )
        m_d->ptAggregate = r->attribToInt("ptAgg");
    if ( r->isAttrib("ptRow") )
        m_d->ptRowSource = r->attribToString("ptRow");
    if ( r->isAttrib("ptColumn") )
        m_d->ptColumnSource = r->attribToString("ptColumn");
    if ( r->isAttrib("ptValue") )
        m_d->ptValueSource = r->attribToString("ptValue");

    readRegionSizes( r );

    // read cell definitions
    while (!r->atEnd()) {
        r->readNext();
        if (r->isEndElement())
            break;

        if (r->isStartElement()) {
            if (r->name() == "cell")
                readCell(r);
            //if (r->name() == "region")
            //	readRegions();
            else
                r->readUnknownElement();
                //qDebug( "Unknown element: %s", qPrintable(r->name().toString()) ) ;
        }
    }
    return true;
}



bool NCReportCrossTabItem::readCell(NCReportXMLReader *r)
{
    Q_ASSERT(r->isStartElement() && r->name() == "cell");

    NCReportCrossTabItemCell::CellType celltype = (NCReportCrossTabItemCell::CellType)r->attribToInt("ct");
    m_cells[celltype].setType(celltype);
    return m_cells[celltype].read( r );
}

bool NCReportCrossTabItem::readRegionSizes(NCReportXMLReader *r)
{
    if ( r->isAttrib("headerColumnWidth") )
        m_regionSizes[HeaderColumn] = r->attribToReal("headerColumnWidth");

    if ( r->isAttrib("dataColumnWidth") )
        m_regionSizes[DataColumn] = r->attribToReal("dataColumnWidth");

    if ( r->isAttrib("totalColumnWidth") )
        m_regionSizes[TotalColumn] = r->attribToReal("totalColumnWidth");

    if ( r->isAttrib("headerRowHeight") )
        m_regionSizes[HeaderRow] = r->attribToReal("headerRowHeight");

    if ( r->isAttrib("dataRowHeight") )
        m_regionSizes[DataRow] = r->attribToReal("dataRowHeight");

    if ( r->isAttrib("totalRowHeight") )
        m_regionSizes[TotalRow] = r->attribToReal("totalRowHeight");

    //HeaderColumn=0, DataColumn, TotalColumn, HeaderRow, DataRow, TotalRow
    return true;
}


bool NCReportCrossTabItem::write(NCReportXMLWriter *w)
{
    w->writeStartElement( d->tagname );
    NCReportItem::write(w);

    if (dataSource())
        w->writeAttribute("datasource",dataSource()->id());
    w->writeReal( "tableSpacing", m_d->tableSpacingMM );
    w->writeReal( "cellPadding", m_d->cellPaddingMM );
    w->writeReal( "cellSpacing", m_d->cellSpacingMM );
    w->writeReal( "leftMargin", m_d->leftMarginMM );
    w->writeReal( "rightMargin", m_d->rightMarginMM );
    w->writeReal( "topMargin", m_d->topMarginMM );
    w->writeReal( "bottomMargin", m_d->bottomMarginMM );
    w->writeReal( "colWidth", m_d->colWidthMM );
    w->writeReal( "rowHeight", m_d->rowHeightMM );
    //w->writeBool( "autoWidth", m_d->autoWidth );
    //w->writeBool( "autoHeight", m_d->autoHeight );
    w->writeBool( "showColHeader", m_d->showColHeader );
    w->writeBool( "showRowHeader", m_d->showRowHeader );
    w->writeBool( "showBottomSummary", m_d->showBottomSummary );
    w->writeBool( "showSideSummary", m_d->showSideSummary );
    w->writeBool( "breakTable", m_d->breakTable );

    if (!m_d->columnTitleSource.isEmpty())
        w->writeAttribute("columnTitleSource",m_d->columnTitleSource);
    if (!m_reservedDSColumns.isEmpty())
        w->writeAttribute("hiddenColumns",hiddenDataSourceColumns());

    w->writeBool("isPivot",m_d->isPivotTable);
    w->writeInt("ptAgg",m_d->ptAggregate);
    if (m_d->ptAutoSort)
        w->writeBool("ptSort",true);
    if (!m_d->ptRowSource.isEmpty())
        w->writeAttribute("ptRow",m_d->ptRowSource);
    if (!m_d->ptColumnSource.isEmpty())
        w->writeAttribute("ptColumn",m_d->ptColumnSource);
    if (!m_d->ptValueSource.isEmpty())
        w->writeAttribute("ptValue",m_d->ptValueSource);

    writeRegionSizes(w);

    for ( int celltype =0; celltype < NCREPORTCROSSTABITEMCELL_COUNT; ++celltype )
        m_cells[celltype].write( w );

    w->writeEndElement();

    return true;
}

bool NCReportCrossTabItem::writeRegionSizes(NCReportXMLWriter *w)
{
    if ( m_regionSizes[HeaderColumn] > 0 )
        w->writeReal("headerColumnWidth",m_regionSizes[HeaderColumn]);

    if (m_regionSizes[DataColumn]>0)
        w->writeReal("dataColumnWidth",m_regionSizes[DataColumn]);

    if (m_regionSizes[TotalColumn]>0)
        w->writeReal("totalColumnWidth",m_regionSizes[TotalColumn]);

    if (m_regionSizes[HeaderRow]>0)
        w->writeReal("headerRowHeight",m_regionSizes[HeaderRow]);

    if(m_regionSizes[DataRow]>0)
        w->writeReal("dataRowHeight",m_regionSizes[DataRow]);

    if(m_regionSizes[TotalRow]>0)
        w->writeReal("totalRowHeight",m_regionSizes[TotalRow]);

    return true;
}

void NCReportCrossTabItem::setDefaultForEditor()
{
    resize( QSizeF(220.0,100.0) );
}

void NCReportCrossTabItem::paint(NCReportOutput *o, const QPointF & mPos )
{
    switch ( o->output() ) {
        case NCReportOutput::Printer:
        case NCReportOutput::Pdf:
        case NCReportOutput::Svg:
        case NCReportOutput::Image:
        case NCReportOutput::Preview:
        case NCReportOutput::QtPreview:
        case NCReportOutput::Null:
            break;
        default:
            return;
    }

    if (!dataSource())
        return;

    if ( dataSourceRowCount() == 0 )
        return;

    NCReportDataSource* originalDataSource = dataSource();

    if (createPivotTable(originalDataSource))
        setDataSource(m_d->ptDataSource);


    QRectF itemRect( o->scale().mmToPoint( mPos + metricPos() ), o->scale().mmToSize( metricSize() ) );
    QRectF cellRect( itemRect.topLeft(), o->scale().mmToSize( QSizeF(m_d->colWidthMM, m_d->rowHeightMM) ) );
    m_tableSpacing = o->scale().mmToRealPixel(m_d->tableSpacingMM);
    m_cellSpacing  = o->scale().mmToRealPixel(m_d->cellSpacingMM);
    m_tableRect = itemRect;
    m_tableRectSet = true;   // calculate table rect. for height but only once
    m_calculatedTableHeight = 0;

    //--------------------
    // Initialize totals
    //--------------------
    totals_bottom.clear();
    totals_side.clear();

    setFlagNoSpaceRight(false);
    setFlagNoSpaceBottom(false);
    setFlagLastRecord(false);
    setFlagRowFinished(false);

    m_printedDataCellIndex.reset();

    // init summary
    dataSource()->first();
    totals_bottom << 0.0;
    while (dataSource()->next())
        totals_bottom << 0.0;

    // Init summary, and calculate table size
    dataSource()->first();
    for ( int column_id=0; column_id < dataSourceColumnCount(); ++column_id ) {
        totals_side << 0.0;
        if (!isHiddenDataColumn(column_id))
            m_calculatedTableHeight += cellRect.height() + m_cellSpacing;
    }

    m_calculatedTableHeight += ((int)m_d->showColHeader+(int)m_d->showBottomSummary)*(cellRect.height() + m_cellSpacing);
    m_tableRect.setHeight(m_calculatedTableHeight+m_tableSpacing);

    NCReportCrossTabIndex tableIndex;
    while (!isFlagRowFinished()) {

        if ( m_d->breakTable ) {
            // nothing to do. Page break happens within table

        } else {
            // Not enough space for whole table and table is not allowed to break
            qreal spBottom = spaceToBottom(o, m_tableRect);

            if ( spBottom < m_tableRect.height() )  {
                director()->pageBreak(false,false);
                cellRect = QRectF( o->scale().mmToPoint(director()->paintPosMM()), o->scale().mmToSize( QSizeF(m_d->colWidthMM, m_d->rowHeightMM) ) );
                m_tableRect.moveTo( itemRect.x(), cellRect.top());
            } else {
                // nothing to do
            }

        }

        paintTableHeaderRow(o, cellRect, itemRect, tableIndex);
        paintTableDataRow(o, cellRect, itemRect, tableIndex);
        paintTableFooterRow(o, cellRect, itemRect, tableIndex);
        tableIndex = m_printedDataCellIndex;

        if (!isFlagRowFinished()) {
            cellRect.translate( 0, m_tableSpacing ); // table to be continued after Y spacing
            m_tableRect.moveTo( itemRect.x(), cellRect.top());
        }

        //TABLE RECT TEST
        //o->painter()->setBrush( QBrush(Qt::NoBrush));
        //o->painter()->drawRect(m_tableRect);
    }
    director()->setPaintPosY( o->scale().pixelToMM(m_tableRect.bottom() ) );

    o->renderItem( this, cellRect );

    if (deletePivotTable())
        setDataSource(originalDataSource);

}

/*!
Paints table's header: Column headers row
*/
void NCReportCrossTabItem::paintTableHeaderRow(NCReportOutput *o, QRectF &cellRect, const QRectF& itemRect, const NCReportCrossTabIndex& index)
{
    if (!m_d->showColHeader)
        return;

    if (m_d->showRowHeader) {
        if (paintCell(o,NCReportCrossTabItemCell::CornerHeader, QVariant(), cellRect, NCReportCrossTabIndex(-1,-1) )) {
            nextCell( o, cellRect, itemRect, Right );
        } else
            return;	// not enough space
    }

    int record_position;
    prepareDataSource(record_position, index);

    if ( !isFlagLastRecord() && dataSource()->isValid()) {

        while (true) {
            QString title;
            if (m_d->columnTitleSource.isEmpty())
                title = dataSource()->rowTitle(record_position);
            else
                title = dataSource()->value(m_d->columnTitleSource).toString();

            if ( paintCell(o, NCReportCrossTabItemCell::ColumnHeader, title, cellRect, NCReportCrossTabIndex(-1, record_position) )) {
                if ( m_tableRectSet )
                    m_tableRect.setTop(cellRect.top());

                nextCell( o, cellRect, itemRect, Right );
                if (dataSource()->next()) {
                    record_position++;
                } else {
                    setFlagLastRecord(true);
                    break;
                }
            } else {
                // not enough space
                break;
            }

        }
    } else {
        setFlagLastRecord(true);
    }

    if ( isFlagLastRecord() ) {  // show side header
        if (m_d->showSideSummary) {
            if (paintCell(o, NCReportCrossTabItemCell::SideSummaryHeader, QVariant(), cellRect, NCReportCrossTabIndex(-1, dataSourceRowCount()) ))
                setFlagRowFinished(true);
        } else {
            setFlagRowFinished(true);
        }
    }

    // next row
    nextCell( o, cellRect, itemRect, NewRow );
}

/*!
Paints table's data row
*/
void NCReportCrossTabItem::paintTableDataRow(NCReportOutput *o, QRectF &cellRect, const QRectF& itemRect, const NCReportCrossTabIndex& index)
{
    dataSource()->first();	// jump to datasource 1st record to set data source valid
    int columnCount = dataSourceColumnCount();

    for ( int column_id=0; column_id < columnCount ; ++column_id ) {

        //--------------------------------------------------
        // Vertical iteration: through Data source columns
        //--------------------------------------------------
        int record_position;
        prepareDataSource(record_position, index);

        // Avoid to render data source column which is reserved for meta data.
        if (isHiddenDataColumn(column_id))
            continue;

        if (m_d->showRowHeader) {
            if (paintCell(o, NCReportCrossTabItemCell::RowHeader, dataSource()->columnTitle(column_id), cellRect, NCReportCrossTabIndex(column_id, record_position) ))
                nextCell( o, cellRect, itemRect, Right );
            else
                return;
        }

        if (!isFlagLastRecord() && dataSource()->isValid()) {
            while (true) {
                // show header
                QVariant value = dataSource()->value(column_id);
                if ( record_position < totals_bottom.size() )
                    totals_bottom[record_position] += value.toDouble(); // vertical total
                if ( column_id < totals_side.size() )
                    totals_side[column_id] += value.toDouble();	// horizontal total
                if (paintCell(o, NCReportCrossTabItemCell::Data, value, cellRect, NCReportCrossTabIndex(column_id, record_position) )) {
                    nextCell( o, cellRect, itemRect, Right );

                    if (dataSource()->next()) {
                        record_position++;
                    } else {
                        setFlagLastRecord(true);
                        break;
                    }
                } else {
                    // not enough space
                    if ( record_position < totals_bottom.size() )
                        totals_bottom[record_position] -= value.toDouble(); // vertical total
                    if ( column_id < totals_side.size() )
                        totals_side[column_id] -= value.toDouble();	// horizontal total
                    break;
                }

            }
        } else {
            setFlagLastRecord(true);
        }

        if ( isFlagLastRecord() ) { // show side header
            if (m_d->showSideSummary) {
                if ( paintCell(o, NCReportCrossTabItemCell::SideSummaryData, totals_side.at(column_id), cellRect, NCReportCrossTabIndex(column_id,dataSourceRowCount()) ) )
                    setFlagRowFinished(true);
            } else {
                setFlagRowFinished(true);
            }
        }
        // next row
        nextCell( o, cellRect, itemRect, NewRow );
    }
}

/*!
Paints table's footer row: bottom summaries
*/
void NCReportCrossTabItem::paintTableFooterRow(NCReportOutput *o, QRectF &cellRect, const QRectF& itemRect, const NCReportCrossTabIndex& index )
{
    if (!m_d->showBottomSummary)
        return;

    int record_position;
    prepareDataSource(record_position, index);

    if (m_d->showRowHeader) {
        if (paintCell(o,NCReportCrossTabItemCell::BottomSummaryHeader, QVariant(), cellRect, NCReportCrossTabIndex(dataSourceColumnCount(),-1) ))
            nextCell( o, cellRect, itemRect, Right );
        else
            return;
    }
    if (!isFlagLastRecord() && dataSource()->isValid()) {
        while (true) {
            //m_cells[NCReportCrossTabItemCell::ColumnHeader].setValue( dataSource()->value( m_d->columnTitleSource ) );
            double totalBottom = 0;
            if (record_position < totals_bottom.size() )
                totalBottom = totals_bottom.at(record_position);

            if (paintCell(o, NCReportCrossTabItemCell::BottomSummaryData, totalBottom, cellRect, NCReportCrossTabIndex(dataSourceColumnCount(), record_position) )) {
                if ( m_tableRectSet ) {
                    m_tableRect.setBottom(cellRect.bottom());
                    m_tableRectSet = false;
                }

                nextCell( o, cellRect, itemRect, Right );
                if (dataSource()->next()) {
                    record_position++;
                } else {
                    setFlagLastRecord(true);
                    break;
                }
            } else {
                // not enough space
                break;
            }
        }
    } else {
        setFlagLastRecord(true);
    }

    if ( isFlagLastRecord() ) {
        if (m_d->showSideSummary ) {
            double total=0;
            for (int i = 0; i < totals_side.size(); ++i)
                total += totals_side.at(i);
            if ( paintCell(o, NCReportCrossTabItemCell::CrossSummaryData, total, cellRect, NCReportCrossTabIndex(dataSourceColumnCount(), dataSourceRowCount()) ))
                setFlagRowFinished(true);
        } else {
            setFlagRowFinished(true);
        }
    }

    // next row
    nextCell( o, cellRect, itemRect, NewRow );
}



bool NCReportCrossTabItem::paintCell( NCReportOutput* o, const NCReportCrossTabItemCell::CellType type, const QVariant& value, QRectF& cellRect, const NCReportCrossTabIndex& index)
{
    switch (type) {
    case NCReportCrossTabItemCell::CornerHeader:
        setCellSize(o, HeaderRow, HeaderColumn, cellRect );
        break;
    case NCReportCrossTabItemCell::ColumnHeader:
        setCellSize(o, HeaderRow, DataColumn, cellRect );
        break;
    case NCReportCrossTabItemCell::SideSummaryHeader:
        setCellSize(o, HeaderRow, TotalColumn, cellRect );
        break;
    case NCReportCrossTabItemCell::RowHeader:
        setCellSize(o, DataRow, HeaderColumn, cellRect );
        break;
    case NCReportCrossTabItemCell::Data:
        setCellSize(o, DataRow, DataColumn, cellRect );
        break;
    case NCReportCrossTabItemCell::SideSummaryData:
        setCellSize(o, DataRow, TotalColumn, cellRect );
        break;
    case NCReportCrossTabItemCell::BottomSummaryHeader:
        setCellSize(o, TotalRow, HeaderColumn, cellRect );
        break;
    case NCReportCrossTabItemCell::BottomSummaryData:
        setCellSize(o, TotalRow, DataColumn, cellRect );
        break;
    case NCReportCrossTabItemCell::CrossSummaryData:
        setCellSize(o, TotalRow, TotalColumn, cellRect );
        break;
    }

    m_cells[type].setValue( value );

    qreal spaceRight = spaceToRight( o, cellRect );
    qreal spaceBottom = spaceToBottom( o, cellRect );

    if ( spaceRight < cellRect.width() ) {

        //qDebug("Not enough space to right.");
        setFlagNoSpaceRight(true);
        return false;
        //cellRect.moveTo( itemRect.bottomLeft().x(),itemRect.bottomLeft().y() + itemRect.height() +5.0 );
    } else if ( spaceBottom < cellRect.height() ) {

        //qDebug("Not enough space to bottom. Page break is needed");

        if ( m_d->breakTable ) {
            director()->pageBreak(false,false);
            cellRect.moveTo( o->scale().mmToPoint(director()->paintPosMM()) );
        } else {
            setFlagNoSpaceBottom(true);
            return false;
        }
    }

    m_cells[type].paint(o, cellRect, o->scale().mmToRealPixel(m_d->cellPaddingMM) );

    if (index.row()>=0)
        m_printedDataCellIndex.setRow(index.row());
    if (index.column()>=0)
        m_printedDataCellIndex.setColumn(index.column());

    setFlagNoSpaceBottom(false);
    setFlagNoSpaceRight(false);

    return true;
}

/*!
  Sets the current cell's size depending its position.
  */
void NCReportCrossTabItem::setCellSize( NCReportOutput* o, const TableRegion horizontalRegion, const TableRegion verticalRegion, QRectF& cellRect )
{
    if ( m_regionSizes[horizontalRegion] > 0 )
        cellRect.setHeight( o->scale().mmToRealPixel(m_regionSizes[horizontalRegion]) );
    else
        cellRect.setHeight(o->scale().mmToRealPixel(m_d->rowHeightMM));

    if ( m_regionSizes[verticalRegion] > 0 )
        cellRect.setWidth(o->scale().mmToRealPixel(m_regionSizes[verticalRegion]) );
    else
        cellRect.setWidth(o->scale().mmToRealPixel(m_d->colWidthMM));
}

void NCReportCrossTabItem::nextCell( NCReportOutput* , QRectF& cellRect, const QRectF& itemRect, const CellChangeDirection ccd )
{
    switch (ccd) {
    case None: break;
    case Down:
        {
            cellRect.translate( 0, cellRect.height() + m_cellSpacing  );
            //m_calculatedTableSize.rheight() += cellRect.height() + m_cellSpacing;
            break;
        }
    case Right:
        {
            cellRect.translate( cellRect.width() + m_cellSpacing, 0 );
            //m_calculatedTableSize.rwidth() += cellRect.width() + m_cellSpacing;
            break;
        }
    case NewColumn:
        {
            cellRect.translate( cellRect.width() + m_cellSpacing, 0 );
            cellRect.moveTop(itemRect.y());
            //m_calculatedTableSize.rwidth() += cellRect.width() + m_cellSpacing;
            break;
        }
    case NewRow:
        {
            cellRect.translate( 0, cellRect.height() + m_cellSpacing  );
            cellRect.moveLeft(itemRect.x());
            //m_calculatedTableSize.rheight() += cellRect.height() + m_cellSpacing;
            break;
        }
    }
}

bool NCReportCrossTabItem::isHiddenDataColumn(int column) const
{
    QString columnName = dataSource()->columnName(column);
    return ( m_reservedDSColumns.contains( columnName ) || columnName == m_d->columnTitleSource );
}

void NCReportCrossTabItem::prepareDataSource(int &recordPosition, const NCReportCrossTabIndex &index)
{
    if ( index.column() <0 ) {
        dataSource()->first();	// jump to 1st record of datasource
        recordPosition=0;
        setFlagLastRecord(false);
    } else {
        setFlagLastRecord(!dataSource()->seek(index.column()+1));
        recordPosition=index.column()+1;
    }
}


void NCReportCrossTabItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    const int cellWidth = 60;
    const int cellHeight = 20;

    QRectF rect( QPointF(0, 0), d->currentSize );
    //QRectF rect( boundingRect() );
    QPen pen(Qt::black);
    pen.setStyle(Qt::DotLine);
    painter->setPen( pen );
    painter->drawRect( rect );

    for (int x=cellWidth; x<rect.width(); x+=cellWidth )
        painter->drawLine( QPointF(x,0), QPointF(x,rect.height()) );
    for (int y=cellHeight; y<rect.height(); y+=cellHeight )
        painter->drawLine( QPointF(0,y), QPointF(rect.width(),y) );

    rect.setLeft( rect.left()+1 );
    painter->drawText( rect, Qt::AlignCenter, QObject::tr("CROSS TABLE"));

    paintSelection( painter );
}

void NCReportCrossTabItem::dummyPaint(NCReportOutput *output, const QPointF &point)
{
    paint(output,point);
}

void NCReportCrossTabItem::adjustSize(NCReportOutput*)
{
    /*
    if ( m_d->autoHeight )
        adjustHeight();

    if ( m_d->autoWidth )
        adjustWidth();

    if ( d->adjusted )
        return;
        */
}

void NCReportCrossTabItem::adjustHeight(NCReportOutput*)
{
    // calculate the real pixel height of cells. Selects the maximum value.
    /*
    QList<qreal> tableHeader;
    QList<qreal> data;
    QList<qreal> bottomSummary;

    for ( int celltype =0; celltype < NCREPORTCROSSTABITEMCELL_COUNT; ++celltype ) {
        switch ( (NCReportCrossTabItemCell::CellType)celltype ) {
        case NCReportCrossTabItemCell::CornerHeader:
        case NCReportCrossTabItemCell::ColumnHeader:
        case NCReportCrossTabItemCell::SideSummaryHeader:
            tableHeader << m_cells[celltype].sizeMM().height();
        }

        m_cells[celltype]
    }


    d->mRealSize = QSizeF( d->mSize.width(), o->scale().pixelToMM( outputRect(o).height()) );
    if ( o->output() != NCReportOutput::Null )
        d->adjusted = true;
    */
}


QRectF NCReportCrossTabItem::outputRect(NCReportOutput*) const
{
    /*
    QFontMetricsF fm( m_d->font, o->device() );

    int aflag = m_d->alignment;
    if ( ld->wordbreak )
        aflag |= Qt::TextWordWrap;
    QRectF rect(fm.boundingRect( QRectF( o->scale().mmToPoint( d->mPos ), o->scale().mmToSize( d->mSize )),
                                 aflag, printableText() ));
    rect.setHeight( rect.height() + o->scale().mmToPixel(LABEL_HEIGHT_TRIM_MARGIN_MM));		//TRIM
    */
    return QRectF();
}

void NCReportCrossTabItem::setDataSource(NCReportDataSource *ds)
{
    m_d->dataSource = ds;
}

NCReportDataSource * NCReportCrossTabItem::dataSource() const
{
    return m_d->dataSource;
}


bool NCReportCrossTabItem::writeCell(NCReportXMLWriter *)
{
    return true;
}

QRectF NCReportCrossTabItem::boundingRect() const
{
    return NCReportItem::boundingRect();
}

/*!
Draws a table scheme for representation of table setting in designer & property dialog
*/
void NCReportCrossTabItem::paintScheme(QPainter *)
{
    NCReportScale scale;	//screen resolution
}

void NCReportCrossTabItem::setDataSourceByID(const QString &id)
{
    setDataSource( d->reportDef->dataSource(id) );
}

qreal NCReportCrossTabItem::spaceToBottom( NCReportOutput* o, const QRectF& rect ) const
{
    qreal footerHeight = 0.0;
    if ( d->reportDef->pageFooter() )
        footerHeight = d->reportDef->pageFooter()->metricHeight();

    qreal mBottomY = d->reportDef->option().pageHeightMM() - d->reportDef->option().bottomMarginMM() - footerHeight;
    qreal bottomY = o->scale().mmToPixel(mBottomY);
    return bottomY - rect.topLeft().y();
}

qreal NCReportCrossTabItem::spaceToRight( NCReportOutput* o, const QRectF& rect ) const
{
    qreal pageWidthMM = d->reportDef->option().pageWidthMM() - d->reportDef->option().rightMarginMM();
    qreal pageWidth = o->scale().mmToPixel(pageWidthMM);
    return pageWidth - rect.topLeft().x();
}

int NCReportCrossTabItem::dataSourceRowCount() const
{
    if ( !dataSource() || !dataSource()->isOpened() )
        return -1;

    int numRows = dataSource()->rowCount();
    return numRows;
}

int NCReportCrossTabItem::dataSourceColumnCount() const
{
    if ( !dataSource() || !dataSource()->isOpened() )
        return -1;

    return dataSource()->columnCount();
}

bool NCReportCrossTabItem::createPivotTable( NCReportDataSource* sourceDS )
{
    if (!m_d->isPivotTable)
        return false;

    if (m_d->ptRowSource.isEmpty() || m_d->ptColumnSource.isEmpty() || m_d->ptValueSource.isEmpty() )
        return false;

    m_d->ptDataSource = new NCReportItemModelDataSource();
    m_d->ptModel = new NCReportPivotTableModel();
    m_d->ptModel->setAggregation(m_d->ptAggregate);

    if (m_d->ptAutoSort) {
        // We have to load sorted values, so it requires to load key data to maps first. Expensive.
        QMap<QString,bool> rows;
        QMap<QString,bool> columns;
        QMap<QString,double> values;
        sourceDS->first();
        do {
            rows.insert(sourceDS->value(m_d->ptRowSource).toString(),true);
            columns.insert(sourceDS->value(m_d->ptColumnSource).toString(),true);
            values.insert(sourceDS->value(m_d->ptRowSource).toString()+'/'+sourceDS->value(m_d->ptColumnSource).toString(), sourceDS->value(m_d->ptValueSource).toDouble());
        } while (sourceDS->next());

        QMap<QString,bool>::const_iterator row_iterator = rows.constBegin();
        while (row_iterator != rows.constEnd()) {

            QMap<QString,bool>::const_iterator column_iterator = columns.constBegin();
            while (column_iterator != columns.constEnd()) {
                QString mKey = row_iterator.key()+'/'+column_iterator.key();
                m_d->ptModel->addData(row_iterator.key(), column_iterator.key(), values.value(mKey) );

                ++column_iterator;
            }
            ++row_iterator;
        }

    } else {
        //CREATE TABLE
        sourceDS->first();
        do {
            m_d->ptModel->addData(sourceDS->value(m_d->ptRowSource), sourceDS->value(m_d->ptColumnSource),sourceDS->value(m_d->ptValueSource).toDouble() );
        } while (sourceDS->next());
    }

    m_d->ptDataSource->setItemModel(m_d->ptModel);
    m_d->ptDataSource->open(NCReportDataSource::OPEN_MODEL_QUERY);//为扩展DataView数据源而添加
	//m_d->ptDataSource->open();

    return true;
}

bool NCReportCrossTabItem::deletePivotTable()
{
    if (!m_d->isPivotTable)
        return false;

    delete m_d->ptModel;
    m_d->ptModel=0;
    delete m_d->ptDataSource;
    m_d->ptDataSource=0;
    return true;
}

/*NCReportCrossTabItemCell & NCReportCrossTabItem::cell(const NCReportCrossTabItemCell::CellType type)*/
NCReportCrossTabItemCell & NCReportCrossTabItem::cell( int cellType)
{
    return m_cells[cellType];
}

QString NCReportCrossTabItem::hiddenDataSourceColumns() const
{
    return m_reservedDSColumns.join(",");
}

void NCReportCrossTabItem::setHiddenDataSourceColumns(const QString &columnlist)
{
    m_reservedDSColumns.clear();
    m_reservedDSColumns << columnlist.split(",", QString::SkipEmptyParts);
}

qreal NCReportCrossTabItem::regionSize(int region) const
{
    return m_regionSizes[region];
}

void NCReportCrossTabItem::setRegionSize(qreal size, int region)
{
    m_regionSizes[region] = size;
}

void NCReportCrossTabItem::setCell(const NCReportCrossTabItemCell &cell, int cellType)
{
    m_cells[cellType]=cell;
}




