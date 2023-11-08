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
#ifndef NCREPORTXMLDEFREADER_H
#define NCREPORTXMLDEFREADER_H

#include "ncreportxmlreader.h"
#include "ncreport_global.h"

class NCReportDef;
class NCReportSection;
class NCReportGroup;
class NCReportDataSource;
QT_BEGIN_NAMESPACE
class QTreeWidgetItem;
QT_END_NAMESPACE

/*!
This class represents the report XML definition stream reader.\n
Responsible for parsing XML definition into NCReportDef class
*/

/*!

section 功能

报表XML文件定义读入器类
【从XML文件中读取报表配置项】

*/
class NCREPORTSHARED_EXPORT NCReportXMLDefReader : public NCReportXMLReader
{
public:
    NCReportXMLDefReader( NCReportDef *r );
    virtual ~NCReportXMLDefReader();
protected:
    virtual void init();
    virtual void readDataSources();
    /////////////////////////////////////
    /// \brief readDataSourcesXml
    ///
    //virtual void readDataSourcesXml(QMap<QString, QStringList> &info);

    virtual void readDataSource();

    /////////////////////////////////////
    /// \brief readDataSource
    ///
    //virtual void readDataSourceXml(QMap<QString, QStringList> &info);

    virtual void readVariables();
    virtual void readVariable();
    virtual void readScripts();
    virtual void readScript();
    virtual void readParameters();
    virtual void readPageHeader();
    virtual void readPageFooter();
    virtual void readReportHeader();
    virtual void readReportFooter();
    virtual void readDetails();

    /////////////////////////////////
    /// \brief readDetailsXml
    /// \param detialDS
    ///2015.12.17 mxl
    //virtual void readDetailsXml(QStringList &detailDS);


    virtual void readDetail();

    ///////////////////////////
    /// \brief readDetailXml
    ///
    //virtual void readDetailXml(QStringList &detailDS);

    virtual void readGroups( NCReportSection* );
    virtual void readGroup( NCReportSection* );
    virtual void readGroupHeader( NCReportGroup* );
    virtual void readGroupFooter( NCReportGroup* );
    virtual void readField( NCReportFieldItem* );

    virtual void afterReadItem( NCReportItem* );
    virtual void setDefaultFont(QFont &);

    virtual NCReportDataSource* createCustomDataSource(const QString& dsID);

private:
    void organizeDataSources();

    //------------------------------------------ 2015.08.18  mxl   添加
private:
    QString m_dataSourceName;
    //------------------------------------------ 2015.08.18  mxl   添加

};

#endif
