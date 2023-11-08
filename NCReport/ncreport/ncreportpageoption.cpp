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
#include "ncreportpageoption.h"
#include "ncreportxmlreader.h"
#include "ncreportxmlwriter.h"
#include "ncreportscale.h"

#include <QLocale>

NCReportPageOption::NCReportPageOption() :
    m_paperSize(QPrinter::A4),
    m_pageOrientation(QPrinter::Portrait),
    m_reportType(NCReport::Report),
    m_reportName(QObject::tr("Untitled")),
    m_pageMetricSize(QSizeF(210.0,297.0)),
    m_topMargin(15),
    m_bottomMargin(15),
    m_leftMargin(20),
    m_rightMargin(20),
    m_columnCount(0),
    m_columnWidth(0),
    m_columnSpacing(0),
    m_defaultFont(QFont("Arial",10)),
    m_encoding("UTF-8"),
    m_backcolor(Qt::white),
    m_zoneSpacing(0.0),
    m_sectionSpacing(0.0),
    m_doublePassMode(false),
    m_languages("00"),
    m_currentLanguage("00"),
    m_langRole(UseDefaultLanguage),
    m_zonesAreBreakable(false),
    m_subreportOnNewPage(false),
    m_htmlBodyStyle("{DEFAULT}"),
    m_htmlHead(""),
    m_customData(QString()),
    m_dataTrimming(true),
    m_updateOnlyCurrentDsVars(false)
{
}

void NCReportPageOption::setPaperSizeByName( const QString psname, const QPrinter::Orientation orient, QPrinter::PaperSize & psid, QSizeF& size )
{
    QString ps = psname.toUpper();
    QVector<NCReportPageOption::PageData> v;
    loadPageDataVector( v );
    for (int i = 0; i < v.size(); ++i) {
        PageData pd = v.at(i);
        if (pd.name == ps ) {
            if ( pd.width > 0 )
                size.setWidth(pd.width);
            if ( pd.height > 0 )
                size.setHeight(pd.height);
            psid = pd.paperSize;
            break;
        }
    }

    if ( orient == QPrinter::Landscape ) {
        qreal w = size.width();
        size.setWidth(size.height());
        size.setHeight(w);
    }
    return;
}

QString NCReportPageOption::paperSizeToName(const QPrinter::PaperSize ps )
{
    QVector<NCReportPageOption::PageData> v;
    loadPageDataVector( v );
    for (int i = 0; i < v.size(); ++i) {
        if (v.at(i).paperSize == ps)
            return v.at(i).name;
    }
    return "CUSTOM";
}

void NCReportPageOption::adjustPageSize()
{
    QString sizename = paperSizeToName( m_paperSize );
    setPaperSizeByName( sizename, m_pageOrientation, m_paperSize, m_pageMetricSize );
}

void NCReportPageOption::swapOrientation()
{
    setOrientation( pageOrientation() == QPrinter::Portrait ? QPrinter::Landscape : QPrinter::Portrait);
}

