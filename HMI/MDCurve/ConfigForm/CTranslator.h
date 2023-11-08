/*!

\author eHVr

\section 功能
    多语言功能函数
*/
#ifndef CTRANSLATOR_H
#define CTRANSLATOR_H
#include "MDMultiLanguage.h"
#include <QDialog>

namespace Translator {

inline void installDialogTranslator(ITranslateWidgetTool *tool,QDialog *dialog,
                             const QStringList & exs = QStringList("QLineEdit"),
                             const QStringList &severedNames = QStringList())
{
    tool = CMDMultiLanguage::createWidgetTool();
    if(tool){
        tool->setExceptionTypes(exs);
        tool->setSeveredNames(severedNames);
        tool->init(dialog);
        tool->changeLanguage();
    }
}
inline void installTranslatorWidget(QObject *obj)
{
    CMDMultiLanguage::instance()->translateWidget(obj);
}

inline QString translatorString(const QString &name)
{
    return CMDMultiLanguage::instance()->value(name,false);
}

inline void addTranslatorWord(const QString &word)
{
    CMDMultiLanguage::instance()->addWord(word,false);
}
}
#endif // CTRANSLATOR_H
