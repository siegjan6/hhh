#include "PropertyDrawPdfViewer.h"
#include "MDDrawPdfViewer.h"
#include <QFileDialog>
#include <QColorDialog>

CPropertyDrawPdfViewer::CPropertyDrawPdfViewer(CMDDrawObj *parent) :
    CMDPropertyDrawObj(parent)
{
    m_filePath = "";

    insertTranslateMap("config",tr("配置"));
    insertTranslateMap("GUID_Data",tr("数据"));
    //insertTranslateMap("FileDialogFileName",tr("文件选择"));
    //insertTranslateMap("ReportBackColor",tr("背景色"));
}

CMDDrawPdfViewer *CPropertyDrawPdfViewer::object()
{
    return dynamic_cast<CMDDrawPdfViewer*>(m_object);
}


//void CPropertyDrawPdfViewer::setFileName(const QString &value)
//{
//    m_XMLFileName = value;
//    object()->setXMLfileName(value);
//    setModify(true);
//}

//QString CPropertyDrawPdfViewer::getFileName()
//{
////    return m_XMLFileName;
//    return object()->getXMLfileName();
//}

void CPropertyDrawPdfViewer::setFilePath(const QString &value)
{
    m_filePath = value;
    object()->setFilePath(m_filePath);
    setModify(true);
}

QString CPropertyDrawPdfViewer::getFilePath()
{
    return object()->getFilePath();
}
