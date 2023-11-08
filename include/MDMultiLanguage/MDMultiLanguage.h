/*!
\author aGRw 2016.01.08
\section 多语言，导出类
*/
#ifndef MDMULTILANGUAGE_H
#define MDMULTILANGUAGE_H

#include "IDictionaryManager.h"
#include "ILanguageObject.h"
#include "ITranslateWidgetTool.h"

#define STRANS(RawText) CMDMultiLanguage::instance()->value(RawText)
#define PTRANS(RawText) CMDMultiLanguage::instance()->value(RawText, false)

#if defined(MDMULTILANGUAGE_LIBRARY)
#  define MULTILANGUAGESHARED_EXPORT Q_DECL_EXPORT
#else
#  define MULTILANGUAGESHARED_EXPORT Q_DECL_IMPORT
#endif

class CLanguageToolWidget;

class MULTILANGUAGESHARED_EXPORT CMDMultiLanguage
{
private:
    CMDMultiLanguage();
    ~CMDMultiLanguage();
/*!
\section static
*/
public:
    //!创建字典
    static IDictionaryManager* createDictionaryManager(ILanguageObject* object, const QString& projectPath);
    static ITranslateWidgetTool* createWidgetTool();
/*!
\section single
*/
public:
    static CMDMultiLanguage* instance();
    static void release();
private:
    static CMDMultiLanguage* m_multiLanguage;
/*!
\section DictionaryManager
*/
public:
    //!设置当前工程的字典
    void setCurrentProject(IDictionaryManager* dictionaryManager);
public:
    //!是否生效
    bool enable();
    //!工具栏
    QWidget* toolWidget();
public:
    //!添加字典
    bool addWord(const QString& value, bool isSystem = true);
    //!获取翻译
    QString value(const QString& key, bool isSystem = true);
    //!翻译widget控件
    void translateWidget(QObject* obj, QStringList* types = NULL, QStringList* names = NULL, bool isSystem = true);
private:
    //!字典
    IDictionaryManager* m_manager;
    //!工具栏
    CLanguageToolWidget* m_toolWidget;
};

#endif // MDMULTILANGUAGE_H
