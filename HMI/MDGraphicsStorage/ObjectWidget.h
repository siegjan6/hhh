/*!
\author aGRw 2015.07.22
\section 图元管理widget
*/

#ifndef OBJECTWIDGET_H
#define OBJECTWIDGET_H

#include <QWidget>
#include "ObjectManager.h"
#include "IHMIForm.h"

class CObjectWidget : public QWidget
{
    Q_OBJECT
public:
    CObjectWidget(const QString& imagePath, QWidget *parent = 0);
    ~CObjectWidget();
//////////////////////////////////////////////////////////////////////////
private:
    CObjectManager* m_manager;
    IHMIForm* m_container;
    QString m_imagePath;
//////////////////////////////////////////////////////////////////////////
public:
    void changePath(const QString& path, IHMIForm *container);
    void calculateSize(int scrollAreaWidth, int scrollAreaHeight);
//////////////////////////////////////////////////////////////////////////
protected:
    void paintEvent(QPaintEvent* e);
    void mousePressEvent(QMouseEvent* e);
    void mouseDoubleClickEvent(QMouseEvent* e);
//////////////////////////////////////////////////////////////////////////
};

#endif // OBJECTWIDGET_H
