#include <QTextEdit>
#include "MDDrawPdfViewer.h"
#include "DrawPdfViewerData.h"
#include "PropertyDrawPdfViewer.h"


CMDDrawPdfViewer::CMDDrawPdfViewer()
{
    m_pdfViewerData = new CDrawPdfViewerData();
    QWidget *widget = &m_pdfViewerData->m_PdfViewer;
    setWidget(widget);
}
CMDDrawPdfViewer::~CMDDrawPdfViewer()
{
    if(m_pdfViewerData)
    {
        delete m_pdfViewerData;
        m_pdfViewerData = NULL;
    }
}

void CMDDrawPdfViewer::open(const QString &filePath)
{
    m_pdfViewerData->m_PdfViewer.open(QUrl::fromLocalFile(filePath));
    setFilePath(filePath);
}

void CMDDrawPdfViewer::setFilePath(const QString& filePath)
{
    m_pdfViewerData->m_filePath = filePath;
}

QString CMDDrawPdfViewer::getFilePath()
{
    return m_pdfViewerData->m_filePath;
}

// mode: 0 - SinglePage, 1 - MultiPage
void CMDDrawPdfViewer::setPageMode(int mode)
{
    m_pdfViewerData->m_PdfViewer.setPageMode(mode);
}

void CMDDrawPdfViewer::changeLanguage()
{
    //m_report->m_report.changeLanguage();
}

void CMDDrawPdfViewer::collectWords()
{
    //m_report->m_report.collectWords();
}

DrawType CMDDrawPdfViewer::type()
{
    return DrawType::PdfViewer;
}

/*!
\section 返回属性框对象
*/
CMDPropertyDrawObj *CMDDrawPdfViewer::createPropertyDrawObj()
{
    return new CPropertyDrawPdfViewer(this);
}

/*!
\section 初始化
*/
void CMDDrawPdfViewer::onInitialization()
{
    m_pdfViewerData->m_PdfViewer.setDrawEnvrionment(environment());

    CMDDrawWidget::onInitialization();
}

/*!
\section 创建自身
*/
CMDDrawObj *CMDDrawPdfViewer::createSelf()
{
    return new CMDDrawPdfViewer();
}


/*!
\section 克隆函数，复制控件
*/
CMDDrawObj *CMDDrawPdfViewer::clone()
{
    CMDDrawPdfViewer* obj = static_cast<CMDDrawPdfViewer*>(CMDDrawWidget::clone());
    *obj->m_pdfViewerData = *m_pdfViewerData;
    return obj;
}

void CMDDrawPdfViewer::serialize(QJsonObject &json)
{
    CMDDrawWidget::serialize(json);

    QJsonObject js;
    m_pdfViewerData->serialize(js);
    json.insert("DrawPdfViewer",js);
}

void CMDDrawPdfViewer::deserialize(const QJsonObject &json)
{
    CMDDrawWidget::deserialize(json);

    QJsonObject js = json.value("DrawPdfViewer").toObject();
    m_pdfViewerData->deserialize(js);
}


