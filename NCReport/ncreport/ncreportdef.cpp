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
#include "ncreportdef.h"
#include "ncreportgroup.h"
#include "ncreportabstractitemrendering.h"
#include "ncreportdatasourcerelation.h"

#include <QAbstractItemModel>
#include <QSettings>
#include <QFile>
#include <QApplication>
#include <QDir>

NCReportError::NCReportError() : m_error(false), m_errorcode(0), m_errormsg( QString() )
{
}

void NCReportError::setError(const QString & msg, int errcode)
{
    m_errorcode = errcode;
    m_error = !msg.isEmpty();
    m_errormsg = msg;
}

bool NCReportError::error() const
{
    return m_error;
}

bool NCReportError::isOK() const
{
    return !m_error;
}

QString NCReportError::errorMsg() const
{
    return m_errormsg;
}

int NCReportError::number() const
{
    return m_errorcode;
}
//----------------------------------------------------------------------------

NCReportDummyDataSourceFactory NCReportDef::m_dummyDataSourcefactory;

/*!
 *
 * \class NCReportDef
 * \brief NCReport definition class. 
 *
 * The class holds the whole information for the report director to be able to generate the report.
 * When the report XML document is loaded it fills an NCReportDef object.
*/

NCReportDef::NCReportDef( QObject * parent ) : QObject( parent ),
  m_showPrintDialog(false),
  m_reportStringID(-1),
  m_reportFileParsed(false),
  m_detailIndex(150),
  m_numcopies(0),
  m_pageHeader(0),
  m_pageFooter(0),
  m_reportHeader(0),
  m_reportFooter(0),
  m_customDSProvider(0),
  m_dataSourceFactory( & m_dummyDataSourcefactory)
{
#if defined(Q_OS_WIN) || defined(Q_WS_WIN)
    QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\App Paths\\qtZint.exe", QSettings::NativeFormat);
    m_zintPath = settings.value("Path").toString();
    m_zintPath = QDir::cleanPath(qApp->applicationDirPath() + QDir::separator() + QString("zint.exe"));
    //m_zintPath.replace('\\','/');
#endif
#if defined(Q_OS_LINUX) || defined(Q_WS_X11)
    m_zintPath = QDir::cleanPath(qApp->applicationDirPath() + QDir::separator() + QString("zint"));
#endif
    //test
    //addScript("test","$D{col0}+$D{col1}");
}

NCReportDef::~ NCReportDef()
{
    reset( true );
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("delete NCReportDef");
#endif
}

NCReportSection* NCReportDef::addPageHeader()
{
    m_pageHeader = addSection( m_pageHeader );
    m_pageHeader->setType( NCReportSection::PageHeader );
    m_pageHeader->setID("PH");
    m_pageHeader->setLayoutIndex(1);
    return m_pageHeader;
}

void NCReportDef::removePageHeader()
{
    deleteSection(m_pageHeader);
    m_pageHeader = 0;
}

NCReportSection * NCReportDef::addPageFooter()
{
    m_pageFooter = addSection( m_pageFooter );
    m_pageFooter->setType(NCReportSection::PageFooter);
    m_pageFooter->setID("PF");
    m_pageFooter->setLayoutIndex(999999);
    return m_pageFooter;
}

void NCReportDef::removePageFooter()
{
    deleteSection(m_pageFooter);
    m_pageFooter = 0;
}

NCReportSection * NCReportDef::addReportHeader()
{
    m_reportHeader = addSection( m_reportHeader );
    m_reportHeader->setType(NCReportSection::ReportHeader);
    m_reportHeader->setID("RH");
    m_reportHeader->setLayoutIndex(5);
    return m_reportHeader;
}

void NCReportDef::removeReportHeader()
{
    deleteSection(m_reportHeader);
    m_reportHeader = 0;
}

NCReportSection * NCReportDef::addReportFooter()
{
    m_reportFooter = addSection( m_reportFooter );
    m_reportFooter->setType(NCReportSection::ReportFooter);
    m_reportFooter->setID("RF");
    m_reportFooter->setLayoutIndex(999995);
    return m_reportFooter;
}

void NCReportDef::removeReportFooter()
{
    deleteSection(m_reportFooter);
    m_reportFooter = 0;
}

NCReportSection* NCReportDef::addDetail()
{
    NCReportSection *d = createReportSection();
    d->setType(NCReportSection::Detail);
    m_details.append( d );

    reindexDetails();

    return d;
}

