#include <QDir>
#include <QCoreApplication>
#include "HMIStudio.h"
#include "PropertyBrowser.h"
#include "FileNode.h"
#include "MDHMIForm.h"
#include "MDDrawTool.h"
#include "CreateTool.h"

CHMIStudio::CHMIStudio()
{
    m_property = new CPropertyBrowser;

    m_storage = NULL;
    m_currentForm = NULL;

    m_fileNode = new CFileNode();

    m_toolKitDialog.init(this);
    initStorageImagePath();

    CMDDrawTool::init(new CCreateTool);
}
CHMIStudio::~CHMIStudio()
{
    clear();
}
//////////////////////////////////////////////////////////////////////////
void CHMIStudio::clear()
{
    CMDHMIForm::clearCopyList();
    CMDExpConfig::clear();
    delete m_property;
    delete m_fileNode;
    if (m_storage != NULL)
        delete m_storage;

    CMDDrawTool::release();
}
//////////////////////////////////////////////////////////////////////////
void CHMIStudio::selectObject(QObject *obj)
{
    if (m_property == NULL)
        return;

    m_property->setObject(obj);
}
void CHMIStudio::refreshProperty()
{
    if (m_property == NULL)
        return;

    m_property->updateProperty();
}
//////////////////////////////////////////////////////////////////////////
CHMIFramework *CHMIStudio::open()
{
    return new CHMIFramework(this);
}
//////////////////////////////////////////////////////////////////////////
void CHMIStudio::setObjectTree(IHMIForm *form)
{
    m_objectTreeDialog.setHMIForm(form);
}
//////////////////////////////////////////////////////////////////////////
IGraphicsStorage *CHMIStudio::graphicsStorage()
{
    if (m_storage == NULL)
    {
        CMDGraphicsStorage nsStorage;
        m_storage = nsStorage.create(m_storageImagePath, NULL);
    }

    return m_storage;
}
//////////////////////////////////////////////////////////////////////////
void CHMIStudio::setCreateState(DrawType type)
{
    if (currentForm() == NULL)
        return;
    dynamic_cast<CMDHMIForm*>(currentForm())->setCreateObjectState(type);
}
void CHMIStudio::endCreate()
{
    if (currentForm() == NULL)
        return;
    dynamic_cast<CMDHMIForm*>(currentForm())->endCreate();
}
void CHMIStudio::resetToolKit()
{
    m_toolKitDialog.resetToolKit();
}
//////////////////////////////////////////////////////////////////////////
void CHMIStudio::changeLanguage()
{
    m_objectTreeDialog.changeLanguage();
}
//////////////////////////////////////////////////////////////////////////
void CHMIStudio::initStorageImagePath()
{
    m_storageImagePath = QCoreApplication::applicationDirPath() + "/image";

    QDir dir(m_storageImagePath);
    if (!dir.exists())
        dir.mkdir(m_storageImagePath);
}
//////////////////////////////////////////////////////////////////////////
void CHMIStudio::initFormPreview(IHMIFramework *framework)
{
    m_formPreviewDialog.init(dynamic_cast<CHMIFramework*>(framework));
}
//////////////////////////////////////////////////////////////////////////
