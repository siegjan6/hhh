/*!
author Y2hvdWps 2016.11.16
section 用于CComboBoxItem
*/
#ifndef GRIDCOMBOBOX_H
#define GRIDCOMBOBOX_H

#include <QComboBox>

class CComboBoxItem;

class CGridComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit CGridComboBox(CComboBoxItem* item, QWidget *parent = 0);

protected:
    virtual void hidePopup();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    CComboBoxItem* m_item;
    bool m_userActivate;    //!由用户手动选择了某一项,用于判断脚本事件onEditFinish
signals:
    void editFinish();
protected slots:
    void onUserActivated(int);
};

#endif // GRIDCOMBOBOX_H
