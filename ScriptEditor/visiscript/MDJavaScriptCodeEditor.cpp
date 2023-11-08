#include "MDJavaScriptCodeEditor.h"
#include "MDVisiScript.h"

CMDJavaScriptCodeEditor::CMDJavaScriptCodeEditor(const QString& sFilePathFullName)
{
    m_pCodeEditor = new MDVisiScript(sFilePathFullName);
    m_pCodeEditor->m_jsCodeEditor = this;
    m_pCodeEditor->setWindowModality(Qt::WindowModality::WindowModal);
}

CMDJavaScriptCodeEditor::~CMDJavaScriptCodeEditor()
{
}

void CMDJavaScriptCodeEditor::onShow()
{
    if(m_pCodeEditor)
        m_pCodeEditor->show();
}

bool CMDJavaScriptCodeEditor::importFile(const QString &sFilePathFullName)
{
    return m_pCodeEditor->ImportFile(sFilePathFullName);
}

bool CMDJavaScriptCodeEditor::importText(const QString &sTitleName, const QString &sText)
{
    return m_pCodeEditor->ImportText(sTitleName,sText);
}

QString CMDJavaScriptCodeEditor::text()
{
    return m_pCodeEditor->GetText();
}

//设置Studio接口
void CMDJavaScriptCodeEditor::setStudioInterface(IStudioInterface* value)
{
    m_pCodeEditor->setStudioInterface(value);
}
