/*!

\author eHVr

\section 功能
    曲线导出类
*/
#ifndef MDCURVE_H
#define MDCURVE_H

#include <QtCore/qglobal.h>
#include <QJsonObject>

#if defined(MDCURVE_LIBRARY)
#  define MDCURVESHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDCURVESHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QWidget>
#include <QObject>

class IDrawEnvrionment;
class CCurveWidget;
class CMDBrushManager;
class MDCURVESHARED_EXPORT CMDCurve
{
public:
    CMDCurve();
    ~CMDCurve();

public:
    QWidget *widget();
    void setDrawEnvrionment(IDrawEnvrionment* value);  //! 设置控件框架指针
    void setIsRealTime(bool);
    int setProperty();

/*!
\section 脚本接口
*/
public:
    void setHistoryDataSearchFilter(const QString &filter);
    void setHistoryTagSearchFilter(const QString &filter);
    void query();

    void setDataSourceParameter(int type, const QString &parameter); //! 设置数据库参数


public:
    CMDCurve &operator =(const CMDCurve &other);
/*!
\section 序列化和反序列化
*/
public:
    virtual void serialize(QJsonObject& json);
    virtual void deserialize(const QJsonObject& json);

private:
    CCurveWidget *m_widget;
public:
    void changeLanguage();
    void collectWords();
    /*!
    \section 加载图片以及图片拷贝
    */
public:
    void getBrushes(QList<CMDBrushManager*>& list);

    /*!
    \section 基准曲线设置起始值
    */
public:
    //! 设置起始数值
    void setValue(int index, QVariant value);
    //! 获取表达式和与之对应的索引
    void getExpressionList(QList<int> &indexs, QList<QString> &expressions);
    //! 设置起始时间
    void setStartTime(const QString &curveName,const QVariant &startTime);
    /*!
    \section 基准曲线设置数据点
    */
    void setReferenceCurveData(const QString &curveName, const QVariant &xValue, const QVariant &yValue);

};

#endif // MDCURVE_H
