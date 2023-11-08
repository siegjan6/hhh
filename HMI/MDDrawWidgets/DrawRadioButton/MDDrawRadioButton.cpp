#include <QButtonGroup>
#include "MDDrawRadioButton.h"
#include "DrawRadioButtonData.h"
#include "MDDrawRadioButtonEvent.h"
#include "MDPropertyDrawRadioButton.h"

CMDDrawRadioButton::CMDDrawRadioButton()
{
    m_data = new CDrawRadioButtonData;
    m_data->m_widget = new QRadioButton();
    m_data->m_event = new CMDDrawRadioButtonEvent();
    setWidget(m_data->m_widget,m_data->m_event);
}

CMDDrawRadioButton::~CMDDrawRadioButton()
{
    delete m_data;
}

QRadioButton *CMDDrawRadioButton::widget()
{
    return m_data->m_widget;
}

/*!
\section   属性函数
*/
void CMDDrawRadioButton::setText(const QString &value)
{
    m_data->m_text = value;
    changeLanguage();
}

QString CMDDrawRadioButton::text()
{
    return m_data->m_text;
}

void CMDDrawRadioButton::setChecked(bool value)
{
    m_data->m_checked = value;
    m_data->m_widget->setChecked(value);
}

bool CMDDrawRadioButton::checked()
{
    return m_data->m_widget->isChecked();
}

void CMDDrawRadioButton::setGroupId(int value)
{
    m_data->m_groupId = value;
    resetMap();
}

int CMDDrawRadioButton::groupId()
{
    return m_data->m_groupId;
}

void CMDDrawRadioButton::setCheckValue(qreal value)
{
    m_data->m_checkValue = value;
}

qreal CMDDrawRadioButton::checkValue()
{
    return m_data->m_checkValue;
}

/*!
\section  控件对象属性
*/
DrawType CMDDrawRadioButton::type()
{
    return DrawType::RadioButton;
}

CMDPropertyDrawObj *CMDDrawRadioButton::createPropertyDrawObj()
{
    return new CMDPropertyDrawRadioButton(this);
}

void CMDDrawRadioButton::onInitialization()
{
    CMDDrawBasicWidget::onInitialization();
    m_data->m_widget->setText((QString)m_data->m_text);
    m_data->m_widget->setChecked(m_data->m_checked);
    resetMap();
    // changeLanguage();
}

CMDDrawObj *CMDDrawRadioButton::createSelf()
{
    return new CMDDrawRadioButton();
}

/*!
\section   序列化
*/
CMDDrawObj *CMDDrawRadioButton::clone()
{
    CMDDrawRadioButton* obj = static_cast<CMDDrawRadioButton*>(CMDDrawBasicWidget::clone());
    *obj->m_data = *m_data;
    return obj;
}

void CMDDrawRadioButton::serialize(QJsonObject &json)
{
    CMDDrawBasicWidget::serialize(json);

    QJsonObject js;
    m_data->serialize(js);
    json.insert("DrawRadioButton",js);
}

void CMDDrawRadioButton::deserialize(const QJsonObject &json)
{
    CMDDrawBasicWidget::deserialize(json);

    QJsonObject js = json.value("DrawRadioButton").toObject();
    m_data->deserialize(js);
}

void CMDDrawRadioButton::resetMap()
{
    if (isStudioMode())
    {
        QButtonGroup* g = new QButtonGroup(dynamic_cast<QMainWindow*>(parent()));
        g->addButton(m_data->m_widget);
        g->setExclusive(false);
    }
    else
    {
        long formId = (long)parent();
        QString key = QString::number(formId) + "_" +QString::number(m_data->m_groupId);
        QButtonGroup* g = m_data->m_buttonGroups.value(key,NULL);
        if(g == NULL)
        {
            g = new QButtonGroup(dynamic_cast<QMainWindow*>(parent()));
            m_data->m_buttonGroups.insert(key,g);
        }
        g->addButton(m_data->m_widget);
    }
}

QList<QStringList> CMDDrawRadioButton::expNames()
{
    QList<QStringList> list;
    list << QStringList({"#appearance"});
    list << QStringList({"drawradiobutton.checked",       QObject::tr("选中")});
    return list;
}

void CMDDrawRadioButton::onDataChanged(CMDExpInfo *exp)
{
    switch (exp->index())
    {
    case 0:     //checkValue
        if (exp->decimalValue() == m_data->m_checkValue)
        {
            setChecked(true);
        }
        else
        {
            m_data->m_widget->setChecked(false);
            m_data->m_widget->setCheckable(false);
            m_data->m_widget->update();
            m_data->m_widget->setCheckable(true);
        }
        break;
    default:
        break;
    }
}
void CMDDrawRadioButton::checkChanged(bool value)
{
    if (value)
    {
        m_data->m_checkExpression = expManager()->list()->last();

        QString name = m_data->m_checkExpression->lowerExpression();
        if (name.length() > 0)
        {
            parent()->framework()->setVariableValue(name,m_data->m_checkValue);
            parent()->framework()->variableValue(name);
        }
    }
}

void CMDDrawRadioButton::changeLanguage()
{
    m_data->m_showText = language()->value(m_data->m_text,false);
    m_data->m_widget->setText(m_data->m_showText);
}

void CMDDrawRadioButton::collectWords()
{
    language()->addWord(m_data->m_widget->text(),false);
}

//////////////////////////////////////////////////////////////////////////
CMDPropertyDrawObj *CMDDrawRadioButton::createPropertyDrawObj(const QString &className)
{
    if(className.compare("DrawRadioButton", Qt::CaseInsensitive) == 0)
        return new CMDPropertyDrawRadioButton(this);

    return CMDDrawBasicWidget::createPropertyDrawObj(className);
}

void CMDDrawRadioButton::initClassInfo(QStringList &classInfo)
{
    CMDDrawBasicWidget::initClassInfo(classInfo);
    classInfo.append("DrawRadioButton");
}
//////////////////////////////////////////////////////////////////////////

