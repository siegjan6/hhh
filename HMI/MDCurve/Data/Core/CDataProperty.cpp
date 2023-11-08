#include "CDataProperty.h"
#include "Data.h"
#include "CCurveObjectManager.h"
#include "CCurveObject.h"
#include "CXAxis.h"

#include "CRealTimeTagManager.h"
#include "CHistoryTagManager.h"
#include "CTagManager.h"

#include "CFeatureTag.h"
#include "CFeatureTagItem.h"
#include "CDataSource.h"
#include "CYAxis.h"

#include "IMDVariableArchivesManager.h"
#include "IDrawEnvironment.h"
#include "MDDBQueryServiceFactory.h"
#include "IMDProjectExpert.h"

#include "CFind.h"

#include "IMDDatabaseParam.h"
//#include "MDDataSourceFactory.h"

#include "CReferenceCurveManager.h"
#include "CReferenceCurve.h"

#include "MDDBQueryEngineFactory.h"

#include <math.h>
//#include <sys/time.h>
#include <time.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#define PI 3.1415926

void queryTimeDataFromRecordSet(CCurveData &,CMDDatabaseRecordset*);
/*!
 \brief CDataProperty
      数据管理类
*/
CDataProperty::CDataProperty(QObject *parent):
    QObject(parent),
    m_isStudioModel(false),
    m_isRealTime(false),
    m_variantManager(NULL),
    m_environment(NULL),
    m_curves(NULL),
    m_historyTag(NULL),
    m_realTimeTag(NULL),
    m_filterCount(0),
    m_databaseParam(NULL),
    m_freshTime(1000),
    m_curveResultNotify(new CCurveDBResultNotify),
    m_tagResultNotify(new CCurveDBResultNotify)
{
    connect(m_curveResultNotify, SIGNAL(sendQuery(CMDDatabaseRecordset*)),
            this, SLOT(slotQueryCurveHistoryData(CMDDatabaseRecordset*)));

    connect(m_tagResultNotify, SIGNAL(sendQuery(CMDDatabaseRecordset*)),
            this, SLOT(slotQueryTagHData(CMDDatabaseRecordset*)));

}

CDataProperty::~CDataProperty()
{
    if(m_curveResultNotify){
        delete m_curveResultNotify;
        m_curveResultNotify = NULL;
    }

    if(m_tagResultNotify){
        delete m_tagResultNotify;
        m_tagResultNotify = NULL;
    }

    m_realCurveDataList.clear();
}

void CDataProperty::setCurves(CCurveObjectManager *curves)
{
    m_curves = curves;
}

void CDataProperty::setReferenceCurves(CReferenceCurveManager *reCurves)
{
    m_reCurves = reCurves;
}

void CDataProperty::setHistoryTags(CHistoryTagManager *tagManager)
{
    m_historyTag = tagManager;
}

void CDataProperty::setRealTags(CRealTimeTagManager *tagManager)
{
    m_realTimeTag = tagManager;
}

void CDataProperty::setData(IMDVariableArchivesManager *variantManager)
{
    m_variantManager = variantManager;
}

void CDataProperty::setData(IDrawEnvrionment *envrioment)
{
    m_environment = envrioment;
}

