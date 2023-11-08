#include "MDScriptUIImp.h"
#include "../../include/MDProjectConfiger/MDProjectConfiger.h"
#include <QDir>
#include <QMessageBox>
#include "MDJavaScriptCodeEditor.h"
#include "../MDScriptEditor/MDScriptEditor.h"

CMDScriptUIImp::CMDScriptUIImp()
    : QObject(NULL)
{
    m_studioInterface = NULL;
    m_rootInStudio = NULL;
}

CMDScriptUIImp::~CMDScriptUIImp()
{
}

/*!
\section 初始化
*/

void CMDScriptUIImp::load()
{
    m_fileNode.initNodes(treeRoot(), this);
}

QTreeWidget *CMDScriptUIImp::treeWidget()
{
    return (m_studioInterface ? m_studioInterface->treeWidget() : NULL);
}

QTreeWidgetItem *CMDScriptUIImp::treeRoot()
{
    return m_rootInStudio;
}

QString CMDScriptUIImp::scriptFolder()
{
    return m_studioInterface->projectConfiger()->path() + "/Script";
}

bool CMDScriptUIImp::createScriptFile(const QString& fileName)
{
    QFile configFile(fileName);
    if(!configFile.exists())
    {
        configFile.open(QIODevice::WriteOnly);
        configFile.close();
    }
    return true;
}

void CMDScriptUIImp::unLoad()
{
    closeAll();

    scriptWindowClose();
}

QString CMDScriptUIImp::fileItemPath(QTreeWidgetItem *pItem,int column)
{
    bool isFileItem = (QTreeWidgetItem::ItemType::UserType == pItem->type());
    if(!isFileItem)
        return "";

    QString pText;
    QTreeWidgetItem* p = pItem->parent();
    bool isInScriptRootFolder = (p && p->type() != QTreeWidgetItem::ItemType::Type);
    while (isInScriptRootFolder)
    {
        pText = p->text(column) + "/" + pText;
        p = p->parent();

        isInScriptRootFolder = (p && p->type() != QTreeWidgetItem::ItemType::Type);
    }
    pText += pItem->text(column);

    return pText;
}

void CMDScriptUIImp::scriptWindowClose()
{
    close(dynamic_cast<CMDJavaScriptCodeEditor2*>(sender()));
}

void CMDScriptUIImp::on_actionTreeWindow_destroyed(QObject* obj)
{
}

/*!
\section 管理打开的脚本编辑器界面
*/
void CMDScriptUIImp::open(const QString& fileName)
{
    CMDJavaScriptCodeEditor2* editor = find(fileName);
    if(editor)
    {
        editor->onShow();
        editor->showMaximized();
    }
    else
    {
        editor = new CMDJavaScriptCodeEditor2(fileName);
        //editor = new CMDScriptEditor();
        m_editorMap.insert(fileName, editor);

        editor->setStudioInterface(m_studioInterface);
        editor->onShow();
        editor->showMaximized();
        connect(editor, &CMDSaveInterface::signalClose, this, &CMDScriptUIImp::scriptWindowClose);
    }
}

bool CMDScriptUIImp::close(CMDJavaScriptCodeEditor2* editor)
{
    QMap<QString, CMDJavaScriptCodeEditor2*>::iterator iPosition = m_editorMap.begin();
    while(iPosition != m_editorMap.end())
    {
        if(iPosition.value() == editor)
        {
            editor->close();
            delete editor;

            m_editorMap.erase(iPosition);
            return true;
        }
        iPosition ++;
    }
    return false;
}

void CMDScriptUIImp::closeAll()
{
    QMap<QString, CMDJavaScriptCodeEditor2*>::const_iterator iPosition = m_editorMap.begin();
    while(iPosition != m_editorMap.end())
    {
        CMDJavaScriptCodeEditor2* editor = iPosition.value();
        editor->close();
        //delete editor;
        iPosition ++;
    }
    m_editorMap.clear();
}

CMDJavaScriptCodeEditor2* CMDScriptUIImp::find(const QString& fileName)
{
    QMap<QString, CMDJavaScriptCodeEditor2*>::const_iterator iPosition = m_editorMap.find(fileName);
    return (iPosition != m_editorMap.end() ? iPosition.value() : NULL);
}
