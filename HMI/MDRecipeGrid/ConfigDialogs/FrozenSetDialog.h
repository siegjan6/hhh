/*!
author Y2hvdWps 2016.08.30
section 固定行列功能设置对话框
*/
#ifndef FROZENSETDIALOG_H
#define FROZENSETDIALOG_H

#include <QDialog>
#include "GridItems/GridBaseItem.h"

namespace Ui {
class CFrozenSetDialog;
}

class CFrozenSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CFrozenSetDialog(qint32 rowFrozen, qint32 columnFrozen, const frameData& data, QWidget *parent = 0);
    virtual ~CFrozenSetDialog();

public:
    frameData m_frameData;
    qint32 m_rowFrozen;
    qint32 m_columnFrozen;
protected:
    virtual void showEvent(QShowEvent * event);

private slots:
    void on_toolbuttonColor_clicked();
    virtual void accept();

private:
    Ui::CFrozenSetDialog *ui;

};

#endif // FROZENSETDIALOG_H
