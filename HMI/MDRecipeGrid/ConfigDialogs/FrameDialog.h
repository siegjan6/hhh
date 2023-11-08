/*!
author Y2hvdWps 2016.08.29
section 单元格的边框设置对话框
*/
#ifndef FRAMEDIALOG_H
#define FRAMEDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include "GridItems/GridBaseItem.h"
#include <QFrame>

namespace Ui {
class CFrameDialog;
}

class CFrameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CFrameDialog(const frameData& data, QWidget *parent = 0);
    virtual ~CFrameDialog();

public:
    frameData m_frameData;

protected:
    virtual void showEvent(QShowEvent * event);
private slots:
    void on_toolbuttonColor_clicked();

    void buttonCheckedHandler(int type, bool checked);
    void buttonFrameAllClicked();

    virtual void accept();
private:
    Ui::CFrameDialog *ui;
    QButtonGroup* m_typeButtons;

private:
    void initFrameTypeWidgets();
};

class CFrameTypeWidget : public QFrame
{
    Q_OBJECT
public:
    explicit CFrameTypeWidget(QWidget *parent = 0);
    ~CFrameTypeWidget(){}
public://var
    qint32 m_typeShowed;
protected:
    void paintEvent(QPaintEvent* event);
    qint32 m_cornerWidth;
};

#endif // FRAMEDIALOG_H
