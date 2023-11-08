/****************************************************************************
*
*  Copyright (C) 2002-2014 Helta Ltd. - NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: office@nocisoft.com
*  Web: www.nocisoft.com
*
*  This file is part of the NCReport reporting software
*  Created: 2014.03.25. (nocisoft)
*
*  Licensees holding a valid NCReport License Agreement may use this
*  file in accordance with the rights, responsibilities, and obligations
*  contained therein. Please consult your licensing agreement or contact
*  office@nocisoft.com if any conditions of this licensing are not clear
*  to you.
*
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#include "ncreportexcelhtmloutput.h"
#include "ncreportlabelitem.h"
#include "ncreportfielditem.h"
#include "ncreporttextitem.h"
#include "ncreportimageitem.h"
#include "ncreportbarcodeitem.h"

#include "ncreportsection.h"


NCReportExcelHtmlOutput::NCReportExcelHtmlOutput(QObject *parent) : NCReportHtmlOutput(parent)
{
    setOutput(ExcelHtml);
    setImageMode(File); // Excel cannot handle embedded base64 images.
    setStrategy(SectionOneTable);
}

NCReportExcelHtmlOutput::~ NCReportExcelHtmlOutput()
{
    qDebug("NCReportExcelHtmlOutput deleted.");
}

void NCReportExcelHtmlOutput::writeHeader()
{
    contentStream() << "<html xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:x=\"urn:schemas-microsoft-com:office:excel\" ";
    //contentStream() << "xmlns=\"http://www.w3.org/TR/REC-html40\" lang=\"" <<  option().defaultLanguage() << "\">" << crlf();
    contentStream() << "xmlns=\"http://www.w3.org/TR/REC-html40\">" << crlf();

    contentStream() << "<head>" << crlf();
    contentStream() << "<title>" << option().reportName() << "</title>" << crlf();
    //contentStream() << "<meta http-equiv=\"CACHE-CONTROL\" content=\"NO-STORE\" />" << crlf();
    contentStream() << "<meta content='application/vnd.ms-excel;charset=" << option().encoding() << "' http-equiv=Content-type />" << crlf();
    contentStream() << "<meta name=ProgId content=Excel.Sheet />" << crlf();
    contentStream() << "<meta name=Generator content=\"Microsoft Excel 11\">" << crlf();

    contentStream() << "<style type=\"text/css\">" << crlf();
    contentStream() << "<!--table" << crlf();
    contentStream() << "@page{}" << crlf();
    contentStream() << "-->" << crlf();
    //contentStream() << styleSheetMarker() << crlf();
    contentStream() << "</style>" << crlf();

    contentStream() << "<!--[if gte mso 9]><xml>" << crlf();
    contentStream() << "  <x:ExcelWorkbook><x:ExcelWorksheets><x:ExcelWorksheet>" << crlf();
    contentStream() << "    <x:Name>Sheet1</x:Name>" << crlf();
    contentStream() << "    <x:WorksheetOptions><x:Panes>" << crlf();
    contentStream() << "    </x:Panes></x:WorksheetOptions>" << crlf();
    contentStream() << "    </x:ExcelWorksheet></x:ExcelWorksheets>" << crlf();
    contentStream() << "  </x:ExcelWorkbook>" << crlf();
    contentStream() << "</xml>" << crlf();
    contentStream() << "<![endif]-->" << crlf();

    contentStream() << "</head>" << crlf() << "<body>" << crlf();
}

void NCReportExcelHtmlOutput::writeFooter()
{
    contentStream() << "</body>" << crlf() << "</html>";
}

void NCReportExcelHtmlOutput::writeSpace(NCReportSection *section, int row, int width)
{
    Q_UNUSED(section)
    Q_UNUSED(row)
    contentStream() << "<td width=\"" << width << "\">&nbsp;</td>" << crlf();
}


void NCReportExcelHtmlOutput::writeLabel(NCReportLabelItem *item, QTextStream& cellStyle)
{
    setFontAttributesStart(item);

    Q_UNUSED(cellStyle)
    if (item->objectType() == NCReportItem::Field && item->dataType() == NCReportData::Numeric) {
        NCReportFieldItem *field = (NCReportFieldItem*)item;
        contentStream() << field->value().toDouble();
    } else {
        contentStream() << item->printableText().trimmed();
    }

    setFontAttributesEnd(item);
}


void NCReportExcelHtmlOutput::writeSectionStart(NCReportSection *section)
{
    contentStream() << "<!-- SECTION: " << fixedID(section->id()) << " -->" << crlf();
    if (strategy()==SectionOneTable)
        contentStream() << "<table class=\"" << fixedID(section->id()) << "\">" << crlf();

}

void NCReportExcelHtmlOutput::writeSectionEnd(NCReportSection *section)
{
    if (strategy()==SectionOneTable)
        contentStream() << "</table>" << crlf();

#ifdef HTMLOUTPUT_DEBUGMODE
    if (section)
        contentStream() << "<!-- END SECTION: " << fixedID(section->id()) << " -->" << crlf();
#else
    Q_UNUSED(section)
#endif
}

void NCReportExcelHtmlOutput::writeRowStart(NCReportSection *section)
{
    if (strategy()==SectionMultiTables) {
        contentStream() << "<table class=\"" << fixedID(section->id()) << "\">" << crlf();
    }
    contentStream() << "<tr>" << crlf();
}

void NCReportExcelHtmlOutput::writeRowEnd(NCReportSection *section)
{
    Q_UNUSED(section)
    contentStream() << "</tr>";

    if (strategy()==SectionMultiTables)
        contentStream() << "</table>";

    contentStream() << crlf();
}

QString NCReportExcelHtmlOutput::styleSheetReference(const QString &css) const
{
    QString s;
    s +="<link rel=\"stylesheet\" type=\"text/css\" href=\"" + css + "\">";
    return s;
}

void NCReportExcelHtmlOutput::cellStarts(NCReportItem *item, QTextStream& cellStyle)
{
    Q_UNUSED(cellStyle)
    QString style;

    contentStream() << "<td";

    if (item->objectType()==NCReportItem::Label || item->objectType()==NCReportItem::Field)  {
        NCReportLabelItem *label = (NCReportLabelItem*)item;
        if (!label->isWordBreak())
            contentStream() << " nowrap=\"nowrap\"";

        Qt::Alignment a = label->alignment();
        contentStream() << " align=\"" << alignmentName(a) << "\"";

        if (label->foreColor() != Qt::black)
            style += ";color:" + label->foreColor().name();

        if (label->backgroundMode()==NCReportLabelItem::FilledBackground && label->backColor()!= Qt::white)
            style += ";background-color:" + label->backColor().name();

        if (item->objectType()==NCReportItem::Field) {
            NCReportFieldItem *field = (NCReportFieldItem*)item;

            if (field->dataType()==NCReportData::Numeric) {
                QString zero("0");
                if ( field->isNumFormat()) {
                    style += ";vnd.ms-excel.numberformat:#,##0." + zero.repeated(field->precision());
                } else {
                    style += ";vnd.ms-excel.numberformat:#,##0";
                }
            }
        }
    }

    if (!item->htmlOptions().isEmpty())
        contentStream() << " " << item->htmlOptions();

    if (style.startsWith(';'))
        style.remove(0,1);

    if (!style.isEmpty())
        contentStream() << " style=\"" << style << "\"";

    contentStream() << ">";
}

void NCReportExcelHtmlOutput::cellEnds(NCReportItem *item)
{
    Q_UNUSED(item)
    contentStream() << "</td>" << crlf();
}

QString NCReportExcelHtmlOutput::alignmentName(const Qt::Alignment a) const
{
    QString as;
    if (a & Qt::AlignLeft) {
        as = "left";
    } else if (a & Qt::AlignHCenter) {
        as = "center";
    } else if (a & Qt::AlignRight) {
        as = "right";
    } else if (a & Qt::AlignJustify) {
        as = "justify";
    }
    return as;
}

void NCReportExcelHtmlOutput::fixNumericStringValue(QString &value)
{
    if ( value.contains(',') && !value.contains('.') )
        value.replace(',',".");
    if (value.contains(' '))
        value.replace(' ',QString());
}

void NCReportExcelHtmlOutput::setFontAttributes(NCReportLabelItem *label, QString &value)
{
    if (label->fontRef().bold()) {
        value.prepend("<b>");
        value.append("</b>");
    }
    if (label->fontRef().italic()) {
        value.prepend("<i>");
        value.append("</i>");
    }
    if (label->fontRef().underline()) {
        value.prepend("<u>");
        value.append("</u>");
    }
    if (label->fontRef().strikeOut()) {
        value.prepend("<s>");
        value.append("</s>");
    }
}

void NCReportExcelHtmlOutput::setFontAttributesStart(NCReportLabelItem *label)
{
    if (label->fontRef().bold())
        contentStream() << "<b>";
    if (label->fontRef().italic())
        contentStream() << "<i>";
    if (label->fontRef().underline())
        contentStream() << "<u>";
    if (label->fontRef().strikeOut())
        contentStream() << "<s>";
}

void NCReportExcelHtmlOutput::setFontAttributesEnd(NCReportLabelItem *label)
{
    if (label->fontRef().strikeOut())
        contentStream() << "</s>";
    if (label->fontRef().underline())
        contentStream() << "</u>";
    if (label->fontRef().italic())
        contentStream() << "</i>";
    if (label->fontRef().bold())
        contentStream() << "</b>";
}

void NCReportExcelHtmlOutput::writeText(NCReportTextItem * item)
{
    contentStream() << "<div width=\"" << item->paintRect().width() << "\">" + crlf();
    QString content(item->htmlContent());
    cleanHtml( content );
    contentStream() << content << crlf();
    contentStream() << "</div>" << crlf();
}


