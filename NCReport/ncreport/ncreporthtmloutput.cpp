/***************************************************************************
 *   Copyright (C) 2010 by NociSoft Software Solutions
 *   support@nocisoft.com
 ***************************************************************************/
#include "ncreporthtmloutput.h"
#include "ncreportlabelitem.h"
#include "ncreportlineitem.h"
#include "ncreportrectitem.h"
#include "ncreporttextitem.h"
#include "ncreportimageitem.h"
#include "ncreportbarcodeitem.h"
#include "ncreportsection.h"
#include "ncreportchartitem.h"
#include "ncreportutils.h"

#include <QDir>
#include <QBuffer>
#include <QTextDocument>
#include <QTextDocumentWriter>

//#define HTMLOUTPUT_DEBUGMODE

NCReportHtmlOutput::NCReportHtmlOutput(QObject *parent) : NCReportMarkupLanguageOutput(parent), m_imageMode(Base64)
{
    setOutput( Html );
}

NCReportHtmlOutput::~ NCReportHtmlOutput()
{
    qDebug("NCReportHtmlOutput deleted.");
}

void NCReportHtmlOutput::setImageMode(NCReportHtmlOutput::ImageMode mode)
{
    m_imageMode = mode;
}

NCReportHtmlOutput::ImageMode NCReportHtmlOutput::imageMode() const
{
    return m_imageMode;
}

#ifdef USE_QIMAGE_IMDTEAD_OF_QPIXMAP
QByteArray NCReportHtmlOutput::imageToHtml(const QImage &image, const QSize& size, ImageMode mode )
#else
QByteArray NCReportHtmlOutput::imageToHtml(const QPixmap &image, const QSize& size, ImageMode mode)
#endif
{
    QByteArray html = "<img ";

    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer,"PNG");

    if (size.isValid())
        html += (QByteArray("width=\"")+QByteArray::number(size.width())+"\" height=\""+QByteArray::number(size.height())+"\" ");

    html += "src=\"";

    if (mode==Base64) {
        html += "data:image/png;base64," + bytes.toBase64();
    } else {
        QString fname = fileName()+ QString("_files/image%1.png").arg(++fileCounter(),4,10,QLatin1Char('0'));
        QFileInfo fi(fname);
        QDir dir;
        dir.mkpath(fi.path());
        image.save(fname);

        html += fname;
    }

    html += "\"/>";

    return html;
}

void NCReportHtmlOutput::writeHeader()
{
    contentStream() << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">" << crlf();
    contentStream() << "<html>" << crlf() << "<head>" << crlf();
    contentStream() << "<meta charset=\"" << option().encoding() << "\">" << crlf();
    contentStream() << "<title>" << option().reportName() << "</title>" << crlf();
    contentStream() << option().htmlHead();
    if (styleSheetFile().isEmpty())
        contentStream() << "<style type=\"text/css\">" << crlf();
    contentStream() << styleSheetMarker() << crlf();
    if (styleSheetFile().isEmpty())
        contentStream() << "</style>" << crlf();
    contentStream() << "</head>" << crlf() << "<body>" << crlf();

    styleStream() << "body{";

    QString opt(option().htmlBodyStyle());
    if (opt.contains("{DEFAULT}",Qt::CaseInsensitive)) {
        opt.replace("{DEFAULT}","",Qt::CaseInsensitive);
        // set default options
        styleStream() << "width:" << scale().mmToPixel(option().purePageSize().width()) << "px;" << "font-family:" << option().defaultFont().family() << ";";
        styleStream() << "font-size:" << option().defaultFont().pointSize() << "pt;";
        styleStream() << "margin-left:" << scale().mmToPixel(option().leftMarginMM()) << "px;";
        styleStream() << "margin-right:" << scale().mmToPixel(option().rightMarginMM()) << "px;";
        styleStream() << "margin-top:" << scale().mmToPixel(option().topMarginMM()) << "px;";
        styleStream() << "margin-bottom:" << scale().mmToPixel(option().bottomMarginMM()) << "px;";
    }
    styleStream() << opt;
    styleStream() << "}" << crlf();
    styleStream() << "table.ncr{width:" << scale().mmToPixel(option().purePageSize().width()) << "px;";
    styleStream() << "border-spacing:3px;}" << crlf();

}

void NCReportHtmlOutput::writeFooter()
{
    contentStream() << "</body>" << crlf() << "</html>";
}

void NCReportHtmlOutput::writeSpace(NCReportSection *section, int row, int width)
{
    QString itemClass = QString("%1_spc_row%2").arg(fixedID(section->id())).arg(row);

    QTextStream tsStyle;
    initStyleStream(tsStyle, QString("td.")+itemClass );

    if (tsStyle.string()) {
        tsStyle << "width:" << width << "px;";
    }
    contentStream() << "<td class=\"" << itemClass << "\">&nbsp;</td>";
}



