/*!
\author aGRw 2014.09.05
\section 矩形控件
*/
#ifndef MDDRAWRECT_H
#define MDDRAWRECT_H

#include "ICustomEdit.h"
#include "MDDrawVector.h"

#if defined(MDDRAWVECTORS_LIBRARY)
#  define MDDRAWVECTORSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWVECTORSSHARED_EXPORT Q_DECL_IMPORT
#endif

class CDrawRectData;


class MDDRAWVECTORSSHARED_EXPORT CMDDrawRect : public CMDDrawVector, public ICustomEdit
{
public:
    CMDDrawRect();
    virtual ~CMDDrawRect();

private:
    CDrawRectData* m_data;

/*!
\section property
*/
public:
    void setXRound(qreal value);
    qreal xRound();
    void setYRound(qreal value);
    qreal yRound();
    bool canCombine(){return true;}
/*!
\section virtual property
*/
public:
    DrawType type() ;
    QString className(){return "CMDDrawRect";}
/*!
\section protected function
*/
protected:
    CMDDrawObj *createSelf() ;
    void onGeneratePath() ;
    void backupData() ;
    void onInitialization() ;

/*!
\section custom edit
*/
public:
    QList<QPointF>* customDatas();
    QPointF customCenter();
public:
    void generateCustom();
    void moveCustom(const QPointF& point, int pos);
private:
    void onMouseMove(const QPointF& offset, int pos);

/*!
\section common
*/
public:
    void serialize(QJsonObject &json);
    void deserialize(const QJsonObject &json);
    CMDDrawObj* clone() ;

/*!
\section private function
*/
private:
    void generateRoundedPath(QPainterPath& path);
    void limitRound();
public:
    void setRound(qreal value, bool isX);

/*!
\section public function
*/
public:
    CMDPropertyDrawObj *createPropertyDrawObj() ;
    void onPaint(QPainter* p) ;
private:
/*!
\section undo
*/
protected:
    IUndoDataObj* createUndoData() ;
public:
    bool revertUndoData(IUndoDataObj* value) ;
    IUndoDataObj *fillUndoData(bool isOld = true) ;
    bool undoEqual() ;
private:
    bool equal(qreal xRound, qreal yRound);

};

#endif // MDDRAWRECT_H


