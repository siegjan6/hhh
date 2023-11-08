/*!
author Y2hvdWps 2016.11.16
section QRadioButton的集合类,用于CRdionButtonItem
*/
#ifndef RADIOGROUP_H
#define RADIOGROUP_H

#include <QGroupBox>
#include <QButtonGroup>
#include "ValueMap/ValueMapManager.h"
#include "RecipeGridDelegate.h"
#include <QEvent>
#include <QMouseEvent>
#include <QRadioButton>

class CRadioButtonItem;

class CGridRadioGroup : public QGroupBox
{
    Q_OBJECT
public:
    explicit CGridRadioGroup(CRadioButtonItem* item, const CRecipeGridDelegate *delegate, QWidget *parent = 0);

    void enableEdit(bool edit);
    void setCheckState(qint32 index);
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    CRadioButtonItem* m_item;
    QButtonGroup* m_group;
    bool m_userActivate;
signals:
    void editFinish();
protected slots:
    void onUserActivated(int);
    void onButtonPressed(int);
};

//! group用到的radiobutton
class CGridRadioButton : public QRadioButton
{
    Q_OBJECT
public:
    explicit CGridRadioButton(const QString &text, QWidget *parent = 0) :QRadioButton(text,parent){}
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};
#endif // RADIOGROUP_H
