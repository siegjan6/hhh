#include "cdemoclass.h"

CDemoClass::CDemoClass(QObject *parent) :
    QObject(parent)
{
    connect(this,SIGNAL(nameChanged(QVariant)),this,SLOT(slotInforChanged(QVariant)));
    connect(this,SIGNAL(otherChanged(QVariant)),this,SLOT(slotInforChanged(QVariant)));
}
QString CDemoClass::name() const
{
    return m_name;
}

void CDemoClass::setName(QString name)
{
    if(m_name != name)
    {
        m_name = name;
        emit nameChanged(name);
    }
}
QPixmap CDemoClass::pixmap() const
{
    return m_pixmap;
}

void CDemoClass::setPixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
    emit pixmapChanged(m_pixmap);
}
QString CDemoClass::other() const
{
    return m_other;
}

void CDemoClass::setOther(const QString &other)
{
    if(m_other != other){
        m_other = other;
        emit otherChanged(other);
    }
}

void CDemoClass::slotInforChanged(QVariant)
{
    QString infor = m_other + m_name;
    emit inforChanged(infor);
}



