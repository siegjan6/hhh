#ifndef MDRUNTIMELANGUAGEOBJECT_H
#define MDRUNTIMELANGUAGEOBJECT_H

#include "ILanguageObject.h"

class MainWindow;
class CMDRuntimeLanguageObject : public ILanguageObject
{
public:
    CMDRuntimeLanguageObject();
    void setMainWindow(MainWindow *pWnd);
    MainWindow *m_mainWnd;
public:
    //!切换语言
    void changeLanguage();
    //!获取项目字典
    void collectWords();
};

#endif // MDRUNTIMELANGUAGEOBJECT_H
