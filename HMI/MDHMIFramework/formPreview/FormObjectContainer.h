/*!
\author aGRw 2016.03.14
\section 图形窗体管理对象
*/
#ifndef CFORMOBJECTCONTAINER_H
#define CFORMOBJECTCONTAINER_H

#include <QWidget>
#include "FormObject.h"
#include "MDHMIForm.h"

class CPreviewDialog;

class CFormObjectContainer : public QWidget
{
    Q_OBJECT
public:
    explicit CFormObjectContainer(QWidget *parent = 0);
    virtual ~CFormObjectContainer();
/*!
\section enum
*/
public:
    //!鼠标操作类型
    enum class MoveType
    {
        None,
        Left,
        Right,
        Top,
        Bottom,
        Move,
    };
/*!
\section property
*/
public:
    void setAutoJoin(bool value){m_autoJoin = value;}
    bool autoJoin(){return m_autoJoin;}
    void setFrameSize(const QSize& value);
    QSize frameSize(){return m_frameSize;}
    void setIsTransparent(bool value){m_isTransparent = value;}
    bool isTransparent(){return m_isTransparent;}
    void setCurrentObj(CFormObject* value);
    CFormObject* currentObj(){return m_currentObj;}
private:
    bool m_autoJoin;//!是否是自动拼接
    QSize m_frameSize;//!框架大小
    bool m_isTransparent;//!是否半透明
    CFormObject* m_currentObj;//!当前对象
/*!
\section field
*/
private:
    QList<CFormObject*> m_list;//!当前的图形窗体列表
    CPreviewDialog* m_container;
/*!
\section public function
*/
public:
    //!添加窗体对象
    void appendForm(CMDHMIForm* form);
    //!查找已存在的窗体对象
    CFormObject* findOpened(const QString& fullName);
    //!保存
    void saveForms();
    //!选中对象
    void select(CFormObject* obj);
    void init(CPreviewDialog* container){m_container = container;}
    void onKeyPress(QKeyEvent *e);
    void onClose(QCloseEvent* e);
/*!
\section private function
*/
private:
    void clear();

/*!
\section event
*/
protected:
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent * e);
/*!
\section mouse operate
*/
private:
    MoveType getOperateType(CFormObject* obj, QPoint p);
    void resetCursor(MoveType type);
    void selectObject(const QPoint& p);
    void mouseOperate(const QPoint& currentPos);
    //!自动拼接
    void joinRect(CFormObject* obj, QRect &rect, MoveType type);
    void setRect(const QRect& rect);
private:
    MoveType m_operateType;//!操作类型
    QPoint m_mouseDownPoint;//!鼠标按下时坐标
    QRect m_backupRect;//!鼠标按下时当前窗体对象坐标
/*!
\section signal
*/
signals:
    void rectChanged();
/*!
\section test
*/
public:
    QList<CFormObject*>* list(){return &m_list;}
};

#endif // CFORMOBJECTCONTAINER_H
