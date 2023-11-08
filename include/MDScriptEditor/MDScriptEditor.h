/*!
author Y2hvdWps 2016.12.14
section 脚本编辑器共享库，可提供CBaseForm和模态对话框两种形态
*/
#ifndef MDSCRIPTEDITOR_H
#define MDSCRIPTEDITOR_H


#include <QtCore/qglobal.h>
#include <QDialog>

#if defined(MDSCRIPTEDITOR_LIBRARY)
#  define MDSCRIPTEDITORSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDSCRIPTEDITORSHARED_EXPORT Q_DECL_IMPORT
#endif


class CBaseForm;

class MDSCRIPTEDITORSHARED_EXPORT CMDScriptEditor
{

public:
    CMDScriptEditor();
    virtual ~CMDScriptEditor();

    /*!
    objs是js脚本对象列表：其中每个pair代表一个obj
    pair第一个参数:obj对象名称，例如"rect1"
    pair第二个参数:obj对象所属类的名称，例如"IRect"

    函数返回QDialog::Accepted时候content是最新值.
    如果返回QDialog::Rejected，代表放弃修改。content不变.
    */
    int showDialog(QString& content, const QList< QPair<QString, QString> >& objs, QWidget* parent=0);

    CBaseForm *createForm(QString filePath, const QList< QPair<QString, QString> >& objs, QWidget* parent=0);

    //! 退出window之前调用，比如QMainWindow的析构函数
    static void cleanUp(void);
};

#endif // MDSCRIPTEDITOR_H
