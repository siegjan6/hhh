//by luyj 2023.01.23
#ifndef MDDRAWPDFVIEWERWRAPPER_H
#define MDDRAWPDFVIEWERWRAPPER_H

#if defined(MDHMIFORM_LIBRARY)
#  define MDHMIFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDHMIFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

#include "MDObjectWrapper.h"

#include <QString>

class CMDDrawPdfViewer;


class CDrawPdfViewerWrapper : public CMDObjectWrapper
{
    Q_OBJECT
public:
    CDrawPdfViewerWrapper(void* object, const QString& name);
    virtual ~CDrawPdfViewerWrapper(){}
//////////////////////////////////////////////////////////////////////////
public  Q_SLOTS:
    void setFilePath(const QString& filePath);
    QString getFilePath();
    void open(const QString& filePath);
    // mode: 0 - SinglePage, 1 - MultiPage
    void setPageMode(int mode);
//////////////////////////////////////////////////////////////////////////
};

#endif // MDDRAWPDFVIEWERWRAPPER_H

