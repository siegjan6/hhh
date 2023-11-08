#include "ValueMapManager.h"
#include "EditValueMapDialog.h"
#include "IDrawEnvironment.h"

#include <QtXml>
#include <QFile>

CValueMapManager* CValueMapManager::g_ptr = NULL;

CValueMapManager* gValueMapManager = NULL;

CValueMapManager::CValueMapManager()
{

}



CValueMapManager *CValueMapManager::getInstance()
{
    if( !g_ptr )
        g_ptr = new CValueMapManager;

    return g_ptr;
}

QString CValueMapManager::editValueMapUI(IDrawEnvrionment *environment, QString mapName, QWidget *parent)
{
    CEditValueMapDialog editDlg(environment, mapName, parent);
    if( editDlg.exec() == QDialog::Accepted )
        return editDlg.m_currentMapName;

    return mapName;
}

bool CValueMapManager::isValueMapExist(IDrawEnvrionment *environment, QString mapName)
{
    QString prjpath = environment->projectPath();
    TotalValues::iterator iterprj = m_prjValues.find(prjpath);
    if( iterprj == m_prjValues.end() )
        return false;
    ListIndex::iterator iterindex = iterprj->index.find(mapName);

    if( iterindex == iterprj->index.end() )
        return false;

    return true;
}

void CValueMapManager::valueMapNames(IDrawEnvrionment *environment, QList<QString> &names)
{
    QString prjpath = environment->projectPath();
    TotalValues::iterator iterprj = m_prjValues.find(prjpath);
    if( iterprj == m_prjValues.end() )
        return;

    ValueList& lists = iterprj->list;

    foreach(const ValueInfo& info, lists)
        names.append(info.name);

    return;
}

void CValueMapManager::readFile(IDrawEnvrionment *environment)
{
    QString prjpath = environment->projectPath();
    TotalValues::iterator iterprj = m_prjValues.find(prjpath);
    if( iterprj != m_prjValues.end() )
        return;

    iterprj = m_prjValues.insert(prjpath, PrjInfo());

    ValueList& lists = iterprj->list;
    ListIndex& indexs = iterprj->index;
    //! 读取当前工程的valuemap.xml文件
    QString filePath = prjpath + "/" + ValueMapFileName;
    QFile file(filePath);
    if( !file.open(QIODevice::ReadOnly) )
        return;
    QDomDocument doc;
    if( !doc.setContent(&file) )
    {
        file.close();
        return;
    }

    file.close();
    //! 读取根节点
    QDomNode rootNode = doc.documentElement();

    QDomNode mapRootNode = rootNode.firstChild();
    while( !mapRootNode.isNull() )
    {
        QDomElement mapRootElement = mapRootNode.toElement();
        QString mapName = mapRootElement.attribute("mapname");
        QString mapDesc = mapRootElement.attribute("mapdesc");
        ValueInfo vinfo;
        vinfo.name = mapName;
        vinfo.description = mapDesc;
        vinfo.values.clear();

        QDomNode vnode = mapRootNode.firstChild();
        while( !vnode.isNull() )
        {
            QDomElement vElement = vnode.toElement();
            vinfo.values.append(ValuePair(vElement.attribute("value"), vElement.attribute("desc")));

            vnode = vnode.nextSibling();
        }

        lists.append(vinfo);
        indexs.insert(mapName, lists.size()-1);

        mapRootNode = mapRootNode.nextSibling();
    }



}

void CValueMapManager::deleteValueMap(IDrawEnvrionment *environment, QString mapName)
{
    QString prjpath = environment->projectPath();
    TotalValues::iterator iterprj = m_prjValues.find(prjpath);
    if( iterprj == m_prjValues.end() )
        return;

    ListIndex::iterator iterindex = iterprj->index.find(mapName);
    if( iterindex == iterprj->index.end() )
        return;

    iterprj->list.removeAt(*iterindex);

    //! 重建index索引表
    iterprj->index.clear();
    for( qint32 i = 0; i < iterprj->list.count(); i++ )
        iterprj->index.insert(iterprj->list.at(i).name, i);
}

