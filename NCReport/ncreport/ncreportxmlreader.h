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
#ifndef NCREPORTXMLREADER_H
#define NCREPORTXMLREADER_H

#include "ncreport_global.h"


#include <QXmlStreamReader>
#include <QPointF>
#include <QFont>
#include <QPen>
#include <QBrush>
#include <QSize>

class NCReportDef;
class NCReportSection;
class NCReportPageOption;
class NCReportFieldItem;
class NCReportItem;
QT_BEGIN_NAMESPACE
class QTreeWidgetItem;
class QIODevice;
QT_END_NAMESPACE

/*!
This abstract class represents the base report XML definition stream reader.\n
Contains methods for reading elementary item's properties.
*/

/*!

section 功能

报表XML文件定义读入器类
【这个抽象类表示报表的定义流读取器，包含读取XML节点元素属性的方法】

*/
class NCREPORTSHARED_EXPORT NCReportXMLReader : public QXmlStreamReader
{
public:
    NCReportXMLReader();
    virtual ~NCReportXMLReader();
    void setPageOption( NCReportPageOption* );
    virtual bool read(QIODevice *device, const QString& def);
    ////////////////////////////////////////////////////////////////////////
    /// \brief readXml
    /// \param device
    /// \param def
    /// \return
    ///2015.12.16
    //virtual bool readXml(QIODevice *device, const QString& def, QMap<QString, QStringList> &info);
    /////////////////////////////////////////////////////////////////////////
    void readUnknownElement();

    NCReportDef *reportDef() const;
    void setReportDef( NCReportDef* );

    bool isAttrib( const QString& name );
    QStringRef attrib( const QString& name );
    QString attribToString( const QString& name );
    int attribToInt( const QString& name );
    qreal attribToReal( const QString& name );
    bool attribToBool( const QString& name );

    void readFont( QFont& );
    void readPen( QPen& );
    void readBrush( QBrush& );
    void readColor( const QString& property, QColor& );
    QPointF readPos() const;
    QPointF readPosFrom() const;
    QPointF readPosTo() const;
    QSizeF readSize() const;
    //Qt::Alignment readAlignmentH();
    //Qt::Alignment readAlignmentV();
    Qt::Alignment readAlignment();
    qreal readLineWidth();
    void setLanguage( const QString& langcode );
    QString language() const;

    void readParameters(QHash<QString, QVariant> &values);

public:
    //virtual QString getDsID() const = 0;
    //virtual void setDsID(const QString &value) = 0;

protected:
    QString basetag;
    NCReportPageOption *pageoption;
    NCReportDef *rdef;

    virtual void init()=0;
    virtual void readReport();

    ///////////////////////////////////
    /// \brief readReportXmlFile
    ///2015.12.16
    //virtual void readReportXml(QMap<QString, QStringList> &info);
    ///////////////////////////////////

    virtual void readOptions();
    virtual void readHtmlText();
    virtual void readPage();
    virtual void readItems( NCReportSection* );
    virtual void readSection( NCReportSection* );

    virtual void readDataSources();
    ///////////////////////////////////////////
    /// \brief readDataSourcesXml
    ///
   // virtual void readDataSourcesXml(QMap<QString, QStringList> &);
    //////////////////////////////////////////////

    virtual void readDataSource();

    ////////////////////////////////////////
    /// \brief readDataSourceXml
    ///
    //virtual void readDataSourceXml(QMap<QString, QStringList> &);
    ///////////////////////////////////////

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

    ////////////////////////////////
    /// \brief readDetailsXml
    ///2015.12.17 mxl
   // virtual void readDetailsXml(QStringList &);
    ///////////////////////////////

    virtual void readField( NCReportFieldItem* );
    virtual void readParameter(QHash<QString, QVariant>& values);

    virtual void beforeReadItem( NCReportItem* );
    virtual void afterReadItem( NCReportItem* );
    virtual void addItem( NCReportItem*, NCReportSection* );
    virtual void afterAddItem( NCReportItem* );

    virtual void setDefaultFont( QFont& );
    virtual void setDefaultPen( QPen& );
    virtual void setDefaultBrush( QBrush& );
private:
    QString m_langcode;

};

#endif
