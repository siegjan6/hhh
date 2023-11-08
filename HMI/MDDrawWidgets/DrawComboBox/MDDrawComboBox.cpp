#include "MDDrawComboBox.h"
#include "MDDrawComboBoxEvent.h"
#include "MDPropertyDrawComboBox.h"
#include "DrawComboBoxData.h"
#include "MDDrawBasicWidget.h"
#include <QDebug>

CMDDrawComboBox::CMDDrawComboBox()
{

    m_data =  new CDrawComboBoxData;
    m_data->setComboBox(new QComboBox());
    m_data->setEvent(new CMDDrawComboBoxEvent());
    setWidget(m_data->comboBox(),m_data->event());
    m_writeVarEnabled = true;
}

CMDDrawComboBox::~CMDDrawComboBox()
{
    delete m_data;
}

QComboBox *CMDDrawComboBox::widget()
{
    return m_data->comboBox();
}
/*!
\section  属性函数
*/
void CMDDrawComboBox::setItemStyle(int value)
{
    m_data->setItemStyle(value);
    m_data->comboBox()->setStyleSheet(getBasicWidgetColor());
    switch (value)
    {
    case 0:
        m_data->comboBox()->setEditable(true);
        if(m_data->comboBox()->lineEdit())
        {
            m_data->comboBox()->lineEdit()->setFont(*CMDDrawBasicWidget::font());
            m_data->comboBox()->lineEdit()->setStyleSheet(getBasicWidgetColor());
        }
        break;
    case 1:
        m_data->comboBox()->setEditable(false);
        break;
    default:
        break;
    }
}

int CMDDrawComboBox::itemStyle()
{
    return m_data->itemStyle();
}

void CMDDrawComboBox::setItemAdd(const QStringList &value)
{
    m_data->m_itemAdd = value;
    m_data->comboBox()->clear();
    m_data->comboBox()->addItems(value);
    if(m_data->sort())
    {
        sortItems();
    }
}

QStringList CMDDrawComboBox::itemAdd()
{
    return m_data->m_itemAdd;
}

void CMDDrawComboBox::setLayoutDirection(bool value)
{
    m_data->setLayoutDirection(value);
    m_data->comboBox()->setLayoutDirection(value ? Qt::LayoutDirection::RightToLeft : Qt::LayoutDirection::LeftToRight);
}

bool CMDDrawComboBox::layoutDirection()
{
    return m_data->layoutDirection();
}
/*!
\section   脚本属性函数
*/
void CMDDrawComboBox::setCurIndex(int num)
{
    m_data->setCurrentIndex(num);
    m_data->comboBox()->setCurrentIndex(num);
}

int CMDDrawComboBox::getCurrentIndex()
{
    return m_data->comboBox()->currentIndex();
}

void CMDDrawComboBox::setCurText(QString str)
{
    m_data->setCurrentText(str);
    m_data->comboBox()->setCurrentText(str);
}

QString CMDDrawComboBox::getCurText()
{
    return m_data->comboBox()->currentText();
}

void CMDDrawComboBox::setSorted(bool value)
{
    m_data->setSort(value);
    if(value)
    {
        sortItems();
    }
}

bool CMDDrawComboBox::Sorted()
{
    return m_data->sort();
}

QString CMDDrawComboBox::indexString(int index)
{
    return m_data->comboBox()->itemText(index);
}

void CMDDrawComboBox::insertItem(int index, QString value)
{
    m_data->comboBox()->insertItem(index,value);
}

void CMDDrawComboBox::addItem(const QString &value)
{
    m_data->comboBox()->addItem(value);
}

void CMDDrawComboBox::clearItems()
{
    m_data->comboBox()->clear();
}

void CMDDrawComboBox::removeItem(int index)
{
    m_data->comboBox()->removeItem(index);
}

int CMDDrawComboBox::listCount()
{
    return m_data->comboBox()->count();
}

int CMDDrawComboBox::findString(int index, QString str)
{
    for(int i = index; i < m_data->comboBox()->count();i++)
    {
        if(m_data->comboBox()->itemText(i).contains(str))
        {
            return i;
            break;
        }
    }
    return -1;
}

int CMDDrawComboBox::selectString(int index, QString str)
{
    int selectIndex = findString(index,str);
    m_data->comboBox()->setCurrentIndex(selectIndex);
    return selectIndex;
}

/*!
\section  控件对象属性
*/
DrawType CMDDrawComboBox::type()
{
    return DrawType::ComboBox;
}

CMDPropertyDrawObj *CMDDrawComboBox::createPropertyDrawObj()
{
    return new CMDPropertyDrawComboBox(this);
}

void CMDDrawComboBox::onInitialization()
{
    CMDDrawBasicWidget::onInitialization();
    m_data->setSort(m_data->sort());
    m_data->comboBox()->addItems(m_data->m_itemAdd);
    m_data->comboBox()->setCurrentIndex(0);
    switch (m_data->itemStyle())
    {
    case 0:
        m_data->comboBox()->setEditable(true);
        if(m_data->comboBox()->lineEdit())
        {
            m_data->comboBox()->lineEdit()->setFont(*CMDDrawBasicWidget::font());
            m_data->comboBox()->lineEdit()->setStyleSheet(getBasicWidgetColor());
        }
        break;
    case 1:
        m_data->comboBox()->setEditable(false);
        break;
    }
    m_data->comboBox()->setStyleSheet(getBasicWidgetColor());
    m_data->comboBox()->setLayoutDirection((Qt::LayoutDirection)m_data->layoutDirection());
}

