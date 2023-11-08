#include "DrawListDataCover.h"
#include "DrawListData.h"
#include "MDDrawList.h"
#include <QDebug>
CDrawListDataCover::CDrawListDataCover(QObject *parent):
    QObject(parent)
{
    m_data = new CDrawListData;
    m_data->m_widget = new QListWidget();
    m_data->m_widget->installEventFilter(this);
    connect(m_data->m_widget,&QListWidget::itemSelectionChanged,this,
            &CDrawListDataCover::selChanged);
}

CDrawListDataCover::~CDrawListDataCover()
{
    delete m_data;
}


qint32 CDrawListDataCover::version() const
{
    return m_data->m_version;
}

void CDrawListDataCover::setVersion(const qint32 &version)
{
    m_data->m_version = version;
}

bool CDrawListDataCover::isShowScrollBar() const
{
    return m_data->m_isShowScrollBar;
}

void CDrawListDataCover::setIsShowScrollBar(bool isShowScrollBar)
{
    m_data->m_isShowScrollBar = isShowScrollBar;
}

QColor CDrawListDataCover::backColor() const
{
    return m_data->m_backColor;
}

void CDrawListDataCover::setBackColor(const QColor &backColor)
{
    m_data->m_backColor = backColor;
}

QColor CDrawListDataCover::disableBackColor() const
{
    return m_data->m_disableBackColor;
}

void CDrawListDataCover::setDisableBackColor(const QColor &disableBColor)
{
    m_data->m_disableBackColor = disableBColor;
}

QFont CDrawListDataCover::font() const
{
    return m_data->m_font;
}

void CDrawListDataCover::setFont(const QFont &font)
{
    m_data->m_font = font;
}

qint32 CDrawListDataCover::borderStyle() const
{
    return m_data->m_borderStyle;
}

void CDrawListDataCover::setBorderStyle(const qint32 &borderStyle)
{
    m_data->m_borderStyle = borderStyle;
}

qint32 CDrawListDataCover::selectionMode() const
{
    return m_data->m_selectionMode;
}

void CDrawListDataCover::setSelectionMode(const qint32 &selectionMode)
{
    m_data->m_selectionMode = selectionMode;
}

qint32 CDrawListDataCover::frameShadow() const
{
    return m_data->m_frameShadow;
}

void CDrawListDataCover::setFrameShadow(const qint32 &frameShadow)
{
    m_data->m_frameShadow = frameShadow;
}

QStringList CDrawListDataCover::items() const
{
    return m_data->m_items;
}

void CDrawListDataCover::setItems(const QStringList &items)
{
    m_data->m_items = items;
}

void CDrawListDataCover::addItem(int index, const QString &str)
{
    m_data->m_items.insert(index,str);
}

void CDrawListDataCover::removeItem(int index)
{
    m_data->m_items.removeAt(index);
}

void CDrawListDataCover::cleareAllItem()
{
    m_data->m_items.clear();
}

int CDrawListDataCover::findItem(int index, const QString &str)
{
    int count = m_data->m_items.count();
    if(index >= count)
        return -1;
    for(int i = index;i <count;i++)
    {
           if(m_data->m_items.at(i) == str)
               return i;
    }
    return -1;
}

bool CDrawListDataCover::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == m_data->m_widget)
    {
        if(event->type() == QEvent::EnabledChange)
        {
            if(!m_data->m_widget->isEnabled())
            {
               if(m_data->m_disableBackColor.isValid())
               {
                   QColor color = m_data->m_disableBackColor;
                   QString style = QString("background-color: rgb(%1, %2, %3);")
                           .arg(color.red())
                           .arg(color.green())
                           .arg(color.blue());
                   m_data->m_widget->setStyleSheet(style);
               }
            }
            else
            {
                if(m_data->m_backColor.isValid())
                {
                    QColor color = m_data->m_backColor;
                    QString style = QString("background-color: rgb(%1, %2, %3);")
                            .arg(color.red())
                            .arg(color.green())
                            .arg(color.blue());
                    m_data->m_widget->setStyleSheet(style);
                }
            }
            return false;
        }
    }
    return QObject::eventFilter(obj,event);
}
CMDDrawList *CDrawListDataCover::list() const
{
    return m_list;
}

void CDrawListDataCover::setList(CMDDrawList *list)
{
    m_list = list;
}

void CDrawListDataCover::selChanged()
{
    //TODO
    qDebug() << "changed"<<endl;
}


QListWidget *CDrawListDataCover::widget() const
{
    return m_data->m_widget;
}

void CDrawListDataCover::setWidget(QListWidget *widget)
{
    m_data->m_widget = widget;
}

