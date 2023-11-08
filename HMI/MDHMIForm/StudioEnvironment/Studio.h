/*!
\author aGRw 2014.09.19
\section 编辑版环境
*/
#ifndef STUDIO_H
#define STUDIO_H

#include <QRubberBand>
#include <QUndoStack>

#include "../Environment.h"
#include "IStudio.h"
#include "IObjectChangedEvent.h"
#include "CreateDrawObject.h"

class CControlPointContainer;
class IHMIForm;


class CStudio : public CEnvironment, public IStudio
{
public:
    CStudio(IHMIForm* control);
    virtual ~CStudio();
/*!
\section public function
*/
public:
    CMDDrawObj *studioCreateDrawObj(DrawType type, const QRectF& rf);
    void statusLabelChanged();
    void selectedObjChanged();
    void itemSelectObject(QList<CMDDrawObj*>* objs);
    void selectPropetyObject(CMDDrawObj* obj);
    void setLastSelectedObject(CMDDrawObj *obj);
    void addToObjectTree(QList<CMDDrawObj*>* objs);
/*!
\section private function
*/
private:
    IObjectChangedEvent* objectChangedEvent();
    void selectAll();
/*!
\section event
*/
public:
    void mouseMove(Qt::MouseButton button, const QPointF& location) ;
    void mouseDown(Qt::MouseButton button, const QPointF& location) ;
    void mouseUp(Qt::MouseButton button, const QPointF& location) ;
    void keyPressEvent(QKeyEvent *e) ;
    void mouseDoubleClickEvent(const QPointF &point);
/*!
\section field
*/
private:
    bool m_isSelectFrame;

    QPointF m_downPoint;
    QCursor m_rotateCursor;
/*!
\section property
*/
public:
    CControlPointContainer* controlPoint(){return m_controlPoint;}
    void setIsOrtho(bool value){m_isOrtho = value;}
    bool isOrtho(){return m_isOrtho;}
    void setIsGrid(bool value){m_isGrid = value;}
    bool isGrid(){return m_isGrid;}
    void* property();
    QList<CMDDrawObj*>* selectObjs(){return &m_selectedObjs;}
    QRubberBand* rubberBand(){return m_rubberBand;}
private:
    CControlPointContainer* m_controlPoint;
    bool m_isOrtho;
    bool m_isGrid;

