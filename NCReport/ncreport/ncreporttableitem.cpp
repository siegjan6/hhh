/****************************************************************************
*
*  Copyright (C) 2002-2013 Helta Kft. / NociSoft Software Solutions
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
#include "ncreporttableitem.h"
#include "ncreportoutput.h"
#include "ncreportdatasource.h"
#include "ncreportxmlreader.h"
#include "ncreportxmlwriter.h"
#include "ncreportdef.h"
#include "ncreportdirector.h"
#include "ncreportutils.h"

#include <QPainter>
#include <QColor>
#include <QTableView>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QTextLine>
#include <QTextLayout>

#define D_font_trimmer          0.8800
#define D_icon_margin_trimmer   1.1000
#define D_icon_size             16
#define D_indentation           10
#define D_headerBackgroundColor 0xe0e0e0
#define D_headerLineColor       Qt::darkGray
#define D_lineColor             Qt::gray

NCReportTableItem::NCReportTableItem( NCReportDef* rdef, QGraphicsItem * parent ) : NCReportItem(parent),
    m_rate(1.0),
    m_deviceRate(1.0),
    m_tableWidth(0),
    m_calculatedTableHeight(0),
    m_tableSpacing(0),
    m_cellSpacing(0)
{
    d = new NCReportTableItemData;
    m_d = (NCReportTableItemData*)d;
    d->objectType = Table;
    d->tagname = "table";
    d->reportDef = rdef;
}

NCReportTableItem::~NCReportTableItem()
{
}

bool NCReportTableItem::read(NCReportXMLReader *reader )
{
    NCReportItem::read( reader );

    if ( reader->isAttrib("datasource") )
        setDataSourceByID(reader->attribToString("datasource"));

    if ( reader->isAttrib("tableSpacing") )
        setTableSpacingMM(reader->attribToReal("tableSpacing"));
    if ( reader->isAttrib("cellPadding") )
        setCellPaddingMM(reader->attribToReal("cellPadding"));
    if ( reader->isAttrib("cellSpacing") )
        setCellSpacing(reader->attribToReal("cellSpacing"));

    if ( reader->isAttrib("leftMargin") )
        setLeftMarginMM(reader->attribToReal("leftMargin"));
    if ( reader->isAttrib("rightMargin") )
        setRightMarginMM(reader->attribToReal("rightMargin"));

    if ( reader->isAttrib("topMargin") )
        setTopMarginMM(reader->attribToReal("topMargin"));
    if ( reader->isAttrib("bottomMargin") )
        setBottomMarginMM(reader->attribToReal("bottomMargin"));

    if ( reader->isAttrib("model")) {
        m_d->modelID = reader->attribToString("model");
        m_d->model = reportDef()->itemModels().value( m_d->modelID );
    }

#ifndef NCREPORT_NO_TABLE_VIEW
    if (reader->isAttrib("tableView")) {
        m_d->viewID = reader->attribToString("tableView");
        m_d->view = reportDef()->tableViews().value( m_d->viewID );
    }
#endif
    if (reader->isAttrib("elidedMode"))
        m_d->elidedMode = reader->attribToBool("elidedMode");

    if (reader->isAttrib("showBorder"))
        m_d->showBorderAndBG = reader->attribToBool("showBorder");

    if (reader->isAttrib("showHeaderH"))
        m_d->horizontalHeader = reader->attribToBool("showHeaderH");

    if (reader->isAttrib("showHeaderV"))
        m_d->verticalHeader = reader->attribToBool("showHeaderV");

    if (reader->isAttrib("hiddenRows"))
        setHiddenRows(reader->attribToString("hiddenRows"));

    if (reader->isAttrib("hiddenCols"))
        setHiddenColumns(reader->attribToString("hiddenCols"));

    // read cell definitions
    while (!reader->atEnd()) {
        reader->readNext();
        if (reader->isEndElement())
            break;

        if (reader->isStartElement()) {
            reader->readElementText();
        }
    }
    return true;
}

bool NCReportTableItem::write(NCReportXMLWriter *writer)
{
    writer->writeStartElement( d->tagname );
    NCReportItem::write(writer);

    if (dataSource())
        writer->writeAttribute("datasource",dataSource()->id());

    writer->writeAttribute("model", modelID());
    writer->writeAttribute("tableView", tableViewID());
    writer->writeReal( "tableSpacing", tableSpacingMM() );
    //w->writeReal( "cellPadding", cellPaddingMM() );
    writer->writeReal( "cellSpacing", cellSpacing() );
    //w->writeReal( "leftMargin", leftMarginMM() );
    //w->writeReal( "rightMargin", rightMarginMM() );
    //w->writeReal( "topMargin", topMarginMM() );
    //w->writeReal( "bottomMargin", bottomMarginMM() );
    writer->writeBool("elidedMode", isElidedMode() );
    writer->writeBool("showBorder", showBorderAndBG() );
    writer->writeBool("showHeaderH", isHorizontalHeaderVisible());
    writer->writeBool("showHeaderV", isVerticalHeaderVisible());
    if (!m_d->hiddenRows.isEmpty())
        writer->writeAttribute("hiddenRows", hiddenRowsList());
    if (!m_d->hiddenCols.isEmpty())
        writer->writeAttribute("hiddenCols", hiddenColumnsList());

    writer->writeEndElement();

    return true;
}

void NCReportTableItem::setDefaultForEditor()
{
    resize( QSizeF(220.0,100.0) );
}

void NCReportTableItem::paint(NCReportOutput *output, const QPointF & painterPosMM )
{
    if (!m_d->model || !m_d->view)
        return;

    switch ( output->output() ) {
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

    if (rowCount()==0 || columnCount()==0)
        return;

    m_itemRect = itemRect( output, painterPosMM );
    m_tableWidth = m_itemRect.width();
    int calculatedScreenTableWidth = calculateSourceTableWidth();
    m_deviceRate = output->scale().dpi()/output->scale().screenDpi();

    if (calculatedScreenTableWidth < m_tableWidth/m_deviceRate)
        m_rate = m_deviceRate;
    else
        m_rate = m_tableWidth/calculatedScreenTableWidth;

    m_tableSpacing = output->scale().mmToRealPixel(m_d->tableSpacingMM);
    m_cellSpacing = m_d->cellSpacing * m_rate;
    m_calculatedTableHeight = 0;
    m_defaultIconSize = QSizeF( D_icon_size*m_rate, D_icon_size*m_rate );

    QPointF topLeft(m_itemRect.topLeft());

    if (isHorizontalHeaderVisible())
        paintRow(output,topLeft,-1); //header

    for (int row=0; row<rowCount(); ++row)
    {
        paintRow(output, topLeft, row );
        if (director()->flag(NCReportDirector::FlagFinished)) {
            return;
        }
    }

    director()->setPaintPosY( output->scale().pixelToMM(topLeft.y()) );
    output->renderItem( this, m_itemRect );
}

/*!
Paints table's header: Column headers row
*/
void NCReportTableItem::paintRow( NCReportOutput* output, QPointF &topLeft, int row)
{
    if ( row>=0 && m_d->view->isRowHidden(row) )
        return;

    if (m_d->hiddenRows.contains(row))
        return;

    qreal rowHeight = 0;
    if ( row<0 )
        rowHeight = m_d->view->horizontalHeader()->height() * m_rate;
    else
        rowHeight = m_d->view->rowHeight(row) * m_rate;

    QRectF rowRect( topLeft, QSizeF(m_tableWidth, rowHeight ) );    //row rectangle
    qreal spBottom = spaceToBottom(output, rowRect);

    if ( spBottom < rowRect.height() )  {
        director()->pageBreak(false,false);
        if (director()->flag(NCReportDirector::FlagFinished)) {
            return;
        }
        topLeft = output->scale().mmToPoint(director()->paintPosMM());   // reset row top left position
        topLeft.setX( m_itemRect.x() );

        if (isHorizontalHeaderVisible())
            paintRow(output,topLeft,-1);    // recursively reprint header on new page
    } else {
        // nothing to do
    }

    QPointF cellTopLeft(topLeft);

    if (isVerticalHeaderVisible()) {
        QRectF cellRect(cellTopLeft, QSizeF(columnWidthPixel(-1), rowHeightPixel(row)));
        paintCell( output, cellRect, row, -1 );
        cellTopLeft.rx() += columnWidthPixel(-1);
    }

    for (int col=0; col<columnCount(); ++col)
    {
        if (!m_d->view->isColumnHidden(col) && !m_d->hiddenCols.contains(col))
        {
            QRectF cellRect(cellTopLeft, QSizeF(columnWidthPixel(col), rowHeightPixel(row)));
            paintCell( output, cellRect, row, col );
            cellTopLeft.rx() += columnWidthPixel(col);
        }
    }
    topLeft.ry() += rowHeight;
}