bool CValueMapManager::valueMap(IDrawEnvrionment *environment, QString mapName, ValueInfo &info)
{
    QString prjpath = environment->projectPath();
    TotalValues::iterator iterprj = m_prjValues.find(prjpath);
    if( iterprj == m_prjValues.end() )
        return false;

    ListIndex::iterator iterindex = iterprj->index.find(mapName);
    if( iterindex == iterprj->index.end() )
        return false;

    const ValueInfo& srcInfo = iterprj->list.at(*iterindex);

    info.name = srcInfo.name;
    info.description = srcInfo.description;
    info.values.clear();
    info.values.append(srcInfo.values);

    return true;
}

bool CValueMapManager::valueList(IDrawEnvrionment *environment, QString mapName, CValueMapManager::Values &values)
{
    QString prjpath = environment->projectPath();
    TotalValues::iterator iterprj = m_prjValues.find(prjpath);
    if( iterprj == m_prjValues.end() )
        return false;

    ListIndex::iterator iterindex = iterprj->index.find(mapName);
    if( iterindex == iterprj->index.end() )
        return false;

    const ValueInfo& srcInfo = iterprj->list.at(*iterindex);
    values.clear();
    values.append(srcInfo.values);

    return true;
}

void CValueMapManager::saveValueMap(IDrawEnvrionment *environment, CValueMapManager::ValueInfo &info)
{
    QString prjpath = environment->projectPath();
    TotalValues::iterator iterprj = m_prjValues.find(prjpath);
    if( iterprj == m_prjValues.end() )
        iterprj = m_prjValues.insert(prjpath, PrjInfo());
    ListIndex::iterator iterindex = iterprj->index.find(info.name);

    if( iterindex == iterprj->index.end() )
    {
        iterprj->list.append(info);
        //!同步更新表容器的索引表
        iterindex = iterprj->index.insert(info.name, iterprj->list.size()-1);
    }
    else
    {
        ValueInfo& tmpInfo = iterprj->list[*iterindex];
        tmpInfo.values.clear();
        tmpInfo.values.append(info.values);
    }
}

void CValueMapManager::saveFile(IDrawEnvrionment *environment)
{
    QString prjpath = environment->projectPath();
    TotalValues::iterator iterprj = m_prjValues.find(prjpath);
    if( iterprj == m_prjValues.end() )
        iterprj = m_prjValues.insert(prjpath, PrjInfo());


    QString filePath = environment->projectPath() + "/" + ValueMapFileName;
    //! 保存数据到当前工程的valuemap.xml文件
    QDomDocument doc;
    //! xml开头说明
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);
    //! 根节点
    QDomElement root = doc.createElement("ValueMaps");
    doc.appendChild(root);

    ValueList& valuelist = iterprj->list;
    ValueList::const_iterator iterlist = valuelist.constBegin();
    while (iterlist != valuelist.constEnd())
    {
        const ValueInfo& currentInfo = *iterlist;

        QDomElement mapRootNode = doc.createElement("map");
        root.appendChild(mapRootNode);
        //! 保存表名
        QDomAttr nameAttr = doc.createAttribute("mapname");
        nameAttr.setValue(currentInfo.name);
        mapRootNode.setAttributeNode(nameAttr);
        //! 保存表描述
        QDomAttr descAttr = doc.createAttribute("mapdesc");
        descAttr.setValue(currentInfo.description);
        mapRootNode.setAttributeNode(descAttr);

        const Values& currentValues = currentInfo.values;
        //! 遍历保存所有的键值对
        Values::const_iterator itervalue = currentValues.constBegin();
        while (itervalue != currentValues.constEnd())
        {
            QDomElement valueNode = doc.createElement("pair");
            QDomAttr valueAttr = doc.createAttribute("value");
            valueAttr.setValue(itervalue->first);
            valueNode.setAttributeNode(valueAttr);
            QDomAttr descAttr = doc.createAttribute("desc");
            descAttr.setValue(itervalue->second);
            valueNode.setAttributeNode(descAttr);

            mapRootNode.appendChild(valueNode);
            ++itervalue;
        }

        ++iterlist;

    }

    //! 保存数据到xml文件
    QFile file(filePath);
    if( !file.open(QIODevice::WriteOnly | QIODevice::Truncate) )
        return;

    QTextStream out(&file);
    doc.save(out, 4);
    file.close();

}
