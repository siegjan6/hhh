#ifndef MDBRUSHDATA_H
#define MDBRUSHDATA_H

#include <QtCore/qglobal.h>


#if defined(MDCOLORMANAGER_LIBRARY)
#  define MDColorMANAGERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDColorMANAGERSHARED_EXPORT Q_DECL_IMPORT
#endif


#include <qbrush.h>

/*
 * class: CMDBrushManager
 * biref: 生成QBrush的数据类，可序列化
 * 平均功能
        |——对话框QBrushSelectDialog CMDBrushManager
        |——每次调用brush()函数都会去生成一个新的QBrush　需要自己临时保存好
 */
class MDColorMANAGERSHARED_EXPORT CMDBrushManager
{
public:
    enum class  BrushType
    {
        Nullptr,Solid,LinearGradient,RadialGradient,ConicalGradient,FillPicture
    };

    CMDBrushManager();
    virtual ~CMDBrushManager();

    void init(const QColor& clr);
    void init(const QColor& clr1,const QColor& clr2,qreal angle);

    void setAngle(int angle);
    int angle();

    void setColor(const QColor& clr);
    QColor color();

    void setStops(QGradientStops stops);
    QGradientStops stops();

    void setBrushType(BrushType type);
    BrushType brushType();

    QBrush brush(QRectF rt);

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

    bool showDialog();

private:
    qint32 m_angle;
    QColor m_color;
    QGradientStops m_stops;
public:
    BrushType m_type;

    //picture
public:
    qint32 m_position;
    QString m_imageName;
private:
    QBrush m_brush;
public:
    QImage m_image;
    QString m_projectPath;
public:
    void generate(const QRectF& rf);
    void draw(QPainter *p, const QRectF& rf, QPainterPath* path);
    QBrush getBrush();
public:
    void setProjectPath(const QString& path);
    bool isImage();
    void copyImage(const QString& sourcePath, const QString& targetPath, bool needRename);
private:
    QString findType(const QString &sourceName);

public:
    QString text() const;

};

#endif // MDBRUSHDATA_H
