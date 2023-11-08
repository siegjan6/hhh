#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QVariant>

#include "MDDrawObjectConfig.h"
#include "MDHMIForm.h"
#include "HMIConfigData.h"

void CMDDrawObjectConfig::createToolkit(QTreeWidget* parent)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(CHMIConfigData::DrawObject.toUtf8(), &error);
    Q_ASSERT(error.error == QJsonParseError::NoError);

    QJsonArray array = doc.array();
    for(int i = 0, count = array.count();i < count; i++)
    {
        QJsonObject group = array.at(i).toObject();
        bool notInToolkit = group.value("notInToolkit").toBool();
        if(notInToolkit)
            continue;
        QString groupText = group.value("text").toString();
        QJsonArray objArray = group.value("items").toArray();
        QString path = group.value("imagePath").toString();

        QTreeWidgetItem* groupItem = new QTreeWidgetItem(parent, QStringList(groupText));

        for(int j = 0, objCount = objArray.count(); j < objCount; j++)
        {
            QJsonObject obj = objArray.at(j).toObject();
            QString text = obj.value("text").toString();
            int type = obj.value("type").toInt();
            QString image = obj.value("image").toString();

            QTreeWidgetItem* item = new QTreeWidgetItem(groupItem, QStringList(text));
            item->setData(0, Qt::UserRole, type);
            item->setIcon(0, QIcon(path+image));
        }
    }
}
QMenu *CMDDrawObjectConfig::createMenu(CMDHMIForm *parent)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(CHMIConfigData::DrawObject.toUtf8(), &error);
    Q_ASSERT(error.error == QJsonParseError::NoError);

    QMenu* createMenu = new QMenu(QObject::tr("控件"), parent);

    QJsonArray array = doc.array();
    for(int i = 0, count = array.count();i < count; i++)
    {
        QJsonObject group = array.at(i).toObject();
        bool notInToolkit = group.value("notInToolkit").toBool();
        if(notInToolkit)
            continue;
        QString groupText = group.value("text").toString();
        QJsonArray objArray = group.value("items").toArray();

        QMenu* groupMenu = createMenu->addMenu(groupText);

        for(int j = 0, objCount = objArray.count(); j < objCount; j++)
        {
            QJsonObject obj = objArray.at(j).toObject();
            QString text = obj.value("text").toString();
            int type = obj.value("type").toInt();

            QAction* action = new QAction(text, createMenu);
            action->setProperty("type", type);
            QObject::connect(action, &QAction::triggered, parent, &CMDHMIForm::createObjectState);
            groupMenu->addAction(action);
        }
    }

    return createMenu;
}
QMap<int, QIcon *>* CMDDrawObjectConfig::createDrawIcons()
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(CHMIConfigData::DrawObject.toUtf8(), &error);
    Q_ASSERT(error.error == QJsonParseError::NoError);

    QMap<int, QIcon*>* map = new QMap<int, QIcon*>();
    QJsonArray array = doc.array();
    for(int i = 0, count = array.count();i < count; i++)
    {
        QJsonObject group = array.at(i).toObject();
        QJsonArray objArray = group.value("items").toArray();
        QString path = group.value("imagePath").toString();

        for(int j = 0, objCount = objArray.count(); j < objCount; j++)
        {
            QJsonObject obj = objArray.at(j).toObject();
            int type = obj.value("type").toInt();
            QString image = obj.value("image").toString();
            map->insert(type, new QIcon(path+image));
        }
    }

    return map;
}
