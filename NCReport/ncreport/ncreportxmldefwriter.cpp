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
#include "ncreportxmldefwriter.h"
#include "ncreportdef.h"
#include "ncreporttxtdatasource.h"
#include "ncreportgroup.h"
#include <QHash>

NCReportXMLDefWriter::NCReportXMLDefWriter()
{
}

NCReportXMLDefWriter::NCReportXMLDefWriter(QString * string) : NCReportXMLWriter( string )
{
}

void NCReportXMLDefWriter::setReportDef(NCReportDef *rd )
{
    rdef = rd;
}

void NCReportXMLDefWriter::writeOptions()
{
    rdef->option().write( this );

}

void NCReportXMLDefWriter::writeDataSources()
{
    writeStartElement("datasources");

    QHash<QString, NCReportDataSource*>::const_iterator i = rdef->dataSources().constBegin();
    while (i != rdef->dataSources().constEnd()) {
        NCReportDataSource* ds = i.value();
        writeDataSource( ds );
        ++i;
    }
    writeEndElement();
}

void NCReportXMLDefWriter::writeDataSource(NCReportDataSource *ds)
{
    writeStartElement("datasource");
    writeAttribute("id", ds->id() );
    writeAttribute("allowEmpty", ds->allowEmpty() ? "true" : "false" );
    if (ds->primaryKeyColumn()>0)
        writeInt("primaryKeyColumn", ds->primaryKeyColumn());

    writeAttribute("enableSampling", ds->EnableSampling() ? "true" : "false" );
    writeAttribute("samplingField", ds->samplingField() );
    writeAttribute("samplingPeriodPara", ds->samplingPeriodPara() );
    writeAttribute("samplingNullGap", ds->samplingNullGap() );

    //写数据源类型 先将type转换成对应的字符串
    QString dst("custom");
    switch ( ds->dataSourceType() ) {
        case NCReportDataSource::Sql: dst="sql"; break;
        case NCReportDataSource::Text: dst="txt"; break;
        case NCReportDataSource::Xml: dst="xml"; break;
        case NCReportDataSource::StringList: dst="list"; break;
        case NCReportDataSource::ItemModel: dst="model"; break;
        case NCReportDataSource::Custom: dst="custom"; break;
        case NCReportDataSource::DataView: dst="dataview";break; //me:扩展
    }
    writeAttribute("type", dst );

    QString role("begin");
    switch ( ds->openRole() ) {
        case NCReportDataSource::BeginReport: role="begin"; break;
        case NCReportDataSource::ChildDataSource: role="child"; break;
        case NCReportDataSource::GroupChange: role="group"; break;
        case NCReportDataSource::SubReport: role="sub"; break;
    }
    writeAttribute("openRole", role );

    QString attrib;
    switch ( ds->locationType() ) {
        case NCReportDataSource::Static: attrib = "static"; break;
        case NCReportDataSource::File: attrib = "file"; break;
        case NCReportDataSource::Ftp: attrib = "ftp"; break;
        case NCReportDataSource::Http: attrib = "http"; break;
        case NCReportDataSource::Parameter: attrib = "parameter"; break;
    }
    writeAttribute("source", attrib );
    if  (!ds->briefDescription().isEmpty()) {
        writeAttribute( "briefDesc", ds->briefDescription() );
    }

    ds->write( this );
    writeCharacters( ds->locationType()==NCReportDataSource::Static ? ds->specText() : ds->rawFileName() );

    writeEndElement();
}

void NCReportXMLDefWriter::writeVariables()
{
    writeStartElement("variables");
    QHash<QString,NCReportVariable*>::const_iterator i = rdef->variables().begin();
    while (i != rdef->variables().end()) {
        NCReportVariable *var = i.value();
        writeVariable( var );
        ++i;
    }
    writeEndElement();
}

void NCReportXMLDefWriter::writeParameters()
{
    writeStartElement("parameters");
    NCReportXMLWriter::writeParameters(rdef->parameters());
    writeEndElement();
}

void NCReportXMLDefWriter::writeScripts()
{
    writeStartElement("scripts");
    QHash<QString,QString>::const_iterator i = rdef->scripts().begin();
    while (i != rdef->scripts().end()) {
        writeScript(i.key(),i.value());
        ++i;
    }
    writeEndElement();
}

