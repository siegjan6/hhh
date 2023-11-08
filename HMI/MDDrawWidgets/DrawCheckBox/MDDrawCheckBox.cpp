#include <QButtonGroup>
#include "MDDrawCheckBox.h"
#include "DrawCheckBoxData.h"
#include "MDDrawCheckBoxEvent.h"
#include "MDPropertyDrawCheckBox.h"

CMDDrawCheckBox::CMDDrawCheckBox()
{
    m_data = new CDrawCheckBoxData;
    m_data->m_widget = new QCheckBox();
    m_data->m_event = new CMDDrawCheckBoxEvent();
    setWidget(m_data->m_widget,m_data->m_event);
    m_writeVarEnabled = true;
}

CMDDrawCheckBox::~CMDDrawCheckBox()
{
    delete m_data;
}

QCheckBox *CMDDrawCheckBox::widget()
{
    return m_data->m_widget;
}
/*!
\section 属性函数
*/
void CMDDrawCheckBox::setText(const QString &value)
{
    m_data->m_text = value;
    changeLanguage();
}

QString CMDDrawCheckBox::text()
{
    return m_data->m_text;
}

void CMDDrawCheckBox::setChecked(bool value)
{
     m_data->m_checked = value;
     m_data->m_widget->setAutoExclusive(false);
     m_data->m_widget->setChecked(value);
}

bool CMDDrawCheckBox::checked()
{
    return m_data->m_widget->checkState();
}

/*!
\section 脚本函数，读写文本
*/
void CMDDrawCheckBox::setScriptText(QString str)
{
    m_data->m_widget->setText(str);
}

QString CMDDrawCheckBox::getScriptText()
{
    QString string;
    string = m_data->m_widget->text();
    return string;
}

/*!
 \section 控件对象属性
*/
DrawType CMDDrawCheckBox::type()
{
    return DrawType::CheckBox;
}

CMDPropertyDrawObj *CMDDrawCheckBox::createPropertyDrawObj()
{
    return new CMDPropertyDrawCheckBox(this);
}

void CMDDrawCheckBox::onInitialization()
{
    CMDDrawBasicWidget::onInitialization();
    m_data->m_widget->setText((QString)m_data->m_text);
    m_data->m_widget->setChecked(m_data->m_checked);
//    changeLanguage();
}

CMDDrawObj *CMDDrawCheckBox::createSelf()
{
    return new CMDDrawCheckBox();
}
/*!
\section 序列化
*/
CMDDrawObj *CMDDrawCheckBox::clone()
{
    CMDDrawCheckBox* obj = static_cast<CMDDrawCheckBox*>(CMDDrawBasicWidget::clone());
    *obj->m_data = *m_data;
    return obj;
}

void CMDDrawCheckBox::serialize(QJsonObject &json)
{
    CMDDrawBasicWidget::serialize(json);
    QJsonObject js;
    m_data->serialize(js);
    json.insert("DrawCheckBox",js);
}

void CMDDrawCheckBox::deserialize(const QJsonObject &json)
{
    CMDDrawBasicWidget::deserialize(json);

    QJsonObject js = json.value("DrawCheckBox").toObject();
    m_data->deserialize(js);
}

QList<QStringList> CMDDrawCheckBox::expNames()
{
    QList<QStringList> list;
    list << QStringList({"#appearance"});
    list << QStringList({"drawcheckbox.checked",       QObject::tr("选中")});
    return list;
}
void CMDDrawCheckBox::onDataChanged(CMDExpInfo *exp)
{
    switch (exp->index())
    {
    case 0:
        m_writeVarEnabled = false;
        if(exp->decimalValue() == 0)
            setChecked(false);
        else
            setChecked(true);
        m_writeVarEnabled = true;
        break;
    default:
        break;
    }
}

void CMDDrawCheckBox::writeVariable()
{
    if (parent() == NULL)
        return;
    if (isStudioMode())
        return;

    QString textExpression = expManager()->list()->last()->lowerExpression();
    if (textExpression.count() == 0)
        return;

    if(m_data->m_widget->checkState() == 0)
        parent()->framework()->setVariableValue(textExpression, 0);
    else
        parent()->framework()->setVariableValue(textExpression, 2);

    setChecked(parent()->framework()->variableValue(textExpression).toBool());
}

void CMDDrawCheckBox::changeLanguage()
{
    m_data->m_showText = language()->value(m_data->m_text,false);
    m_data->m_widget->setText(m_data->m_showText);
}

void CMDDrawCheckBox::collectWords()
{
    language()->addWord(m_data->m_widget->text(),false);
}

bool CMDDrawCheckBox::writeVarEnabled()
{
    return m_writeVarEnabled;
}

void CMDDrawCheckBox::setWriteVarEnabled(bool writeVarEnabled)
{
    m_writeVarEnabled = writeVarEnabled;
}

//////////////////////////////////////////////////////////////////////////
CMDPropertyDrawObj *CMDDrawCheckBox::createPropertyDrawObj(const QString &className)
{
    if(className.compare("DrawCheckBox", Qt::CaseInsensitive) == 0)
        return new CMDPropertyDrawCheckBox(this);

    return CMDDrawBasicWidget::createPropertyDrawObj(className);
}

void CMDDrawCheckBox::initClassInfo(QStringList &classInfo)
{
    CMDDrawBasicWidget::initClassInfo(classInfo);
    classInfo.append("DrawCheckBox");
}
//////////////////////////////////////////////////////////////////////////




