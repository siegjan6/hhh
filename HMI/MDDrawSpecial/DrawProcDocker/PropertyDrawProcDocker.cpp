#include "PropertyDrawProcDocker.h"
#include "MDDrawProcDocker.h"
#include <QFileDialog>
#include <QColorDialog>

CPropertyDrawProcDocker::CPropertyDrawProcDocker(CMDDrawObj *parent) :
    CMDPropertyDrawObj(parent)
{
    m_cmd = "";

    insertTranslateMap("config",tr("配置"));
    insertTranslateMap("GUID_Data",tr("数据"));
    //insertTranslateMap("FileDialogFileName",tr("文件选择"));
    //insertTranslateMap("ReportBackColor",tr("背景色"));
}

CMDDrawProcDocker *CPropertyDrawProcDocker::object()
{
    return dynamic_cast<CMDDrawProcDocker*>(m_object);
}


//void CPropertyDrawReport::setFileName(const QString &value)
//{
//    m_XMLFileName = value;
//    object()->setXMLfileName(value);
//    setModify(true);
//}

//QString CPropertyDrawReport::getFileName()
//{
////    return m_XMLFileName;
//    return object()->getXMLfileName();
//}

void CPropertyDrawProcDocker::setCmd(const QString &value)
{
    m_cmd = value;
    object()->setCmd(m_cmd);
    setModify(true);
}

QString CPropertyDrawProcDocker::getCmd()
{
    return object()->getCmd();
}