void CDataProperty::studioModeCurveData()
{
    double value = 0;
    if(m_curves)
    {
        QDateTime second = QDateTime::currentDateTime();
        double amplitude = 0;
        double setover = 0;

        m_curveRawDataManager.clear();
        for(int i = 0; i < m_curves->list().count();i++){
            CCurveObject *curve = m_curves->list().at(i);
            if(!curve->curveVisible())
                continue;

            if(CXAxis::ValueType::Time == curve->xAxis()->valueType())
            {
                second = curve->xAxis()->endTime();
                int xLength = curve->xAxis()->totalLength();
                amplitude = (double)(curve->yAxis()->maxValue()-curve->yAxis()->minValue())/2;
                setover = (double) (curve->yAxis()->maxValue()+curve->yAxis()->minValue())/2;
                second = second.addSecs(-1.1*xLength);


                QString variantName = curve->variantDepict();
                CCurveData data;
                data.setName(variantName);

                for(int i = 0; i < 400;++i)
                {
                    CTimeValuePoint point;
                    second = second.addSecs(xLength/50 >1?xLength/50:1);
                    point.setXTime(second);
                    point.setY(sin(++i *PI/36 -value)*amplitude +setover);
                    data.timeList().append(point);
                }
                m_curveRawDataManager.append(data);
                curve->setRawData(m_curveRawDataManager.last());
                curve->setPreviewData(m_curveRawDataManager.last());
            }

            if(CXAxis::ValueType::Value == curve->xAxis()->valueType()) {
                double maxValue = curve->xAxis()->maxValue();

                int xLength = curve->yAxis()->maxValue()-curve->yAxis()->minValue();
                amplitude = (double) (curve->yAxis()->maxValue()-curve->yAxis()->minValue())/2;
                setover = (double) (curve->yAxis()->maxValue()+curve->yAxis()->minValue())/2;
                maxValue  = maxValue-2*xLength;

                QString variantName = curve->variantDepict();
                CCurveData data;
                data.setName(variantName);

                for(int i = 0; i < 400;i++)
                {
                    CPureValuePoint point;
                    maxValue = maxValue + xLength/50;
                    point.setXDouble(maxValue);
                    point.setY(sin(i++ *PI/36 -value)*amplitude +setover);
                    data.valueList().append(point);
                }
                m_curveRawDataManager.append(data);
                curve->setRawData(m_curveRawDataManager.last());
            }

            value++;
        }
    }

    if(m_reCurves)
    {
        QDateTime second = QDateTime::currentDateTime();
        double amplitude = 0;
        double setover = 0;

        m_reCurveRawDataManager.clear();
        for(int i = 0; i < m_reCurves->list().count();i++)
        {
            CReferenceCurve *curve = m_reCurves->list().at(i);
            if(!curve->curveVisible())
                continue;
            if(CXAxis::ValueType::Time == curve->xAxis()->valueType())
            {
                second = curve->xAxis()->endTime();

                int xLength = curve->xAxis()->totalLength();
                amplitude = (double) (curve->yAxis()->maxValue()-curve->yAxis()->minValue())/2;
                setover = (double) (curve->yAxis()->maxValue()+curve->yAxis()->minValue())/2;
                second = second.addSecs(-1.1*xLength);

                QString variantName = curve->variantDepict();
                CCurveData data;
                data.setName(variantName);

                for(int i = 0; i < 400;i++)
                {
                    CTimeValuePoint point;
                    second = second.addSecs(xLength/50 >1?xLength/50:1);
                    point.setXTime(second);
                    point.setY(sin(i++ *PI/36 -value)*amplitude +setover);
                    data.timeList().append(point);
                }
                m_reCurveRawDataManager.append(data);
                curve->setRawData(m_reCurveRawDataManager.last());
            }

            if(CXAxis::ValueType::Value == curve->xAxis()->valueType())
            {
                double maxValue = curve->xAxis()->maxValue();
                int xLength = curve->xAxis()->maxValue() - curve->xAxis()->minValue();
                amplitude = (double) (curve->yAxis()->maxValue()-curve->yAxis()->minValue())/2;
                setover = (double) (curve->yAxis()->maxValue()+curve->yAxis()->minValue())/2;
                maxValue  = maxValue-2*xLength;

                QString variantName = curve->variantDepict();
                CCurveData data;
                data.setName(variantName);

                for(int i = 0; i < 400;i++){
                    CPureValuePoint point;
                    maxValue = maxValue + xLength/50;
                    point.setXDouble(maxValue);
                    point.setY(sin(i++ *PI/36 -value)*amplitude +setover);
                    data.valueList().append(point);
                }
                m_reCurveRawDataManager.append(data);
                curve->setRawData(m_reCurveRawDataManager.last());
            }

            value++;
        }
    }

}

void CDataProperty::studioModeTagData()
{
    if(m_historyTag)
    {
        QDateTime second = QDateTime::currentDateTime();

        m_HTagRawDataManager.clear();
        for(int i = 0; i < m_historyTag->tagList().at(0)->itemList().count();i++)
        {
            CFeatureTagItem *item = m_historyTag->tagList().at(0)->itemList().at(i);
            if(!item->visible())
                continue;

            int length = 2* m_historyTag->referenceAxis()->totalLength();
            second = second.addSecs(-length/2);

            QString variantName = item->itemName();
            CCurveData data;
            data.setName(variantName);

            for(int i = 0; i < length;i++)
            {
                CTimeValuePoint point;

                point.setXTime(second.addSecs(1));
                point.setY((1+sin(i++ *PI/36)) *50);
                data.timeList().append(point);
            }
            m_HTagRawDataManager.append(data);
        }
    }
}

