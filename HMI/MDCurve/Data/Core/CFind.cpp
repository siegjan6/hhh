#include "CFind.h"
#include "CXAxis.h"
#include "Data.h"
#include "CCurveObject.h"
#include <QDebug>
#include <QFile>
#include <QThread>

void debug(const QString & content);
//二分查找法找寻下界
int bSearchLowerBound(const QList<CBasePoint> &list, double key)
{
    int start = 0;
    int end = list.count() - 1;

    if(end < start  || key < list.at(start).xDouble() ) return ERRORFINDING;

    int mid = (start + end + 1) / 2;
    while (start < end)
    {
        if (list.at(mid).xDouble() <= key)
            start = mid;
        else
            end = mid - 1;

        mid = (start + end + 1) / 2;
    }

    return mid;
}
int bSearchLowerBound(const QList<CPureValuePoint> &list, double key)
{
    int start = 0;
    int end = list.count() - 1;

    if(end < start  || key < list.at(start).xDouble() ) return ERRORFINDING;

    int mid = (start + end + 1) / 2;
    while (start < end)
    {
        if (list.at(mid).xDouble() <= key)
            start = mid;
        else
            end = mid - 1;

        mid = (start + end + 1) / 2;
    }

    return mid;
}
int bSearchLowerBound(const QList<CBasePoint> &list,const QDateTime &key)
{
    int start = 0;
    int end = list.count() - 1;

    if(end < start  || key < list.at(start).xTime() ) return ERRORFINDING;

    int mid = (start + end + 1) / 2;
    while (start < end)
    {
        if (list.at(mid).xTime() <= key)
            start = mid;
        else
            end = mid - 1;

        mid = (start + end + 1) / 2;
    }

    return mid;
}

int bSearchLowerBound(const QList<CTimeValuePoint> &list,const QDateTime &key)
{
    int start = 0;
    int end = list.count() - 1;

    if(end < start  || key < list.at(start).xTime() ) return ERRORFINDING;

    int mid = (start + end + 1) / 2;
    while (start < end)
    {
        if (list.at(mid).xTime() <= key)
            start = mid;
        else
            end = mid - 1;

        mid = (start + end + 1) / 2;
    }

    return mid;
}

int bSearchLowerBound(const QList<QPoint> &list,double key)
{
    int start = 0;
    int end = list.count() - 1;

    if(end < start  || key <= list.at(start).x() ) return ERRORFINDING;

    int mid = (start + end + 1) / 2;
    while (start < end)
    {
        if (list.at(mid).x() <= key)
            start = mid;
        else
            end = mid - 1;

        mid = (start + end + 1) / 2;
    }

    return mid;
}

//二分查找法找寻上界
int bSearchUpperBound(const QList<CBasePoint> &list, double key)
{
    int start  = 0;
    int end = list.count() -1;

    if(end < start || key > list.at(end).xDouble()) return ERRORFINDING;

    int mid = (start + end) / 2;
    while (end > start)
    {
        if (list.at(mid).xDouble() >= key)
            end = mid;
        else
            start = mid + 1;

        mid = (start + end) / 2;
    }

    return mid;
}
int bSearchUpperBound(QList<QPoint> list, double key)
{
    int start = 0;
    int end = list.count() - 1;

    if(start > end || key >= list.at(end).x()) return ERRORFINDING;

    int mid = (start + end) / 2;
    while (end > start)
    {
        if (list.at(mid).x() >= key)
            end = mid;
        else
            start = mid + 1;

        mid = (start + end) / 2;
    }

    return mid;
}

int bSearchUpperBound(const QList<CBasePoint> &list, QDateTime key)
{
    int start = 0;
    int end = list.count() - 1;

    if(start > end || key > list.at(end).xTime()) return ERRORFINDING;

    int mid = (start + end) / 2;
    while (end > start)
    {
        if (list.at(mid).xTime() >= key)
            end = mid;
        else
            start = mid + 1;

        mid = (start + end) / 2;
    }
    return mid;
}


int bSearchUpperBound(const QList<CTimeValuePoint> &list, QDateTime key)
{
    int start = 0;
    int end = list.count() - 1;

    if(start > end || key > list.at(end).xTime()) return ERRORFINDING;

    int mid = (start + end) / 2;
    while (end > start)
    {
        if (list.at(mid).xTime() >= key)
            end = mid;
        else
            start = mid + 1;

        mid = (start + end) / 2;
    }
    return mid;
}

