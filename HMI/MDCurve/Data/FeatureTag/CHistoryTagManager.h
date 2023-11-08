/*!

\author eHVr

\section 功能

特征标签组：使用一个特征标签（CFeatureTag）来保存配置，设置一个参考变量，
之后当参考变量触发时，在屏幕上显示此时的所有变量在这一时间点的最近值。

*/
#ifndef CHISTORYTAGMANAGER_H
#define CHISTORYTAGMANAGER_H

#include <QList>
#include <QVector>
#include <QMap>

#include "CPoint.h"
#include "Data.h"
#include "CTagPosition.h"
#include "CFeatureTag.h"
#include "CDataSource.h"
#include "CTagManager.h"

class CXAxis;
class CPoint;
class CFeatureTagLine;
class CFeatureTagItem;
class CXAxisManager;
class CHistoryTagManager:public CTagManager
{
public:
    CHistoryTagManager();
    ~CHistoryTagManager();

/*!
\section
*/
public:
    //! 标签变量链表
    QList<CFeatureTag*> &tagList();
    void setTagList(const QList<CFeatureTag*> &tagList);
    QMap<QString, CTagPos> itemPosMap() const;
    void setItemPosMap(const QMap<QString,CTagPos> & map);

private:
    QList<CFeatureTag *>     m_tagList;       //! 特征标签个数
    QList<CFeatureTag *>     m_itemsTagList;  //! 所有的特征标签
    QMap<QString,CTagPos>    m_itemPosMap;    //! 特征标签的位置
/*!
\section 位置
*/
public:
    //! 标签垂直方向位置
    CTagVPos vPos() const;
    void setVPos(const CTagVPos &vPos);

    //! 水平方向位置
    CTagHPos hPos() const;
    void setHPos(const CTagHPos &hPos);

    //! 项目路径
    void setProjectPath(const QString &path);
    QString projectPath() const;
private:
    CTagVPos  m_vPos;
    CTagHPos  m_hPos;
    QString   m_projectPath;

/*!
\section 计算，绘制
*/
public:
    void setXList(); // setting the x axis pointers.
    void calculate();
    void draw(QPainter *p);
private:
    bool calCheck();
    void clearOldData();
    int  findItem(const QString &name, const QList<CFeatureTagItem *> &list);
    void calculateStudioMode();
    bool calculateRunTimeValueMode(CFeatureTagItem *item, const QDateTime &rawTime);    
/*!
\section 序列化和反序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
public:
    CHistoryTagManager &operator =(const CHistoryTagManager &other);

public:
    void changeLanguage();
    void collectWords();
};
#endif //CHISTORYTAGMANAGER_H