bool NCReportPageOption::read( NCReportXMLReader* r )
{
    setDoublePassMode(false);
    Q_ASSERT( r->isStartElement() && r->name() == "options");
    while (!r->atEnd()) {
        r->readNext();
        if (r->isEndElement())
            return false;

        if (r->isStartElement()) {
            if (r->name() == "encoding")
                setEncoding(r->readElementText());

            else if (r->name() == "language")
                setLanguages(r->readElementText());
            else if (r->name() == "langrole")
                m_langRole = (r->readElementText()=="0" ? UseDefaultLanguage : LeaveEmpty );
            else if (r->name() == "orientation")
                setOrientation( r->readElementText()=="portrait" ? QPrinter::Portrait : QPrinter::Landscape );
            else if (r->name() == "pagesize") {
                QSizeF customSize = r->readSize();
                QString pageSizeExp = r->readElementText();
                setPaperSize(pageSizeExp,customSize);
            }
            else if (r->name() == "topmargin")
                setTopMarginMM(r->readElementText().toDouble());
            else if (r->name() == "bottommargin")
                setBottomMarginMM(r->readElementText().toDouble());
            else if (r->name() == "leftmargin")
                setLeftMarginMM(r->readElementText().toDouble());
            else if (r->name() == "rightmargin")
                setRightMarginMM(r->readElementText().toDouble());
            else if (r->name() == "backcolor")
                setBackColor(QColor(r->readElementText()));
            else if (r->name() == "columns")
                setColumnCount( r->readElementText().toInt());
            else if (r->name() == "columnspace")
                setColumnSpacing(r->readElementText().toDouble());
            else if (r->name() == "fontname")
                setDefaultFontFamily( r->readElementText() ) ;
            else if (r->name() == "fontsize")
                setDefaultFontSize(r->readElementText().toInt());
            else if (r->name() == "zoneSpacing")
                setZoneSpacing(r->readElementText().toDouble());
            else if (r->name() == "doublePass")
                setDoublePassMode(r->readElementText().toLower() == "true");
            else if (r->name() == "zoneBreak")
                setZonesAreBreakable(r->readElementText().toLower() == "true");
            else if (r->name() == "subReportOnNewPage")
                setSubReportOnNewPage(r->readElementText().toLower() == "true");
            else if (r->name() == "htmlOpt" || r->name()=="htmlBodyStyle")
                setHtmlBodyStyle(r->readElementText());
            else if (r->name() == "htmlHead")
                setHtmlHead(r->readElementText());
            else if (r->name() == "fontstyle")
                setDefaultFontStyle( r->readElementText() ) ;
            else if (r->name() == "customdata")
                setCustomData( r->readElementText() ) ;
            else if (r->name() == "dataTrimming")
                setDataTrimming(r->readElementText().toLower() == "true");
            else if (r->name() == "updateOnlyCurrentDataSourceVariables")
                setupdateOnlyCurrentDataSourceVariables(r->readElementText().toLower() == "true");
            else
                r->readUnknownElement();
        }
    }

    return true;
}

bool NCReportPageOption::write(NCReportXMLWriter * w)
{
    w->writeStartElement("options");
    w->writeTextElement("encoding", m_encoding );
    w->writeTextElement("language",  m_languages );
    w->writeTextElement("langrole",  QString::number(m_langRole) );
    w->writeTextElement("orientation",  m_pageOrientation == QPrinter::Portrait ? "portrait" : "landscape" );

    w->writeStartElement("pagesize");
    w->writeSize( m_pageMetricSize );
    w->writeCharacters(paperSizeToName( m_paperSize ));
    w->writeEndElement();

    w->writeTextElement("topmargin",  QString::number(m_topMargin) );
    w->writeTextElement("bottommargin",  QString::number(m_bottomMargin) );
    w->writeTextElement("leftmargin",  QString::number(m_leftMargin) );
    w->writeTextElement("rightmargin",  QString::number(m_rightMargin) );
    w->writeTextElement("backcolor",  m_backcolor.name() );
    w->writeTextElement("columns",  QString::number(m_columnCount) );
    w->writeTextElement("columnspace", QString::number(m_columnSpacing) );
    w->writeTextElement("zoneSpacing", QString::number(m_zoneSpacing) );
    w->writeTextElement("zoneBreak", m_zonesAreBreakable ? "true" : "false" );
    // DEFAULT FONT
    w->writeTextElement("fontname",  m_defaultFont.family() );
    w->writeTextElement("fontsize",  QString::number(m_defaultFont.pointSize()) );
    w->writeTextElement("fontstyle", defaultFontStyle() );
    if (!customData().isEmpty())
        w->writeTextElement("customdata", customData() );

    if (!htmlBodyStyle().isEmpty())
        w->writeTextElement("htmlBodyStyle", htmlBodyStyle() );
    if (!htmlHead().isEmpty())
        w->writeTextElement("htmlHead", htmlHead() );

    if ( isDoublePassMode() )
        w->writeTextElement("doublePass", "true" );

    if (subReportOnNewPage())
        w->writeTextElement("subReportOnNewPage", "true");

    if (!isDataTrimming())
        w->writeTextElement("dataTrimming", "false");

    if (updateOnlyCurrentDataSourceVariables())
        w->writeTextElement("updateOnlyCurrentDataSourceVariables", "true");

    w->writeEndElement();

    return true;
}

