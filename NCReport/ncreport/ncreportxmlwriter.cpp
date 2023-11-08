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
#include "ncreportxmlwriter.h"
#include "ncreportitem.h"
#include "ncreportsection.h"
#include "info.h"

NCReportXMLWriter::NCReportXMLWriter()
{
    setAutoFormatting(true);
    basetag = "NCReport";
}

NCReportXMLWriter::NCReportXMLWriter(QIODevice * device) : QXmlStreamWriter( device )
{
    setAutoFormatting(true);
    basetag = "NCReport";
    //write( 0 );
}

NCReportXMLWriter::NCReportXMLWriter(QString * string) : QXmlStreamWriter( string )
{
    setAutoFormatting(true);
    basetag = "NCReport";
    //write( 0 );
}

NCReportXMLWriter::~ NCReportXMLWriter()
{
}

bool NCReportXMLWriter::write(QIODevice *device)
{
    if ( device )
        setDevice(device);

    writeStartDocument();
    //writeDTD("<!DOCTYPE xbel>");
    writeStartElement(basetag);
    writeAttribute("version", QString(NCREPORTAPP_VERSION));
    writeAttribute("name", reportName() );
    writeAttribute("type", reportType() );

    writeReport();

    writeEndDocument();
    return true;
}

/*
void NCReportXMLWriter::writeItem(QTreeWidgetItem *item)
{
    QString tagName = item->data(0, Qt::UserRole).toString();
    if (tagName == "folder") {
        bool folded = !treeWidget->isItemExpanded(item);
        writeStartElement(tagName);
        writeAttribute("folded", folded ? "yes" : "no");
        writeTextElement("title", item->text(0));
        for (int i = 0; i < item->childCount(); ++i)
            writeItem(item->child(i));
        writeEndElement();
    } else if (tagName == "bookmark") {
        writeStartElement(tagName);
        if (!item->text(1).isEmpty())
            writeAttribute("href", item->text(1));
        writeTextElement("title", item->text(0));
        writeEndElement();
    } else if (tagName == "separator") {
        writeEmptyElement(tagName);
    }
}
*/

void NCReportXMLWriter::writeReport()
{
    writeOptions();
    writeDataSources();
    writeParameters();
    writeVariables();
    writeScripts();
    writePageHeader();
    writePageFooter();
    writeReportHeader();
    writeReportFooter();
    writeDetails();
}

void NCReportXMLWriter::writeOptions()
{
/*	writeStartElement("options");
    writeTextElement("encoding",  encoding );
    writeEndElement();*/
}

void NCReportXMLWriter::writeDataSources()
{
    //writeStartElement("datasources");
    //writeEndElement();
}

void NCReportXMLWriter::writeDataSource(NCReportDataSource*)
{
}

void NCReportXMLWriter::writeFont(const QFont & f )
{
    writeAttribute("fontName", f.family() );
    writeAttribute("fontSize", QString::number(f.pointSize()) );
    if ( f.weight() != 	QFont::Normal ) {
        QString w("normal");
        if ( f.weight() == 25 )
            w = "light";
        else if ( f.weight() == 63 )
            w = "demibold";
        else if ( f.weight() == 75 )
            w = "bold";
        else if ( f.weight() >= 87 )
            w = "black";
        writeAttribute("fontWeight", w );
    }
    if ( f.italic() )
        writeAttribute("fontItalic", "true" );
    if ( f.underline() )
        writeAttribute("fontUnderline", "true" );
    if ( f.strikeOut() )
        writeAttribute("fontStrikeOut", "true" );
    if ( f.letterSpacing()!=0 )
        writeReal("letterSpacing", f.letterSpacing() );
    if ( f.capitalization()!=QFont::MixedCase)
        writeInt("capital", (int)f.capitalization() );
}

