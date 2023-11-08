/*!
\author Y2hhaWhq
\section 日志控件生成类
*/
#ifndef MDDRAWLOG_H
#define MDDRAWLOG_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWSPECIAL_LIBRARY)
#  define MDDRAWSPECIALSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWSPECIALSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QJsonObject>
#include "MDDrawWidget.h"
#include "MDDataGroupGrid/MDGroupGridDefine.h"
class CDrawLogData;

class MDDRAWSPECIALSHARED_EXPORT CMDDrawLog : public CMDDrawWidget
{
public:
    CMDDrawLog();
    virtual ~CMDDrawLog();

/*!
\section  绘制控件
*/
public:
    DrawType type() ;
    QString className(){return "CMDDrawLog";}
private:
    //! 返回生成类型
    CDrawLogData* m_data;
    //! 返回属性框对象
    CMDPropertyDrawObj *createPropertyDrawObj() ;
protected:
    //! 创建自身
    CMDDrawObj *createSelf() ;

/*!
\section  初始化
*/
public:
    void onInitialization() ;
//    void setGridType(GridType type);  //! 控件类型
/*!
\section  打开配置框
*/
public:
    bool openConfigDialog();
    void onDoubleClick();
/*!
\section  触发通知
*/
public:
    void notify(NotifyType type) ;

/*!
\section  序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
    CMDDrawObj* clone();
};

#endif // MDDRAWLOG_H