QString NCReportPageOption::reportTypeName() const
{
    switch ( m_reportType ) {
        case NCReport::Report: return "report";
        case NCReport::TextDocumentPrint: return "textdoc";
    }

    return QString::null;
}

void NCReportPageOption::setReportType(const QString & name)
{
    if ( name.toLower() == "report" )
        m_reportType = NCReport::Report;
    else if ( name.toLower() == "textdoc" )
        m_reportType = NCReport::TextDocumentPrint;
    else
        m_reportType = NCReport::Report;
}

void NCReportPageOption::setReportType( NCReport::ReportType type )
{
    m_reportType = type;
}

QSizeF NCReportPageOption::purePageSize() const
{
    return QSizeF( m_pageMetricSize.width() - m_leftMargin - m_rightMargin, m_pageMetricSize.height() - m_topMargin - m_bottomMargin );
}

void NCReportPageOption::setPaperSize( const QPrinter::PaperSize ps )
{
    m_paperSize = ps;
    adjustPageSize();
}

void NCReportPageOption::setPaperSize(const QPrinter::PaperSize size, const QPrinter::Orientation orientation)
{
    m_paperSize = size;
    m_pageOrientation = orientation;
    adjustPageSize();
}

void NCReportPageOption::setPaperSize( const QString &paperSizeName, const QSizeF& customSize )
{
    QPrinter::PaperSize papersize = QPrinter::A4;
    QSizeF size;
    bool custom = (paperSizeName == "CUSTOM");
    if ( !custom )
        setPaperSizeByName( paperSizeName, m_pageOrientation, papersize, size );

    custom = (custom || !size.isValid());
    if ( custom ) {
        // CUSTOMS size
        m_pageMetricSize = customSize;
        m_paperSize = QPrinter::Custom;
    } else {
        m_pageMetricSize = size;
        m_paperSize = papersize;
    }
}

void NCReportPageOption::setOrientation( const QPrinter::Orientation orientation )
{
    m_pageOrientation = orientation;
    adjustPageSize();
}


void NCReportPageOption::loadPageDataVector( QVector<NCReportPageOption::PageData>& vector )
{
    vector << NCReportPageOption::PageData("A0",QPrinter::A0,841,1189);
    vector << NCReportPageOption::PageData("A1",QPrinter::A1,594,841);
    vector << NCReportPageOption::PageData("A2",QPrinter::A2,420,594);
    vector << NCReportPageOption::PageData("A3",QPrinter::A3,297,420);
    vector << NCReportPageOption::PageData("A4",QPrinter::A4,210,297);
    vector << NCReportPageOption::PageData("A5",QPrinter::A5,148,210);
    vector << NCReportPageOption::PageData("A6",QPrinter::A6,105,148);
    vector << NCReportPageOption::PageData("A7",QPrinter::A7,74,105);
    vector << NCReportPageOption::PageData("A8",QPrinter::A8,52,74);
    vector << NCReportPageOption::PageData("A9",QPrinter::A9,37,52);
    vector << NCReportPageOption::PageData("B0",QPrinter::B0,1030,1456);
    vector << NCReportPageOption::PageData("B1",QPrinter::B1,728,1030);
    vector << NCReportPageOption::PageData("B10",QPrinter::B10,32,45);
    vector << NCReportPageOption::PageData("B2",QPrinter::B2,515,728);
    vector << NCReportPageOption::PageData("B3",QPrinter::B3,364,515);
    vector << NCReportPageOption::PageData("B4",QPrinter::B4,257,364);
    vector << NCReportPageOption::PageData("B5",QPrinter::B5,182,257);
    vector << NCReportPageOption::PageData("B6",QPrinter::B6,128,182);
    vector << NCReportPageOption::PageData("B7",QPrinter::B7,91,128);
    vector << NCReportPageOption::PageData("B8",QPrinter::B8,64,91);
    vector << NCReportPageOption::PageData("B9",QPrinter::B9,45,64);
    vector << NCReportPageOption::PageData("C5",QPrinter::C5E,163,229);
    vector << NCReportPageOption::PageData("COMM10E",QPrinter::Comm10E,105,241);
    vector << NCReportPageOption::PageData("DLE",QPrinter::DLE,110,220);
    vector << NCReportPageOption::PageData("EXECUTIVE",QPrinter::Executive,191,254);
    vector << NCReportPageOption::PageData("FOLIO",QPrinter::Folio,210,330);
    vector << NCReportPageOption::PageData("LEDGER",QPrinter::Ledger,432,279);
    vector << NCReportPageOption::PageData("LEGAL",QPrinter::Legal,216,356);
    vector << NCReportPageOption::PageData("LETTER",QPrinter::Letter,216,279);
    vector << NCReportPageOption::PageData("TABLOID",QPrinter::Tabloid,279,432);
    vector << NCReportPageOption::PageData("CUSTOM",QPrinter::Custom,-1,-1);
}