void NCReportXMLWriter::writePen(const QPen & pen )
{
    writeAttribute("lineColor", pen.color().name() );
    QString ls("solid");
    switch ( pen.style() ) {
        case Qt::SolidLine: ls="solid"; break;
        case Qt::DashLine: ls="dash"; break;
        case Qt::DotLine: ls="dot"; break;
        case Qt::DashDotDotLine: ls="dashdotdot"; break;
        case Qt::DashDotLine: ls="dashdot"; break;
        case Qt::NoPen: ls="nopen"; break;
        default: break;
    }
    writeAttribute("lineStyle", ls );
    //NCReportScale s;
    //writeAttribute("lineWidth", qrealToString(s.pixelToMM( pen.widthF())) );
}

void NCReportXMLWriter::writeBrush(const QBrush &b )
{
    QString fs("solid");
    switch ( b.style() ) {
        case Qt::NoBrush: fs="no"; break;
        case Qt::SolidPattern:  fs="solid"; break;
        case Qt::Dense1Pattern: fs="dense1"; break;
        case Qt::Dense2Pattern: fs="dense2"; break;
        case Qt::Dense3Pattern: fs="dense3"; break;
        case Qt::Dense4Pattern: fs="dense4"; break;
        case Qt::Dense5Pattern: fs="dense5"; break;
        case Qt::Dense6Pattern: fs="dense6"; break;
        case Qt::Dense7Pattern: fs="dense7"; break;
        case Qt::HorPattern: fs="hor"; break;
        case Qt::VerPattern: fs="ver"; break;
        case Qt::CrossPattern: fs="cross"; break;
        case Qt::BDiagPattern: fs="bdiag"; break;
        case Qt::FDiagPattern: fs="fdiag"; break;
        case Qt::DiagCrossPattern: fs="diagcross"; break;
        default: break;
    }
    writeAttribute("fillStyle", fs );
    writeAttribute("fillColor", b.color().name() );
}

void NCReportXMLWriter::writePos(const QPointF & point )
{
    writeAttribute("posX", qrealToString(point.x()) );
    writeAttribute("posY", qrealToString(point.y()) );
}

void NCReportXMLWriter::writePosFrom(const QPointF & point )
{
    writeAttribute("fromX", qrealToString(point.x()) );
    writeAttribute("fromY", qrealToString(point.y()) );
}

void NCReportXMLWriter::writePosTo(const QPointF & point )
{
    writeAttribute("toX", qrealToString(point.x()) );
    writeAttribute("toY", qrealToString(point.y()) );
}

void NCReportXMLWriter::writeSize(const QSizeF & size )
{
    writeAttribute("width", qrealToString(size.width()) );
    writeAttribute("height", qrealToString(size.height()) );
}

void NCReportXMLWriter::writeWidth(const qreal w )
{
    writeAttribute("width", qrealToString(w) );
}

void NCReportXMLWriter::writeHeight(const qreal h)
{
    writeAttribute("height", qrealToString(h) );
}

void NCReportXMLWriter::writeAlignment(const Qt::Alignment al )
{
    if (al == (Qt::AlignLeft | Qt::AlignTop))
        return;		//default
    writeAttribute("alignment", QString::number(al) );
}

/*
void NCReportXMLWriter::writeAlignmentV(const Qt::Alignment al )
{
    QString als("top");
    switch ( al ) {
        case Qt::AlignTop: als="top"; break;
        case Qt::AlignBottom: als="bottom"; break;
        case Qt::AlignVCenter: als="center"; break;
    }
    writeAttribute("alignmentV",als);
}

void NCReportXMLWriter::writeAlignmentH(const Qt::Alignment al )
{
    QString als("left");
    switch ( al ) {
        case Qt::AlignLeft: als="left"; break;
        case Qt::AlignRight: als="right"; break;
        case Qt::AlignHCenter: als="center"; break;
    }
    writeAttribute("alignmentH",als);
}
*/

void NCReportXMLWriter::writeVariables()
{
}

void NCReportXMLWriter::writeParameters()
{
}

void NCReportXMLWriter::writeScripts()
{
}

void NCReportXMLWriter::writeReportHeader()
{
}

void NCReportXMLWriter::writePageFooter()
{
}

void NCReportXMLWriter::writePageHeader()
{
}

void NCReportXMLWriter::writeVariable(NCReportVariable*)
{
}

void NCReportXMLWriter::writeReportFooter()
{
}