void NCReportDef::removeDetail( NCReportSection *detail )
{
    int pos = m_details.indexOf(detail);
    if ( pos<0 )
        return;
    m_details.removeAt( pos );
    removeAllGroups( detail );
    delete detail;
    //reindexDetails();
}


NCReportSection * NCReportDef::addSection(NCReportSection *section )
{
    if (section)
        deleteSection(section);
    section = createReportSection();
    return section;
}

NCReportSection * NCReportDef::addSection()
{
    NCReportSection *section = createReportSection();
    return section;
}

void NCReportDef::deleteSection(NCReportSection *section )
{
    if (section) {
        delete section;
        section =0;
    }
}

void NCReportDef::addParameter( const QString& id, const QVariant& value )
{
    if(m_parameters.contains(id))
        m_parameters[id] = value;
    else
        m_parameters.insert( id, value );
}

void NCReportDef::addParameters(const QHash<QString, QVariant> &parameters, bool clear)
{
    if (clear)
        m_parameters.clear();
    QHash<QString, QVariant>::const_iterator i = parameters.constBegin();
    while (i != parameters.constEnd()) {
        m_parameters[i.key()] = i.value();
        ++i;
    }
}

void NCReportDef::addParameters(const QMap<QString, QVariant> &parameters, bool clear)
{
    if (clear)
        m_parameters.clear();
    QMap<QString, QVariant>::const_iterator i = parameters.constBegin();
    while (i != parameters.constEnd()) {
        m_parameters[i.key()] = i.value();
        ++i;
    }
}

void NCReportDef::setParameters(const QHash<QString, QVariant> &parameters)
{
    m_parameters = parameters;
}

void NCReportDef::removeParameter(const QString &id)
{
    m_parameters.remove(id);
}

void NCReportDef::addVariable( NCReportVariable* variable )
{
    if ( m_variables.contains( variable->id() ) ) {
        NCReportVariable *v = m_variables.value(variable->id());
        *v = *variable;
        return;
    }
    m_variables.insert( variable->id(), variable );
}

NCReportError* NCReportDef::error()
{
    return &m_error;
}

const NCReportVariable *NCReportDef::variable(const QString &varId) const
{
    return m_variables.value(varId);
}

QVariant NCReportDef::parameter(const QString &id) const
{
    return m_parameters.value(id);
}

const QHash<QString, NCReportDataSource *> &NCReportDef::dataSources() const
{
    return m_dataSources;
}

NCReportDataSource *NCReportDef::dataSource(const QString &id) const
{
    return m_dataSources.value(id);
}

NCReportDataSourceFactory & NCReportDef::dataSourceFactory()
{
    return * m_dataSourceFactory;
}

const QHash<QString, NCReportVariable *> &NCReportDef::variables() const
{
    return m_variables;
}

//QHash< QString, NCReportParameter*> & NCReportDef::parameters()
//{
//	return params;
//}

const QHash<QString, QVariant> &NCReportDef::parameters() const
{
    return m_parameters;
}

QHash<QString, QVariant> &NCReportDef::parametersRef()
{
    return m_parameters;
}

const QHash<QString, NCReportUserFunction *> &NCReportDef::userFunctions() const
{
    return m_userFunctions;
}

const QHash<QString, NCReportGraphRenderer *> &NCReportDef::itemRenderers() const
{
    return m_renderers;
}

void NCReportDef::addDataSource(NCReportDataSource *dataSource )
{
    m_dataSources.insert( dataSource->id(), dataSource );
}

void NCReportDef::addDataSources(const QList<NCReportDataSource *> &dataSources)
{
    for (int i = 0; i < dataSources.size(); ++i)
        addDataSource(dataSources.at(i));
}

void NCReportDef::addDataSources(const QHash<QString, NCReportDataSource *> &dataSources)
{
    m_dataSources = dataSources;
}

void NCReportDef::setDataSources(const QHash<QString, NCReportDataSource *> &dataSources)
{
    m_dataSources = dataSources;
}

void NCReportDef::addStringList(const QString &id, const QStringList &list)
{
    m_stringLists.insert( id, list );
}

void NCReportDef::addStringLists(const QHash<QString, QStringList> &stringLists)
{
    m_stringLists = stringLists;
}

void NCReportDef::setStringLists(const QHash<QString, QStringList> &stringLists)
{
    m_stringLists = stringLists;
}

void NCReportDef::addGraphItemRenderer(NCReportGraphRenderer *renderer )
{
    m_renderers.insert( renderer->id(), renderer );
}

void NCReportDef::addGraphItemRenderers(const QList<NCReportGraphRenderer *> &renderers)
{
    for (int i = 0; i < renderers.size(); ++i)
        addGraphItemRenderer(renderers.at(i));
}