void NCReportPageOption::setReportName(const QString& name )
{
    m_reportName = name;
}

void NCReportPageOption::setPageWidthMM(qreal width)
{
    m_pageMetricSize.setWidth(width);
}

void NCReportPageOption::setPageHeightMM(qreal height)
{
    m_pageMetricSize.setHeight(height);
}

void NCReportPageOption::setTopMarginMM(qreal margin)
{
    m_topMargin = margin;
}

void NCReportPageOption::setBottomMarginMM(qreal margin)
{
    m_bottomMargin = margin;
}

void NCReportPageOption::setLeftMarginMM(qreal margin)
{
    m_leftMargin = margin;
}

void NCReportPageOption::setRightMarginMM(qreal margin)
{
    m_rightMargin = margin;
}

void NCReportPageOption::setColumnCount(int columns)
{
    m_columnCount = columns;
}

void NCReportPageOption::setColumnWidth(qreal width)
{
    m_columnWidth = width;
}

void NCReportPageOption::setColumnSpacing(qreal spacing)
{
    m_columnSpacing = spacing;
}

void NCReportPageOption::setDefaultFont(const QFont& font)
{
    m_defaultFont = font;
}

void NCReportPageOption::setDefaultFontFamily(const QString& fontFamily)
{
    m_defaultFont.setFamily( fontFamily );
}

void NCReportPageOption::setDefaultFontSize(int pointsize)
{
    NCReportScale scale;
    m_defaultFont.setPointSize(pointsize);
    //m_defaultFont.setPixelSize(scale.fontPointSizeToPixel(pointsize));
}

void NCReportPageOption::setDefaultFontStyle(const QString &styleExpr)
{
    m_defaultFont.setBold(styleExpr.contains('B'));
    m_defaultFont.setItalic(styleExpr.contains('I'));
    m_defaultFont.setUnderline(styleExpr.contains('U'));
    m_defaultFont.setStrikeOut(styleExpr.contains('S'));
}

void NCReportPageOption::setEncoding(const QString& enc)
{
    m_encoding = enc;
}

void NCReportPageOption::setBackColor(const QColor& color)
{
    m_backcolor = color;
}

void NCReportPageOption::setZoneSpacing(qreal spacing)
{
    m_zoneSpacing = spacing;
}
void NCReportPageOption::setSectionSpacing(qreal spacing)
{
    m_sectionSpacing = spacing;
}

void NCReportPageOption::setDoublePassMode(bool set)
{
    m_doublePassMode = set;
}

void NCReportPageOption::setLanguages(const QString& languageCodes)
{
    if ( languageCodes.isEmpty() )
        m_languages = "00";
    else
        m_languages = languageCodes;

    if ( !m_languages.contains(m_currentLanguage) )
        m_currentLanguage = m_languages.left(2);
}

void NCReportPageOption::setCurrentLanguage(const QString& langCode )
{
    m_currentLanguage = langCode;
}

void NCReportPageOption::setLanguageRole( MultiLanguageRole role )
{
    m_langRole = role;
}

