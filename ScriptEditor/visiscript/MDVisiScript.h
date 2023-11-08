#ifndef MDSCRIPT_H
#define MDSCRIPT_H
#include "visiscript.h"
class CMDSaveInterface;
class IStudioInterface;
class MDVisiScript : public VisiScript
{
public:
    MDVisiScript();
    MDVisiScript(QString sFileName,QWidget* parent = NULL);
    bool ImportFile( const QString & sFileName);
    CMDSaveInterface*  m_jsCodeEditor;
    QString GetText();
    virtual bool OnSave();
    //设置Studio接口
    void setStudioInterface(IStudioInterface* value);
    IStudioInterface* studioInterface();

    virtual void closeEvent( QCloseEvent * pCloseEvent );
private:
    IStudioInterface* m_studioInterface;
};

#endif // MDSCRIPT_H