void CDataProperty::getData()
{
    if(m_isStudioModel)
    {   //! 编辑版
        studioModeCurveData();
    }
    else
    {  //! 运行版
        if(m_isRealTime)
        {
            updateCurveData();
            updateTagData();
            referenceCurveData();
        }
        else
        {
            referenceCurveData();
        }
    }
}
void CDataProperty::updateCurveData()
{
    if(m_variantManager && m_curves)
    {
//        m_curveRawDataManager.clear();
//        m_previewRawDataManager.clear();
        int curvesCount = m_curves->list().count();
        for(int i = 0; i < curvesCount;i++)
        {
            CCurveObject *curve = m_curves->list().at(i);
            if(!curve->curveVisible())
                continue;

            if(curve->xAxis()->valueType() == CXAxis::ValueType::Time)
            {
                //!优化
                CCurveData totalData;
                if(freshTime() == 1)
                    totalData = m_realCurveDataList.at(i);
                QString variantName = curve->variantDepict();
                CMDDatabaseRecordset recordSet;
                QDateTime startTime,endTime;
                if(freshTime() != 1)
                {
                    endTime = curve->xAxis()->endTime();
                    startTime = endTime.addSecs(-curve->xAxis()->valueLength());
                }
                else
                {
                    endTime = QDateTime::currentDateTime();
                    startTime = endTime.addSecs(-1);
                }

                bool ok = m_variantManager->getData(variantName,startTime,endTime,&recordSet);
                if(ok)
                {
                    CCurveData data;
                    data.setValueType(XAxisValueType::Time);
                    data.setName(variantName);
                    queryTimeDataFromRecordSet(data,&recordSet);
                    totalData.timeList().append(data.timeList());
                    curve->setRawData(totalData);
                    if(freshTime() ==1)
                    {
                        m_realCurveDataList.removeAt(i);
                        m_realCurveDataList.insert(i,totalData);

                        if(curve->isBoundedPreview())
                        {
                            m_totalPreviewData.timeList().append(data.timeList());
                            curve->setPreviewData(m_totalPreviewData);
                        }
                    }
                }
                if(curve->isBoundedPreview() && freshTime() != 1)
                {
                    CMDDatabaseRecordset dbRecordSet;
                    bool isGetData = m_variantManager->getAllData(variantName,&dbRecordSet);
                    if(isGetData)
                    {
                        CCurveData preData;
                        preData.setValueType(XAxisValueType::Time);
                        queryTimeDataFromRecordSet(preData,&dbRecordSet);
                        curve->setPreviewData(preData);
                    }
                }
            }
            else if(curve->xAxis()->valueType() == CXAxis::ValueType::Value)
            {
//                CCurveData data;
//                data.setValueType(XAxisValueType::Double);
//                QString variantName = curve->variantDepict();
//                data.setName(variantName);
//                CMDDatabaseRecordset recordSet;
//                bool xOk = m_variantManager->getAllData(variantName,&recordSet);
//                QString yVariant = curve->yVariantDepict();
//                CMDDatabaseRecordset yRecordSet;
//                bool yOk = m_variantManager->getAllData(yVariant,&yRecordSet);
//                if(xOk &&yOk && recordSet.rowCount() == yRecordSet.rowCount())
//                {
//                    for(int i = 0; i < recordSet.rowCount();++i)
//                    {
//                        double x_value  = recordSet.value(i,1).toDouble();
//                        double y_value = yRecordSet.value(i,1).toDouble();
//                        CPureValuePoint point;
//                        point.setXDouble(x_value);
//                        point.setY(y_value);
//                        data.valueList().append(point);
//                    }
//                }
//                curve->setRawData(data);
            }
        }
    }
}

