#include "MDBrushManager.h"
#include <QtMath>
#include "MDBrushSelectDialog.h"
#include "MDBrush_Widget.h"
#include "picture_dialog/FillPictureDialog.h"
#include <QUuid>
#include <QFile>
#include <QTranslator>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

CMDBrushManager::CMDBrushManager()
{
    m_imageName = "";
    m_position =0;
    m_angle = 0;
    m_color = Qt::black;
    m_type = BrushType::Nullptr;
    init(Qt::black,Qt::red,0);
    init(Qt::black);
}

CMDBrushManager::~CMDBrushManager()
{
}

void CMDBrushManager::init(const QColor &clr)
{
    setColor(clr);
    setBrushType(BrushType::Solid);
}

void CMDBrushManager::init(const QColor &clr1, const QColor &clr2, qreal angle)
{
    m_stops.clear();
    QGradientStops stops;
    QGradientStop stop1;
    stop1.first = 0;
    stop1.second = clr1;
    QGradientStop stop2;
    stop2.first = 1;
    stop2.second = clr2;
    stops.append(stop1);
    stops.append(stop2);
    setStops(stops);
    setAngle(angle);
    setBrushType(BrushType::LinearGradient);
}

void CMDBrushManager::setAngle(int angle)
{
    m_angle = angle;
}

int CMDBrushManager::angle()
{
    return m_angle;
}

void CMDBrushManager::setColor(const QColor &clr)
{
    m_color = clr;
}

QColor CMDBrushManager::color()
{
    return m_color;
}

void CMDBrushManager::setStops(QGradientStops stops)
{
    m_stops.clear();
    m_stops = stops;
}

QGradientStops CMDBrushManager::stops()
{
    return m_stops;
}

void CMDBrushManager::setBrushType(CMDBrushManager::BrushType type)
{
    m_type = type;
}

CMDBrushManager::BrushType CMDBrushManager::brushType()
{
    return m_type;
}
QPointF getCoordinate(QRectF rt,float angle);

QBrush CMDBrushManager::brush(QRectF rt)
{
    int angle = m_angle;
    switch(m_type)
    {
    case BrushType::Nullptr:
        m_brush = Qt::NoBrush;
        break;
    case BrushType::Solid:
        m_brush = QBrush(m_color);
        break;
    case BrushType::LinearGradient:
    {
        QPointF pt1 = getCoordinate(rt,angle);
        if(angle >=180)
            angle -= 180;
        else
            angle += 180;
        QPointF pt2 = getCoordinate(rt,angle);
        QLinearGradient  br(pt2,pt1);
        br.setStops(m_stops);
        m_brush = QBrush(br);
    }
        break;
    case BrushType::RadialGradient:
    {
        QPointF pt = rt.center();
        qreal radius = rt.height() > rt.width()? rt.height()/2:rt.width()/2;
        QRadialGradient br(pt,radius);
        br.setStops(m_stops);
        m_brush = QBrush(br);
    }
        break;
    case BrushType::ConicalGradient:
    {
        QPointF pt = rt.center();
        QConicalGradient br(pt,0);
        br.setStops(m_stops);
        m_brush = QBrush(br);
    }
        break;
    case BrushType::FillPicture:
    {
        QBrush b;
        b.setTextureImage(m_image);
        m_brush = b;

        QTransform transform;
        m_brush.setTransform(transform);
        transform.translate(rt.x(),rt.y());
        m_brush.setTransform(transform);
    }
        break;
    default:
        break;
    }
    return m_brush;
}

void CMDBrushManager::serialize(QJsonObject &json)
{
    QJsonObject brushJson;
    brushJson.insert("Type",(int)m_type);
    brushJson.insert("Angle",m_angle);
    brushJson.insert("Color",(double)m_color.rgba());
    brushJson.insert("ImageName",m_imageName);
    brushJson.insert("Position",m_position);
    QJsonArray array;
    int count = m_stops.count();
    for(int i = 0; i <count; i++)
    {
        QJsonObject stopJson;
        stopJson.insert("First",m_stops.at(i).first);
        stopJson.insert("Second",(double)m_stops.at(i).second.rgba());
        array.append(stopJson);
    }
    brushJson.insert("Stops",array);
    json.insert("BrushManager",brushJson);
}

void CMDBrushManager::deserialize(const QJsonObject &json)
{
    QJsonObject brushJson = json.value("BrushManager").toObject();
    int type = brushJson.value("Type").toInt();
    m_type = (BrushType)type;
    m_angle = brushJson.value("Angle").toInt();
    m_color = QColor::fromRgba(brushJson.value("Color").toDouble());
    m_imageName = brushJson.value("ImageName").toString();
    m_position = brushJson.value("Position").toInt();
    m_stops.clear();
    QJsonArray array = brushJson.value("Stops").toArray();
    int count = array.count();
    for(int i = 0; i <count; i++)
    {
        QJsonObject stopJson = array.at(i).toObject();
        double first = stopJson.value("First").toDouble();
        QColor second = QColor::fromRgba(stopJson.value("Second").toDouble());
        QGradientStop stop;
        stop.first = first;
        stop.second = second;
        m_stops.append(stop);
    }
}

bool CMDBrushManager::showDialog()
{
    CMDBrushSelectDialog* dlg = new CMDBrushSelectDialog();
    dlg->p_widgetBrush->m_picture->setProjectPath(m_projectPath + "/");
    if(m_type == BrushType::FillPicture)
    {
        dlg->p_widgetBrush->m_picture->m_position = m_position;
        dlg->p_widgetBrush->m_picture->setImageName(m_imageName);
        dlg->p_widgetBrush->m_picture->inintDialog();
    }
    dlg->setBrushManager(*this);
    bool bresult = false;
    if(QDialog::Accepted  == dlg->exec())
    {
        *this = dlg->brushManager();
        bresult = true;
        m_position = dlg->p_widgetBrush->m_picture->m_position;
        m_imageName = dlg->p_widgetBrush->m_picture->imageName();
        m_image.load(m_projectPath + "/" + m_imageName);
    }
    delete dlg;
    return bresult;
}

