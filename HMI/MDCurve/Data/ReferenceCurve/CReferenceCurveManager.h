#ifndef CREFERENCECURVEMANAGER_H
#define CREFERENCECURVEMANAGER_H
#include <QList>
#include <QObject>
#include <QRectF>
#include <QPainter>

class CReferenceCurve;
class CXAxisManager;
class CYAxisManager;
class CLabelManager;

class CReferenceCurveManager
{
public:
    CReferenceCurveManager();
    ~CReferenceCurveManager();

    /*!
    \section 参考曲线,x,y,绘制区域
    */
public:
    void appendReferenceCurve(CReferenceCurve *reCurve);

    QList<CReferenceCurve *>&list();
    void setList(const QList<CReferenceCurve *>&list);

    CXAxisManager *xs() const;
    void setXs(CXAxisManager *xs);

    CYAxisManager *ys() const;
    void setYs(CYAxisManager *ys);

    CLabelManager *labels() const;
    void setLabels(CLabelManager *labels);

    QRectF referenceCurveArea() const;
    void setReferenceCurveArea(const QRectF &rect);

private:
    QList<CReferenceCurve *>m_referenceList;
    CXAxisManager       *m_xs ;
    CYAxisManager       *m_ys ;
    CLabelManager       *m_labels;
    QRectF              m_referenceCurveArea;

    /*!
    \section 计算，绘制
    */
public:
    void calculate();
    void draw(QPainter*p);
public:
    void setXY(); // setting the x and y axis pointers.


    /*!
    \section 序列化和反序列化
    */
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

public:
    CReferenceCurveManager &operator =(const CReferenceCurveManager &other);

private:
    void clearItemList();

    /*!
    \section 基准曲线设置起始值
    */
public:
    void setValue(int index, QVariant value);                                //! 设置起始数值
    void setStartTime(const QString &curveName, const QVariant &startTime);
    void getExpressionList(QList<int> &indexs, QList<QString> &expressions); //! 获取表达式和与之对应的索引

};

#endif // CREFERENCECURVEMANAGER_H
