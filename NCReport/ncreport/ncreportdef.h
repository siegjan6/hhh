﻿/****************************************************************************
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
#ifndef NCREPORTDEF_H
#define NCREPORTDEF_H

#include "ncreportparameter.h"
#include "ncreportvariable.h"
#include "ncreportpageoption.h"
#include "ncreportsection.h"
#include "ncreportfielditem.h"
#include "ncreportdummydatasourcefactory.h"
#include "ncreport_global.h"
#include "ncreportdatasource.h"
//#include "ncreportsqldatasource.h"

#include <QObject>
#include <QHash>
#include <QList>

class NCReportUserFunction;
class NCReportGraphRenderer;
class NCReportCustomDSProvider;
class NCReportDataSourceRelation;
QT_BEGIN_NAMESPACE
class QAbstractItemModel;
class QTableView;
QT_END_NAMESPACE

/*!
Class for generating NCReport errors.
 */

/*!

section 功能

【报表错误管理类】

*/
class NCREPORTSHARED_EXPORT NCReportError
{
public:
    NCReportError();

    void setError( const QString& msg, int errcode=-1 );
    QString errorMsg() const;
    bool error() const;
    bool isOK() const;
    int number() const;

private:
    bool m_error;
    int m_errorcode;
    QString m_errormsg;
};

/*!

section 功能

报表定义管理类
【管理报表配置界面，如页眉／页脚的添加和删除，细节和分组的添加和删除等】

*/
class NCREPORTSHARED_EXPORT NCReportDef : public QObject
{
    Q_OBJECT
public:
    NCReportDef( QObject * parent = 0 );
    virtual ~NCReportDef();

    void reset( bool all );
    void reset(bool clearRenderers, bool clearParameters, bool clearUserFunctions, bool clearStringLists, bool keepCustomDataSources = true );
    NCReportSection* addPageHeader();
    void removePageHeader();
    NCReportSection *pageHeader();

    NCReportSection* addPageFooter();
    void removePageFooter();
    NCReportSection *pageFooter();

    NCReportSection* addReportHeader();
    void removeReportHeader();
    NCReportSection *reportHeader();

    NCReportSection* addReportFooter();
    void removeReportFooter();
    NCReportSection *reportFooter();

    NCReportGroup* addGroup( NCReportSection* detail );
    void removeGroup( NCReportSection* detail, NCReportGroup* group);
    void removeAllGroups( NCReportSection* detail);
    NCReportSection* addGroupHeader( NCReportGroup* group);
    void removeGroupHeader( NCReportGroup* group );
    NCReportSection* addGroupFooter( NCReportGroup* group );
    void removeGroupFooter( NCReportGroup* group );
    const QList<NCReportSection*>& groupSections() const;

    NCReportSection* addDetail();
    void removeDetail( NCReportSection* detail );
    const QList<NCReportSection*>& details() const;
    void clearDetails();
    void appendDetails(NCReportSection* detail);

    //void addParameter( NCReportParameter* );
    void addParameter( const QString &id, const QVariant &value );
    void addParameters(const QHash<QString,QVariant>& parameters , bool clear = false);
    void addParameters(const QMap<QString,QVariant>& parameters , bool clear = false);
    void setParameters(const QHash<QString,QVariant>& parameters );
    void removeParameter( const QString& id );
    QVariant parameter( const QString& id ) const;
    const QHash<QString,QVariant>& parameters() const;
    QHash<QString,QVariant>& parametersRef();

    void addVariable( NCReportVariable* variable );
    void removeVariable( NCReportVariable* variable );
    const NCReportVariable* variable( const QString& varId ) const;
    const QHash<QString,NCReportVariable*>& variables() const;

    void addDataSource( NCReportDataSource* dataSource );
    void addDataSources( const QList<NCReportDataSource*>& dataSources );
    void addDataSources( const QHash<QString,NCReportDataSource*>& dataSources );
    void setDataSources( const QHash<QString,NCReportDataSource*>& dataSources );
    NCReportDataSource *dataSource( const QString& id ) const;
    const QHash<QString,NCReportDataSource*>& dataSources() const;

    void addStringList( const QString &id, const QStringList &list );
    void addStringLists( const QHash<QString,QStringList>& stringLists );
    void setStringLists( const QHash<QString,QStringList>& stringLists );
    const QHash<QString,QStringList>& stringLists() const;

    void addGraphItemRenderer( NCReportGraphRenderer *renderer );
    void addGraphItemRenderers( const QList<NCReportGraphRenderer*> & m_renderers );
    void addGraphItemRenderers(const QHash<QString,NCReportGraphRenderer*> & renderers );
    const QHash<QString,NCReportGraphRenderer*>& itemRenderers() const;

