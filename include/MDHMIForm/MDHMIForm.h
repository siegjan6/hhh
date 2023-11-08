/*!
\author aGRw
\section 图形窗体
*/
#ifndef MDHMIFORM_H
#define MDHMIFORM_H

#include <QPushButton>
#include <QtPrintSupport/QPrinter>
#include <QMainWindow>
#include <QMdiSubWindow>

#include "BaseForm.h"
#include "IHMIForm.h"
#include "IHMIFramework.h"
#include "MDDrawObj.h"
#include "MDHMIActionManager.h"

#if defined(MDHMIFORM_LIBRARY)
#  define MDHMIFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDHMIFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

class CHMIFormData;

namespace Ui {
class HMIForm;
}

class MDHMIFORMSHARED_EXPORT CMDHMIForm : public CBaseForm, public IHMIForm
{
    Q_OBJECT
//////////////////////////////////////////////////////////////////////////
    Q_PROPERTY(QMap translateName READ translateName)

    Q_PROPERTY(QString GUID_Layout)
    Q_PROPERTY(QRect rect READ rect WRITE setPropertyRect)
    Q_PROPERTY(QString DILFormType READ formType WRITE setFormType)

    Q_PROPERTY(QString GUID_Appearance)
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(QString DILbrush READ brush WRITE setBrush)

    Q_PROPERTY(QString GUID_ActionEvent)
    Q_PROPERTY(QString DILmouseDown READ mouseDown WRITE setMouseDown)
    Q_PROPERTY(QString DILmouseUp READ mouseUp WRITE setMouseUp)
    Q_PROPERTY(QString DILrightMouseDown READ rightMouseDown WRITE setRightMouseDown)
    Q_PROPERTY(QString DILrightMouseUp READ rightMouseUp WRITE setRightMouseUp)
    Q_PROPERTY(QString DILopen READ openProperty WRITE setOpenProperty)
    Q_PROPERTY(QString DILclose READ closeProperty WRITE setCloseProperty)

    Q_PROPERTY(QString GUID_Security)
    Q_PROPERTY(QString DILconfig READ configSecurity WRITE setConfigSecurity)

    Q_PROPERTY(QString GUID_Variable)
    Q_PROPERTY(QString DILvariable READ variable WRITE setVariable)
/*!
\section property function
*/
public:
    void setPropertyRect(const QRect& value);
    QRect rect();
    void setFormType(const QString& value);
    QString formType();

    void setText(const QString& value);
    QString text();
    void setBrush(const QString& value);
    QString brush();

    void setMouseDown(const QString& value);
    QString mouseDown();
    void setMouseUp(const QString& value);
    QString mouseUp();
    void setRightMouseDown(const QString& value);
    QString rightMouseDown();
    void setRightMouseUp(const QString& value);
    QString rightMouseUp();
    void setOpenProperty(const QString& value);
    QString openProperty();
    void setCloseProperty(const QString& value);
    QString closeProperty();

    void setConfigSecurity(const QString& value);
    QString configSecurity(){return "";}

    void setVariable(const QString& value);
    QString variable();
private:
    void changeActions(int id);
/*!
\section translate
*/
public:
    QMap<QString,QVariant> translateName(){return m_translateMap;}
private:
    void initTranslateMap();
private:
    QMap<QString,QVariant> m_translateMap;
//////////////////////////////////////////////////////////////////////////
public:
    CMDHMIForm(IHMIFramework* framework, QWidget *parent = 0);
    virtual ~CMDHMIForm();
private:
    Ui::HMIForm *ui;
    CHMIFormData* m_data;
/*!
\section property
*/
public:
    IHMIFramework* framework();
    IEnvironment* common();
    IStudio* studio();
    IRun* run();

    void setRect(const QRect& value);
/*!
\section public function
*/
public:
    void setModify(bool value) ;
/*!
\section private function
*/
private:
    void init();
    void initStudio();
    void initPopMenu();
    void initToolBar();
    void initScale();
    void initStatusBar();
    void initRun();
    void initMenu();
    void initLanguage();
/*!
\section event
*/
protected:
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void timerEvent(QTimerEvent *e);
    void closeEvent(QCloseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void changeEvent(QEvent * e);
    void mouseDoubleClickEvent(QMouseEvent * e);
    void enterEvent(QEvent* e);
    void leaveEvent(QEvent* e);
    void resizeEvent(QResizeEvent* e);
    void focusInEvent(QFocusEvent * e);
    void focusOutEvent(QFocusEvent * e);
    void showEvent(QShowEvent * e);
    void hideEvent(QHideEvent * e);
/*!
\section pop menu
*/
protected:
    void contextMenuEvent(QContextMenuEvent *event) ;
private:
    void copy();
    void paste();
    void deleteObj();
    void group();
    void unGroup();
    void combine();
    void unCombine();
    void saveToStorage();
    void saveToStoragePath();
/*!
\section common
*/
public:
    bool save() ;
    bool load(const QString& fullName, bool isBackground = false);
/*!
\section menu
*/
public:
    void createObjectState();
/*!
\section baseForm
*/
public:
    //!Active事件
    void setActive(bool active) ;
/*!
\section studio
*/
public:
    static void clearCopyList();
/*!
\section toolbar
*/
public:
    void setToolBarEnabled();
private:
    void ortho();
    void grid();
    void scale(const QString& text);
    void layer();

    void alignLeft();
    void alignCenter();
    void alignRight();
    void alignTop();
    void alignMiddle();
    void alignBottom();
    void sameWidth();
    void sameHeight();
    void sameSize();
    void hSpace();
    void incrHSpace();
    void decrHSpace();
    void vSpace();
    void incrVSpace();
    void decrVSpace();

    void top();
    void last();
    void front();
    void back();

    void flipX();
    void flipY();

    void script();
    void storage();
    void formLayout();

    void undo();
    void redo();

    void batchEdit();
/*!
\section public function
*/
public:
    void setStatusBarText(const QString& location, const QString& size);
/*!
\section print
*/
public:
    void print(bool showPrintDialog = true);
    void printPreview();
    void printConfig();
    void printRegion(const QRect& rect, bool showPrintDialog = true, bool isFullPage = false);
    void printObject(const QString& name, bool showPrintDialog = true, bool isFullPage = false);
private:
    void printFun(QPrinter *printer);
    void menuPrint();
/*!
\section baseForm
*/
public:
    QList<QMenu*>* menus() const;
    QList<QToolBar *> *toolBars() const;
/*!
\section editor
*/
private:
    void saveEditorEvent();
    void closeEditorEvent();
/*!
\section static
*/
public:
    //!创建控件，供外部模块调用，比如图库模块
    static CMDDrawObj* createDrawObj(DrawType type);
/*!
\section tool kit
*/
public:
    void setCreateObjectState(DrawType type);
    void endCreate();
/*!
\section language
*/
public:
    void changeLanguage();
    void collectWords();
};

#endif // MDHMIFORM_H