void NCReportTableItem::paintCell( NCReportOutput* output, const QRectF &cellRect, int row, int col )
{
    if (director()->currentPageStatus() == NCReportDirector::Hidden)
        return;

    CellType cellType = Data;

    if (row<0 && col<0)
        cellType = Corner;
    else if (row<0)
        cellType = HorizontalHeader;
    else if (col<0)
        cellType = VerticalHeader;
    else
        cellType = Data;

    QModelIndex index;
    QVariant data;

    switch (cellType) {
    case HorizontalHeader:
    {
        data = m_d->model->headerData(col, Qt::Horizontal, Qt::DisplayRole);
        break;
    }
    case VerticalHeader:
    {
        data = m_d->model->headerData(row, Qt::Vertical, Qt::DisplayRole);
        break;
    }
    case Data:
    {
        index = m_d->model->index(row,col);
        data = m_d->model->data(index, Qt::DisplayRole );
        break;
    }
    case Corner:
        break;
    }

    if (m_d->showBorderAndBG) {
        QPen borderPen;
        borderPen.setStyle(Qt::SolidLine);
        borderPen.setWidth(1);

        QBrush brush(Qt::NoBrush);

        if (cellType == Corner || cellType == HorizontalHeader || cellType == VerticalHeader) {
            brush.setColor(D_headerBackgroundColor);
            brush.setStyle(Qt::SolidPattern);
            borderPen.setBrush(brush);
            borderPen.setColor(D_headerLineColor);
        } else {
            borderPen.setColor(D_lineColor);
            if ( m_d->model->data(index, Qt::BackgroundColorRole).canConvert(QVariant::Brush))
                brush = qvariant_cast<QBrush>(m_d->model->data(index, Qt::BackgroundColorRole));
        }

        output->painter()->setPen(borderPen);
        output->painter()->setBrush(brush);
        output->painter()->drawRect(cellRect);
    }

    QRectF cellPaintRect(cellRect);
    cellPaintRect.adjust(m_cellSpacing, m_cellSpacing, -m_cellSpacing, -m_cellSpacing);

    if ( cellType == VerticalHeader )
    {
        //--------------
        // INDENTATION
        //--------------
        int _dimension =0;
        bool _hasChildren = false;
        int _indentationValue = m_d->model->data( m_d->model->index(row,0), Qt::UserRole+1).toInt();
        indentation( _indentationValue, _dimension, _hasChildren );
        if (_dimension>0 ) {
            paintIndentationImage( output, cellPaintRect, _dimension );
            cellPaintRect.adjust( _dimension*D_indentation,0,0,0 );
            //qDebug("Left: %f Dimension: %f Identation: %f",cellPaintRect.left(), _dimension, _dimension*_indentation );
        }
    }

    NCReportTableItem::ImageType cellImageType = paintCellImage( output, cellPaintRect, m_d->model->data(index, Qt::DecorationRole) );
    if (cellImageType==NCReportTableItem::Icon)
        cellPaintRect.adjust(m_defaultIconSize.width()*D_icon_margin_trimmer,0,0,0);

    switch (data.type())
    {
    case QVariant::String:
    case QVariant::Int:
    case QVariant::Double:
    case QVariant::Date:
    case QVariant::DateTime:
    {
        //QFlags<Qt::AlignmentFlag> alignmentFlags;
        int alignmentFlags =0;
        QFont font;

        switch (cellType) {
        case Corner:
            break;
        case HorizontalHeader:
            alignmentFlags = QFlag(m_d->model->headerData(col, Qt::Horizontal, Qt::TextAlignmentRole).toInt());
            font = qvariant_cast<QFont>(m_d->model->headerData(col, Qt::Horizontal, Qt::FontRole));
            break;
        case VerticalHeader:
            alignmentFlags = QFlag(m_d->model->headerData(row, Qt::Vertical, Qt::TextAlignmentRole).toInt());
            font = qvariant_cast<QFont>(m_d->model->headerData(row, Qt::Vertical, Qt::FontRole));
            break;
        case Data:
        {
            alignmentFlags = QFlag(m_d->model->data(index, Qt::TextAlignmentRole).toInt());
            font = qvariant_cast<QFont>(m_d->model->data(index, Qt::FontRole));
            //alignmentFlags |= Qt::AlignVCenter;
            //alignmentFlags |= Qt::TextWordWrap;
            break;
        }
        }

        QString txt = data.toString();

        QFontMetricsF fm_screen(font);
        int pixelSize = fm_screen.height() * D_font_trimmer * m_rate;

        if (pixelSize>cellPaintRect.height())
            pixelSize=cellPaintRect.height();

        font.setPixelSize( pixelSize);
        QFontMetricsF fm_device(font);

        output->painter()->setFont( font );

        QPen pen(Qt::black);
        if ( m_d->model->data(index, Qt::ForegroundRole).canConvert(QVariant::Brush)) {
            QBrush brush = qvariant_cast<QBrush>(m_d->model->data(index, Qt::ForegroundRole));
            pen.setBrush(brush);
        } else {
            pen.setColor( m_d->model->data(index, Qt::ForegroundRole).value<QColor>() );
        }

        //alignmentFlags |= Qt::TextSingleLine;

        output->painter()->setPen( pen );

        if (m_d->elidedMode) {
            paintElitedText(output->painter(), txt, cellPaintRect, QFlag(alignmentFlags) );
        } else {
            // direct print mode
            QRectF txtBoundingRect = fm_device.boundingRect(cellPaintRect, Qt::TextWordWrap, txt);
            alignmentFlags |= Qt::TextWordWrap;

            if (txtBoundingRect.height() > cellPaintRect.height() /*|| alignmentFlags == 0*/ )
                alignmentFlags |= Qt::AlignTop; // avoid to show middle-part only

            //txt = fm_device.elidedText(txt, Qt::ElideRight, cellPaintRect.width(), alignmentFlags);

            output->painter()->drawText(cellPaintRect, alignmentFlags, txt );
        }

        break;
    }
    default:
        break;
    }
}