void CDataProperty::updateTagData()
{
    if(m_variantManager && m_realTimeTag)
    {
        m_RTagRawDataManager.clear();
        CMDDatabaseRecordset recordSet;
        QDateTime endtime = QDateTime::currentDateTime();

        for(int i = 0; i < m_realTimeTag->items().count();i++)
        {
            CRealFTag *realTag = m_realTimeTag->items().at(i);
            if(!realTag->visible())
                continue;

            CXAxis *x = realTag->xAxis();
            if(!x)
            {
                return;
            }
            for(int j = 0;j<realTag->itemList().count();j++)
            {
                if(x->valueType() == CXAxis::ValueType::Time)
                {
                    QDateTime startTime = endtime.addSecs(-x->totalLength());

                    CFeatureTagItem *item = realTag->itemList().at(j);

                    if(item->type() == CFeatureTagItem::ItemType::Time ||
                            item->type() == CFeatureTagItem::ItemType::Comment
                            || !item->visible())
                    {
                        continue;
                    }
                    QString variantName = item->itemName();

                    if(m_variantManager->getData(variantName,startTime,endtime,&recordSet))
                    {
                        CCurveData data;
                        data.setValueType(XAxisValueType::Time);
                        data.setName(variantName);
                        queryTimeDataFromRecordSet(data,&recordSet);
                        m_RTagRawDataManager.append(data);
                    }
                }
            }
        }
        m_realTimeTag->setRawDataList(m_RTagRawDataManager);
    }

//    if(m_variantManager && m_realTimeTag)
//    {
//        CMDDatabaseRecordset recordSet;
//        QDateTime endtime = QDateTime::currentDateTime();

//        for(int i = 0; i < m_realTimeTag->items().count();i++)
//        {
//            CCurveData tagTotalData;
//            CCurveDataManager curveDataManager = m_realTagDataMap.value(i);

//            CRealFTag *realTag = m_realTimeTag->items().at(i);
//            if(!realTag->visible())
//                continue;

//            CXAxis *x = realTag->xAxis();
//            if(!x)
//            {
//                return;
//            }
//            for(int j = 0;j<realTag->itemList().count();j++)
//            {
//                if(x->valueType() == CXAxis::ValueType::Time)
//                {
//                    QDateTime startTime = endtime.addSecs(-1);

//                    CFeatureTagItem *item = realTag->itemList().at(j);

//                    if(item->type() == CFeatureTagItem::ItemType::Time ||
//                            item->type() == CFeatureTagItem::ItemType::Comment
//                            || !item->visible())
//                    {
//                        continue;
//                    }
//                    tagTotalData = curveDataManager.at(j);
//                    QString variantName = item->itemName();
//                    tagTotalData.setName(variantName);
//                    if(m_variantManager->getData(variantName,startTime,endtime,&recordSet))
//                    {
//                        CCurveData data;
//                        data.setValueType(XAxisValueType::Time);
//                        queryTimeDataFromRecordSet(data,&recordSet);
//                        tagTotalData.timeList().append(data.timeList());
//                        curveDataManager.removeAt(j);
//                        curveDataManager.insert(j,tagTotalData);
//                    }
//                }
//            }
//            m_realTagDataMap.remove(i);
//            m_realTagDataMap.insert(i,curveDataManager);
//        }
//        m_RTagRawDataManager.clear();
//        for(int i =0 ; i<m_realTagDataMap.keys().count();i++)
//        {
//            CCurveDataManager curveDataManager = m_realTagDataMap.value(i);
//            for(int j = 0 ; j<curveDataManager.count();j++)
//            {
//                CCurveData data = curveDataManager.at(j);
//                if(data.name().isEmpty())
//                    continue;
//                else
//                  m_RTagRawDataManager.append(curveDataManager.at(j));
//            }
//        }
//        m_realTimeTag->setRawDataList(m_RTagRawDataManager);
//    }

}