void NCReportXMLWriter::writeDetails()
{
}

void NCReportXMLWriter::writeItems(NCReportSection *sect)
{
    /* To keep the existing z-order of item they must be sort by zValue() before writing*/
    QMultiMap<qreal,NCReportItem*> map;

    QList<QGraphicsItem*> list = sect->items();
    for (int i = 0; i < list.size(); ++i) {
        NCReportItem *item = (NCReportItem*)list.at(i);
        map.insert( item->zValue(), item );
    }

    QMap<qreal,NCReportItem*>::const_iterator it = map.constBegin();
    while (it != map.constEnd()) {
        NCReportItem *item = it.value();
        item->updateForSave();
        item->write( this );
        ++it;
    }
}

QString NCReportXMLWriter::reportName() const
{
    return "noname";
}

QString NCReportXMLWriter::reportType() const
{
    return "report";
}

QString NCReportXMLWriter::qrealToString(const qreal num ) const
{
    return QString("%1").arg( num, 0,'f',3 );
}

void NCReportXMLWriter::writeLineWidth(const qreal lw )
{
    writeAttribute("lineWidth", qrealToString(lw) );
}

void NCReportXMLWriter::writeSection(NCReportSection *section)
{
    if ( !section->id().isEmpty() )
        writeAttribute("id", section->id() );

    writeHeight( section->metricHeight() );
    if ( !section->printWhenExp().isEmpty() )
        writeAttribute("printWhen", section->printWhenExp() );
    if ( section->isAutoHeight() )
        writeAttribute("autoHeight", "true" );
    if ( section->hideIfBlank() )
        writeAttribute("hideIfBlank", "true" );
    if ( section->checkRole() != NCReportSection::SectionOnly )
        writeAttribute("breakRole", (section->checkRole() == NCReportSection::SectionPlusDetail ? "spd" : "so") );
    if ( section->orphanedTolaranceMM()>0 )
        writeAttribute("orphanedTolerance", QString::number(section->orphanedTolaranceMM()) );
    if ( section->startsOnNewPage() )
        writeAttribute("startsOnNewPage", "true" );
    if (section->isAnchorToPageFooter())
        writeAttribute("anchorToPageFooter","true");
    if (section->isNotAlone())
        writeAttribute("notAlone","true");
    if ( !section->htmlOptions().isEmpty() )
        writeAttribute("htmlOpt", section->htmlOptions() );

    switch (section->alternateRowBackgroundMode())
    {
    case NCReportSection::Disabled: break;
    case NCReportSection::Odd: writeAttribute("altBackRowMode","odd"); break;
    case NCReportSection::Even: writeAttribute("altBackRowMode","even"); break;
    }
    if (section->alternateRowBackgroundMode()!=NCReportSection::Disabled) {
        writeBool("useDefaultAltBackRowColor", section->useDefaultAlternateRowColor());
        writeAttribute("altBackRowColor", section->alternateRowBackgroundColor().name() );
    }
}

void NCReportXMLWriter::writeBool(const QString &property, bool value)
{
    writeAttribute( property, value ? "1" : "0");
}

void NCReportXMLWriter::writeReal(const QString &property, qreal value)
{
    writeAttribute( property, QString::number(value) );
}

void NCReportXMLWriter::writeInt(const QString &property, int value)
{
    writeAttribute( property, QString::number(value) );
}

void NCReportXMLWriter::writeParameters( const QHash<QString, QVariant>& parameters )
{
    QHash<QString, QVariant>::const_iterator i = parameters.constBegin();
    while (i != parameters.constEnd()) {
        writeParameter("par", i.key(), i.value());
        ++i;
    }
}

void NCReportXMLWriter::writeParameter(const QString &tagName, const QString& id, const QVariant &value)
{
    writeStartElement(tagName);
    writeAttribute("id", id);
    writeAttribute("type",value.typeName());
    writeCharacters( value.toString() );
    writeEndElement();
}

void NCReportXMLWriter::writeScript(const QString &id, const QString &script)
{
    writeStartElement("script");
    writeAttribute("id", id);
    writeCharacters( script );
    writeEndElement();
}