void NCReportXMLDefWriter::writeVariable(NCReportVariable *variable)
{
    writeStartElement("variable");
    writeAttribute("id", variable->id() );
    writeAttribute("type", variable->typeName() );
    writeAttribute("funcType", variable->functionToName().toLower() );
    writeAttribute("scope", variable->scopeToName().toLower() );
    writeAttribute("initValue", QString::number(variable->initValue()) );
    writeCharacters( variable->expression() );
    writeEndElement();
}

void NCReportXMLDefWriter::writeGroups(NCReportSection *detail)
{
    if ( detail->groups().count() == 0 )
        return;

    writeStartElement("groups");
    for ( int j=0; j<detail->groups().count(); ++j ) {
        NCReportGroup *group = detail->groups().at(j);
        writeStartElement("group");
        writeAttribute("id", group->groupId() );
        writeAttribute("groupExp", group->expression() );
        writeAttribute("resetVariables", group->resetVarList() );
        writeAttribute("reprintHeader", group->repeatHeaderOnNextPage() ? "true" : "false" );
        writeAttribute("startsOnNewPage", group->startsOnNewPage() ? "true" : "false" );
        if (!group->pageBreakExp().isEmpty())
            writeAttribute("pageBreakExp", group->pageBreakExp() );

        if ( group->header() ) {
            writeStartElement("groupheader");
            writeSection( group->header() );
            writeItems( group->header() );
            writeEndElement();
        }
        if ( group->footer() ) {
            writeStartElement("groupfooter");
            writeSection( group->footer() );
            writeItems( group->footer() );
            writeEndElement();
        }
        writeEndElement();
    }
    writeEndElement(); // groups

}

void NCReportXMLDefWriter::writePageHeader()
{
    if (!rdef->pageHeader())
        return;
    writeStartElement("pageheader");
    writeSection( rdef->pageHeader() );
    writeItems( rdef->pageHeader() );
    writeEndElement();
}

void NCReportXMLDefWriter::writePageFooter()
{
    if (!rdef->pageFooter())
        return;
    writeStartElement("pagefooter");
    writeSection( rdef->pageFooter() );
    writeItems( rdef->pageFooter() );
    writeEndElement();
}

void NCReportXMLDefWriter::writeReportHeader()
{
    if (!rdef->reportHeader())
        return;
    writeStartElement("reportheader");
    writeSection( rdef->reportHeader() );
    writeItems( rdef->reportHeader() );
    writeEndElement();
}

void NCReportXMLDefWriter::writeReportFooter()
{
    if (!rdef->reportFooter())
        return;
    writeStartElement("reportfooter");
    writeSection( rdef->reportFooter() );
    writeItems( rdef->reportFooter() );
    writeEndElement();
}

void NCReportXMLDefWriter::writeDetails()
{
    writeStartElement("details");

    for ( int i=0; i<rdef->details().count(); ++i ) {
        NCReportSection *detail = rdef->details().at(i);
        writeStartElement("detail");

        writeSection( detail );

        if ( detail->dataSourceId().isEmpty() ) {
            // for safety - datasource automatic assignment to detail
            if ( rdef->dataSources().count() > 0 )
                detail->setDataSourceId( rdef->dataSources().constBegin().value()->id() );
        }
        writeAttribute("datasource", detail->dataSourceId() );
        if (!detail->secondaryDatasourceID().isEmpty())
            writeAttribute("datasource2", detail->secondaryDatasourceID() );

        if ( !detail->pageBreakExp().isEmpty() )
            writeAttribute("pageBreakExp", detail->pageBreakExp() );
        //writeAttribute("startsOnNewPage", detail->startsOnNewPage() ? "true" : "false" );

        writeStartElement("items");
        writeItems( detail );
        writeEndElement(); // end items

        //----------------------------
        // WRITE GROUPS
        //----------------------------
        writeGroups( detail );

        writeEndElement(); // end detail
    }

    writeEndElement();  // end details
}

QString NCReportXMLDefWriter::reportName() const
{
    return rdef->option().reportName();
}

QString NCReportXMLDefWriter::reportType() const
{
    return rdef->option().reportTypeName();
}




