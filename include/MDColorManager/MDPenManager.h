#ifndef MDPENMANAGER_H
#define MDPENMANAGER_H
#include <qpen.h>
#include "MDBrushManager.h"

#if defined(MDCOLORMANAGER_LIBRARY)
#  define MDColorMANAGERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDColorMANAGERSHARED_EXPORT Q_DECL_IMPORT
#endif

/*
 * class: CMDPenManager
 * biref: 生成QPen的数据类，可序列化
 * 平均功能
        |——对话框QPenSelectDialog 配置CMDPenManager
        |——每次调用pen函数都会去生成一个新的QPen　需要自己临时保存好
 */
class MDColorMANAGERSHARED_EXPORT CMDPenManager
{
public:
    CMDPenManager();
    void setBrushManager(CMDBrushManager brushMgr);
    CMDBrushManager brushManager();

    void init(const QColor& clr,qreal widthf);

    QPen pen(QRectF rt);

    Qt::PenStyle style() const;//线型
    void setStyle(Qt::PenStyle);

    Qt::PenCapStyle capStyle() const;
    void setCapStyle(Qt::PenCapStyle pcs);

    Qt::PenJoinStyle joinStyle() const;
    void setJoinStyle(Qt::PenJoinStyle pcs);

    int width() const;
    void setWidth(int width);

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

    bool showDialog();
private:
    CMDBrushManager m_brushMgr;
    qint32 m_width;
    Qt::PenStyle m_style;
    Qt::PenJoinStyle m_joinStyle;
    Qt::PenCapStyle m_capStyle;

    //fillpicture
private:
    QPen m_pen;
public:
    void generate(const QRectF& rf);
    void draw(QPainter *p, const QRectF& rf, QPainterPath* path);
    QPen getPen();
    void setProjectPath(const QString& path);
    void copyImage(const QString& sourcePath, const QString& targetPath, bool needRename);

public:
    QString text() const;
};

#endif // MDPENMANAGER_H
