/*!

\author eHVr

\section 功能

曲线管理类，
\brief 管理所有曲线，在计算时，为曲线设定其绑定的x，y轴数据指针。
属性:
\a dataSource
*/
#ifndef CCURVEOBJECTMANAGER_H
#define CCURVEOBJECTMANAGER_H
#include <QList>
#include <QPainter>
#include "CDataSource.h"
#include <QObject>

#include <QPixmap>

class CCurveObject;
class CXAxisManager;
class CYAxisManager;
class CLabelManager;


class CCurveObjectManager
{
public:
    CCurveObjectManager();
    ~CCurveObjectManager();
/*!
\section 曲线，x，y，绘制区域
*/
public:
    void appendCurve(CCurveObject *curve);    

    QList<CCurveObject *> &list();
    void setList(const QList<CCurveObject *> &list);

    CXAxisManager *xs() const;
    void setXs(CXAxisManager *xs);

    CYAxisManager *ys() const;
    void setYs(CYAxisManager *ys);

    CLabelManager *labels() const;
    void setLabels(CLabelManager *labels);

    QRectF curveArea() const;
    void setCurveArea(const QRectF &rect);

    void setType(bool isReal);
    void setFresh(qint32 ms);

private:
    QList<CCurveObject*> m_list;
    CXAxisManager       *m_xs;
    CYAxisManager       *m_ys;
    CLabelManager       *m_labels;
    QRectF               m_curveArea;

/*!
\section 数据库相关
*/
public:
    CDataSource dataSource() const;
    void setDataSource(const CDataSource &dataSource);

    QString sourceName() const;
    void setSourceName(const QString &sourceName);

    QString tableName() const;
    void setTabelName(const QString &tableName);

    QStringList fields() const;
    void setFields(const QStringList &fields);
private:
    CDataSource  m_dataSource;

/*!
\section 计算，绘制
*/
public:
    void calculate();
    void draw(QPainter*p);

/*!
\section 序列化和反序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
private:
    QString    m_projectPath;

public:
    void setXY(); // setting the x and y axis pointers.
    CCurveObjectManager &operator =(const CCurveObjectManager &other);
    void setProjectPath(const QString &path);
    QString projectPath() const;

private:
    void clearItemList();

};

#endif // CCURVEOBJECTMANAGER_H
