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
#include "ncrdcustomdatasource.h"
#include <QVariant>

NCRDCustomDataSource::NCRDCustomDataSource(QObject * parent) : NCReportDataSource( parent )
{
    setDataSourceType(Custom);
    setLocationType(Static);
}

NCRDCustomDataSource::~ NCRDCustomDataSource()
{
}

bool NCRDCustomDataSource::open(OPEN_MODEL model)
{
    return true;
}

bool NCRDCustomDataSource::close()
{
    return true;
}

bool NCRDCustomDataSource::next(bool isSamplingPageBreak)
{
    return true;
}

bool NCRDCustomDataSource::hasNext()
{
    return false;
}

int NCRDCustomDataSource::size() const
{
    return 0;
}

bool NCRDCustomDataSource::previous()
{
    return true;
}

bool NCRDCustomDataSource::first()
{
    return true;
}

bool NCRDCustomDataSource::last()
{
    return true;
}

QVariant NCRDCustomDataSource::value(const QString &, bool*, int /*nQTDataRole*/) const
{
    return QVariant();
}

QVariant NCRDCustomDataSource::value( int , bool*, int /*nQTDataRole*/) const
{
    return QVariant();
}

bool NCRDCustomDataSource::read(NCReportXMLReader *)
{
    return true;
}

bool NCRDCustomDataSource::write(NCReportXMLWriter *)
{
    return true;
}

bool NCRDCustomDataSource::isValid() const
{
    return true;
}

int NCRDCustomDataSource::columnIndexByName(const QString &columnname) const
{
    return NCReportDataSource::columnIndexByName(columnname);
}

bool NCRDCustomDataSource::seek(int)
{
    return true;
}
