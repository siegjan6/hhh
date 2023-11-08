#include <QDir>
#include "ObjectManager.h"
#include "MDDrawMulti.h"
//////////////////////////////////////////////////////////////////////////
CObjectManager::CObjectManager(const QString& path, const QString &imagePath, QWidget* parent)
{
    ObjectSize = 200;//!单个图元的尺寸
    Interval = 5;
    sPattern = "*.gra";
    m_currentObj = NULL;
    m_index = 0;//!图元索引
    m_imagePath = imagePath;

    m_parent = parent;

    loadObj(path);
}
CObjectManager::~CObjectManager()
{
    for(int i = 0; i < m_list.count(); i++)
        delete m_list.at(i);
    m_list.clear();
}
//////////////////////////////////////////////////////////////////////////
QString CObjectManager::currentName()
{
    if (m_currentObj != NULL)
        return m_currentObj->name();

    return "";
}
//////////////////////////////////////////////////////////////////////////
void CObjectManager::paint(QPainter *painter)
{      
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->fillRect(QRect(0, 0, m_size.width(), m_size.height()), QBrush(Qt::white));

    int count = m_list.count();
    int colCount = m_size.width() / ObjectSize;
    for (int i = 0; i < count; i++)
    {
        int col = i % colCount;
        int row = i / colCount;
        QRect r(col*ObjectSize, row*ObjectSize, ObjectSize, ObjectSize);
        r.adjust(Interval, Interval, -Interval, -Interval);

        //background
        painter->fillRect(r, QBrush(Qt::lightGray));
        //drawobj
        paintObject(painter, r, m_list.at(i));
        //select frame
        if (i == m_index)
        {
            r.adjust(-3, -3, 3, 3);
            QPen pen(Qt::blue);
            pen.setWidth(3);
            painter->setPen(pen);
            painter->drawRect(r);
        }
    }
}
void CObjectManager::selectObject(int x, int y)
{
    int count = m_list.count();
    int colCount = m_size.width() / ObjectSize;
    int rowCount = count / colCount;
    if (count % colCount > 0)
        rowCount++;

    int col = x / ObjectSize;
    if (col >= colCount)
        return;
    int row = y / ObjectSize;
    if (row >= rowCount)
        return;
    int index = row * colCount + col;
    if (index >= count)
        return;

    setIndex(index);
}
QSize CObjectManager::calculateSize(int scrollAreaWidth, int scrollAreaHeight)
{
    int min = ObjectSize;
    int w = scrollAreaWidth;
    int h = scrollAreaHeight;
    int count = m_list.count();

    w = (w >= min) ? w : min;

    int colCount = w / ObjectSize;
    int rowCount = count / colCount;
    if (count % colCount > 0)
        rowCount++;

    h = rowCount * ObjectSize;
    h = (h >= min) ? h : min;

    m_size = QSize(w, h);
    return m_size;
}
//////////////////////////////////////////////////////////////////////////
void CObjectManager::loadObj(const QString &path)
{
    QDir directory = QDir(path);
    directory.setNameFilters(QStringList(sPattern));

    //file
    QFileInfoList files = directory.entryInfoList(QDir::Files);
    for(int i = 0; i < files.count(); i++)
    {
        QFileInfo f = files.at(i);
        QFile file(f.absoluteFilePath());
        if (!file.open(QIODevice::ReadOnly))
            continue;

        QByteArray ba;
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        ba = stream.readAll().toUtf8();
        file.close();

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(ba, &error);
        if(error.error != QJsonParseError::NoError)
            continue;

        deserialize(doc.object());
    }

    if (m_list.count() > 0)
    {
        m_index = 0;
        m_currentObj = m_list.at(m_index);
    }
}
void CObjectManager::deserialize(const QJsonObject &json)
{
    DrawType type = (DrawType)json.value("Type").toInt();
    CMDDrawObj* obj = CMDDrawTool::instance()->create()->createDrawObj(type);
    obj->deserialize(json);
    obj->setStorageImagePath(m_imagePath);
    obj->loadInitalizationEvent();
    obj->changeLanguage();

    m_list.append(obj);
}
void CObjectManager::setIndex(int value)
{
    if (m_index == value)
        return;

    int count = m_list.count();
    if (value >= 0 && value < count)
    {
        m_index = value;
        m_currentObj = m_list.at(m_index);
        m_parent->update();
    }
}
void CObjectManager::paintObject(QPainter *p, const QRect &rect, CMDDrawObj *obj)
{
    p->save();
    p->setClipRect(rect);

    //calculate size
    QPoint center = rect.center();
    QRectF bound = obj->bound();
    QPointF boundCenter = bound.center();
    qreal w = bound.width();
    qreal h = bound.height();
    qreal frame = ObjectSize - Interval*4;
    qreal xScale = 1;
    qreal yScale = 1;
    qreal scale = 1;

    //scale
    if (w > frame)
        xScale = frame / w;
    if (h > frame)
        yScale = frame / h;
    scale = (xScale < yScale) ? xScale : yScale;

    //translate
    qreal xOff = center.x() - boundCenter.x();
    qreal yOff = center.y() - boundCenter.y();

    //注意：缩放位移操作，后面的代码先生效
    p->translate(center);
    p->scale(scale, scale);
    p->translate(-center.x(), -center.y());
    p->translate(xOff, yOff);

    obj->draw(p, true);

    p->restore();
}
//////////////////////////////////////////////////////////////////////////
