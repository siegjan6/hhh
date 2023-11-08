// -*- mode: C++ -*-
// Copyright(c) 2011 SENER Ingenieria y Sistemas.
// All rights reserved.

#include "ncreportdatafieldinfo.h"

NCReportDataFieldInfo::NCReportDataFieldInfo()
{
}

NCReportDataFieldInfo::NCReportDataFieldInfo( const QString& name,
                                              const QString& briefDescription,
                                              const QString& longDescription) :
   _name( name),
   _briefDescription( briefDescription),
   _longDescription( longDescription)
{
}  

NCReportDataFieldInfo::
NCReportDataFieldInfo( const NCReportDataFieldInfo& fieldInfo)
{
   *this = fieldInfo;
}

NCReportDataFieldInfo::~NCReportDataFieldInfo()
{
}

NCReportDataFieldInfo&
NCReportDataFieldInfo::operator = ( const NCReportDataFieldInfo& fieldInfo)
{
   _name             = fieldInfo._name;
   _briefDescription = fieldInfo._briefDescription;
   _longDescription  = fieldInfo._longDescription; 
   return *this;
}

QString NCReportDataFieldInfo::name() const
{
   return _name;
}

QString NCReportDataFieldInfo::briefDescription() const
{
   return _briefDescription;
}

QString NCReportDataFieldInfo::longDescription() const
{
   return _longDescription;
}
