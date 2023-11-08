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
#include "ncreportxmldefreader.h"
#include "ncreportdef.h"
#include "ncreportpageoption.h"
#ifndef NCREPORT_NO_SQL
#include "ncreportsqldatasource.h"
#endif
#include "ncreporttxtdatasource.h"
#include "ncreportstringlistdatasource.h"
#include "ncreportitemmodeldatasource.h"
#include "ncreportxmldatasource.h"
#include "ncreportdataviewdatasource.h"
#include "ncreportgroup.h"
#include "ncreportitem.h"
#include "ncreportfielditem.h"
#include "ncreportcustomdsprovider.h"
#include "ncreportdatasourcerelation.h"

NCReportXMLDefReader::NCReportXMLDefReader(NCReportDef * r )
{
    rdef = r;
    init();
}

void NCReportXMLDefReader::init()
{
    basetag = "NCReport";
    setPageOption( &rdef->option() );
}

NCReportXMLDefReader::~ NCReportXMLDefReader()
{
}

void NCReportXMLDefReader::readDataSources()
{
    Q_ASSERT(isStartElement() && name() == "datasources");
    while (!atEnd()) {
        readNext();
        if (isEndElement())
            break;
        if (isStartElement()) {
            if (name() == "datasource")
                readDataSource();
            else
                readUnknownElement();
        }
    }
    organizeDataSources();
}




void NCReportXMLDefReader::readDataSource()
{
    Q_ASSERT(isStartElement() && name() == "datasource");

    NCReportDataSource *ds = 0;
    NCReportDataSource::DataSourceType dstype = NCReportDataSource::Sql;
    NCReportDataSource::LocationType locType = NCReportDataSource::Static;
    NCReportDataSource::OpenRoles role = NCReportDataSource::BeginReport;
    QString dsID = attribToString("id");

    //2013.07.03
    if (rdef->dataSources().contains(dsID)) {
        // it already exists
        readElementText();
        return;
    }

    QStringRef dtype = attrib("type");
    if (dtype=="sql") dstype = NCReportDataSource::Sql;
    else if (dtype=="txt") dstype = NCReportDataSource::Text;
    else if (dtype=="xml") dstype = NCReportDataSource::Xml;
    else if (dtype=="list") dstype = NCReportDataSource::StringList;
    else if (dtype=="model") dstype = NCReportDataSource::ItemModel;
    else if (dtype=="custom") dstype = NCReportDataSource::Custom;
    else if(dtype=="dataview") dstype = NCReportDataSource::DataView; //me:扩展
    else {
        raiseError(QObject::tr("Invalid datasource type: %1").arg(dtype.toString()) );
        return;
    }
    if ( isAttrib("source") ) {
        QStringRef s = attrib("source");
        if ( s.isEmpty() || s=="static" )
            locType = NCReportDataSource::Static;
        else if ( s=="file" )
            locType = NCReportDataSource::File;
        else if ( s=="http" )
            locType = NCReportDataSource::Http;
        else if ( s=="ftp" )
            locType = NCReportDataSource::Ftp;
        else if ( s=="parameter" )
            locType = NCReportDataSource::Parameter;
    }
    if ( isAttrib("openRole") ) {
        QStringRef s = attrib("openRole");
        if ( s.isEmpty() || s=="begin" )
            role = NCReportDataSource::BeginReport;
        else if ( s=="child" )
            role = NCReportDataSource::ChildDataSource;
        else if ( s=="group" )
            role = NCReportDataSource::GroupChange;
        else if ( s=="sub" )
            role = NCReportDataSource::SubReport;
    }
    //--------------------------------------------------------------
    // YET ANOTHER CUSTOM DS PROVIDER ENTRY POINT
    // IT IS GOOD FOR CUSTOM DATA SOURCE WITH SPECIFIED DS.TYPE
    // SUCH AS SQL.
    //--------------------------------------------------------------
    if (rdef->customDSProvider()) {
        ds = rdef->customDSProvider()->createCustomDataSource( dsID, dstype, rdef );
    } else {
        switch (dstype) {
            case NCReportDataSource::Sql:
#ifndef NCREPORT_NO_SQL
                ds = new NCReportSQLDataSource( rdef );
#endif
                break;
            case NCReportDataSource::Text:
                ds = new NCReportTextDataSource( rdef );
                break;
            case NCReportDataSource::StringList:
                ds = new NCReportStringListDataSource( rdef );
                break;
            case NCReportDataSource::ItemModel:
                ds = new NCReportItemModelDataSource( rdef );
                break;
            case NCReportDataSource::Xml:
                ds = new NCReportXMLDataSource( rdef );
                break;
            case NCReportDataSource::Custom:
                ds = createCustomDataSource( dsID ); // Get custom datasource reference
                break;
            case NCReportDataSource::DataView:
                ds = new NCReportDataViewDataSource( rdef ); //me:扩展
                break;
        }
    }
    if (ds) {
        ds->setID( dsID );
        ds->setLocationType( locType );
        ds->setOpenRole( role );

        if (isAttrib("briefDesc")) {
            ds->setBriefDescription( attribToString("briefDesc") );
        }
        if (isAttrib("emptyIsForbidden"))
            ds->setAllowEmpty( !attribToBool("emptyIsForbidden") );
        if (isAttrib("allowEmpty"))
            ds->setAllowEmpty( attribToBool("allowEmpty") );
        if (isAttrib("primaryKeyColumn"))
            ds->setPrimaryKeyColumn(attribToInt("primaryKeyColumn"));

        if (isAttrib("enableSampling"))
            ds->setEnableSampling( attribToBool("enableSampling") );
        if (isAttrib("samplingField"))
            ds->setSamplingField( attribToString("samplingField") );
        if (isAttrib("samplingPeriodPara"))
            ds->setSamplingPeriodPara( attribToString("samplingPeriodPara") );
        if (isAttrib("samplingNullGap"))
            ds->setSamplingNullGap( attribToString("samplingNullGap") );
        ds->read( this );	// read custom properties
        if (ds->connectionID().isEmpty())
            ds->setConnectionID(rdef->connectionID());

        QString text = readElementText().trimmed();

        switch ( locType ) {
            case NCReportDataSource::Static:
                ds->setSpecText( text );
                break;
            case NCReportDataSource::Parameter: {
                //NCReportParameter *par = rdef->parameters().value( text );
                QVariant par = rdef->parameters().value( text );

                if ( par.isValid() ) {
                    ds->setSpecText( par.toString() );
                    //ds->setSpecText( par->valueRef().toString() );
                }
                ds->setFileName( text );
                break;
            }
            case NCReportDataSource::File:
            case NCReportDataSource::Http:
            case NCReportDataSource::Ftp:
                ds->setFileName( text );
                break;
        }

        rdef->addDataSource(ds);
    } else {
        readElementText();
    }
}

