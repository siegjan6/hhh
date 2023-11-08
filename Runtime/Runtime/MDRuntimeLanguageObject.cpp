#include "MDRuntimeLanguageObject.h"
#include "mainwindow.h"

CMDRuntimeLanguageObject::CMDRuntimeLanguageObject()
{
    m_mainWnd = NULL;
}

void CMDRuntimeLanguageObject::setMainWindow(MainWindow *pWnd)
{
    m_mainWnd = pWnd;
}

void CMDRuntimeLanguageObject::changeLanguage()
{
    if(m_mainWnd)
        m_mainWnd->changeLanguage();
}

void CMDRuntimeLanguageObject::collectWords()
{
}