NCReportTableItem::ImageType NCReportTableItem::paintCellImage(NCReportOutput *output, const QRectF &cellRect, const QVariant &imageData) const
{
    if ( imageData.canConvert(QVariant::Pixmap) )
    {
        QPixmap pixmap = qvariant_cast<QPixmap>(imageData);

        QRectF imageCellPaintRect(cellRect);
        imageCellPaintRect.adjust(1, 1, -1, -1);

        QSizeF imageTargetSize( pixmap.size() );
        imageTargetSize.scale( imageCellPaintRect.size(), Qt::KeepAspectRatio );

        QRectF target( imageCellPaintRect.topLeft(), imageTargetSize );

        output->painter()->drawPixmap(target, pixmap, pixmap.rect());

        return NCReportTableItem::Pixmap;
    }
    else if (imageData.canConvert(QVariant::Icon))
    {
        QIcon icon = qvariant_cast<QIcon>(imageData);
        QPixmap pixmap = icon.pixmap(m_defaultIconSize.toSize(), QIcon::Normal, QIcon::On);

        QRectF target(cellRect);
        target.setSize(m_defaultIconSize);

        output->painter()->drawPixmap(target, pixmap, pixmap.rect());

        return NCReportTableItem::Icon;
    }

    return NCReportTableItem::None;
}

