/*!
\author aGRw 2016.01.12
\section Widget翻译工具，接口类
*/
#ifndef ITRANSLATEWIDGETTOOL_H
#define ITRANSLATEWIDGETTOOL_H

#include <QObject>
#include <QStringList>

class ITranslateWidgetTool
{
public:
    virtual ~ITranslateWidgetTool(){}
public:
    //!不需要翻译的控件类型
    virtual void setExceptionTypes(const QStringList& types) = 0;
    //！在不翻译的控件中需要翻译的控件名称
    virtual void setTranslateWidgetName(const QStringList& names) = 0;
    //!需要中间隔断的对象名称
    virtual void setSeveredNames(const QStringList& names) = 0;
public:
    //!初始化
    virtual void init(QObject* obj, bool isSystem = true) = 0;
    //!语言切换
    virtual void changeLanguage() = 0;
};

#endif // ITRANSLATEWIDGETTOOL_H
