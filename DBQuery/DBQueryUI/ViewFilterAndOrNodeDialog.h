#ifndef VIEWFILTERANDORNODEDIALOG_H
#define VIEWFILTERANDORNODEDIALOG_H

#include <QDialog>
#include <MDDBQueryConfigure.h>

namespace Ui {
class ViewFilterAndOrNodeDialog;
}

class ViewFilterAndOrNodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewFilterAndOrNodeDialog(QWidget *parent = 0);
    ~ViewFilterAndOrNodeDialog();
    void setType(DBQ::VIEW_FILTER_NODE_TYPE type);
    DBQ::VIEW_FILTER_NODE_TYPE getType();
private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::ViewFilterAndOrNodeDialog *ui;
};

#endif // VIEWFILTERANDORNODEDIALOG_H
