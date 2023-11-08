/*!
\author aGRw 2015.05.11
\section 基础圆弧控件
*/
#ifndef MDDRAWBASICARC_H
#define MDDRAWBASICARC_H

#include "MDDrawVector.h"
#include "ICustomEdit.h"

#if defined(MDDRAWVECTORS_LIBRARY)
#  define MDDRAWVECTORSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWVECTORSSHARED_EXPORT Q_DECL_IMPORT
#endif

class CDrawBasicArcData;

class MDDRAWVECTORSSHARED_EXPORT CMDDrawBasicArc : public CMDDrawVector, public ICustomEdit
{
public:
    CMDDrawBasicArc();
    virtual ~CMDDrawBasicArc();
//////////////////////////////////////////////////////////////////////////
private:
    CDrawBasicArcData* m_data;

/*!
\section property
*/
public:
    void setStartAngle(qreal value);
    qreal startAngle();
    void setSpanAngle(qreal value);
    qreal spanAngle();
    bool canCombine(){return true;}
//////////////////////////////////////////////////////////////////////////
public:
    CMDPropertyDrawObj *createPropertyDrawObj();
    static QList<QStringList> expNames();
    void onDataChanged(CMDExpInfo *exp);

/*!
\section protected function
*/
protected:
    void backupData() ;

/*!
\section custom edit
*/
public:
    QList<QPointF>* customDatas();
    QPointF customCenter();

public:
    void generateCustom();
    void moveCustom(const QPointF& point, int pos);

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
    QPointF calculateAnglePoint(const QRectF& rect, qreal angle);
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
    bool equal(qreal startAngle, qreal spanAngle);

/*!
\section batch edit
*/
public:
    virtual CMDPropertyDrawObj* createPropertyDrawObj(const QString& className);
protected:
    //初始化类信息
    virtual void initClassInfo(QStringList &classInfo);
};

#endif // MDDRAWBASICARC_H














