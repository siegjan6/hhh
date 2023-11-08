#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class NewProjectDialog;
}

class NewProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewProjectDialog(QWidget *parent = 0);
    ~NewProjectDialog();
    //!返回结果为空 则无效
    QString GetProjectPath();
private slots:
    void on_browserBtn_clicked();

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::NewProjectDialog *ui;
    QString m_ProjectPath;
};

#endif // NEWPROJECTDIALOG_H