void NCReportHtmlOutput::writeLabel(NCReportLabelItem *item, QTextStream& cellStyle)
{
    Qt::Alignment a = item->alignment();

    if (cellStyle.string()) {
        // write only if style streem is valid. Otherwise the style item is already exists

        if (a & Qt::AlignLeft) {
            //tsStyle << "text-align:left;";
        } else if (a & Qt::AlignHCenter) {
            cellStyle << "text-align:center;";
        } else if (a & Qt::AlignRight) {
            cellStyle << "text-align:right;";
        } else if (a & Qt::AlignJustify) {
            cellStyle << "text-align:justify;";
        }

        cellStyle << "vertical-align:";
        if (a & Qt::AlignTop) {
            cellStyle << "top;";
        } else if (a & Qt::AlignVCenter) {
            cellStyle << "middle;";
        } else if (a & Qt::AlignBottom) {
            cellStyle << "bottom;";
        } else {
            cellStyle << "top;";
        }

        if (item->foreColor() != Qt::black )
            cellStyle << "color:" << item->foreColor().name() << ";";
        if ( item->backgroundMode() == NCReportLabelItem::FilledBackground )
            cellStyle << "background-color:" << item->backColor().name() << ";";

        if ( option().defaultFont().family() != item->font().family())
            cellStyle << "font-family:" << item->font().family() << ";";

        if ( option().defaultFont().pointSize() != item->font().pointSize() )
            cellStyle << "font-size:" << item->font().pointSize() << "pt;";

        if (item->font().bold())
            cellStyle << "font-weight:bold;";
        if (item->font().italic())
            cellStyle << "font-style:italic;";
        if (item->font().underline())
            cellStyle << "text-decoration:underline;";
        if (item->font().letterSpacing()!=0)
            cellStyle << "letter-spacing:" << item->font().letterSpacing() << "px;";
        if (item->font().strikeOut())
            cellStyle << "text-decoration:line-through;";
        if ( item->font().capitalization()==QFont::SmallCaps)
            cellStyle << "font-variant:small-caps;";
    }

    contentStream() << item->printableText().trimmed();
}

void NCReportHtmlOutput::writeLine(NCReportLineItem * item)
{
    Q_UNUSED(item)
//    QRectF rect = item->paintRect();
    //contentStream() << "<hr noshade size=\"" << ((int)item->lineWidth())+1 << "\" width=\""<< rect.width() << "px\" ";
    //contentStream() << "style=\"position:absolute;left:" << rect.topLeft().x() << "px;top:" << m_pagePos+rect.topLeft().y()+7 << "px;z-index:" << item->zValue() << "\">\n";
}


void NCReportHtmlOutput::writeRectangle(NCReportRectItem * item)
{
    Q_UNUSED(item)
//    QRectF rect = item->paintRect();

//    contentStream() << "<div style=\"position:absolute;left:" << rect.topLeft().x() << "px;top:" << m_pagePos+rect.topLeft().y() << "px;z-index:" << item->zValue();
//    contentStream() << ";width:" << rect.width() << "px;height:" << rect.height() << "px";
//    contentStream() << ";border: " << item->pen().width() << "px " << penStyleName(item->pen().style()) << " " << item->pen().color().name();
//    if ( item->brush().style() != Qt::NoBrush )
//        contentStream() << ";background-color:" << item->brush().color().name();
//    contentStream() << "\">";
//    contentStream() << "</div>" << crlf();
}

void NCReportHtmlOutput::writeSectionStart(NCReportSection *section)
{
    contentStream() << "<!-- SECTION: " << fixedID(section->id()) << " -->" << crlf();
    contentStream() << "<div class=\"" << fixedID(section->id()) << "\">"  << crlf();
    //contentStream() << "<div class=\"" << fixedID(section->id()) << "\"" + (section->htmlOptions().isEmpty() ? "" : section->htmlOptions().prepend(" ") ) +  ">" << crlf();
    if (strategy()==SectionOneTable)
        contentStream() << "<table class=\"" << "ncr " << "section_" << fixedID(section->id()) << "\">" << crlf();

    if (!section->htmlOptions().isEmpty()) {
        addCSSItem( QString(".")+fixedID(section->id()), section->htmlOptions());
    }
}