void CMDBrushManager::generate(const QRectF &rf)
{
    brush(rf);
}
void CMDBrushManager::draw(QPainter *p, const QRectF &rf, QPainterPath *path)
{
    if (m_type == BrushType::FillPicture && m_position == 1)
    {
        QRectF clipRect = p->clipBoundingRect();
        p->setClipRect(rf, Qt::IntersectClip);

        QSize s = m_image.size();
        qreal x = rf.x() + (rf.width() - s.width()) / 2;
        qreal y = rf.y() + (rf.height() - s.height()) / 2;
        p->drawImage(QPointF(x,y),m_image);

        if(clipRect.isEmpty())
            p->setClipRect(clipRect, Qt::NoClip);
        else
            p->setClipRect(clipRect, Qt::ReplaceClip);
    }
    else if (m_type == BrushType::FillPicture && m_position == 2)
    {
        p->drawImage(rf, m_image);
    }
    else
    {
        p->fillPath(*path, m_brush);
    }
}
QBrush CMDBrushManager::getBrush()
{
    return m_brush;
}
void CMDBrushManager::setProjectPath(const QString &path)
{
    m_projectPath = path;
    if (m_type == BrushType::FillPicture)
    {
        m_image.load(m_projectPath + "/" + m_imageName);
    }
}

bool CMDBrushManager::isImage()
{
    return (m_type == BrushType::FillPicture);
}

void CMDBrushManager::copyImage(const QString &sourcePath, const QString &targetPath, bool needRename)
{
     if (sourcePath == targetPath)
     {
       return;
     }
     if (!QFile::exists(sourcePath))
     {
       return;
     }
     QString sourceP = sourcePath + "/" + m_imageName;
     QString targetP = targetPath + "/" + m_imageName;
     if(!QFile::copy(sourceP, targetP))
     {
       return;
     }
     if(needRename)
     {
         QUuid quid;
         quid.Random;
         QString guidString;
         guidString = quid.createUuid().toString();
         int count = guidString.count();
         QString newName = guidString.mid(1, count-2);
         QString suffix = findType(m_imageName);
         QString newFileName = newName + suffix;
         QFile::rename(targetP,targetPath+"/"+newFileName);
         m_imageName = newFileName;
     }
     return;
}

QString CMDBrushManager::findType(const QString &sourceName)
{
    int j =0;
    for(int i = 0;i<sourceName.count();i++)
    {
        if( sourceName.at(i)== '.')
        {
             j = i;
        }
    }
    QString suffix = sourceName.mid(j, sourceName.count()-1);
    return suffix;
}

QString CMDBrushManager::text() const
{
    QString str = "";
    switch(m_type)
    {
    case BrushType::Nullptr:
        str = QObject::tr("空画刷");
        break;
    case BrushType::Solid:
        str = QObject::tr("单色");
        break;
    case BrushType::LinearGradient:
        str = QObject::tr("线性渐变");
        break;
    case BrushType::RadialGradient:
        str = QObject::tr("辐射渐变");
        break;
    case BrushType::ConicalGradient:
        str = QObject::tr("圆锥渐变");
        break;
    case BrushType::FillPicture:
        str = QObject::tr("图片");
        break;
    default:
        break;
    }
    return str;
}

//! 弧度转角度
//float tan(float f)
//{
//    float r = qTan(f*3.1416/180);
//    return r;
//}

//! 计算角度
QPointF getCoordinate(QRectF rt,float angle)
{
    qreal x1,y1,x2,y2,x3,y3,x4,y4;
    x1 = rt.x();
    y1  = rt.y();
    x2 = rt.right();
    y2 = rt.y();
    x3 = rt.x();
    y3 = rt.bottom();
    x4 = rt.right();
    y4 = rt.bottom();
    float crossx;
    float crossy;
    //! 中心点
    float cx = x1+rt.width()/2;
    float cy = y1+rt.height()/2;
    float width = x2 - x1;
    float height = y3 - y1;
    //! 对角线与中心横线的角度
    qreal fff = qAtan2(height,width);
    float b = qRadiansToDegrees(fff);//! 根据正切值求角度
    if (angle <= 90)
    {
        if (angle < b)
        {
            crossx = x2;
            double t = tan(angle);
            crossy = cy + t*(width/2);
        }
        else
        {
            crossy = y3;
            double t = tan(90-angle);
            crossx = cx + t*(height/2);
        }
    }
    else if (angle <= 180)
    {
        if (180-angle < b)
        {
            crossx = x1;
            double t = tan(180-angle);
            crossy = cy + t*(width/2);
        }
        else
        {
            crossy = y3;
            double t = tan(angle-90);
            crossx = cx - t*(height/2);
        }
    }
    else if (angle <= 270)
    {
        if (angle-180 < b)
        {
            crossx = x1;
            double t = tan((angle-180));
            crossy = cy - t*(width/2);
        }
        else
        {
            crossy = y1;
            double t = tan(270-angle);
            crossx = cx - t*(height/2);
        }
    }
    else if (angle <= 360)
    {
        if (360-angle < b)
        {
            crossx = x2;
            double t = tan(360-angle);
            crossy = cy - t*(width/2);
        }
        else {
            crossy = y1;
            double t = tan(angle-270);
            crossx = cx + t*(height/2);
        }
    }
    return QPointF(crossx,crossy);
}
