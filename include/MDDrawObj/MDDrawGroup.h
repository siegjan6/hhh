/*!
\author aGRw 2014.10.13
\section 成组控件
*/
#ifndef MDDRAWGROUP_H
#define MDDRAWGROUP_H

#include "MDDrawMulti.h"
#include "MDCustomProperty.h"

class IHMIForm;
class CDrawGroupData;
class CMDCustomProperty;

class MDDRAWOBJSHARED_EXPORT CMDDrawGroup : public CMDDrawMulti
{
public:
    CMDDrawGroup(QList<CMDDrawObj*>* list = NULL);
    virtual ~CMDDrawGroup();
    void clear();
//////////////////////////////////////////////////////////////////////////
private:
    CDrawGroupData* m_group;
/*!
\section property
*/
public:
    DrawType type(){return DrawType::Group;}
    QString className(){return "CMDDrawGroup";}
    bool isVector();
    bool canCombine(){return false;}
/*!
\section virtual function
*/
public:
    void draw(QPainter* painter, bool isPrinting = false) ;
    bool isVisible(const QPointF& point) ;
    void loadInitalizationEvent() ;
    void generateBound() ;
    CMDPropertyDrawObj *createPropertyDrawObj() ;
/*!
\section public function
*/
public:
    //!获取point所在的最底层子控件
    CMDDrawObj* visibleChild(const QPointF& point);
/*!
\section protected function
*/
protected:
    void onGeneratePath() ;
    CMDDrawObj *createSelf() ;
/*!
\section common
*/
public:
    void serialize(QJsonObject &json);
    void deserialize(const QJsonObject &json);
    CMDDrawObj *clone();
/*!
\section event
*/
public: 
    void loadDoubleClickEvent(const QPointF& point) ;
/*!
\section var
*/
public:
    static QList<QStringList> expNames();
    void replaceCustomProperty() ;
    void analyseExpression() ;
private:
    //如果是非矢量组，删除矢量表达式
    void removeVectorExp();
/*!
\section widget
*/
public:
    void updateFormScale() ;
    void updateVisible();
    void updateEnabled();
    void top() ;
    void last() ;
/*!
\section custom property
*/
public:
    QList<CMDCustomProperty> *customPropertys();
/*!
 \section batch edit
*/
public:
    virtual CMDPropertyDrawObj* createPropertyDrawObj(const QString& className);
protected:
    //初始化类信息,成组控件需要特殊处理
    virtual void initClassInfo(QStringList &classInfo);
};

#endif // MDDRAWGROUP_H

