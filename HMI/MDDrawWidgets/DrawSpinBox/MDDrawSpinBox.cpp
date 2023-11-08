#include "MDDrawSpinBox.h"
#include "DrawSpinBoxData.h"
#include "MDPropertyDrawSpinBox.h"
#include "MDDrawSpinBoxEvent.h"

CMDDrawSpinBox::CMDDrawSpinBox()
{
    m_data = new CDrawSpinBoxData;
    m_data->m_widget = new QSpinBox;
    m_data->m_event = new CMDDrawSpinBoxEvent();
    setWidget(m_data->m_widget,m_data->m_event);
    m_writeVarEnabled = true;
}

CMDDrawSpinBox::~CMDDrawSpinBox()
{
    delete m_data;
}

QSpinBox *CMDDrawSpinBox::widget()
{
    return m_data->m_widget;
}

/*!
\section 属性函数
*/
void CMDDrawSpinBox::setSingleStep(int value)
{
    m_data->m_singleStep = value;
    m_data->m_widget->setSingleStep(value);
}

int CMDDrawSpinBox::singleStep()
{
    return m_data->m_singleStep;
}

void CMDDrawSpinBox::setMaximum(int max)
{
    m_data->m_maximum = max;
    m_data->m_widget->setMaximum(max);
}

int CMDDrawSpinBox::maximum()
{
    return m_data->m_maximum;
}

void CMDDrawSpinBox::setMinimum(int min)
{
    m_data->m_minimum = min;
    m_data->m_widget->setMinimum(min);
}

int CMDDrawSpinBox::minimum()
{
    return m_data->m_minimum;
}

void CMDDrawSpinBox::setInitialValue(int value)
{
    m_data->m_initialValue = value;
    m_data->m_widget->setValue(value);
}

int CMDDrawSpinBox::initialValue()
{
    return m_data->m_initialValue;
}

/*!
\section 脚本属性函数
*/
void CMDDrawSpinBox::setCurrentValue(int value)
{
    m_data->m_widget->setValue(value);
}

int CMDDrawSpinBox::getCurValue()
{
    int value;
    value = m_data->m_widget->value();
    return value;
}
/*!
\section 控件对象属性
*/
DrawType CMDDrawSpinBox::type()
{
    return DrawType::SpinBox;
}

CMDPropertyDrawObj *CMDDrawSpinBox::createPropertyDrawObj()
{
    return new CMDPropertyDrawSpinBox(this);
}

void CMDDrawSpinBox::onInitialization()
{
    CMDDrawBasicWidget::onInitialization();

    if(!widget())
        return;
    m_data->m_widget->setSingleStep(m_data->m_singleStep);
    m_data->m_widget->setMaximum(m_data->m_maximum);
    m_data->m_widget->setMinimum(m_data->m_minimum);
    m_data->m_widget->setValue(m_data->m_initialValue);

    QLineEdit *lineEdit = widget()->findChild<QLineEdit *>("qt_spinbox_lineedit", Qt::FindDirectChildrenOnly);
    if(lineEdit)
    {
        widget()->findChild<QLineEdit *>("qt_spinbox_lineedit", Qt::FindDirectChildrenOnly)->setStyleSheet(getBasicWidgetColor());
    }
    widget()->setStyleSheet(CMDDrawBasicWidget::getBasicWidgetColor());
}

CMDDrawObj *CMDDrawSpinBox::createSelf()
{
    return new CMDDrawSpinBox();
}
/*!
\section 序列化
*/
CMDDrawObj *CMDDrawSpinBox::clone()
{
    CMDDrawSpinBox* obj = static_cast<CMDDrawSpinBox*>(CMDDrawBasicWidget::clone());
    *obj->m_data = *m_data;
    return obj;
}

void CMDDrawSpinBox::serialize(QJsonObject &json)
{
    CMDDrawBasicWidget::serialize(json);

    QJsonObject js;
    m_data->serialize(js);
    json.insert("DrawSpinBox",js);
}

void CMDDrawSpinBox::deserialize(const QJsonObject &json)
{
    CMDDrawBasicWidget::deserialize(json);

    QJsonObject js = json.value("DrawSpinBox").toObject();
    m_data->deserialize(js);
}

QList<QStringList> CMDDrawSpinBox::expNames()
{
    QList<QStringList> list;
    list << QStringList({"#appearance"});
    list << QStringList({"drawspinbox.value",       QObject::tr("数值")});
    return list;
}

void CMDDrawSpinBox::onDataChanged(CMDExpInfo *exp)
{
    switch (exp->index())
    {
    case 0: //! 值
        m_writeVarEnabled = false;
        setInitialValue(exp->decimalValue());
        m_writeVarEnabled = true;
        break;
    default:
        break;
    }
}

void CMDDrawSpinBox::writeVariable()
{
    if (parent() == NULL)
        return;
    if (isStudioMode())
        return;

    QString textExpression = expManager()->list()->last()->lowerExpression();
    if(textExpression.count() == 0)
        return;

    parent()->framework()->setVariableValue(textExpression,m_data->m_widget->value());

    setInitialValue(parent()->framework()->variableValue(textExpression).toInt());
}

bool CMDDrawSpinBox::writeVarEnabled() const
{
    return m_writeVarEnabled;
}

void CMDDrawSpinBox ::setWriteVarEnabled(bool writeVarEnabled)
{
    m_writeVarEnabled = writeVarEnabled;
}

//////////////////////////////////////////////////////////////////////////
CMDPropertyDrawObj *CMDDrawSpinBox::createPropertyDrawObj(const QString &className)
{
    if(className.compare("DrawSpinBox", Qt::CaseInsensitive) == 0)
        return new CMDPropertyDrawSpinBox(this);

    return CMDDrawBasicWidget::createPropertyDrawObj(className);
}

void CMDDrawSpinBox::initClassInfo(QStringList &classInfo)
{
    CMDDrawBasicWidget::initClassInfo(classInfo);
    classInfo.append("DrawSpinBox");
}

void CMDDrawSpinBox::setBackColor(const QColor &value)
{
    CMDDrawBasicWidget::setBackColor(value);


    QLineEdit *lineEdit = widget()->findChild<QLineEdit *>("qt_spinbox_lineedit", Qt::FindDirectChildrenOnly);
    if(lineEdit)
    {
        widget()->findChild<QLineEdit *>("qt_spinbox_lineedit", Qt::FindDirectChildrenOnly)->setStyleSheet(getBasicWidgetColor());
    }
    widget()->setStyleSheet(getBasicWidgetColor());
}

void CMDDrawSpinBox::setDisableBackColor(const QColor &value)
{
    CMDDrawBasicWidget::setDisableBackColor(value);

    QLineEdit *lineEdit = widget()->findChild<QLineEdit *>("qt_spinbox_lineedit", Qt::FindDirectChildrenOnly);
    if(lineEdit)
    {
        lineEdit->setStyleSheet(getBasicWidgetColor());
    }
    widget()->setStyleSheet(getBasicWidgetColor());
}
//////////////////////////////////////////////////////////////////////////
