#include "DrawReportWrapper.h"
#include "MDDrawReport.h"

//////////////////////////////////////////////////////////////////////////
CDrawReportWrapper::CDrawReportWrapper(void *object, const QString &name)
    :CMDObjectWrapper(object, name)
{
}
//////////////////////////////////////////////////////////////////////////
void CDrawReportWrapper::setReportPath(const QString& rpath)
{
    CMDDrawReport* obj = (CMDDrawReport*)MDObject();
    obj->setReportPath(rpath);
}
//////////////////////////////////////////////////////////////////////////
QString CDrawReportWrapper::getReportPath()
{
    CMDDrawReport* obj = (CMDDrawReport*)MDObject();
    return obj->getReportPath();
}
//////////////////////////////////////////////////////////////////////////
void CDrawReportWrapper::setReportFileName(const QString& name)
{
    CMDDrawReport* obj = (CMDDrawReport*)MDObject();
    obj->setXMLfileName(name);
}
QString CDrawReportWrapper::getReportFileName()
{
    CMDDrawReport* obj = (CMDDrawReport*)MDObject();
    return obj->getXMLfileName();
}
//////////////////////////////////////////////////////////////////////////
void CDrawReportWrapper::setReportParameter(const QString& key, const QString& value)
{
    CMDDrawReport* obj = (CMDDrawReport*)MDObject();
    obj->setReportParameter(key, value);
}
QString CDrawReportWrapper::getReportParameter(const QString& key)
{
    CMDDrawReport* obj = (CMDDrawReport*)MDObject();
    return obj->getReportParameter(key);
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
void CDrawReportWrapper::setReportLanguage(const QString &langcode)
{
    CMDDrawReport* obj = (CMDDrawReport*)MDObject();
    obj->setReportLanguage(langcode);
}
QString CDrawReportWrapper::getReportLanguage()
{
    CMDDrawReport* obj = (CMDDrawReport*)MDObject();
    return obj->getReportLanguage();
}
//////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////
void CDrawReportWrapper::setDataSourceParameter(const QString &dataSourceName,int type,const QString &parameter)
{
    CMDDrawReport* obj = (CMDDrawReport*)MDObject();
    obj->setDataSourceParameter(dataSourceName,type,parameter);
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
void CDrawReportWrapper::query()
{
    CMDDrawReport* obj = (CMDDrawReport*)MDObject();
    obj->query();
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
void CDrawReportWrapper::exportTo(const QString &fileName, const QString& password/* = ""*/, const QString& perm/* = ""*/)
{
    CMDDrawReport* obj = (CMDDrawReport*)MDObject();
    obj->exportTo(fileName, password/* = ""*/, perm/* = ""*/);
}
//////////////////////////////////////////////////////////////////////////
