/*!
\author aGRw 2016.01.11
\section Widget控件翻译类
*/
#ifndef MDTRANSLATEWIDGETTOOL_H
#define MDTRANSLATEWIDGETTOOL_H

#include <QObject>
#include <QStringList>
#include "ITranslateWidgetTool.h"

class CTranslateItem;

class CTranslateWidgetTool : public ITranslateWidgetTool
{
public:
    CTranslateWidgetTool();
    ~CTranslateWidgetTool();

public:
    bool isSystem(){return m_isSystem;}
private:
    bool m_isSystem;
    CTranslateItem* m_translate;

public:
    void init(QObject* obj, bool isSystem = true);
    void changeLanguage();
    bool isValidObject(QObject* obj);
    bool isSeveredObject(QObject* obj);
public:
    void setExceptionTypes(const QStringList& types){m_execptionTypes = types;}
    void setTranslateWidgetName(const QStringList& names){m_translateNames = names;}
    void setSeveredNames(const QStringList& names){m_severedNames = names;}
private:
    //!不需要翻译的控件类型
    QStringList m_execptionTypes;
    //！在不翻译的控件中需要翻译的控件名称
    QStringList m_translateNames;
    //!需要中间隔断的对象名称
    QStringList m_severedNames;
};

#endif // MDTRANSLATEWIDGETTOOL_H
