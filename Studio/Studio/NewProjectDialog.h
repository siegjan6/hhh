#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include <QDialog>
#include <QDir>
#include <QDialog>
#include <QFileDialog>
#include "MDProjectConfiger.h"

namespace Ui {
class NewProjectDialog;
}

class IMDProjectManager;
class CNewProjectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CNewProjectDialog(QWidget *parent = 0);
    virtual ~CNewProjectDialog();
private slots:
    void on_OkButton_clicked();
    void on_CancelButton_clicked();
    void on_BrowseButton_clicked();
private:
    Ui::NewProjectDialog *ui;
public:
    QString m_projectName;
    QString m_projectPath;
    QString m_description;
};

#endif // NEWPROJECTDIALOG_H