void NCReportTableItem::paintIndentationImage(NCReportOutput *output, const QRectF &cellRect, int dimension)
{
    return; // out of service

    if (dimension<=0)
        return;

    QRectF imageCellPaintRect(cellRect);
    imageCellPaintRect.adjust(dimension*D_indentation - D_indentation , 1, -1, -1);

    QPixmap pixmap(D_icon_size,D_icon_size);
    QStyleOption option;
    option.state = QStyle::State_Children;
    QPainter painter;
    painter.begin(&pixmap);
    m_d->view->style()->drawPrimitive( QStyle::PE_IndicatorBranch, &option, &painter );
    painter.end();

    QSizeF imageTargetSize( pixmap.size() );
    imageTargetSize.scale( imageCellPaintRect.size(), Qt::KeepAspectRatio );

    QRectF target( imageCellPaintRect.topLeft(), imageTargetSize );

    output->painter()->drawPixmap(target, pixmap, pixmap.rect());
}


void NCReportTableItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    const int cellWidth = 60;
    const int cellHeight = 20;

    QRectF rect( QPointF(0, 0), currentSize() );
    //QRectF rect( boundingRect() );
    QPen pen(Qt::darkGray);
    pen.setStyle(Qt::DashLine);
    painter->setPen( pen );
    painter->drawRect( rect );

    for (int x=cellWidth; x<rect.width(); x+=cellWidth )
        painter->drawLine( QPointF(x,0), QPointF(x,rect.height()) );
    for (int y=cellHeight; y<rect.height(); y+=cellHeight )
        painter->drawLine( QPointF(0,y), QPointF(rect.width(),y) );

    rect.setLeft( rect.left()+1 );
    painter->drawText( rect, Qt::AlignCenter, QObject::tr("TABLE\nModel: %1 View: %2").arg(m_d->modelID).arg(m_d->viewID));

    paintSelection( painter );
}

