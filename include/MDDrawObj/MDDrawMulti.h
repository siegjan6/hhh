/*!
\author aGRw 2016.08.17
\section 多组控件
*/
#ifndef MDDRAWMULTI_H
#define MDDRAWMULTI_H

#include "MDDrawVector.h"
#include "MDDrawTool.h"

class IHMIForm;
class CDrawMultiData;
class CMDDrawGroup;
class CMDDrawCombine;

class MDDRAWOBJSHARED_EXPORT CMDDrawMulti : public CMDDrawVector
{
public:
    CMDDrawMulti(QList<CMDDrawObj*>* list = NULL);
    virtual ~CMDDrawMulti();
    void clear();
//////////////////////////////////////////////////////////////////////////
private:
    CDrawMultiData* m_multi;
/*!
\section property
*/
public:
    QPainterPath* matrixPath();
    QPainterPath* path();
    QTransform* matrix();
    QList<CMDDrawObj *> *objList();
    CMDDrawGroup* toGroup();
    CMDDrawCombine* toCombine();
    int baseVersion();
/*!
\section 解组操作，解组时调用
*/
public:
    void unMulti();
/*!
\section virtual function
*/
public:
    void mouseMove(const QPointF& point);
    void mouseFrameMove(const QPointF &point, int pos) ;
    void boundMove(qreal xOff, qreal yOff, qreal wOff, qreal hOff) ;
    void multiRotate(qreal angle, const QPointF &center) ;
    void loadInitalizationEvent() ;
    void setFactRect(const QRectF& rect) ;
    void generateMatrix() ;
/*!
\section private function
*/
private:
    void childObjBoundMove(qreal xOff, qreal yOff, qreal wOff, qreal hOff);
/*!
\section common
*/
public:
    void serialize(QJsonObject &json);
    void deserialize(const QJsonObject &json);
    CMDDrawObj *clone();
/*!
\section virtual property
*/
public:
    void setParent(IHMIForm* value) ;
/*!
\section data backup
*/
public:
    void backupData() ;
    void backupMousePos(const QPointF& point) ;
/*!
\section undo
*/
protected:
    IUndoDataObj* createUndoData();
public:
    bool revertUndoData(IUndoDataObj* value) ;
    IUndoDataObj *fillUndoData(bool isOld = true) ;
    void undoAdd() ;
    void undoDelete() ;
private:
    void clearData();
/*!
\section language
*/
public:
    virtual void changeLanguage();
    virtual void collectWords();
/*!
\section image
*/
public:
    virtual void getBrushes(QList<CMDBrushManager*>& list);
    virtual void getPens(QList<CMDPenManager*>& list);
    virtual void resetImagePath(const QString& path);
    virtual void copyImage(const QString& sourcePath, const QString& targetPath, bool needRename);
};

#endif // MDDRAWMULTI_H

