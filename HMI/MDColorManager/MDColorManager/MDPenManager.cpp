#include "MDPenManager.h"
#include "MDPenSelectDialog.h"
#include "MDBrush_Widget.h"
#include "picture_dialog/FillPictureDialog.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

CMDPenManager::CMDPenManager()
{
    m_width = 1;
    m_style = Qt::PenStyle::SolidLine;
    m_joinStyle = Qt::PenJoinStyle::MPenJoinStyle;
    m_capStyle= Qt::PenCapStyle::MPenCapStyle;
}

void CMDPenManager::setBrushManager(CMDBrushManager brushMgr)
{
    m_brushMgr = brushMgr;
}

CMDBrushManager CMDPenManager::brushManager()
{
    return m_brushMgr;
}

void CMDPenManager::init(const QColor &clr, qreal widthf)
{
    setWidth(widthf);
    m_brushMgr.init(clr);
}
QPen CMDPenManager::pen(QRectF rt)
{
    QPen pen;
    pen.setWidth(width());
    pen.setStyle(style());
    pen.setCapStyle(capStyle());
    pen.setJoinStyle(joinStyle());
    pen.setBrush(brushManager().brush(rt));

    if(width() == 0)
    {
        pen.setColor(QColor(0,0,0,0));
    }

    m_pen = pen;

    return m_pen;
}

Qt::PenStyle CMDPenManager::style() const
{
    return m_style;
}

void CMDPenManager::setStyle(Qt::PenStyle style)
{
    m_style = style;
}

Qt::PenCapStyle CMDPenManager::capStyle() const
{
    return m_capStyle;
}

void CMDPenManager::setCapStyle(Qt::PenCapStyle pcs)
{
    m_capStyle = pcs;
}

Qt::PenJoinStyle CMDPenManager::joinStyle() const
{
    return m_joinStyle;
}

void CMDPenManager::setJoinStyle(Qt::PenJoinStyle pcs)
{
    m_joinStyle = pcs;
}

int CMDPenManager::width() const
{
    return m_width;
}

void CMDPenManager::setWidth(int width)
{
    m_width = width;
}

void CMDPenManager::serialize(QJsonObject &json)
{
    QJsonObject penJson;
    penJson.insert("CapStyle",(int)m_capStyle);
    penJson.insert("JoinStyle",(int)m_joinStyle);
    penJson.insert("Style",(int)m_style);
    penJson.insert("Width",m_width);
    json.insert("PenManager",penJson);
    m_brushMgr.serialize(json);

}

void CMDPenManager::deserialize(const QJsonObject &json)
{
    QJsonObject penJson = json.value("PenManager").toObject();
    int cap = penJson.value("CapStyle").toInt();
    m_capStyle = (Qt::PenCapStyle)cap;
    int join = penJson.value("JoinStyle").toInt();
    m_joinStyle = (Qt::PenJoinStyle)join;
    int style = penJson.value("Style").toInt();
    m_style = (Qt::PenStyle)style;
    m_width = penJson.value("Width").toInt();
    m_brushMgr.deserialize(json);

}

bool CMDPenManager::showDialog()
{
    CMDPenSelectDialog* dlg = new CMDPenSelectDialog();
    dlg->p_widgetBrush->m_picture->setCSDisable();
    dlg->p_widgetBrush->m_picture->setProjectPath(m_brushMgr.m_projectPath+"/");
    if(m_brushMgr.m_type == CMDBrushManager::BrushType::FillPicture)
    {
        dlg->p_widgetBrush->m_picture->m_position = m_brushMgr.m_position;
        dlg->p_widgetBrush->m_picture->setImageName(m_brushMgr.m_imageName);
        dlg->p_widgetBrush->m_picture->inintDialog();
    }

    dlg->setPenManager(*this);
    bool bresult = false;
    if(QDialog::Accepted == dlg->exec())
    {
        *this = dlg->penManager();
        bresult = true;
        m_brushMgr.m_position = dlg->p_widgetBrush->m_picture->m_position;
        m_brushMgr.m_imageName = dlg->p_widgetBrush->m_picture->m_imageName;
        m_brushMgr.m_image.load(dlg->p_widgetBrush->m_picture->m_pathName);
    }
    delete dlg;
    return bresult;
}

void CMDPenManager::generate(const QRectF &rf)
{
    pen(rf);
}

void CMDPenManager::draw(QPainter *p, const QRectF &rf, QPainterPath *path)
{
    //线宽=0时某些情况下依然会绘制线条 by aGRw
    if(m_pen.width() == 0)
       return;

    if(m_brushMgr.isImage())
    {
        QTransform transform;
        QBrush b = m_pen.brush();
        b.setTransform(transform);
        transform.translate(rf.x(),rf.y());
        b.setTransform(transform);
        m_pen.setBrush(b);
    }

    p->setPen(m_pen);
    p->drawPath(*path);

}
QPen CMDPenManager::getPen()
{
    return m_pen;
}
void CMDPenManager::setProjectPath(const QString &path)
{
    m_brushMgr.setProjectPath(path);
}

void CMDPenManager::copyImage(const QString &sourcePath, const QString &targetPath, bool needRename)
{
    m_brushMgr.copyImage(sourcePath,targetPath,needRename);
}

QString CMDPenManager::text() const
{
    QString str = "";
    switch(m_brushMgr.m_type)
    {
    case CMDBrushManager::BrushType::Nullptr:
        str = QObject::tr("空画刷");
        break;
    case CMDBrushManager::BrushType::Solid:
        str = QObject::tr("单色");
        break;
    case CMDBrushManager::BrushType::LinearGradient:
        str = QObject::tr("线性渐变");
        break;
    case CMDBrushManager::BrushType::RadialGradient:
        str = QObject::tr("辐射渐变");
        break;
    case CMDBrushManager::BrushType::ConicalGradient:
        str = QObject::tr("圆锥渐变");
        break;
    case CMDBrushManager::BrushType::FillPicture:
        str = QObject::tr("图片");
        break;
    default:
        break;
    }
    return str;
}