int bSearchUpperBound(const QList<CPureValuePoint> &list, double key)
{
    int start  = 0;
    int end = list.count() -1;

    if(start > end || key > list.at(end).xDouble()) return -1;

    int mid = (start + end) / 2;
    while (end > start)
    {
        if (list.at(mid).xDouble() >= key)
            end = mid;
        else
            start = mid + 1;

        mid = (start + end) / 2;
    }

    return mid;
}
int bSeach(const QList<CPureValuePoint> &list,double key)
{
    int lowerIndex =  bSearchLowerBound(list,key);
    int upperIndex = bSearchUpperBound(list,key);

    if(lowerIndex == ERRORFINDING)
    {
        return 0;
    }
    else if(upperIndex == ERRORFINDING)
    {
        return list.count()-1; // index
    }
    else if(upperIndex == -1)
        return lowerIndex;
    return (key - list.at(lowerIndex).xDouble() > list.at(upperIndex).xDouble()- key  ? upperIndex : lowerIndex);
}

int bSeach(const QList<CBasePoint> &list,QDateTime key)
{
    int lowerIndex =  bSearchLowerBound(list,key);
    int upperIndex = bSearchUpperBound(list,key);

    if(lowerIndex == ERRORFINDING){
        return 0;
    }
    else if(upperIndex == ERRORFINDING)
    {
        return list.count()-1; // index
    }
    return (list.at(lowerIndex).xTime().secsTo(key) > key.secsTo(list.at(upperIndex).xTime()) ? upperIndex : lowerIndex);
}

int bSeach(const QList<CTimeValuePoint> &list,QDateTime key)
{
    int lowerIndex =  bSearchLowerBound(list,key);
    int upperIndex = bSearchUpperBound(list,key);

    if(lowerIndex == ERRORFINDING){
        return 0;
    } else if(upperIndex == ERRORFINDING) {
        return list.count()-1; // index
    }
    return (list.at(lowerIndex).xTime().secsTo(key) > key.secsTo(list.at(upperIndex).xTime()) ? upperIndex : lowerIndex);
}

int bSeach(const QList<QPoint> &list,double key)
{
    int lowerIndex =  bSearchLowerBound(list,key);
    int upperIndex = bSearchUpperBound(list,key);

    if(lowerIndex == ERRORFINDING)
    {
        return 0;
    }
    else if(upperIndex == ERRORFINDING)
    {
        return list.count()-1; // index
    }
    return (key - list.at(lowerIndex).x() > list.at(upperIndex).x() - key  ? upperIndex : lowerIndex);
}


int CFind::binarySearch(const QList<QPoint> &list, double key)
{
    if(list.isEmpty())
        return -1;
    return ::bSeach(list,key);
}

int CFind::binarySearch(const QList<CTimeValuePoint> &list, QDateTime key)
{
    if(list.isEmpty())
        return -1;
    return ::bSeach(list,key);
}

int CFind::binarySearch(const QList<CPureValuePoint> &list, double key)
{
    if(list.isEmpty())
        return -1;
    return ::bSeach(list,key);
}

void CFind::processData(QList<QPoint> &points)
{
    if(points.isEmpty())
        return;

    QList<QPoint> tempList;
    tempList.clear();

    QPoint bigPoint(points.first());
    QPoint littlePoint(points.first());
    QPoint prePoint (points.first());
    QPoint curPoint(points.first());

    QListIterator<QPoint> i(points);

    while(i.hasNext())
    {
        prePoint = curPoint;
        curPoint = i.next();

        if (curPoint.x() == prePoint.x()){ // x点相同
            if (curPoint.y() < littlePoint.y())
                littlePoint = curPoint;
            else if (curPoint.y() > bigPoint.y())
                bigPoint = curPoint;
            else {};
        } else {
            prePoint = curPoint;
            littlePoint = curPoint;
            bigPoint = curPoint;
        }

        if(i.hasNext()){
            QPoint nextPoint = i.peekNext();
            if(nextPoint.x() != curPoint.x()){
                if(littlePoint != bigPoint){
                    tempList.append(littlePoint);
                    tempList.append(bigPoint);
                }
                tempList.append(curPoint);
            }
        } else {
            if(littlePoint != bigPoint){
                tempList.append(littlePoint);
                tempList.append(bigPoint);
            }
            tempList.append(curPoint);
        }
    }
    points = tempList;
}

void CFind::debug(const QString &content)
{
    ::debug(content);
}
void CFind::proData( const QList<CTimeValuePoint>* points, qint64 range,
                         int begin,int end, QList<CTimeValuePoint>* dList)
{
    if( (*points).isEmpty()|| begin < 0 || end < 0 || begin > end || !dList)
        return;
    if(begin == end)
    {
        begin = 0;
        end = points->count()-1;
    }
    dList->clear();

    CTimeValuePoint bigPoint((*points).at(begin));
    CTimeValuePoint littlePoint(bigPoint);
    CTimeValuePoint prePoint (bigPoint);
    CTimeValuePoint curPoint(bigPoint);
    QDateTime endTime = curPoint.xTime().addSecs(range);


    for(int i = begin;i < end;){
        prePoint = curPoint;
        curPoint = points->at(i);

        if(curPoint.xTime() < endTime)
        {
            if (curPoint.y() < littlePoint.y())
                littlePoint = curPoint;
            else if (curPoint.y() > bigPoint.y())
                bigPoint = curPoint;
            else {};
        } else {
            if(littlePoint != bigPoint){
                dList->append(littlePoint);
                dList->append(bigPoint);
            }
            dList->append(prePoint);

            prePoint = curPoint;
            littlePoint = curPoint;
            bigPoint = curPoint;

            endTime = endTime.addSecs(range);

            if(endTime.secsTo(curPoint.xTime()) > range){
                endTime = curPoint.xTime().addSecs(range);
            }
        }
        i++;
        if(i == end){
            if(littlePoint != bigPoint){
                dList->append(littlePoint);
                dList->append(bigPoint);
            }
            dList->append(prePoint);
        }
    }
}