void NCReportDef::addGraphItemRenderers(const QHash<QString, NCReportGraphRenderer *> &renderers)
{
    m_renderers = renderers;
}

void NCReportDef::addItemModel(const QString &id, QAbstractItemModel *model)
{
    m_itemModels.insert( id, model );
}

void NCReportDef::setItemModels(const QHash<QString, QAbstractItemModel *> &models)
{
    m_itemModels = models;
}

#ifndef NCREPORT_NO_TABLE_VIEW
void NCReportDef::addTableView(const QString &id, QTableView *tableView)
{
    m_tableViews.insert(id,tableView);
}
#endif

void NCReportDef::addUserFunction(const QString &id, NCReportUserFunction *function)
{
    m_userFunctions.insert(id,function);
}

void NCReportDef::addUserFunctions(const QHash<QString, NCReportUserFunction *> &functions)
{
    m_userFunctions = functions;
}

NCReportUserFunction *NCReportDef::userFunction(const QString &id)
{
    return m_userFunctions.value(id);
}

void NCReportDef::addScript(const QString &id, const QString &script)
{
    m_scripts.insert(id, script);
}

void NCReportDef::addScripts(const QHash<QString, QString> &scripts)
{
    m_scripts = scripts;
}

QString NCReportDef::script(const QString &id) const
{
    return m_scripts.value(id);
}

const QHash<QString, QString> &NCReportDef::scripts() const
{
    return m_scripts;
}

void NCReportDef::removeScript(const QString &id)
{
    m_scripts.remove(id);
}

void NCReportDef::setDataSourceFactory(NCReportDataSourceFactory *dsf )
{
    m_dataSourceFactory= dsf;
}

void NCReportDef::addFieldItem(NCReportFieldItem *item )
{
    m_fields.append( item );
}

void NCReportDef::addDynamicItem(NCReportItem *item)
{
    m_dynamicItems.append( item );
}

void NCReportDef::clearDetails()
{
    m_details.clear();
}

void NCReportDef::appendDetails(NCReportSection *detail)
{
    m_details.append(detail);
}

NCReportPageOption & NCReportDef::option()
{
    return m_option;
}

NCReportSection * NCReportDef::pageHeader()
{
    return m_pageHeader;
}

NCReportSection * NCReportDef::pageFooter()
{
    return m_pageFooter;
}

NCReportSection * NCReportDef::reportFooter()
{
    return m_reportFooter;
}

NCReportSection * NCReportDef::reportHeader()
{
    return m_reportHeader;
}


const QList<NCReportFieldItem *> &NCReportDef::fieldItems() const
{
    return m_fields;
}

const QList<NCReportSection *> &NCReportDef::details() const
{
    return m_details;
}

const QList<NCReportSection *> &NCReportDef::groupSections() const
{
    return m_groupSections;
}

const QList<NCReportItem *> &NCReportDef::dynamicItems() const
{
    return m_dynamicItems;
}

void NCReportDef::reset( bool all )
{
    reset(all,all,all,all,true);
}


void NCReportDef::reset(bool clearRenderers, bool clearParameters, bool clearUserFunctions, bool clearStringLists, bool keepCustomDataSources )
{
    //------------------------------------
    // Delete all required objects
    //------------------------------------
    m_fields.clear();   // delete fields
    m_dynamicItems.clear();   // delete dynamic items
    // Delete variables
    qDeleteAll(m_variables.begin(), m_variables.end());
    m_variables.clear();

    if ( m_pageHeader ) {
        delete m_pageHeader;
        m_pageHeader =0;
    }
    if ( m_pageFooter ) {
        delete m_pageFooter;
        m_pageFooter =0;
    }
    if ( m_reportHeader ) {
        delete m_reportHeader;
        m_reportHeader =0;
    }
    if ( m_reportFooter ) {
        delete m_reportFooter;
        m_reportFooter =0;
    }
    // delete details
//    while (!m_details.isEmpty()) {
//        NCReportSection* value = *m_details.begin();
//        m_details.erase(m_details.begin());
//        delete value;
//    }
    qDeleteAll(m_details.begin(), m_details.end());
    m_details.clear();
    m_detailIndex = 150;

//    while (!m_groupSections.isEmpty()) {
//        NCReportSection* value = *m_groupSections.begin();
//        m_groupSections.erase(m_groupSections.begin());
//        delete value;
//    }
    qDeleteAll(m_groupSections.begin(), m_groupSections.end());
    m_groupSections.clear();
    removeDataSources(keepCustomDataSources);

    //------------------------------------
    // Delete optional items
    //------------------------------------
    if (clearRenderers)
        m_renderers.clear();
    if (clearParameters)
        m_parameters.clear();
    if (clearUserFunctions)
        m_userFunctions.clear();
    if (clearStringLists)
        m_stringLists.clear();
}