/*
NCReportDataSource * NCReportXMLDefReader::createCustomDataSource( const QString& dsID )
{
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("Load custom datasource: %s", qPrintable(dsID) );
#endif
    NCReportDataSource *ds = rdef->dataSources().value( dsID );
    if (ds == 0)
    {
        ds = rdef->dataSourceFactory().createCustomDataSource( dsID );
    }
    return ds;
}
*/

NCReportDataSource * NCReportXMLDefReader::createCustomDataSource( const QString& dsID )
{
   qDebug("Load custom datasource: %s", qPrintable(dsID) );
   NCReportDataSource *ds = rdef->dataSource( dsID );
   if ( !ds)
   {
      NCReportDef *rptDef = reportDef();
      NCReportCustomDSProvider *dsProvider = rptDef->customDSProvider();
      if ( dsProvider)
         ds = dsProvider->createCustomDataSource( dsID, NCReportDataSource::Custom, rdef );
      else
         ds = rdef->dataSourceFactory().createCustomDataSource( dsID );
   }
   return ds;
}


void NCReportXMLDefReader::readVariables()
{
    Q_ASSERT(isStartElement() && name() == "variables");
    while (!atEnd()) {
        readNext();
        if (isEndElement())
            break;
        if (isStartElement()) {
            if (name() == "variable")
                readVariable();
            else
                readUnknownElement();
        }
    }
}

void NCReportXMLDefReader::readVariable()
{
    Q_ASSERT(isStartElement() && name() == "variable");

    NCReportVariable *var = new NCReportVariable();
    var->setID( attributes().value("id").toString() );
    var->setFunction( attribToString("funcType") );
    var->setScope( attribToString("scope") );

    double initVal = attributes().value("initValue").toString().toDouble();
    var->setInitValue( initVal );

    var->setExpression( readElementText() );

    var->setValue( 0, NCReportData::Numeric );
    rdef->addVariable(var);
}

void NCReportXMLDefReader::readScripts()
{
    Q_ASSERT(isStartElement() && name() == "scripts");
    while (!atEnd()) {
        readNext();
        if (isEndElement())
            break;
        if (isStartElement()) {
            if (name() == "script")
                readScript();
            else
                readUnknownElement();
        }
    }
}

void NCReportXMLDefReader::readScript()
{
    Q_ASSERT(isStartElement() && name() == "script");
    QString id = attributes().value("id").toString();
    rdef->addScript(id, readElementText());
}

void NCReportXMLDefReader::readParameters()
{
    NCReportXMLReader::readParameters(rdef->parametersRef());
}