void CDataProperty::referenceCurveData()
{
    if(m_reCurves)
    {
        m_reCurveRawDataManager.clear();
        for(int i = 0; i < m_reCurves->list().count();i++){
            CReferenceCurve *curve = m_reCurves->list().at(i);
            if(!curve->curveVisible())
                continue;

            if(CXAxis::ValueType::Time == curve->xAxis()->valueType())
            {
                qint64 time = curve->startTime().toLongLong();
                qDebug()<<"time:"<<time;
                QDateTime setTime = QDateTime::fromMSecsSinceEpoch(time);
                qDebug()<<"startTime:"<<setTime;


                QString variantName = curve->variantDepict();
                CCurveData data;
                data.setName(variantName);
                if(curve->name() == m_referenceName)
                {
                    if(m_xValueList.count() == m_yValueList.count())
                    {
                        for(int i = 0; i < m_xValueList.count();i++)
                        {
                            CTimeValuePoint point;
                            QDateTime time = setTime.addSecs(m_xValueList.at(i));
                            time.addMSecs(m_mTimeList.at(i));

                            point.setXTime(time);
                            point.setY(m_yValueList.at(i));
                            data.timeList().append(point);
                        }
//                        m_reCurveRawDataManager.append(data);
//                        curve->setRawData(m_reCurveRawDataManager.last());
                        curve->setRawData(data);
                    }
                }
            }
            else if(curve->xAxis()->valueType() == CXAxis::ValueType::Value)
            {
                QString variantName = curve->variantDepict();
                CCurveData data;
                data.setName(variantName);
                if(curve->name() == m_referenceName)
                {
                    if(m_xValueList.count() == m_yValueList.count())
                    {
                        for(int i = 0; i < m_xValueList.count();i++)
                        {
                            CPureValuePoint point;
                            double xValue = curve->startValue().toDouble() + m_xValueList.at(i);

                            point.setXDouble(xValue);
                            point.setY(m_yValueList.at(i));
                            data.valueList().append(point);
                        }
//                        m_reCurveRawDataManager.append(data);
//                        curve->setRawData(m_reCurveRawDataManager.last());
                        curve->setRawData(data);
                    }
                }
            }
        }
    }
}

void CDataProperty::updateHistoryData(bool isCurve)
{
    if(m_environment)
    {
        IMDProjectExpert *project = m_environment->projectExpert();
        if(project)
        {
            IMDDBQueryEngine*engine =  project->queryEngine();
            QString str;
            bool ok = false;
            if(isCurve)
            {
                for(int i = 0; i<getCurvesViewCount();i++)
                {
                     ok = engine->queryView(m_curveViewList.at(i),m_curveResultNotify,str);
                }
            }
            else
            {
                for(int i = 0; i< getTagsViewCount(); i++)
                {
                    ok = engine->queryView(m_tagViewList.at(i),m_tagResultNotify,str);
                }
            }
        }
    }
}

void CDataProperty::queryHistoryCurveData(CMDDatabaseRecordset * recordSet)
{
    if(!m_curves)
        return;
    m_curveRawDataManager.clear();
    m_previewRawDataManager.clear();
    for(int i = 0; i < m_curves->list().count();i++)
    {
        CCurveObject *curve = m_curves->list().at(i);
        if(!curve->curveVisible())
            continue;
        QString viewName = curve->viewName();
        if(viewName != recordSet->name())
            continue;
        CCurveData data;
        if(CXAxis::ValueType::Time == curve->xAxis()->valueType())
        {
            QString variantName = curve->variantDepict();
            QString timeVariant = curve->timeVariantDepict();
            if(variantName.isEmpty() || timeVariant.isEmpty())
                continue;
            data.setValueType(XAxisValueType::Time);
            data.setName(variantName);
            int variantColumn = recordSet->getFiledIndex(variantName);
            int dateTimeColumn = recordSet->getFiledIndex(timeVariant);
            if(variantColumn == -1 || dateTimeColumn == -1)
                continue;

            for(int j = 0; j < recordSet->rowCount();j++)
            {
                CTimeValuePoint point;
                QDateTime time = recordSet->value(j,dateTimeColumn).toDateTime();
                double value   = recordSet->value(j,variantColumn).toDouble();
                point.setXTime(time);
                point.setY(value);
                data.timeList().append(point);
//                qDebug()<<time<<value;
            }
            // if there is no time,should not set the end time of x axis;
            if(!data.timeList().isEmpty())
            {
                if(CXAxis *x = curve->xAxis())
                {
                    x->setFinalEnd(data.timeList().last().xTime(),true);
                }
            }
        }

        m_curveRawDataManager.append(data);
        m_previewRawDataManager.append(data);
        curve->setRawData(m_curveRawDataManager.last());
        curve->setPreviewData(m_previewRawDataManager.last());

    }
    emit resultReady();
}

