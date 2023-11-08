/*!

\author bWVpeGw=

\section 功能

管理【线和管道控件基类.
    用来设置线和管道的相关属性】。

*/

#ifndef MDDRAWNODE_H
#define MDDRAWNODE_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWNODE_LIBRARY)
#  define MDDRAWNODESHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWNODESHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QJsonObject>
#include "MDDrawVector.h"
#include "INodeEdit.h"

class CMDPropertyDrawNode;
class CMDNodeStream;

class MDDRAWNODESHARED_EXPORT CMDDrawNode : public CMDDrawVector, public INodeEdit
{
public:
    CMDDrawNode();
    virtual ~CMDDrawNode();

/*!
\section CMDDrawVector
*/
public:
    CMDPropertyDrawObj *createPropertyDrawObj();
    CMDDrawObj* clone();

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);


protected:
    void onPaint(QPainter* p);
    void onGeneratePath();
    bool isVisible(const QPointF& point);
    void onInitialization();
    void generateBound();

/*!
\section IEdit
*/
public:
    int minCount();                                         //! 节点最小基数
    QList<QPointF>* nodeDatas();                            //！节点链表
    NodeState nodeState();                                  //! 获取节点状态
    void setNodeState(NodeState value);                     //! 设置编辑状态
    void setIsNodeCreating(bool value);                     //! 判断节点状态
    bool isNodeCreating();                                  //! 判断是否使用鼠标创建
    bool isNodeOperate();                                   //! 判断鼠标操作
    bool createSuccess();                                   //! 判断是否创建成功
    bool createFinish();                                    //! true:自动完成创建
    void backupData();                                      //! 备份数据
    void moveNode(const QPointF& point,int pos);            //! 移动节点
    void addNode(const QPointF& point);                     //! 增加节点
    void deleteNode(int pos);                               //! 删除节点
    void creatingPaint(QPainter* p);                        //! 鼠标创建控件时绘制图形
    qreal widenWidth();

/*!
\section function
*/
private:
    void scaleDatas(QRectF current, QRectF backup, QList<QPointF>& datas, QList<QPointF>& datasBk);
    void offsetDatas(QList<QPointF>* datas, QList<QPointF>* datasBk, QPointF offset);
    void onMove(QPointF point, QPointF offset, int pos);
    QPointF calculateOffset(QList<QPointF> current, QList<QPointF> backup);
protected:
    virtual void onAdd(QPointF point);
    virtual void onDelete(int pos);
    virtual bool findIndex(QList<QPointF> datas, QPointF point,int& index);         //! 点point是否和datas数据构成的线段相交
    virtual void onNodeChanged();
    virtual void generateNodePath(QPainterPath* path);
    virtual void drawPath(QPainter* painter);

/*!
\section undo
*/
protected:
    IUndoDataObj* createUndoData();
public:
    bool revertUndoData(IUndoDataObj* value);
    IUndoDataObj *fillUndoData(bool isOld = true);
    bool undoEqual();
    void undoOperate(IUndoDataObj *value);

public:
    static QList<QStringList> expNames();
    void onDataChanged(CMDExpInfo *exp);

public:
    virtual void getPens(QList<CMDPenManager*>& list);

    void generateContent();

    virtual void drawLine(QPainter* painter);               //! 绘制线条
    void drawPipe(QPainter* painter);                       //! 绘制管道
    void drawStreamLine(QPainter* painter);                 //! 绘制流动线条

    CMDNodeStream *pLineStream() const;
    void setPLineStream(CMDNodeStream *pLineStream);

    bool pipeBackGroundVisible()const;
    void setPipeBackGroundVisible(bool visible);

    qreal pipewidth() const;
    void setPipewidth(const qreal &pipewidth);

    QColor pipeClr1() const;
    void setPipeClr1(const QColor &pipeClr1);

    QColor pipeClr2() const;
    void setPipeClr2(const QColor &pipeClr2);

private:
    void drawDirection(QPainter* p);                        //! 绘制方向标记

/*!
\section batch edit
*/
public:
    virtual CMDPropertyDrawObj* createPropertyDrawObj(const QString& className);
protected:
    //初始化类信息
    virtual void initClassInfo(QStringList &classInfo);

private:
    bool m_pipeBackGroundVisible;                           //! 是否显示管道
    qreal m_pipewidth;                                      //! 管道宽度
    QColor m_pipeClr1;                                      //! 管道前景色
    QColor m_pipeClr2;                                      //! 管道背景色
    qreal m_Density;                                        //! 分辨率
    QList<QPointF> m_nodes;                                 //! 节点链表
    QList<QPointF> m_nodesBk;                               //! 节点备份链表
    NodeState m_state;                                      //! 节点模式
    bool m_isNodeCreating;                                  //! 是否在用鼠标创建
    bool m_isNodeOperate;                                   //! 是否是使用鼠标编辑节点
    CMDNodeStream* m_pLineStream;                           //! 流动类指针
};

#endif // MDDRAWNODE_H