QString NCReportDef::defaultDataSourceName() const
{
    return m_defaultDataSourceName;
}

void NCReportDef::setDefaultDataSourceName( const QString& dsname)
{
    m_defaultDataSourceName = dsname;
}

NCReportSection * NCReportDef::createReportSection()
{
    return new NCReportSection( this );
}

NCReportSection * NCReportDef::addGroupHeader( NCReportGroup *group )
{
    NCReportSection * gh = addSection();
    gh->setType(NCReportSection::GroupHeader);
    gh->setRelatedGroup(group);
    m_groupSections.append(gh);
    group->m_header = gh;
    gh->setLayoutIndex(groupHeaderIndex( group, group->detail()->groups().indexOf(group) ) );
    return gh;
}

void NCReportDef::removeGroupHeader( NCReportGroup *group )
{
    if ( !group->m_header )
        return;
    int idx = m_groupSections.indexOf( group->m_header );
    if ( idx>=0 )
        m_groupSections.removeAt( idx );
    deleteSection(group->m_header);
    group->m_header=0;
}

NCReportSection * NCReportDef::addGroupFooter(NCReportGroup *group )
{
    NCReportSection *gf = addSection();
    gf->setType(NCReportSection::GroupFooter);
    gf->setRelatedGroup(group);
    m_groupSections.append(gf);
    group->m_footer = gf;
    gf->setLayoutIndex(groupFooterIndex( group, group->detail()->groups().indexOf(group) )) ;
    return gf;
}

void NCReportDef::removeGroupFooter(NCReportGroup *group)
{
    if ( !group->m_footer )
        return;
    int idx = m_groupSections.indexOf( group->m_footer );
    if ( idx>=0 )
        m_groupSections.removeAt( idx );
    deleteSection(group->m_footer);
    group->m_footer=0;
}

NCReportGroup * NCReportDef::addGroup( NCReportSection *detail )
{
    NCReportGroup *g = new NCReportGroup();
    g->m_detail = detail; 	// detail section pointer;
    g->setNestedLevel( detail->groups().count() );	//the next sort ID
    detail->groups().append( g );
    return g;
}

void NCReportDef::reindexDetails()
{
    for ( int i=0; i<m_details.count(); ++i ) {
        NCReportSection *detail = m_details.at(i);

        detail->setLayoutIndex( (i+1)*100+50 );
        if ( detail->id().isEmpty() )
            detail->setID( QString("Detail%1").arg(i+1) );

        for ( int j=0; j<detail->groups().count(); ++j ) {
            NCReportGroup *group = detail->groups().at(j);
            if ( group->header() )
                group->header()->setLayoutIndex( groupHeaderIndex( group, j ) );

            if ( group->footer() )
                group->footer()->setLayoutIndex( groupFooterIndex( group, j ) );

        }
    }
}

int NCReportDef::groupHeaderIndex( NCReportGroup *group, int gidx ) const
{
    if ( !group )
        return -1;
    int idx = group->detail()->layoutIndex() - (49-gidx);
    qDebug("groupHeaderIndex: %i", idx );
    return idx;
}

int NCReportDef::groupFooterIndex( NCReportGroup *group, int gidx ) const
{
    if ( !group )
        return -1;
    int idx = group->detail()->layoutIndex() + (49-gidx);
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("groupFooterIndex: %i", idx );
#endif
    return idx;
}

void NCReportDef::setCustomDSProvider( NCReportCustomDSProvider *customDSProvider)
{
   m_customDSProvider = customDSProvider;
}

NCReportCustomDSProvider *NCReportDef::customDSProvider()
{
    return m_customDSProvider;
}

/*!
  Sets template directory for additional necessary files such as charts
  */
void NCReportDef::setTemplateDir(const QString &dir)
{
    m_templateDir = dir;
}

/*!
Returns template directory
  */
QString NCReportDef::templateDir() const
{
    return m_templateDir;
}

void NCReportDef::setConnectionID(const QString &connectionID)
{
    m_connectionId = connectionID;
}

QString NCReportDef::connectionID() const
{
    return m_connectionId;
}

QString NCReportDef::zintPath() const
{
    return m_zintPath;
}

void NCReportDef::setZintPath(const QString &dir)
{
    m_zintPath = dir;
}

