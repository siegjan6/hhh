#ifndef MDDOWNLOADDIALOG_H
#define MDDOWNLOADDIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class CMDDownloadDialog;
}

class CMDRecipeExpert;
class QStringListModel;
class CMDDownloadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CMDDownloadDialog(QWidget *parent = 0);
    ~CMDDownloadDialog();
    QTimer m_timer;
    CMDRecipeExpert* m_recipeExpert;
    QStringListModel* m_taskListModel;
public slots:
    void handleDownloadTaskAdded(const QString &name, int showProcessDialog=1);
    void handleBeginDownloading(const QString &name, int total);
    void handleDownloadProcess(const QString &msg, int total, int completed);
    void handleTaskOnTimer();
private:
    Ui::CMDDownloadDialog *ui;
};

#endif // MDDOWNLOADDIALOG_H