    QList<CMDDrawObj*> m_selectedObjs;
    QRubberBand* m_rubberBand;  //!绘制方框

/*!
\section virtual function
*/
public:
    void paint(QPainter* p, const QRect &r) ;
    //!绘制背景
    void drawBackground(QPainter* p) ;
    void objectCanSelectChanged(CMDDrawObj* obj) ;
    void initalization() ;
    void setFullName(const QString& value) ;
/*!
\section refresh
*/
public:
    void invalidateObject(CMDDrawObj* obj) ;
    void invalidateControlPoint(CControlPointContainer* control);
    void timeInvalidate();
    //批量操作后，修改控制点
    void invalidateBatchControlPoint();
/*!
\section form scroll and scale
*/
public:
    void setRect(const QRect& value) ;
    void setFormScale(const QPointF& value);
    QPointF formScale()  {return m_formScale;}
private:
    QPointF m_formScale; //!窗体缩放比例
private:
    void setScrollSize();
/*!
\section group
*/
public:
    void group();
    void unGroup();
    void combine();
    void unCombine();
/*!
\section common
*/
public:
    void serialize(QJsonObject &json);
    void deserialize(const QJsonObject &json);
/*!
\section drawobj status changed
*/
private:
    void refreshProperty();
    void selectObject(QObject *obj);
public:
    void resetSelectObjs(bool isNullObj = false);
/*!
\section key event
*/
public:
    static void clearCopyList();
private:
    static QList<CMDDrawObj*> m_copyList;
    static QString m_copyImagePath;//!拷贝控件所在的工程图形目录
public:
    void copy();
    void paste();
    void deleteObj();
    void toLeft();
    void toRight();
    void toTop();
    void toBottom();
    void increaseWidth();
    void decreaseWidth();
    void increaseHeight();
    void decreaseHeight();
private:
    CMDDrawObj* cloneDrawObj(CMDDrawObj* orig);
/*!
\section state
*/
public:
    void ortho();
    void grid();
    void scale(const QString &text);
    void layer();
/*!
\section layout
*/
private:
    enum class LayoutType{ Left, Top };
    int MoveSize;
private:
    qreal getSortValue(CMDDrawObj* obj, LayoutType type);
    void sortList(QList<CMDDrawObj*>* sorts, LayoutType type);
    void incrSpace(LayoutType type);
public:
    void alignLeft();
    void alignCenter();
    void alignRight();
    void alignTop();
    void alignMiddle();
    void alignBottom();
    void sameWidth();
    void sameHeight();
    void sameSize();
    void hSpace();
    void incrHSpace();
    void decrHSpace();
    void vSpace();
    void incrVSpace();
    void decrVSpace();
/*!
\section sequence of object
*/
public:
    enum class SequenceType{Top, Last, Front, Back};
public:
    void sequenceFun(SequenceType type);
/*!
\section flip
*/
public:
    void flipX();
    void flipY();
/*!
\section create
*/
public:
    CCreateDrawObject* create(){return m_create;}
private:
    //!控件构建类
    CCreateDrawObject* m_create;
public:
    void setCreateObjectState(DrawType type);
    void endCreate();
    void createDrawObjectItem(DrawType type);
/*!
\section graphics storage
*/
public:
    //!粘帖控件,只限制图库调用
    void pasteObject(CMDDrawObj* obj);
    void saveToStorage();
    void saveToStoragePath();
    void storage();
/*!
\section undo
*/
public:
    QUndoStack* undo(){return &m_undo;}
private:
    QUndoStack m_undo;
    QList<CMDDrawObj*> m_deletedObjs;//!已被删除的控件
    QList<CMDDrawObj*> m_deletedGroups;//!已解组的成组控件
    QList<CMDDrawObj*> m_deletedCombines;//已解组的联合控件
public:
    void undoRefresh();//!发生redo/undo时的刷新函数
    void undoAdd(QList<CMDDrawObj*>* addObjs, QList<int> *indexs);
    void undoDelete(QList<CMDDrawObj*>* delOjbs, QList<int> *indexs);
    void undoGroup(QList<CMDDrawObj*>* groupObjs, QList<int> *indexs);
    void undoUnGroup(QList<CMDDrawObj*>* unGroupObjs, QList<int> *indexs);
    void undoCombine(QList<CMDDrawObj*>* combineObjs, QList<int> *indexs);
    void undoUnCombine(QList<CMDDrawObj*>* unCombineObjs, QList<int> *indexs);
    void undoLayer(QVector<bool>* visibles, QVector<bool>* lockeds);
    void undoBrush(CMDBrushManager *value);
    void undoIndex(CMDDrawObj *obj, int newIndex);
    void undoAction(int id, CMDHMIActionManager* action);
    void undoText(const QString& value);
    void undoFormType(int type, bool hasFrame, bool fullScreen);
    void undoSecurity(CMDActionSP* open, CMDActionSP* close);
private:
    QList<int> objectIndexs(QList<CMDDrawObj*>* objects);
    QList<QVariant> objectUndoDatas(QList<CMDDrawObj*>* objects);
/*!
\section language
*/
public:
    void collectWords();
/*!
\section priview dialog
*/
public:
    //!绘制预览窗体
    void drawPreview(QPainter* p);
    //!显示布局预览窗口
    void formLayout();
/*!
\section batch edit
*/
public:
    void batchEdit();
private:
    //获取基类名称
    QString basicClassName(QList<CMDDrawObj*>* objs);
};

#endif // STUDIO_H


