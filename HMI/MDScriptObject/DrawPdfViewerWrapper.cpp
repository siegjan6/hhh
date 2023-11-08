#include "DrawPdfViewerWrapper.h"
#include "MDDrawPdfViewer.h"

//////////////////////////////////////////////////////////////////////////
CDrawPdfViewerWrapper::CDrawPdfViewerWrapper(void *object, const QString &name)
    :CMDObjectWrapper(object, name)
{
}
//////////////////////////////////////////////////////////////////////////
void CDrawPdfViewerWrapper::setFilePath(const QString& filePath)
{
    CMDDrawPdfViewer* obj = (CMDDrawPdfViewer*)MDObject();
    obj->setFilePath(filePath);
}
//////////////////////////////////////////////////////////////////////////
QString CDrawPdfViewerWrapper::getFilePath()
{
    CMDDrawPdfViewer* obj = (CMDDrawPdfViewer*)MDObject();
    return obj->getFilePath();
}
//////////////////////////////////////////////////////////////////////////
void CDrawPdfViewerWrapper::open(const QString& filePath)
{
    CMDDrawPdfViewer* obj = (CMDDrawPdfViewer*)MDObject();
    obj->open(filePath);
}
//////////////////////////////////////////////////////////////////////////
// mode: 0 - SinglePage, 1 - MultiPage
void CDrawPdfViewerWrapper::setPageMode(int mode)
{
    CMDDrawPdfViewer* obj = (CMDDrawPdfViewer*)MDObject();
    obj->setPageMode(mode);
}
//////////////////////////////////////////////////////////////////////////
