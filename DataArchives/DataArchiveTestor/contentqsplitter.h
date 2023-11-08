#ifndef CONTENT_SPLITTER_H
#define CONTENT_SPLITTER_H

#include <QWidget>
#include <QSplitter>
#include <QTreeWidget>
#include <QPlainTextEdit>
#include <QTabWidget>
#include "MainTreeWidget.h"

class ContentQSplitter : public QSplitter
{
    Q_OBJECT
public:
    explicit ContentQSplitter(QWidget *parent = 0);
public:
    void DisplayReSize();
    void ReloadUI();
private:
    void InitLeftTreeWidget();
    void SplitWidget();
private:
    //QSplitter * m_pMainSplitter;
    MainTreeWidget  * m_pLeftDisplayTreeWidget;
    QSplitter *  m_pRightDisplayWidget;
    QTabWidget *  m_pTabWidget;
    QPlainTextEdit *  m_pOutputWidget;
signals:

public slots:

};

#endif // CONTENTWIDGET_H
