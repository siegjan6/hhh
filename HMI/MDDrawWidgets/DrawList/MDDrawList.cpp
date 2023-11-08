#include "MDDrawList.h"
#include "DrawListData.h"
#include "MDPropertyDrawList.h"
#include "MDDrawListEvent.h"

CMDDrawList::CMDDrawList()
{
    m_data = new CDrawListData;
    m_data->m_widget = new QListWidget;
    m_data->m_event = new CMDDrawListEvent();
    setWidget(m_data->m_widget,m_data->m_event);
}

CMDDrawList::~CMDDrawList()
{
    delete m_data;
}

QListWidget *CMDDrawList::widget()
{
    return m_data->m_widget;
}

/*!
\section 属性函数
*/
void CMDDrawList::setHasScrollBar(bool value)
{
    m_data->m_hasScrollBar = value;
    m_data->m_widget->setVerticalScrollBarPolicy(value ? Qt::ScrollBarAsNeeded : Qt::ScrollBarAlwaysOff);
}

bool CMDDrawList::hasScrollBar()
{
    return m_data->m_hasScrollBar;
}

void CMDDrawList::setBorderStyle(QFrame::Shape value)
{
    m_data->m_borderStyle = value;
    m_data->m_widget->setFrameShape(value);
}

QFrame::Shape CMDDrawList::borderStyle()
{
    return (QFrame::Shape)m_data->m_borderStyle;
}

void CMDDrawList::setSelectionMode(QAbstractItemView::SelectionMode value)
{
    m_data->m_selectionMode = value;
    m_data->m_widget->setSelectionMode(value);
}

QAbstractItemView::SelectionMode CMDDrawList::selectionMode()
{
    return (QAbstractItemView::SelectionMode)m_data->m_selectionMode;
}

void CMDDrawList::setLayoutDirection(Qt::LayoutDirection value)
{
    m_data->m_layoutDirection = value;
    m_data->m_widget->setLayoutDirection(value);
}

Qt::LayoutDirection CMDDrawList::layoutDirection()
{
    return (Qt::LayoutDirection)m_data->m_layoutDirection;
}

void CMDDrawList::setItemAdd(const QStringList &value)
{
    m_data->m_itemAdd = value;
    m_data->m_widget->clear();
    m_data->m_widget->addItems(value);
    setSortMode(m_data->m_sorted);
}

QStringList CMDDrawList::itemAdd()
{
    return m_data->m_itemAdd;
}

/*!
\section   脚本属性函数
*/
void CMDDrawList::setCurrentIndex(int num)
{
    m_data->m_widget->setCurrentRow(num);
}

int CMDDrawList::currentIndex()
{
    return m_data->m_widget->currentRow();
}

QString CMDDrawList::currentItem()
{
    if(m_data->m_widget->currentItem())
    {
        return m_data->m_widget->currentItem()->text();
    }
    else
    {
        return "";
    }
}

void CMDDrawList::setSorted(int value)
{
    m_data->m_sorted = value;
    setSortMode(value);
}

int CMDDrawList::sorted()
{
    return m_data->m_sorted;
}

int CMDDrawList::listCount()
{
    return m_data->m_widget->count();
}

int CMDDrawList::selectCount()
{
    if(m_data->m_selectionMode == QAbstractItemView::MultiSelection)
    {
        return m_data->m_widget->selectedItems().count();
    }
    return -1;
}

QString CMDDrawList::indexString(int index)
{
    return m_data->m_widget->item(index)->text();
}

void CMDDrawList::insertItem(int index, QString value)
{
    m_data->m_widget->insertItem(index,value);
    m_data->m_itemAdd.insert(index,value);
    setSortMode(m_data->m_sorted);
}

void CMDDrawList::addItem(const QString &value)
{
    m_data->m_widget->addItem(value);
    m_data->m_itemAdd.append(value);
    setSortMode(m_data->m_sorted);
}

void CMDDrawList::clearItems()
{
    m_data->m_widget->clear();
}

void CMDDrawList::removeItem(int index)
{
    m_data->m_widget->takeItem(index);
}

int CMDDrawList::findString(int index, QString str)
{
    for(int i = index; i < m_data->m_widget->count();i++)
    {
        if(m_data->m_widget->item(i)->text().contains(str))
        {
            return i;
        } 
    }
    return -1;
}

void CMDDrawList::setSortMode(qint32 value)
{
    switch (value)
    {
    case 0: //! 不排序
        m_data->m_widget->clear();
        m_data->m_widget->addItems(m_data->m_itemAdd);
        break;
    case 1: //! 正序
        m_data->m_widget->sortItems(Qt::AscendingOrder);
        break;
    case 2: //! 反序
        m_data->m_widget->sortItems(Qt::DescendingOrder);
        break;
    default:
        break;
    }
}

/*!
\section  控件对象属性
*/
DrawType CMDDrawList::type()
{
    return DrawType::List;
}

CMDPropertyDrawObj *CMDDrawList::createPropertyDrawObj()
{
    return new CMDPropertyDrawList(this);
}

void CMDDrawList::onInitialization()
{
    CMDDrawBasicWidget::onInitialization();

    m_data->m_widget->setVerticalScrollBarPolicy(m_data->m_hasScrollBar ? Qt::ScrollBarAsNeeded : Qt::ScrollBarAlwaysOff);
    m_data->m_widget->setFrameShape((QFrame::Shape)m_data->m_borderStyle);
    m_data->m_widget->setSelectionMode((QAbstractItemView::SelectionMode)m_data->m_selectionMode);
    m_data->m_widget->setLayoutDirection((Qt::LayoutDirection)m_data->m_layoutDirection);
    m_data->m_widget->clear();
    m_data->m_widget->addItems(m_data->m_itemAdd);
    setSortMode(m_data->m_sorted);
}

CMDDrawObj *CMDDrawList::createSelf()
{
    return new CMDDrawList();
}
/*!
\section   序列化
*/

CMDDrawObj *CMDDrawList::clone()
{
    CMDDrawList* obj = static_cast<CMDDrawList*>(CMDDrawBasicWidget::clone());
    *obj->m_data = *m_data;
    return obj;
}

void CMDDrawList::serialize(QJsonObject &json)
{
    CMDDrawBasicWidget::serialize(json);

    QJsonObject js;
    m_data->serialize(js);
    json.insert("DrawList",js);
}

//////////////////////////////////////////////////////////////////////////
void CMDDrawList::deserialize(const QJsonObject &json)
{
    CMDDrawBasicWidget::deserialize(json);

    QJsonObject js = json.value("DrawList").toObject();
    m_data->deserialize(js);
}

CMDPropertyDrawObj *CMDDrawList::createPropertyDrawObj(const QString &className)
{
    if(className.compare("DrawList", Qt::CaseInsensitive) == 0)
        return new CMDPropertyDrawList(this);

    return CMDDrawBasicWidget::createPropertyDrawObj(className);
}

void CMDDrawList::initClassInfo(QStringList &classInfo)
{
    CMDDrawBasicWidget::initClassInfo(classInfo);
    classInfo.append("DrawList");
}
//////////////////////////////////////////////////////////////////////////
