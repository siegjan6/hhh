/*!
\author aGRw 2016.08.09
\section 联合控件
*/
#ifndef MDDRAWCOMBINE_H
#define MDDRAWCOMBINE_H

#include "MDDrawMulti.h"

class IHMIForm;
class CDrawCombineData;

class MDDRAWOBJSHARED_EXPORT CMDDrawCombine : public CMDDrawMulti
{
public:
    CMDDrawCombine(QList<CMDDrawObj*>* list = NULL);
    virtual ~CMDDrawCombine();
//////////////////////////////////////////////////////////////////////////
private:
    CDrawCombineData* m_combine;
/*!
\section property
*/
public:
    DrawType type() {return DrawType::Combine;}
    QString className(){return "CMDDrawCombine";}
    bool canCombine(){return true;}
    void setConnectLine(bool value);
    bool connectLine();
    void setFillMode(Qt::FillRule value);
    Qt::FillRule fillMode();
/*!
\section virtual function
*/
public:
    bool isVisible(const QPointF& point);
    CMDPropertyDrawObj *createPropertyDrawObj();
/*!
\section protected function
*/
protected:
    void onGeneratePath();
    CMDDrawObj *createSelf();
/*!
\section common
*/
public:
    void serialize(QJsonObject &json);
    void deserialize(const QJsonObject &json);
public:
    CMDDrawObj *clone();
/*!
\section var
*/
public:
    static QList<QStringList> expNames();
/*!
\section batch edit
*/
public:
    virtual CMDPropertyDrawObj* createPropertyDrawObj(const QString& className);
protected:
    //初始化类信息
    virtual void initClassInfo(QStringList &classInfo);
};

#endif // MDDRAWCOMBINE_H