    void addItemModel( const QString &id, QAbstractItemModel *model );
    void setItemModels( const QHash<QString, QAbstractItemModel *>& models );
    const QHash<QString,QAbstractItemModel*>& itemModels();

#ifndef NCREPORT_NO_TABLE_VIEW
    void addTableView( const QString &id, QTableView *tableView );
    const QHash<QString,QTableView*>& tableViews();
#endif

    void addUserFunction( const QString &id, NCReportUserFunction *function );
    void addUserFunctions( const QHash<QString,NCReportUserFunction*>& functions );
    NCReportUserFunction *userFunction( const QString &id );
    const QHash<QString,NCReportUserFunction*>& userFunctions() const;

    void addScript( const QString &id, const QString& script );
    void addScripts(const QHash<QString, QString> &scripts );
    QString script( const QString &id ) const;
    const QHash<QString,QString>& scripts() const;
    void removeScript( const QString& id );

    void removeDataSource( NCReportDataSource* ds );
    //void removeSQLDataSource( NCReportSQLDataSource* ds );
    void removeDataSources( bool keepCustomDataSources );
    void removeDataSourcesNoDelete();

    void setDataSourceFactory(NCReportDataSourceFactory *dsf );

    void addFieldItem(NCReportFieldItem* item);
    const QList<NCReportFieldItem*>& fieldItems() const;
    void addDynamicItem(NCReportItem* item);
    const QList<NCReportItem*>& dynamicItems() const;

    NCReportError* error();

    NCReportPageOption & option();
    NCReportDataSourceFactory & dataSourceFactory();

    void setDefaultDataSourceName(const QString& dsname );
    QString defaultDataSourceName() const;
    /*!Generate or re-generate layout index for all of details and groups */
    void reindexDetails();
    /*!Sorts or Re-orders groups of the specified detail by group order index. */
    void sortGroups( NCReportSection *detail );
    QStringList dataSourceIdList();
    QStringList dataSourceIdList( NCReportDataSource::OpenRoles openRole );
    QStringList parameterIdList() const;
    QStringList variableIdList() const;

    void setCustomDSProvider( NCReportCustomDSProvider *customDSProvider);
    NCReportCustomDSProvider *customDSProvider();
    void setTemplateDir(const QString& dir);
    QString templateDir() const;

    /*!
     * \brief setConnectionID
     * \param connectionID id
     * Sets an application level connection id for SQL database connections.
     */
    void setConnectionID( const QString& connectionID );
    QString connectionID() const;

    QString zintPath() const;
    void setZintPath( const QString& dir );
    void emitZintNotFound();

signals:
    void zintNotFound();

protected:
    /*! Virtual method for creating report section scene. The function supports using derived section class*/
    virtual NCReportSection* createReportSection();

    NCReportSection* addSection( NCReportSection* );
    NCReportSection* addSection();
    int groupHeaderIndex( NCReportGroup *group, int gidx ) const;
    int groupFooterIndex( NCReportGroup *group, int gidx ) const;

private:
    bool m_showPrintDialog;
    int m_reportStringID;
    bool m_reportFileParsed;
    /*!layout index for designer*/
    int m_detailIndex;
    int m_numcopies;
    NCReportSection *m_pageHeader;
    NCReportSection *m_pageFooter;
    NCReportSection *m_reportHeader;
    NCReportSection *m_reportFooter;
    NCReportCustomDSProvider *m_customDSProvider;
    NCReportDataSourceFactory *m_dataSourceFactory;

private:
    NCReportPageOption m_option;
    NCReportError m_error;

    QHash<QString,NCReportDataSource*> m_dataSources;
    static NCReportDummyDataSourceFactory m_dummyDataSourcefactory;
    QHash<QString,NCReportVariable*> m_variables;
    QHash<QString,QVariant> m_parameters;
    QHash<QString,NCReportUserFunction*> m_userFunctions;
    QHash<QString,QString> m_scripts;
    QHash<QString,NCReportGraphRenderer*> m_renderers;
    QHash<QString,QStringList> m_stringLists;
    QHash<QString,QAbstractItemModel*> m_itemModels;
#ifndef NCREPORT_NO_TABLE_VIEW
    QHash<QString,QTableView*> m_tableViews;
#endif
    QList<NCReportSection*> m_details;
    QList<NCReportSection*> m_groupSections;
    QList<NCReportFieldItem*> m_fields;
    QList<NCReportItem*> m_dynamicItems;

    QString m_reportFileName;
    QString m_reportId;
    QString m_defaultDataSourceName;
    QString m_templateDir;
    QString m_connectionId;
    QString m_zintPath;

private:
    void deleteSection( NCReportSection* section );

};


#endif
