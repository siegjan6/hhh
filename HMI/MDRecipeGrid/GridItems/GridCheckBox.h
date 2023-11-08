/*!
author Y2hvdWps 2016.11.16
section 用于CCheckBoxItem
*/
#ifndef GRIDCHECKBOX_H
#define GRIDCHECKBOX_H

#include <QCheckBox>

class CCheckBoxItem;

class CGridCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    explicit CGridCheckBox(CCheckBoxItem* item, QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    CCheckBoxItem* m_item;
    bool m_userActivate;    //!由用户手动选择了某一项,用于判断脚本事件onEditFinish
signals:
    void editFinish();
protected slots:
    void onUserActivated(int);
};

#endif // GRIDCHECKBOX_H
