#include "ImageItem.h"
#include <QPainter>
#include "ShareTool.h"
CImageItem::CImageItem()
{  
}

CImageItem *CImageItem::clone() const
{
    return new CImageItem(*this);
}

void CImageItem::onInitialization(IDrawEnvrionment *environment)
{
    CGridBaseItem::onInitialization(environment);
    m_imageName = text();
    if( !CShareTool::m_isStudioMode )
    {
        //setData("", Qt::DisplayRole);
        //setData("", Qt::EditRole);
        if( !m_valueMapName.isEmpty() && dataSource() == dataSourceType::realTimeVariable )
            m_imageName = ":/noimage";
    }
    setEditable(CShareTool::m_isStudioMode);
}

bool CImageItem::needCustomPaint(QPainter *painter, const QStyleOptionViewItem &option)
{
    QRect rect = option.rect;
    Q_ASSERT(m_environment);



    QString imagePath;
    if( m_imageName.isEmpty() || 0 == m_imageName.compare(":/noimage") )
        imagePath = ":/noimage";
    else
        imagePath = m_environment->projectPath() + "/image/" + m_imageName;

    QImage image(imagePath);
    painter->drawImage(rect, image);

    //qDebug("image = %s", qPrintable(imagePath));

    return true;
}

bool CImageItem::updateShowString(QVariant value, QWidget *editor)
{
    Q_UNUSED(editor);

    qint32 index = indexOfValue(value);

    if( index >= 0 && index < m_values.count() )
        m_imageName = m_values.at(index).second;
    else
        m_imageName = ":/noimage";

    bool forceUpdate = false;
    if( 0 != m_preImageName.compare(m_imageName) )
        forceUpdate =  true;

    m_preImageName = m_imageName;
    return forceUpdate;
    //qDebug("image = %s", qPrintable(m_imageName));
}

void CImageItem::setImageName(QString name)
{
    m_imageName = name;
    setText(name);
}