CMDDrawObj *CMDDrawComboBox::createSelf()
{
    return new CMDDrawComboBox();
}
/*!
\section   序列化
*/
CMDDrawObj *CMDDrawComboBox::clone()
{
    CMDDrawComboBox* obj = static_cast<CMDDrawComboBox*>(CMDDrawBasicWidget::clone());
    *obj->m_data = *m_data;
    return obj;
}

void CMDDrawComboBox::serialize(QJsonObject &json)
{
    CMDDrawBasicWidget::serialize(json);

    QJsonObject js;
    m_data->serialize(js);
    json.insert("DrawComboBox",js);
}

void CMDDrawComboBox::deserialize(const QJsonObject &json)
{
    CMDDrawBasicWidget::deserialize(json);

    QJsonObject js = json.value("DrawComboBox").toObject();
    m_data->deserialize(js);
}

QList<QStringList> CMDDrawComboBox::expNames()
{
    QList<QStringList> list;
    list << QStringList({"#appearance"});
    list << QStringList({"drawcombobox.index",       QObject::tr("索引")});
    return list;
}

void CMDDrawComboBox::onDataChanged(CMDExpInfo *exp)
{
    switch (exp->index())
    {
    case 0: //! 当前索引
        m_writeVarEnabled = false;
        setCurIndex(exp->decimalValue());
        m_writeVarEnabled = true;
        break;
    default:
        break;
    }
}

void CMDDrawComboBox::writeVariable()
{
    if (parent() == NULL)
        return;
    if (isStudioMode())
        return;

    QString textExpression = expManager()->list()->last()->lowerExpression();
    if (textExpression.count() == 0)
        return;

    parent()->framework()->setVariableValue(textExpression,m_data->comboBox()->currentIndex());

    setCurIndex(parent()->framework()->variableValue(textExpression).toInt());

}

void CMDDrawComboBox::sortItems()
{
    m_data->m_itemAdd.clear();
    for(int i =0; i < m_data->comboBox()->count(); i++)
    {
        m_data->m_itemAdd.append(m_data->comboBox()->itemText(i));
    }
    m_data->m_itemAdd.sort();
    m_data->comboBox()->clear();
    m_data->comboBox()->addItems(m_data->m_itemAdd);
}

bool CMDDrawComboBox::writeVarEnabled()
{
    return m_writeVarEnabled;
}

void CMDDrawComboBox::setWriteVarEnabled(bool writeVarEnabled)
{
    m_writeVarEnabled = writeVarEnabled;
}

//////////////////////////////////////////////////////////////////////////
CMDPropertyDrawObj *CMDDrawComboBox::createPropertyDrawObj(const QString &className)
{
    if(className.compare("DrawComboBox", Qt::CaseInsensitive) == 0)
        return new CMDPropertyDrawComboBox(this);

    return CMDDrawBasicWidget::createPropertyDrawObj(className);
}

void CMDDrawComboBox::initClassInfo(QStringList &classInfo)
{
    CMDDrawBasicWidget::initClassInfo(classInfo);
    classInfo.append("DrawComboBox");
}

void CMDDrawComboBox::setTextColor(const QColor &value)
{
    CMDDrawBasicWidget::setTextColor(value);

    QColor textColor = CMDDrawBasicWidget::textColor();
    QString textColorString = QString("background-color:rgb(%1,%2,%3,%4); color:rgb(%5,%6,%7,%8)")
            .arg(textColor.red())
            .arg(textColor.green())
            .arg(textColor.blue())
            .arg(textColor.alpha());
    if(m_data->comboBox()->lineEdit())
    {
        m_data->comboBox()->lineEdit()->setStyleSheet(textColorString);
    }
}

void CMDDrawComboBox::setBackColor(const QColor &value)
{
    CMDDrawBasicWidget::setBackColor(value);

    QColor backColor = CMDDrawBasicWidget::backColor();
    QString backColorString = QString("background-color:rgb(%1,%2,%3,%4); color:rgb(%5,%6,%7,%8)")
            .arg(backColor.red())
            .arg(backColor.green())
            .arg(backColor.blue())
            .arg(backColor.alpha());
    if(m_data->comboBox()->lineEdit())
    {
        m_data->comboBox()->lineEdit()->setStyleSheet(backColorString);
    }
}

void CMDDrawComboBox::setDisableBackColor(const QColor &value)
{
    CMDDrawBasicWidget::setDisableBackColor(value);

    QColor disableBackColor = CMDDrawBasicWidget::disableBackColor();
    QString disableBackColorString = QString("background-color:rgb(%1,%2,%3,%4); color:rgb(%5,%6,%7,%8)")
            .arg(disableBackColor.red())
            .arg(disableBackColor.green())
            .arg(disableBackColor.blue())
            .arg(disableBackColor.alpha());
    if(m_data->comboBox()->lineEdit())
    {
        m_data->comboBox()->lineEdit()->setStyleSheet(disableBackColorString);
    }
}

void CMDDrawComboBox::setFont(const QFont *value)
{
    CMDDrawBasicWidget::setFont(value);
    if(m_data->comboBox()->lineEdit())
    {
        m_data->comboBox()->lineEdit()->setFont(*CMDDrawBasicWidget::font());
    }
}
//////////////////////////////////////////////////////////////////////////

