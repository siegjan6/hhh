/*!

\author eHVr

\section 功能

    数据管理类，获取，转换,管理原始数据。
*/
#ifndef CDATAPROPERTY_H
#define CDATAPROPERTY_H

#include "Data.h"
#include "IMDDBQueryService.h"
#include "MDDatabaseRecordset.h"
#include "IMDDBQueryEngine.h"

class CTagManager;
class IDrawEnvrionment;
class CHistoryTagManager;
class CCurveObjectManager;
class CReferenceCurveManager;
class IMDVariableArchivesManager;
class CRealTimeTagManager;
class IMDDBQueryEngine;

/*!
 \section CMDDBResultNotify
   回调。
*/
class CCurveDBResultNotify :public QObject,public IMDDBRecordSetNotify
{
    Q_OBJECT
public:
    CCurveDBResultNotify(QObject *parent = 0):QObject(parent){}
    ~CCurveDBResultNotify(){}
public:
    virtual void NotityDBQueryResult(const QString &,CMDDatabaseRecordset *e){emit sendQuery(e);}
signals:
    void sendQuery(CMDDatabaseRecordset *);
};
/*!
 \section CDataProperty
    数据管理类
*/
class CDataProperty :public QObject
{
    Q_OBJECT
public:
    CDataProperty(QObject *parent = 0);
    virtual ~CDataProperty();
private:
    CCurveDataManager   m_curveRawDataManager;   //! 原始曲线数据
    CCurveDataManager   m_previewRawDataManager;   //! 原始曲线预览数据
    CCurveDataManager   m_HTagRawDataManager;    //! history tag data Mangager
    CCurveDataManager   m_RTagRawDataManager;    //! real time tag data manager

    CCurveDataManager   m_reCurveRawDataManager; //! 原始基准曲线数据


    CCurveDataManager   m_realCurveDataList;     //! 保存实时曲线数据
    CCurveData          m_totalPreviewData;      //! 保存预览曲线数据
    QMap<int,CCurveDataManager> m_realTagDataMap;//! 保存实时特征标签数据


/*!
\section  获取曲线等模块指针
*/
public:
    void setCurves(CCurveObjectManager *curves);
    void setReferenceCurves(CReferenceCurveManager *reCurves);
    void setHistoryTags(CHistoryTagManager *tagManager); //! history
    void setRealTags(CRealTimeTagManager *tagManager);

    void setData(IMDVariableArchivesManager *variantManager);
    void setData(IDrawEnvrionment *envrioment);
public:
    void setIsStudioModel(bool isStudioModel);
    // must be called before the func setTagManager
    void setIsRealTime(bool isRealTime);
    void setFreshTime(const int &freshTime);
private:
    int freshTime() const;
private:
    bool   m_isStudioModel; //! 是否为Studio
    bool   m_isRealTime;    //! 是否为实时
    int    m_freshTime;


    IMDVariableArchivesManager  *m_variantManager;
    IDrawEnvrionment            *m_environment;
    CCurveObjectManager         *m_curves;
    CReferenceCurveManager      *m_reCurves;
    CHistoryTagManager          *m_historyTag;
    CRealTimeTagManager         *m_realTimeTag;
/*!
\section 脚本接口
*/
public:
    void setDBSetting(IMDDataBaseParam *dbParam);         //! 数据库配置
    void setCurveDataSearchFilter(const QString &filter); //! 曲线数据过滤条件
    QString curveDataSearchFilter() const;

    void setTagDataSearchFilter(const QString &filter);   //! 特征标签过滤条件
    QString tagDataSearchFilter() const;

    void query(); //! 查询

/*!
\section 序列化与反序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

private:
    QString                   m_curveHistoryFilter;
    QString                   m_tagHistoryFilter;

    qint32                    m_filterCount;
    IMDDataBaseParam         *m_databaseParam;

private:
    int getCurvesViewCount();
    int getTagsViewCount();

private:
    QList<QString> m_curveViewList;
    QList<QString> m_tagViewList;

/*!
\section 克隆函数
*/
public:
    CDataProperty *clone();
/*!
\section 模拟数据
*/
private:
    void studioModeCurveData();
    void studioModeTagData();
/*!
\section 更新数据
*/
public:
    virtual void getData();
signals:
    void resultReady();
private:
    //! 实时数据
    void updateCurveData();
    void updateTagData();
    void referenceCurveData();

    //! 历史数据
    void updateHistoryData(bool isCurve);
    void queryHistoryCurveData(CMDDatabaseRecordset *);
    void queryHistoryTagData(CMDDatabaseRecordset *);
private slots:
    void slotQueryCurveHistoryData(CMDDatabaseRecordset *);
    void slotQueryTagHData(CMDDatabaseRecordset *);
private:
    CCurveDBResultNotify        *m_curveResultNotify;
    CCurveDBResultNotify        *m_tagResultNotify;

private:
    int           m_timeInterval;    //! 时间间隔
    int           m_mTimeInterval;   //! 毫秒间隔
    QString       m_referenceName;   //! 基准曲线名称
    QList<int>    m_mTimeList;
    QList<double> m_xValueList;      //! X坐标数据
    QList<double> m_yValueList;      //! Y坐标数据
public:
    void setReferenceCurveData(const QString &curveName, const QVariant &xValue, const QVariant &yValue);

};


#endif // CDATAPROPERTY_H
