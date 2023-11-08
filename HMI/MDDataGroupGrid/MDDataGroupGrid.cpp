#include <QDataStream>
#include "MDDataGroupGrid.h"
#include "DataGroupWidget.h"
#include "MDGroupGridDefine.h"

CMDDataGroupGrid::CMDDataGroupGrid()
{
    m_widget = new CDataGroupWidget();
}

CMDDataGroupGrid::~CMDDataGroupGrid()
{
    if(m_widget)
    {
        delete m_widget;
        m_widget = NULL;
    }
}

QWidget *CMDDataGroupGrid::widget()
{
    return m_widget;
}

void CMDDataGroupGrid::setDrawEnvrionment(IDrawEnvrionment *value)
{
    m_widget->setDrawEnvrionment(value);
}

void CMDDataGroupGrid::onInitialization()
{
    m_widget->onInitialization();
}

void CMDDataGroupGrid::setNotify(bool notify)
{
    m_widget->setNotify(notify);
}

void CMDDataGroupGrid::setQueryEngine(IMDDBQueryEngine *pEngine)
{
    m_widget->setQueryEngine(pEngine);
}

void CMDDataGroupGrid::query()
{
    m_widget->query();
}

bool CMDDataGroupGrid::openConfigDialog()
{
    return m_widget->openConfigDialog();
}

void CMDDataGroupGrid::onDoubleClick()
{
    openConfigDialog();
}

void CMDDataGroupGrid::setGridType(GridType type)
{
    m_widget->setGridType(type);
}

void CMDDataGroupGrid::setGridName(const QString &name)
{
    m_widget->setGridName(name);
}

CMDDataGroupGrid &CMDDataGroupGrid::operator =(const CMDDataGroupGrid &other)
{
    if(this == &other)
    {
        return *this;
    }
    *m_widget = *other.m_widget;
    return *this;
}

void CMDDataGroupGrid::serialize(QDataStream &stream, bool isOrigVersion)
{
    m_widget->serialize(stream, isOrigVersion);
}

void CMDDataGroupGrid::deserialize(QDataStream &stream)
{
    m_widget->deserialize(stream);
}

void CMDDataGroupGrid::serialize_1(QDataStream &stream)
{
    m_widget->serialize_1(stream);
}

void CMDDataGroupGrid::serialize(QJsonObject &json)
{
    m_widget->serialize(json);
}

void CMDDataGroupGrid::deserialize(const QJsonObject &json)
{
    m_widget->deserialize(json);
}
