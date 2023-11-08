/*!

\author luyj

\section 功能

管理【图形窗体，特殊控件进程主窗体停靠】。

*/

#ifndef MDDRAWPROCDOCKER_H
#define MDDRAWPROCDOCKER_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWSPECIAL_LIBRARY)
#  define MDDRAWSPECIALSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWSPECIALSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QJsonObject>
#include "MDDrawWidget.h"
#include "IDrawEnvironment.h"

class CDrawProcDockerData;

//! 报表控件
class MDDRAWSPECIALSHARED_EXPORT CMDDrawProcDocker : public CMDDrawWidget
{
public:
    CMDDrawProcDocker();
    virtual ~CMDDrawProcDocker();

/*!
\section 报表输出控件相关属性接口
*/
public:
    //! 将cmd运行后的主界面停靠于ProcDocker预留位置
    void run(const QString& cmd);

    void setCmd(const QString& cmd);
    QString getCmd();

    //! 多语言
public:
    void changeLanguage();
    void collectWords();

private:
    CDrawProcDockerData* m_procDockerData;

/*!
\section DrawWidget
*/
public:
    DrawType type() ;
    QString className(){return "CMDDrawProcDocker";}

/*!
\section virtual function
*/
public:
    CMDPropertyDrawObj *createPropertyDrawObj() ; //! 返回属性框对象
    void onInitialization() ; //! 初始化

protected:

    CMDDrawObj *createSelf() ; //! 创建自身

/*!
\section 序列化
*/
public:
    CMDDrawObj* clone() ; //! 克隆函数，复制报表控件

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

#endif // MDDRAWPROCDOCKER_H