void CDataProperty::queryHistoryTagData(CMDDatabaseRecordset * recordSet)
{
    if(!m_historyTag || m_historyTag->tagList().count() == 0)
        return;
    m_HTagRawDataManager.clear();

    for(int j = 0; j< m_historyTag->tagList().count(); j++)
    {
        CFeatureTag *tag = m_historyTag->tagList().at(j);
        if(!tag->visible())
            continue;
        if(tag->viewName() != recordSet->name())
            continue;
        if(tag->timeVariant().isEmpty())
            continue;
        for(int i = 0; i < tag->itemList().count();i++)
        {
            CFeatureTagItem *item = tag->itemList().at(i);

            if(item->type() == CFeatureTagItem::ItemType::Time ||
                    item->type() == CFeatureTagItem::ItemType::Comment
                    || !item->visible())
            {
                continue; //if the curve is not visible,continue
            }

            QString variantName = item->itemName();
            CCurveData data;
            data.setValueType(XAxisValueType::Time);
            data.setName(variantName);
            QString valueVariant = variantName.remove(tag->viewName()+".");
            int variantColumn = recordSet->getFiledIndex(valueVariant);
            int dataTimeColumn = recordSet->getFiledIndex(tag->timeVariant());

            for(int k = 0; k < recordSet->rowCount();k++)
            {
                CTimeValuePoint point;
                QDateTime time = recordSet->value(k,dataTimeColumn).toDateTime();
                double value = recordSet->value(k,variantColumn).toDouble();
//                qDebug()<<time<<value;
                point.setXTime(time);
                point.setY(value);
                data.timeList().append(point);
            }
            // if there is no time,should not set the end time of x axis;
            if(!data.timeList().isEmpty())
            {
                if(CXAxis *x = m_historyTag->tagList().at(j)->xAxis())
                {
                    x->setFinalEnd(data.timeList().last().xTime(),true);
                }
            }
            m_HTagRawDataManager.append(data);
        }
    }
    m_historyTag->setRawDataList(m_HTagRawDataManager);

    emit resultReady();
}

void CDataProperty::slotQueryCurveHistoryData(CMDDatabaseRecordset *recordSet)
{
    if(recordSet && m_curves)
    {
        queryHistoryCurveData(recordSet);
    }
}

void CDataProperty::slotQueryTagHData(CMDDatabaseRecordset * recordSet)
{
    if(recordSet && m_historyTag)
    {
        queryHistoryTagData(recordSet);
    }
}

void CDataProperty::setReferenceCurveData(const QString &curveName, const QVariant &xValue, const QVariant &yValue)
{
    m_mTimeList.clear();
    m_xValueList.clear();
    m_yValueList.clear();
    m_referenceName = curveName;
    for(int i = 0; i < m_reCurves->list().count();i++)
    {
        if(m_reCurves->list().at(i)->name() == m_referenceName)
        {
            if(m_reCurves->list().at(i)->xAxis()->valueType() == CXAxis::ValueType::Time)
            {
                QVariantList xList = xValue.toList();
                for(int i = 0; i < xList.length(); i++)
                {

                    QString xValueString = xList.at(i).toString();
                    QStringList strList = xValueString.split("-", QString::SkipEmptyParts);
                    if(strList.count() < 5)
                        return;

                    int day = strList.at(0).toInt();
                    int hour = strList.at(1).toInt();
                    int minute = strList.at(2).toInt();
                    int second = strList.at(3).toInt();
                    m_mTimeInterval = strList.at(4).toInt();

                    m_timeInterval = day *24*60*60+hour*60*60+minute*60+second;

                    m_xValueList.append(m_timeInterval);
                    m_mTimeList.append(m_mTimeInterval);
                }

                QVariantList yList = yValue.toList();
                for(int i = 0; i < yList.length(); i++)
                {
                    m_yValueList.append(yList.at(i).toDouble());
                }
            }
            else if(m_reCurves->list().at(i)->xAxis()->valueType() == CXAxis::ValueType::Value)
            {
                QVariantList xList = xValue.toList();
                for(int i = 0; i < xList.length(); i++)
                {
                    m_xValueList.append(xList.at(i).toDouble());
                }

                QVariantList yList = yValue.toList();
                for(int i = 0; i < yList.length(); i++)
                {
                    m_yValueList.append(yList.at(i).toDouble());
                }
            }
        }
    }

    emit resultReady();

}

void CDataProperty::setIsRealTime(bool isRealTime)
{
    m_isRealTime = isRealTime;
}

