#ifndef MDJAVASCRIPTCODEEDITOR_H
#define MDJAVASCRIPTCODEEDITOR_H
#if defined(MDJAVASCRIPTCODEEDITOR_LIBRARY)
#  define MDJAVASCRIPTCODEEDITOR_EXPORT Q_DECL_EXPORT
#else
#  define MDJAVASCRIPTCODEEDITOR_EXPORT Q_DECL_IMPORT
#endif
#include<QtCore>
class MDVisiScript;
class QVBoxLayout;
#include "BaseForm.h"

class  MDJAVASCRIPTCODEEDITOR_EXPORT CMDSaveInterface:public QObject
{
    Q_OBJECT
public:
    CMDSaveInterface(){m_pCodeEditor = NULL;}
    virtual ~CMDSaveInterface()
    {
        if(m_pCodeEditor != NULL)
            delete m_pCodeEditor;
        m_pCodeEditor = NULL;
    }
    MDVisiScript* m_pCodeEditor;
Q_SIGNALS:
    void signalSave();
    void signalClose();
};

class IStudioInterface;
class  MDJAVASCRIPTCODEEDITOR_EXPORT CMDJavaScriptCodeEditor:public CMDSaveInterface
{
    Q_OBJECT
public:
    CMDJavaScriptCodeEditor(const QString& filePathFullName);
    virtual ~CMDJavaScriptCodeEditor();
    void onShow();
    bool importFile( const QString & filePathFullName);
    bool importText(const QString& titleName,const QString& text);
    QString text();

    //设置Studio接口
    void setStudioInterface(IStudioInterface* value);
};

//baseform
class MDJAVASCRIPTCODEEDITOR_EXPORT CMDJavaScriptCodeEditor2: public CMDSaveInterface, public CBaseForm
{
public:
     explicit CMDJavaScriptCodeEditor2(const QString& sFilePathFullName,QWidget *parent = 0);
    virtual ~CMDJavaScriptCodeEditor2();

    void onShow();
    virtual bool save();
    bool importFile( const QString & filePathFullName);
    bool importText(const QString& titleName,const QString& text);
    QString text();

    //设置Studio接口
    void setStudioInterface(IStudioInterface* value);

protected:
    void closeEvent(QCloseEvent *e);

public:
    QVBoxLayout* p_vLayoutMain;
};

#endif // MDJAVASCRIPTCODEEDITOR_H
