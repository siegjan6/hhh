//by aGRw 2015.08.19
#ifndef MDDRAWREPORTWRAPPER_H
#define MDDRAWREPORTWRAPPER_H

#if defined(MDHMIFORM_LIBRARY)
#  define MDHMIFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDHMIFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

#include "MDObjectWrapper.h"

#include <QString>

class CMDDrawReport;


class CDrawReportWrapper : public CMDObjectWrapper
{
    Q_OBJECT
public:
    CDrawReportWrapper(void* object, const QString& name);
    virtual ~CDrawReportWrapper(){}
//////////////////////////////////////////////////////////////////////////
public  Q_SLOTS:
    void setReportPath(const QString& rpath);
    QString getReportPath();
    void setReportFileName(const QString& name);
    QString getReportFileName();
    void setReportParameter(const QString& key, const QString& value);
    QString getReportParameter(const QString& key);
    void setReportLanguage(const QString &langcode);
    QString getReportLanguage();
    void setDataSourceParameter(const QString &dataSourceName,int type,const QString &parameter);
    void query();
    void exportTo(const QString &fileName, const QString& password = "", const QString& perm = "");
//////////////////////////////////////////////////////////////////////////
};

#endif // MDDRAWREPORTWRAPPER_H





