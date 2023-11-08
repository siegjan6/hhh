#include "MDRecipeUIImp.h"
#include "MDRecipeExpert.h"
#include "MDRecipeConfiger.h"
#include "MDRecipeFile.h"
#include "MDRecipe.h"
#include "MDProjectConfiger.h"
#include <QFile>
#include "MDRecipeMainDialog.h"
#include "MDStringResource.h"

/*!
 * \brief 实现 配方专家 的联系人类
 */
CMDRecipeContacterImp_dl::CMDRecipeContacterImp_dl()
{
    m_UIImp = NULL;
}

bool CMDRecipeContacterImp_dl::writeVarGeneral(CMDRecipe* recipe)
{
    return false;
}

bool CMDRecipeContacterImp_dl::writeVarQuick(CMDRecipe* recipe)
{
    return false;
}

QVariant CMDRecipeContacterImp_dl::varValue(const QString &varName)
{
    return false;
}

bool CMDRecipeContacterImp_dl::isQuickVar(const QString &varName)
{
    return false;
}

void CMDRecipeContacterImp_dl::onDownloadTaskAdded(const QString &name, int showProcessDialog/*=1*/)
{
    m_UIImp->onDownloadTaskAdded(name, showProcessDialog);
}

void CMDRecipeContacterImp_dl::onBeginDownloading(const QString &name, int total)
{
    m_UIImp->onBeginDownloading(name, total);
}

void CMDRecipeContacterImp_dl::onEndDownloading(const QString &name, DownloadFinishFlag finishFlag)
{

}

void CMDRecipeContacterImp_dl::onDownloadProcess(const QString &msg, int total, int completed)
{
    m_UIImp->onDownloadProcess(msg, total, completed);
}

CMDRecipeUIImp::CMDRecipeUIImp(QObject *parent) :
    QObject(parent)
{
    m_studioInterface = NULL;
    m_rootInStudio = NULL;

    m_recipeExpert = NULL;
    m_contacter.m_UIImp = this;
}

void CMDRecipeUIImp::load()
{
    if(treeRoot() != NULL)
        m_fileNode.initNodes(treeRoot(), this);

    m_downloadDialog.m_recipeExpert = m_recipeExpert;

    connect(this, SIGNAL(downloadTaskAdded(const QString &, int)),
                  &m_downloadDialog, SLOT(handleDownloadTaskAdded(const QString &, int)));
    connect(this, SIGNAL(beginDownloading(const QString &, int)),
                  &m_downloadDialog, SLOT(handleBeginDownloading(const QString &, int)));
    connect(this, SIGNAL(downloadProcess(const QString &, int, int)),
                  &m_downloadDialog, SLOT(handleDownloadProcess(const QString &, int, int)));
}

void CMDRecipeUIImp::unload()
{
    scriptWindowClose();

    m_downloadDialog.hide();
}

void CMDRecipeUIImp::onDownloadTaskAdded(const QString &name, int showProcessDialog/*=1*/)
{
    emit downloadTaskAdded(name, showProcessDialog);
}

void CMDRecipeUIImp::onBeginDownloading(const QString &name, int total)
{
    emit beginDownloading(name, total);
}

void CMDRecipeUIImp::onDownloadProcess(const QString &msg, int total, int completed)
{
    emit downloadProcess(msg, total, completed);
}

////

/*!
\section 初始化
*/

QTreeWidget *CMDRecipeUIImp::treeWidget()
{
    return (m_studioInterface ? m_studioInterface->treeWidget() : NULL);
}

QTreeWidgetItem *CMDRecipeUIImp::treeRoot()
{
    return m_rootInStudio;
}

QString CMDRecipeUIImp::recipeFolder()
{
    if(m_studioInterface)
        return m_studioInterface->projectConfiger()->path() + "/Recipe";

    if(m_recipeExpert)
        return m_recipeExpert->configer()->recipePath();

    return "";
}

bool CMDRecipeUIImp::createScriptFile(const QString& fileName)
{
    QFile configFile(fileName);
    if(!configFile.exists())
    {
        configFile.open(QIODevice::WriteOnly);
        configFile.close();
    }
    return true;
}

QString CMDRecipeUIImp::fileItemPath(QTreeWidgetItem *pItem,int column)
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

void CMDRecipeUIImp::scriptWindowClose()
{
//    close(dynamic_cast<CMDJavaScriptCodeEditor2*>(sender()));
}

void CMDRecipeUIImp::on_actionTreeWindow_destroyed(QObject* obj)
{
}

/*!
\section 管理打开的配方编辑器界面
*/
void CMDRecipeUIImp::open(const QString& fileName)
{
    CMDRecipeFile rFile;
    rFile.setFileName(fileName);
    rFile.load();

    CMDRecipeMainDialog dlg;
    dlg.setProjectConfiger(m_studioInterface->projectConfiger());
    dlg.initial(this, NULL, rFile.recipe()->configProperty());
    dlg.setWindowTitle(STRANS(STRING_RECIPE_TEXT) + " - " + fileName);
    dlg.exec();
    if(QDialog::Accepted == dlg.result())
    {
        CMDRecipeProperty_Config* dest = (CMDRecipeProperty_Config*)dlg.m_para.destProperty();
        CMDRecipeProperty_Config* src = rFile.recipe()->configProperty();
        *src = *dest;
        rFile.save();
    }
}
