/*!
\author aGRw 2015.06.12
\section 图形控件所需要的框架接口
*/
#ifndef IDRAWENVIRONMENT_H
#define IDRAWENVIRONMENT_H

#include <QString>
#include <QRect>

class IMDProjectExpert;
class IMDDataArchives;
class IStudioInterface;

class IDrawEnvrionment
{
public:
    virtual ~IDrawEnvrionment(){}
/*!
\section property
*/
public:
    //!编辑环境还是运行环境
    virtual bool isStudioMode() = 0;
    //!项目路径
    virtual QString projectPath() = 0;
    //!数据归档
    virtual IMDDataArchives *dataArchives() = 0;
    //!工程环境
    virtual IMDProjectExpert* projectExpert() = 0;
    //!编辑版环境
    virtual IStudioInterface* studioProject() = 0;
/*!
\section function
*/
public:
    //!刷新画面
    virtual void update(const QRect &rect) = 0;
};

#endif // IDRAWENVIRONMENT_H
