#include "MDScriptEditor.h"
#include "EditorForm.h"
//#include "BrowserManager.h"
#include "EditorForm.h"
#include "EditorDialog.h"

//static CBrowserManager* gManagerGlobal = NULL;

CMDScriptEditor::CMDScriptEditor()
{
    qDebug("new Editor DLl Obj");

//    if( !gManagerGlobal ){
//        gBrowserManager = CBrowserManager::getInstance();
//        Q_ASSERT(gBrowserManager);

//        gManagerGlobal = gBrowserManager;
//    }

}

CMDScriptEditor::~CMDScriptEditor()
{

}

CBaseForm *CMDScriptEditor::createForm(QString filePath, const QList< QPair<QString, QString> >& objs, QWidget* parent)
{
    return new CEditorForm(filePath, objs, parent);
}

void CMDScriptEditor::cleanUp()
{
//    if(gManagerGlobal)
//        delete gManagerGlobal;
}

int CMDScriptEditor::showDialog(QString &content, const QList< QPair<QString, QString> >& objs, QWidget *parent)
{
    CEditorDialog dlg(content, objs, parent);

    int result = dlg.exec();
    content = dlg.m_content;
    return result;
}