void NCReportTableItem::dummyPaint(NCReportOutput *output, const QPointF &point)
{
    paint(output,point);
}

void NCReportTableItem::adjustSize(NCReportOutput*)
{
}

void NCReportTableItem::setDataSource(NCReportDataSource *ds)
{
    m_d->dataSource = ds;
}

NCReportDataSource * NCReportTableItem::dataSource() const
{
    return m_d->dataSource;
}

QRectF NCReportTableItem::boundingRect() const
{
    return NCReportItem::boundingRect();
}

void NCReportTableItem::setHiddenRows(const QList<int> &rows)
{
    m_d->hiddenRows = rows;
}

void NCReportTableItem::setHiddenRows(const QString &list)
{
    NCReportUtils::stringToIntList( list, m_d->hiddenRows );
}

QString NCReportTableItem::hiddenRowsList() const
{
    return NCReportUtils::intListToString(m_d->hiddenRows);
}

void NCReportTableItem::setHiddenColumns(const QList<int> &columns)
{
    m_d->hiddenCols = columns;
}

void NCReportTableItem::setHiddenColumns(const QString &list)
{
    NCReportUtils::stringToIntList( list, m_d->hiddenCols );
}

QString NCReportTableItem::hiddenColumnsList() const
{
    return NCReportUtils::intListToString(m_d->hiddenCols);
}