QString NCReportPageOption::reportName() const
{
    return m_reportName;
}
QSizeF NCReportPageOption::pageSizeMM() const
{
    return m_pageMetricSize;
}
qreal NCReportPageOption::pageWidthMM() const
{
    return m_pageMetricSize.width();
}
qreal NCReportPageOption::pageHeightMM() const
{
    return m_pageMetricSize.height();
}
qreal NCReportPageOption::topMarginMM() const
{
    return m_topMargin;
}
qreal NCReportPageOption::bottomMarginMM() const
{
    return m_bottomMargin;
}
qreal NCReportPageOption::leftMarginMM() const
{
    return m_leftMargin;
}
qreal NCReportPageOption::rightMarginMM() const
{
    return m_rightMargin;
}
int NCReportPageOption::columnCount() const
{
    return m_columnCount;
}
qreal NCReportPageOption::columnWidth() const
{
    return m_columnWidth;
}
qreal NCReportPageOption::columnSpacing() const
{
    return m_columnSpacing;
}
QFont NCReportPageOption::defaultFont() const
{
    return m_defaultFont;
}
QString NCReportPageOption::encoding() const
{
    return m_encoding;
}
QColor NCReportPageOption::backColor() const
{
    return m_backcolor;
}
qreal NCReportPageOption::zoneSpacing() const
{
    return m_zoneSpacing;
}
qreal NCReportPageOption::sectionSpacing() const
{
    return m_sectionSpacing;
}
bool NCReportPageOption::isDoublePassMode() const
{
    return m_doublePassMode;
}
QString NCReportPageOption::languages() const
{
    return m_languages;
}
QString NCReportPageOption::currentLanguage() const
{
    return m_currentLanguage;
}
QString NCReportPageOption::defaultLanguage() const
{
    return m_languages.left(2);
}
QPrinter::PaperSize NCReportPageOption::paperSize() const
{
    return m_paperSize;
}
QPrinter::Orientation NCReportPageOption::pageOrientation() const
{
    return m_pageOrientation;
}
NCReport::ReportType NCReportPageOption::reportType() const
{
    return m_reportType;
}

NCReportPageOption::MultiLanguageRole NCReportPageOption::languageRole() const
{
    return m_langRole;
}

bool NCReportPageOption::isMultiLanguage() const
{
    return m_languages!="00"; //.count(',')>0;
}

bool NCReportPageOption::zonesAreBreakable() const
{
    return m_zonesAreBreakable;
}

bool NCReportPageOption::subReportOnNewPage() const
{
    return m_subreportOnNewPage;
}

QString NCReportPageOption::htmlBodyStyle() const
{
    return m_htmlBodyStyle;
}

QString NCReportPageOption::htmlHead() const
{
    return m_htmlHead;
}

QString NCReportPageOption::defaultFontStyle() const
{
    QString style;
    if (m_defaultFont.bold())
        style += "B";
    if (m_defaultFont.italic())
        style += "I";
    if (m_defaultFont.underline())
        style += "U";
    if (m_defaultFont.strikeOut())
        style += "S";

    return style;
}

void NCReportPageOption::setZonesAreBreakable(bool set)
{
    m_zonesAreBreakable = set;
}

void NCReportPageOption::setSubReportOnNewPage(bool set)
{
    m_subreportOnNewPage = set;
}

void NCReportPageOption::setHtmlBodyStyle(const QString &options)
{
    m_htmlBodyStyle = options;
}

void NCReportPageOption::setHtmlHead(const QString &options)
{
    m_htmlHead = options;
}

void NCReportPageOption::setCustomData(const QString &data)
{
    m_customData = data;
}

QString NCReportPageOption::customData() const
{
    return m_customData;
}

/*!
 * \brief NCReportPageOption::isDataTrimming
 * \return
 * Returns true if data trimming mode is enabled. This setting is on by default.
 * Data trimming is an internal function for keeping the correct record status in group footers and page footers and page breaks.
 * If you don't want to print any detail related fields at group footer or at page footers you can turn this
 * setting off. The performance is better if this setting is off.
 */
bool NCReportPageOption::isDataTrimming() const
{
    return m_dataTrimming;
}

/*!
 * \brief NCReportPageOption::setDataTrimming
 * \param trimming
 * Turns data trimming on or off. \see isDataTrimming()
 */
void NCReportPageOption::setDataTrimming(bool trimming)
{
    m_dataTrimming = trimming;
}

bool NCReportPageOption::updateOnlyCurrentDataSourceVariables() const
{
    return m_updateOnlyCurrentDsVars;
}

void NCReportPageOption::setupdateOnlyCurrentDataSourceVariables(bool update)
{
    m_updateOnlyCurrentDsVars = update;
}