void NCReportDef::emitZintNotFound()
{
    emit zintNotFound();
}

void NCReportDef::removeGroup(NCReportSection *detail, NCReportGroup *group)
{
    int idx = detail->groups().indexOf( group );
    if ( idx<0 )
        return;
    detail->groups().removeAt(idx);
    removeGroupHeader( group );
    removeGroupFooter( group );
    delete group;
}

void NCReportDef::removeAllGroups(NCReportSection *detail)
{
    for ( int i=0; i<detail->groups().count(); ++i )
        removeGroup( detail, detail->groups().at(i) );

}

void NCReportDef::removeVariable(NCReportVariable *variable )
{
    if ( m_variables.contains( variable->id() ) ) {
        m_variables.remove(variable->id());
        delete variable;
    }
}

void NCReportDef::removeDataSource(NCReportDataSource *ds)
{
    if ( m_dataSources.contains( ds->id() ) ) {
        m_dataSources.remove(ds->id());

        // remove references
//        QHash<QString,NCReportDataSource*>::const_iterator i = dsources.constBegin();
//        while (i != dsources.constEnd()) {
//            NCReportDataSource* childDS = i.value();
//            if ( childDS->parentID() == ds->id() )
//                childDS->setParentID( QString() );
//            ++i;
//        }

        delete ds;
    }
}

/*
void NCReportDef::removeSQLDataSource(NCReportSQLDataSource *ds)
{
    if ( m_dataSources.contains( QString("CustomDS_")+ds->customDataSourceTable()) ) {
        m_dataSources.remove(QString("CustomDS_")+ds->customDataSourceTable());
        delete ds;
    }
}
*/

void NCReportDef::removeDataSources(bool keepCustomDataSources)
{
    if (keepCustomDataSources) {
        //----------------------------------------------
        // Delete, the non-custom datasources only!
        //----------------------------------------------
        QList<NCReportDataSource*> customDSList;
        QHash<QString,NCReportDataSource*>::const_iterator i = m_dataSources.constBegin();
        while (i != m_dataSources.constEnd()) {
            NCReportDataSource* ds = i.value();
            if ( ds->dataSourceType() == NCReportDataSource::Custom )
                customDSList << ds;
            else
                delete ds;
            ++i;
        }
        m_dataSources.clear();
        for (int i2=0; i2<customDSList.size(); ++i2)	// restore the custom data sources
            addDataSource(customDSList.at(i2));
    } else {
        qDeleteAll(m_dataSources.begin(), m_dataSources.end());
        m_dataSources.clear();
    }
}

void NCReportDef::removeDataSourcesNoDelete()
{
    m_dataSources.clear();
}


const QHash<QString, QStringList> &NCReportDef::stringLists() const
{
    return m_stringLists;
}

const QHash<QString, QAbstractItemModel *> &NCReportDef::itemModels()
{
    return m_itemModels;
}

#ifndef NCREPORT_NO_TABLE_VIEW
const QHash<QString, QTableView *> &NCReportDef::tableViews()
{
    return m_tableViews;
}
#endif

void NCReportDef::sortGroups( NCReportSection *detail )
{
    QMap<short,NCReportGroup*> map;
    for ( int i=0; i<detail->groups().count(); ++i ) {
        NCReportGroup *group = detail->groups().at(i);
        map[group->nestedLevel()] = group;
    }
    // check
    if ( detail->groups().count() != map.count() )
        return;

    detail->groups().clear();
    QMap<short,NCReportGroup*>::const_iterator iterator = map.constBegin();
    while (iterator != map.constEnd()) {
        detail->groups().append( iterator.value() );
        ++iterator;
    }
    reindexDetails();
}

QStringList NCReportDef::dataSourceIdList()
{
    QStringList names;
    QHash<QString, NCReportDataSource*>::const_iterator i = dataSources().constBegin();
    while (i != dataSources().constEnd()) {
        NCReportDataSource* ds = i.value();
        names << ds->id();
        ++i;
    }

    return names;
}

QStringList NCReportDef::dataSourceIdList( NCReportDataSource::OpenRoles openRole )
{
    QStringList names;
    QHash<QString, NCReportDataSource*>::const_iterator i = dataSources().constBegin();
    while (i != dataSources().constEnd()) {
        NCReportDataSource* ds = i.value();
        if (ds->openRole() == openRole)
            names << ds->id();
        ++i;
    }

    return names;
}

QStringList NCReportDef::parameterIdList() const
{
    return m_parameters.keys();
}

QStringList NCReportDef::variableIdList() const
{
    return m_variables.keys();
}


