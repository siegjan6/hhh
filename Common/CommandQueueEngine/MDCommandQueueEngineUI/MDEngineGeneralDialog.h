#ifndef MDENGINEGENERALDIALOG_H
#define MDENGINEGENERALDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QDateTime>

namespace Ui {
class CMDEngineGeneralDialog;
}

class CMDCommandQueueEngine;
class CMDEngineGeneralDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CMDEngineGeneralDialog(QWidget *parent = 0);
    ~CMDEngineGeneralDialog();
public:
    void init(CMDCommandQueueEngine* engine);
protected:
    CMDCommandQueueEngine* m_engine;
    qint64 m_totalProcessed;
    qint64 m_totalSubmit;
public:
    QTimer m_updateTimer;
    void startUpdate();
    void stopUpdate();
private slots:
    void onUpdateTimedOut();

private:
    Ui::CMDEngineGeneralDialog *ui;
};

#endif // MDENGINEGENERALDIALOG_H
