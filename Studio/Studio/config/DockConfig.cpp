#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMainWindow>
#include <QVariant>
#include <QAction>

#include "DockConfig.h"
#include "ConfigData.h"
#include "MDMultiLanguage.h"
////////////////////////////////////////////////////////////////
CDockConfig::CDockConfig(QMainWindow *parent)
{
    Q_ASSERT(parent);
    init(parent);
}
CDockConfig::~CDockConfig()
{
    qDeleteAll(m_list);
}
////////////////////////////////////////////////////////////////
void CDockConfig::loadWidget(QList<QWidget *>* widgets)
{
    int count = m_list.count();
    int widgetCount = widgets->count();
    Q_ASSERT(count == widgetCount);

    for(int i = 0, count = m_list.count(); i < count; i++)
        m_list.at(i)->setWidget(widgets->at(i));
}
void CDockConfig::fillDockMenu(QMenu* m)
{
    for(int i = 0; i < m_titles.count(); i++)
    {
        QAction* a = new QAction(m_titles.at(i), m);
        a->setData(i);
        connect(a, &QAction::triggered, this, &CDockConfig::showDock);
        m->addAction(a);
    }
}
////////////////////////////////////////////////////////////////
void CDockConfig::init(QMainWindow *parent)
{
    qDeleteAll(m_list);
    m_list.clear();
    m_titles.clear();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(CConfigData::Dock.toUtf8(), &error);
    Q_ASSERT(error.error == QJsonParseError::NoError);

    QJsonArray array = doc.array();
    QDockWidget* tabDock = NULL;
    for(int i = 0, count = array.count();i < count; i++)
    {
        QJsonObject obj = array.at(i).toObject();
        QString title = obj.value("title").toString();
        Qt::DockWidgetArea area = (Qt::DockWidgetArea)(obj.value("area").toInt());
        bool visible = obj.value("defaultVisible").toBool();
        bool isTab = obj.value("isTab").toBool();

        QDockWidget* dock = new QDockWidget(parent);
        dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);
        dock->setAllowedAreas(Qt::AllDockWidgetAreas);
        dock->setWindowTitle(CMDMultiLanguage::instance()->value(title));
        qobject_cast<QMainWindow*>(parent)->addDockWidget(area, dock);
        dock->setVisible(visible);
        m_titles.append(title);
        m_list.append(dock);

        if(isTab)
        {
            if(tabDock == NULL)
                tabDock = dock;
            else
            {
                //保持tabDock选中
                dock->setVisible(false);
                parent->tabifyDockWidget(tabDock, dock);
                dock->setVisible(visible);
            }
        }
        else
        {
            tabDock = NULL;
        }

    }
}
void CDockConfig::showDock()
{
    QAction* a = qobject_cast<QAction*>(sender());
    if(a == NULL)
        return;
    int index = a->data().toInt();
    m_list.at(index)->show();
    m_list.at(index)->raise();
}
////////////////////////////////////////////////////////////////
