/*!

\author eHVr

\section 功能

    查找算法
*/

#ifndef CFIND_H
#define CFIND_H
#include <QRectF>
#include <QList>
#include <QDateTime>

#include "Data.h"
#include <QObject>

class CXAxis;
class CCurveObject;

#define ERRORFINDING    -3


class CFind
{
public:
    CFind(){}
    ~CFind(){}
/*!
\section 搜索算法,找到小于等于Key的最大值的索引
*/

    static int binarySearch(const QList<QPoint> &list, double key);
    static int binarySearch(const QList<CTimeValuePoint> &list, QDateTime key);
    static int binarySearch(const QList<CPureValuePoint> &list, double key);
    static void processData(QList<QPoint> &points);
    static void debug(const QString & content);

    static void proData( QList<CTimeValuePoint>* points, qint64 range, int begin = 0,int end = 0);

    //! 数据过滤算法。对目标链表中，每个range 范围内的数据进行筛选，取出最大值最小值。
    //! dList 中为最小值，最大值和最后一个数据点
    static void proData(const QList<CTimeValuePoint>* points, qint64 range,
                         int begin, int end, QList<CTimeValuePoint>* dList);

    //! 精确查找
    static int binarySearchExact(const QList<CTimeValuePoint> &list, QDateTime key);
};


#endif // CFIND_H