void NCReportXMLDefReader::readPageHeader()
{
    Q_ASSERT(isStartElement() && name() == "pageheader");
    NCReportSection *section = rdef->addPageHeader();
    readSection( section );
    readItems( section );
}
void NCReportXMLDefReader::readPageFooter()
{
    Q_ASSERT(isStartElement() && name() == "pagefooter");
    NCReportSection *section = rdef->addPageFooter();
    readSection( section );
    readItems( section );
}
void NCReportXMLDefReader::readReportHeader()
{
    Q_ASSERT(isStartElement() && name() == "reportheader");
    NCReportSection *section = rdef->addReportHeader();
    readSection( section );
    readItems( section );
}

void NCReportXMLDefReader::readReportFooter()
{
    Q_ASSERT(isStartElement() && name() == "reportfooter");
    NCReportSection *section = rdef->addReportFooter();
    readSection( section );
    readItems( section );
}

void NCReportXMLDefReader::readDetails()
{
    Q_ASSERT(isStartElement() && name() == "details");
    while (!atEnd()) {
        readNext();
        if (isEndElement())
            break;
        if (isStartElement()) {
            if (name() == "detail")
                readDetail();
            else
                readUnknownElement();
        }
    }
}

void NCReportXMLDefReader::readDetail()
{
    Q_ASSERT(isStartElement() && name() == "detail");

    NCReportSection* d = rdef->addDetail();
    readSection( d );
    d->setDataSourceId( attribToString("datasource") );
    d->setSecondaryDatasourceID( attribToString("datasource2") );
    d->setPageBreakExp( attribToString("pageBreakExp") );
    d->setDataSource( rdef->dataSource(d->dataSourceId()) );  // set datasource;

    while (!atEnd()) {
        readNext();
        if (isEndElement())
            break;
        if (isStartElement()) {
            if (name() == "items")
                readItems( d );
            else if (name() == "groups")
                readGroups( d );
            else
                readUnknownElement();
        }
    }
}



void NCReportXMLDefReader::readGroups( NCReportSection* d )
{
    Q_ASSERT(isStartElement() && name() == "groups");
    while (!atEnd()) {
        readNext();
        if (isEndElement())
            break;
        if (isStartElement()) {
            if (name() == "group") readGroup( d );
            else
                readUnknownElement();
        }
    }
}

void NCReportXMLDefReader::readGroup( NCReportSection* d )
{
    Q_ASSERT(isStartElement() && name() == "group");
    NCReportGroup *g = rdef->addGroup( d );
    g->setId( attribToString("id") );
    g->setGroupExp(attribToString("groupExp"));
    if (g->expression()=="0")
        g->setGroupExp("%CONST");
    g->setResetVarList(attribToString("resetVariables"));
    g->setReprintHeaderOnNextPage( attribToBool("reprintHeader") );
    g->setStartsOnNewPage( attribToBool("startsOnNewPage") );
    if (isAttrib("pageBreakExp"))
        g->setPageBreakExp( attribToString("pageBreakExp") );

    while (!atEnd()) {
        readNext();
        if (isEndElement())
            break;
        if (isStartElement()) {
            if (name() == "groupheader") readGroupHeader( g );
            else if (name() == "groupfooter") readGroupFooter( g );
            else readUnknownElement();
        }
    }
}

void NCReportXMLDefReader::readGroupHeader( NCReportGroup* g )
{
    Q_ASSERT(isStartElement() && name() == "groupheader");

    NCReportSection * gh = rdef->addGroupHeader( g );
    gh->setID( QString("GH_")+g->groupId() );
    readSection( gh );
    readItems( gh );
}

void NCReportXMLDefReader::readGroupFooter( NCReportGroup* g )
{
    Q_ASSERT(isStartElement() && name() == "groupfooter");

    NCReportSection * gf = rdef->addGroupFooter( g );
    gf->setID( QString("GF_")+g->groupId() );
    readSection( gf );
    readItems( gf );
}

void NCReportXMLDefReader::readField(NCReportFieldItem *item )
{
    rdef->addFieldItem(item); // register field
}


void NCReportXMLDefReader::afterReadItem( NCReportItem* item )
{
    if ( item->isDynamic() )
        rdef->addDynamicItem( item );   // register item
}

void NCReportXMLDefReader::setDefaultFont(QFont & font)
{
    font = rdef->option().defaultFont();
}

void NCReportXMLDefReader::organizeDataSources()
{
    // find the parents
    QHash<QString, NCReportDataSource*>::const_iterator i = rdef->dataSources().constBegin();
    while (i != rdef->dataSources().constEnd()) {
        NCReportDataSource *ds = i.value();
        if ( ds->openRole() == NCReportDataSource::ChildDataSource && rdef->dataSources().contains(ds->parentId()) )
            ds->setParentDataSource(rdef->dataSources()[ds->parentId()]);
        ++i;
    }
}


