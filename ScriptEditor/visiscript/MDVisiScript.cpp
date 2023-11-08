#include "MDVisiScript.h"
#include <QList>
#include "MDJavaScriptCodeEditor.h"


QList<IVisiScriptExtension *> temp;
MDVisiScript::MDVisiScript()
    :VisiScript(temp,"")
{
    m_studioInterface = NULL;
}

MDVisiScript::MDVisiScript(QString sFileName, QWidget *parent)
    :VisiScript(temp,sFileName,parent)
{
    m_studioInterface = NULL;
}

//DoImportFile( QDir::toNativeSeparators(sPath)+QString(DEMO_JS) );
bool MDVisiScript::ImportFile(const QString &sFileName)
{
    return VisiScript::DoImportFile(sFileName);
}

QString MDVisiScript::GetText()
{
    return GetActScript()->text();
}
bool MDVisiScript::OnSave()
{
    bool bUseFile = VisiScript::OnSave();
    if(bUseFile == false)
    {
        if(m_jsCodeEditor)
            emit m_jsCodeEditor->signalSave();
    }
    return bUseFile;
}

void MDVisiScript::setStudioInterface(IStudioInterface* value)
{
    m_studioInterface = value;
}

IStudioInterface *MDVisiScript::studioInterface()
{
    return m_studioInterface;
}

void MDVisiScript::closeEvent(QCloseEvent *pCloseEvent)
{
    VisiScript::closeEvent(pCloseEvent);
    m_jsCodeEditor->m_pCodeEditor = NULL;
    deleteLater();
    if(m_jsCodeEditor)
        emit m_jsCodeEditor->signalClose();
}