int NCReportTableItem::columnCount() const
{
    if (m_d->model)
        return m_d->model->columnCount();
    return 0;
}

int NCReportTableItem::rowCount() const
{
    if (m_d->model)
        return m_d->model->rowCount();
    return 0;
}

qreal NCReportTableItem::columnWidthPixel(int column) const
{
    qreal width = 0;
    if (column<0)
        width = m_d->view->verticalHeader()->width() * m_rate;
    else
        width = m_d->view->columnWidth(column) * m_rate;
    return width;
}

qreal NCReportTableItem::rowHeightPixel(int row) const
{
    qreal height = 0;
    if (row<0)
        height = m_d->view->horizontalHeader()->height() * m_rate;
    else
        height = m_d->view->rowHeight(row) * m_rate;
    return height;
}

int NCReportTableItem::calculateSourceTableWidth() const
{
    int width=0;
    for (int col=0; col<m_d->model->columnCount(); ++col)
    {
        if (!m_d->view->isColumnHidden(col))
            width += m_d->view->columnWidth(col);
    }
    if (isVerticalHeaderVisible()) {
        if (m_d->view->verticalHeader()->isVisible())
            width += m_d->view->verticalHeader()->width();
    }

    if (width==0)
        width = 1;

    return width;
}

void NCReportTableItem::setDataSourceByID(const QString &id)
{
    setDataSource( d->reportDef->dataSource(id) );
}

qreal NCReportTableItem::spaceToBottom( NCReportOutput* output, const QRectF& rect ) const
{
    qreal footerHeight = 0.0;
    if ( reportDef()->pageFooter() )
        footerHeight = reportDef()->pageFooter()->metricHeight();

    qreal mBottomY = reportDef()->option().pageHeightMM() - reportDef()->option().bottomMarginMM() - footerHeight;
    qreal bottomY = output->scale().mmToPixel(mBottomY);
    return bottomY - rect.topLeft().y();
}

void NCReportTableItem::paintElitedText( QPainter *painter, const QString& text, const QRectF& cellRect, Qt::Alignment alignment )
{
     QFontMetricsF fontMetrics(painter->font());

     //bool didElide = false;
     int lineSpacing = fontMetrics.lineSpacing();
     int y = cellRect.y();

     QTextLayout textLayout(text, painter->font(), painter->device() );
     QTextOption textOption(alignment);
     textOption.setWrapMode(QTextOption::WordWrap);
     textLayout.setTextOption(textOption);

     textLayout.beginLayout();
     forever {
         QTextLine line = textLayout.createLine();

         if (!line.isValid())
             break;

         line.setLineWidth(cellRect.width());
         int nextLineY = y + lineSpacing;

         if ( cellRect.bottom() >= nextLineY + lineSpacing) {
             line.draw(painter, QPoint(cellRect.x(), y));
             y = nextLineY;
         } else {
             QString lastLine = text.mid(line.textStart());
             QString elidedLastLine = fontMetrics.elidedText(lastLine, Qt::ElideRight, cellRect.width());
             painter->drawText(QPoint( cellRect.x(), y + fontMetrics.ascent()), elidedLastLine);
             line = textLayout.createLine();
             //didElide = line.isValid();
             break;
         }
    }
    textLayout.endLayout();

//    if (didElide != elided) {
//        elided = didElide;
//        emit elisionChanged(didElide);
//    }
}


void NCReportTableItem::indentation(int role, int &dimension, bool &hasChildren)
{
    bool ok = true;
    QByteArray ba;
    ba.setNum(role, 2);
    ba = ba.rightJustified(32,'0');
    dimension = ba.right(8).toInt(&ok,2);    // first 8 bits
    hasChildren = ba.mid(23).toInt(&ok,10);  // 9. bit

    if (!ok) {
        dimension = 0;
        hasChildren = false;
    }
}