void NCReportHtmlOutput::writeSectionEnd(NCReportSection *section)
{
    if (strategy()==SectionOneTable)
        contentStream() << "</table>" << crlf();

    contentStream() << "</div>" << crlf();

#ifdef HTMLOUTPUT_DEBUGMODE
    if (section)
        contentStream() << "<!-- END SECTION: " << fixedID(section->id()) << " -->" << crlf();
#else
    Q_UNUSED(section)
#endif
}

void NCReportHtmlOutput::writeRowStart(NCReportSection *section)
{
    if (strategy()==SectionMultiTables) {
        contentStream() << "<table class=\"" << "ncr " << "section_" << fixedID(section->id()) << "\">" << crlf();
    }
    QString rowClass = QString("r_%1_%2").arg( fixedID(section->id()), rowCounter()%2 ? "even" : "odd" );
    contentStream() << "<tr class=\"" << rowClass << "\">" ;
}

void NCReportHtmlOutput::writeRowEnd(NCReportSection *section)
{
    Q_UNUSED(section)
    contentStream() << "</tr>";
    if (strategy()==SectionMultiTables)
        contentStream() << "</table>";

    contentStream() << crlf();
}

QString NCReportHtmlOutput::styleSheetReference(const QString &css) const
{
    QString s;
    s +="<link rel=\"stylesheet\" type=\"text/css\" href=\"" + css + "\">";
    return s;
}

void NCReportHtmlOutput::cellStarts(NCReportItem *item, QTextStream& cellStyle)
{
    QString itemClass = itemClassId(item);

    initStyleStream(cellStyle, QString("td.")+itemClass );
    if ( !item->htmlOptions().isEmpty() && cellStyle.string() )
        cellStyle << item->htmlOptions() << (item->htmlOptions().endsWith(';') ? "" : ";");

    if (cellStyle.string())
        cellStyle << "width:" << item->paintRect().width() << "px;";    // set cell width <TD>

    contentStream() << "<td class=\"" << itemClass << "\">";
}

void NCReportHtmlOutput::cellEnds(NCReportItem *item)
{
    Q_UNUSED(item)
    contentStream() << "</td>";
}


void NCReportHtmlOutput::cleanHtml(QString &html)
{
    html.replace("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">","",Qt::CaseInsensitive);
    html.replace("<html>","",Qt::CaseInsensitive);
    html.replace("</html>","",Qt::CaseInsensitive);
    html.replace("<head>","",Qt::CaseInsensitive);
    html.replace("</head>","",Qt::CaseInsensitive);
    html.replace("<body","<div",Qt::CaseInsensitive);
    html.replace("</body>","</div>",Qt::CaseInsensitive);
}

void NCReportHtmlOutput::writeImage(NCReportImageItem *item)
{
//    QRectF rect = item->paintRect();
//    styleStream() << "width:" << rect.width() << "px;";
//    styleStream() << "height:" << rect.width() << "px;";
//    styleStream() << "z-index:" << item->zValue() << ";";
    contentStream() << imageToHtml(item->image(), QSize(item->paintRect().size().toSize()), m_imageMode );
}

void NCReportHtmlOutput::writeText(NCReportTextItem * item)
{
    QString itemClass = QString("text%1").arg(fixedID(item->id()));

    QTextStream textStyle;
    initStyleStream(textStyle, QString("div.")+itemClass );

    if (textStyle.string()) {
        // <div> style options
        textStyle << "width:" << item->paintRect().width() << "px;";

        if (item->foreColor() != Qt::black )
            textStyle << "color:" << item->foreColor().name() << ";";

        if (!item->isUsedDefaultFont() && option().defaultFont().family() != item->font().family())
            textStyle << "font-family:" << item->font().family() << ";";

        if ( option().defaultFont().pointSize() != item->font().pointSize() )
            textStyle << "font-size:" << item->font().pointSize() << "pt;";
    }
    contentStream() << "<div class=\"" << itemClass << "\">" + crlf();

    QString content(item->htmlContent());
    cleanHtml( content );
    contentStream() << content << crlf();
    contentStream() << "</div>" << crlf();
}

void NCReportHtmlOutput::writeBarcode(NCReportBarcodeItem *item)
{
#ifdef USE_QIMAGE_IMDTEAD_OF_QPIXMAP
    contentStream() << imageToHtml(item->toImage(item->paintRect().size().toSize()), QSize(), m_imageMode );
#else
    contentStream() << imageToHtml(item->toPixmap(item->paintRect().size().toSize()), QSize(), m_imageMode );
#endif

}

void NCReportHtmlOutput::writeChart(NCReportChartItem *item)
{
#ifdef HIGHCHARTS_INTEGRATION
    contentStream() << imageToHtml(item->pixmap(), QSize(), m_imageMode );
#else
    Q_UNUSED(item)
#endif
}
