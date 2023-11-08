#include "MDJavaScriptCodeEditor.h"
#include "MDVisiScript.h"
#include "IStudioInterface.h"

CMDJavaScriptCodeEditor2::CMDJavaScriptCodeEditor2(const QString &sFilePathFullName, QWidget *parent):
    CBaseForm(parent)
{
    p_vLayoutMain = NULL;
    m_pCodeEditor = new MDVisiScript(sFilePathFullName, this);
    m_pCodeEditor->m_jsCodeEditor = this;
//    m_pCodeEditor->setWindowModality(Qt::WindowModality::ApplicationModal);
//    m_pCodeEditor->setWindowFlags(Qt::SubWindow);
//    p_vLayoutMain  = new QVBoxLayout();
//    p_vLayoutMain->addWidget(m_pCodeEditor);
//    setLayout(p_vLayoutMain);

    setCentralWidget(m_pCodeEditor);
    QFileInfo fi = QFileInfo(sFilePathFullName);
    QString titleName = fi.fileName();
    setWindowTitle(titleName);
}

CMDJavaScriptCodeEditor2::~CMDJavaScriptCodeEditor2()
{
    if (m_pCodeEditor)
    {
        delete m_pCodeEditor;
        m_pCodeEditor = NULL;
    }
}

bool CMDJavaScriptCodeEditor2::save()
{
    if(this->m_pCodeEditor)
    {
        m_pCodeEditor->sltFileSave();
    }
    this->setModify(false);
    return true;
}

void CMDJavaScriptCodeEditor2::onShow()
{
    IStudioInterface *studioInterface = m_pCodeEditor->studioInterface();
    if(studioInterface)
    {
        if(m_pCodeEditor && !m_pCodeEditor->isVisible())
        {
            studioInterface->openMDIWindow(this);
        }
        studioInterface->setActiveForm(this);
    }
}

bool CMDJavaScriptCodeEditor2::importFile(const QString &sFilePathFullName)
{
    return m_pCodeEditor->ImportFile(sFilePathFullName);
}


bool CMDJavaScriptCodeEditor2::importText(const QString &sTitleName, const QString &sText)
{
    setWindowTitle(sTitleName);
    return m_pCodeEditor->ImportText(sTitleName,sText);
}

QString CMDJavaScriptCodeEditor2::text()
{
   return m_pCodeEditor->GetText();
}

void CMDJavaScriptCodeEditor2::setStudioInterface(IStudioInterface* value)
{
    m_pCodeEditor->setStudioInterface(value);
}

void CMDJavaScriptCodeEditor2::closeEvent(QCloseEvent *e)
{
    CBaseForm::closeEvent(e);
    m_pCodeEditor->close();
    e->accept();
}
