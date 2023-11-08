/*!
\author aGRw 2016.01.05
\section 翻译控件接口类
*/
#ifndef ILANGUAGEOBJECT_H
#define ILANGUAGEOBJECT_H

#include <QString>

class ILanguageObject
{
public:
    virtual ~ILanguageObject(){}
public:
    //!切换语言
    virtual void changeLanguage() = 0;
    //!获取项目字典
    virtual void collectWords() = 0;
};

#endif // ILANGUAGEOBJECT_H