int CFind::binarySearchExact(const QList<CTimeValuePoint> &list, QDateTime key)
{
    if(list.isEmpty())
        return -1;

    int start=0;
    int end=list.count()-1;
    int mid=(start+end)/2;
    while(start<end&&list.at(mid).xTime()!=key)
    {
        if(list.at(mid).xTime()<key) start=mid+1;
        if(list.at(mid).xTime()> key)end=mid-1;
        mid=(start+end)/2;
    }
    if(list.at(mid).xTime()!= key)
        return -1;
    else
        return mid;
}

void CFind::proData( QList<CTimeValuePoint>* points, qint64 range, int begin ,int end)
{

    if( (*points).isEmpty()|| begin < 0 || end < 0 || begin > end)
        return;
    if(begin == end && !begin){
        begin = 0;
        end = points->count()-1;
    }

    QList<CTimeValuePoint> dList;
    dList.clear();

    CTimeValuePoint bigPoint((*points).at(begin));
    CTimeValuePoint littlePoint(bigPoint);
    CTimeValuePoint prePoint (bigPoint);
    CTimeValuePoint curPoint(bigPoint);
    QDateTime endTime = curPoint.xTime().addSecs(range);


    for(int i = begin;i < end;){
        prePoint = curPoint;
        curPoint = points->at(i);

        if(curPoint.xTime() < endTime)
        {
            if (curPoint.y() < littlePoint.y())
                littlePoint = curPoint;
            else if (curPoint.y() > bigPoint.y())
                bigPoint = curPoint;
            else {};
        } else {
            if(littlePoint != bigPoint){
                dList.append(littlePoint);
                dList.append(bigPoint);
            }
            dList.append(prePoint);

            prePoint = curPoint;
            littlePoint = curPoint;
            bigPoint = curPoint;

            endTime = endTime.addSecs(range);

            if(endTime.secsTo(curPoint.xTime()) > range){
                endTime = curPoint.xTime().addSecs(range);
            }
        }
        i++;
        if(i == end){
            if(littlePoint != bigPoint){
                dList.append(littlePoint);
                dList.append(bigPoint);
            }
            dList.append(curPoint);
        }
    }

    (*points) = dList;
}
#if 0
void CFind::processData( QList<CTimeValuePoint>* points, qint64 range)
{
    if( (*points).isEmpty())
        return;

    QList<CTimeValuePoint> dList;
    dList.clear();

    CTimeValuePoint bigPoint((*points).first());
    CTimeValuePoint littlePoint(bigPoint);
    CTimeValuePoint prePoint (bigPoint);
    CTimeValuePoint curPoint(bigPoint);
    QDateTime endTime = curPoint.xTime().addSecs(range);

    QListIterator<CTimeValuePoint> iter(*points);
    while(iter.hasNext())
    {
        prePoint = curPoint;
        curPoint = iter.next();

        if(curPoint.xTime() < endTime)
        {
            if (curPoint.y() < littlePoint.y())
                littlePoint = curPoint;
            else if (curPoint.y() > bigPoint.y())
                bigPoint = curPoint;
            else {};
        } else {
            if(littlePoint != bigPoint){
                dList.append(littlePoint);
                dList.append(bigPoint);
            }
            dList.append(prePoint);

            prePoint = curPoint;
            littlePoint = curPoint;
            bigPoint = curPoint;

            endTime = endTime.addSecs(range);

            if(endTime.secsTo(curPoint.xTime()) > range){
                endTime = curPoint.xTime().addSecs(range);
            }
        }

        if(!iter.hasNext()){
            if(littlePoint != bigPoint){
                dList.append(littlePoint);
                dList.append(bigPoint);
            }
            dList.append(curPoint);
        }
    }
    (*points) = dList;
}
#endif
void debug(const QString & content)
{
    QFile file("out.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        return;

    //    QTextStream out(&file);
    //    out  << "Time: "<< QDateTime::currentDateTime().toString("hh:mm:ss")
    //         << " "<< content << "\n";

    QTextStream out(&file);
    out  << " "<< content << "\n";
    file.close();
}

