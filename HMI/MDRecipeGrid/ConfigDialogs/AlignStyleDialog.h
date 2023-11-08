/*!
author Y2hvdWps 2016.08.29
section 单元格的字体设置对话框
*/
#ifndef ALIGNStyleDialog_H
#define ALIGNStyleDialog_H

#include <QDialog>
#include <QButtonGroup>

namespace Ui {
class CAlignStyleDialog;
}

class CAlignStyleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CAlignStyleDialog(Qt::Alignment align, QWidget *parent = 0);
    virtual ~CAlignStyleDialog();
public:
    Qt::Alignment m_align;

protected:
     void paintEvent(QPaintEvent*);
private:
     QButtonGroup* m_buttonManager;

private slots:
    void on_confirmButton_clicked();

    void on_cancelButton_clicked();

    void buttonCheckedHandler(int id, bool);

private:
    Ui::CAlignStyleDialog *ui;
};

#endif // ALIGNStyleDialog_H
