/*!
\author aGRw 2016.02.17
\section 为窗体添加滚动条
*/
#ifndef FORMSCROLLAREA_H
#define FORMSCROLLAREA_H

#include <QScrollArea>

class CFormScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    CFormScrollArea(QWidget* parent = 0);
    virtual ~CFormScrollArea();
public:
    void addWidget(QWidget* w);
private:
    void childTitleChanged(const QString& title);
};

#endif // FORMSCROLLAREA_H
