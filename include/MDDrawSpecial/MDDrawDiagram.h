#ifndef MDDRAWDIAGRAM_H
#define MDDRAWDIAGRAM_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWSPECIAL_LIBRARY)
#  define MDDRAWSPECIALSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWSPECIALSHARED_EXPORT Q_DECL_IMPORT
#endif
//////////////////////////////////////////////////////////////////////////
#include "MDDrawObj/MDDrawVector.h"
#include "MDDrawWidget.h"

class CDrawDiagramData;
//////////////////////////////////////////////////////////////////////////
//统计图控件
class MDDRAWSPECIALSHARED_EXPORT CMDDrawDiagram : public CMDDrawWidget
{
public:
    CMDDrawDiagram();
    virtual ~CMDDrawDiagram();
private:
    //数据
    CDrawDiagramData *m_data ;
//////////////////////////////////////////////////////////////////////////
public:
    bool openDesign();
    void onDoubleClick();
//////////////////////////////////////////////////////////////////////////
//DrawVector
//virtual property
public:
    DrawType type();
    QString className(){return "CMDDrawDiagram";}
//////////////////////////////////////////////////////////////////////////
//virtual function
public:
    //返回属性框对象
    CMDPropertyDrawObj *createPropertyDrawObj();
    //初始化
    void onInitialization();
protected:
    //创建自身
    CMDDrawWidget *createSelf();
    void onGeneratePath();
//////////////////////////////////////////////////////////////////////////
//common
public:
    //序列化
    CMDDrawObj* clone() ;
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

    //脚本
    void setString1(const QString &str);
    void setString2(const QString &str);
    void setString3(const QString &str);
    void setValue(const QString &str);

};

#endif // MDDRAWDIAGRAM_H
