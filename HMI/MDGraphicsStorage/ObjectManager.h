/*!
\author aGRw 2015.07.20
\section 图元管理类
*/
#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <QString>
#include <QPainter>
#include "MDDrawObj.h"

class CObjectManager
{
public:
    CObjectManager(const QString& path, const QString& imagePath, QWidget* parent = NULL);
    ~CObjectManager();
//////////////////////////////////////////////////////////////////////////
private:
    int ObjectSize;//!单个图元的尺寸
    int Interval;
    QString sPattern;
//////////////////////////////////////////////////////////////////////////
public:
    QString currentName();
    CMDDrawObj* currentObj(){return m_currentObj;}
    QSize size(){return m_size;}
private:
    CMDDrawObj* m_currentObj;
    QSize m_size;//!尺寸
private:
    QList<CMDDrawObj*> m_list;//!图元列表
    int m_index;//!图元索引
    QWidget* m_parent;
    QString m_imagePath;//!图片目录
//////////////////////////////////////////////////////////////////////////
public:
    void paint(QPainter* painter);
    //!选择图元
    void selectObject(int x, int y);
    //!计算尺寸,根据scrollArea大小，得出widget大小
    QSize calculateSize(int scrollAreaWidth, int scrollAreaHeight);
//////////////////////////////////////////////////////////////////////////
private:
    //!加载图元
    void loadObj(const QString& path);
    void deserialize(const QJsonObject& json);
    //!设置图元索引
    void setIndex(int value);
    //!绘制控件
    void paintObject(QPainter* p, const QRect& rect, CMDDrawObj* obj);
};

#endif // OBJECTMANAGER_H
