/*!

\author eHVr

\section 功能

    标签管理基类
    派生：CHistoryTagManager
          CRealTimeTagManager
*/
#ifndef CTAGMANAGER_H
#define CTAGMANAGER_H

#include <QString>
#include <QFont>
#include <QVector>
#include <QRectF>

#include "CDataSource.h"
#include "Data.h"

class CPoint;
class CXAxis;
class CFeatureTagLine;
class CXAxisManager;

class CTagManager
{
public:
    CTagManager();
    virtual ~CTagManager();

    //! 可见
    bool visible() const;
    void setVisible(bool visible);

    //! 描述
    QString comment() const;
    void setComment(const QString &comment);

    //! 项间距
    qint32 interval() const;
    void setInterval(const qint32 &interval);

    //! 字体
    QFont font() const;
    void setFont(const QFont &font);

    //! 字段
    CDataSource dataSource() const;
    void setDataSource(const CDataSource &dataSource);

    QStringList fields() const;
    void setFields(const QStringList &fields);

    //! Studio模式
    bool isStudioMode() const;
    void setIsStudioMode(bool isStudioMode);

    //! 历史曲线参考X轴线名称
    QString reAxisName() const;
    void setReAxisName(const QString &reAxisName);
    //! 历史曲线参考X轴线指针
    CXAxis *referenceAxis() const;
    void setReferenceAxis(CXAxis *referenceAxis);

    //! 实时曲线参考X轴
    CXAxisManager *xs() const;
    void setXs(CXAxisManager *xs);

    //! 特征标签线
    CFeatureTagLine *tagLine() const;
    //! 顶端端点
    CPoint *topPoint() const;
    //! 底端端点
    CPoint *bottomPoint() const;

    //! 是否经过计算
    bool isCalculated() const;
    void setIsCalculated(bool isCalculated);

    //! 绘制区域
    QRectF drawArea() const;
    void setDrawArea(const QRectF &drawArea);

public:
    virtual void calculate() = 0;
    virtual void draw(QPainter *p) = 0;

    //! 序列化与反序列化
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
public:
    CTagManager &operator = (const CTagManager &other);

    //! 原始数据管理类
    CCurveDataManager &rawDataList();
    void setRawDataList(const CCurveDataManager &rawDataList);
private:
    bool           m_visible;
    QString        m_comment;
    qint32         m_interval;

    QFont          m_font;
    CDataSource    m_dataSource;
    CXAxisManager  *m_xs;
private:
    bool              m_isStudioMode;
private:
    QString            m_reAxisName;
    CXAxis            *m_referenceAxis ;
    CCurveDataManager  m_rawDataList;
    QRectF             m_drawArea;
private:
    CFeatureTagLine   *m_tagLine ;
    CPoint            *m_topPoint ;
    CPoint            *m_bottomPoint ;
    bool               m_isCalculated;

    //! 多语言
public:
    void changeLanguage();
    void collectWords();
};

#endif // CTAGMANAGER_H
