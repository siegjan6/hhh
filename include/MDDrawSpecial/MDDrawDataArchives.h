#ifndef MDDRAWDATAARCHIVES_H
#define MDDRAWDATAARCHIVES_H

#include <QJsonObject>
#include <QtCore/qglobal.h>

#if defined(MDDRAWSPECIAL_LIBRARY)
#  define MDDRAWSPECIALSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWSPECIALSHARED_EXPORT Q_DECL_IMPORT
#endif
//////////////////////////////////////////////////////////////////////////
#include "MDDrawWidget.h"

class CDrawDataArchivesData;
//数据归档控件
class MDDRAWSPECIALSHARED_EXPORT CMDDrawDataArchives : public CMDDrawWidget
{
public:
    CMDDrawDataArchives();
    virtual ~CMDDrawDataArchives();
private:
    CDrawDataArchivesData* m_data;
public:
    bool openConfigDialog();
    void onDoubleClick();

    void query();
public:
    DrawType type();
    QString className(){return "CMDDrawDataArchives";}
//////////////////////////////////////////////////////////////////////////
//virtual function
public:
    //返回属性框对象
    CMDPropertyDrawObj *createPropertyDrawObj();
    //初始化
    void onInitialization();
protected:
    //创建自身
    CMDDrawObj *createSelf();
//////////////////////////////////////////////////////////////////////////
//common
public:
    //序列化

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
    CMDDrawObj* clone();
//////////////////////////////////////////////////////////////////////////
};

#endif // MDDRAWDATAARCHIVES_H