void CDataProperty::setFreshTime(const int &freshTime)
{
    m_freshTime = freshTime;
}

int CDataProperty::freshTime() const
{
    return m_freshTime;
}

void CDataProperty::setDBSetting(IMDDataBaseParam *dbParam)
{
    m_databaseParam = dbParam;
}

void CDataProperty::setCurveDataSearchFilter(const QString &filter)
{
    m_curveHistoryFilter = filter;
}

QString CDataProperty::curveDataSearchFilter() const
{
    return m_curveHistoryFilter; 
}

void CDataProperty::setTagDataSearchFilter(const QString &filter)
{
    m_tagHistoryFilter = filter;
}

QString CDataProperty::tagDataSearchFilter()const
{
    return m_tagHistoryFilter;
}

void CDataProperty::query()
{
    updateHistoryData(true);
    updateHistoryData(false);
}

void CDataProperty::serialize(QJsonObject &json)
{
    json.insert("CurveHistoryFilter",m_curveHistoryFilter);
    json.insert("TagHistoryFilter",m_tagHistoryFilter);
    json.insert("CurveCount",m_curves->list().count());
    json.insert("FreshTime",m_freshTime);
    QJsonArray tagArray;
    for(int i = 0; i<m_realTimeTag->items().count();i++)
    {
        QJsonObject json;
        json.insert("itemCount",m_realTimeTag->items().at(i)->itemList().count());
        tagArray.append(json);
    }
    json.insert("TagArray",tagArray);
}

void CDataProperty::deserialize(const QJsonObject &json)
{
    m_curveHistoryFilter = json.value("CurveHistoryFilter").toString();
    m_tagHistoryFilter = json.value("TagHistoryFilter").toString();
    int curveCount = json.value("CurveCount").toInt();
    m_freshTime = json.value("FreshTime").toInt();
    for(int i = 0; i<curveCount;++i)
    {
        CCurveData curveData;
        m_realCurveDataList.append(curveData);
    }

    QJsonArray tagArray = json.value("TagArray").toArray();
    int count = tagArray.count();
    for(int i = 0; i<count;i++)
    {
        CCurveDataManager dataManager;
        QJsonObject json = tagArray.at(i).toObject();
        int itemCount = json.value("itemCount").toInt();
        for(int j = 0; j<itemCount;j++)
        {
            CCurveData data;
            dataManager.insert(j,data);
        }
        m_realTagDataMap.insert(i,dataManager);
    }
}

int CDataProperty::getCurvesViewCount()
{
    QList<QString>viewNameList;
    if(m_curves)
    {
        for(int i = 0; i< m_curves->list().count();i++)
        {
            QString name = m_curves->list().at(i)->viewName();
            if(!viewNameList.contains(name))
                viewNameList.append(name);
        }
    }
    m_curveViewList = viewNameList;
    return viewNameList.count();
}

int CDataProperty::getTagsViewCount()
{
    QList<QString>viewNameList;
    if(m_historyTag)
    {
        for(int i = 0; i< m_historyTag->tagList().count();i++)
        {
            QString name = m_historyTag->tagList().at(i)->viewName();
            if(!viewNameList.contains(name))
                viewNameList.append(name);
        }
    }
    m_tagViewList = viewNameList;
    return viewNameList.count();
}

CDataProperty *CDataProperty::clone()
{
    CDataProperty *rh = new CDataProperty;
    rh->m_curveHistoryFilter = this->m_curveHistoryFilter;
    rh->m_tagHistoryFilter = this->m_tagHistoryFilter;
    rh->m_filterCount = this->m_filterCount;
    rh->m_freshTime = this->m_freshTime;
    return rh;
}

void CDataProperty::setIsStudioModel(bool isStudioModel)
{
    m_isStudioModel = isStudioModel;
}

void queryTimeDataFromRecordSet(CCurveData &data,CMDDatabaseRecordset *recordSet)
{
    CTimeValuePoint point;
    for(int i = 0; i < recordSet->rowCount();++i)
    {
        QDateTime time = recordSet->value(i,0).toDateTime();
        double value  = recordSet->value(i,1).toDouble();
//        qDebug()<<time<<value;
        point.setXTime(time);
        point.setY(value);
        data.timeList().append(point);
    }
}

