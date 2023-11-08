#ifndef MDENGINEMGRDIALOG_H
#define MDENGINEMGRDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QStackedWidget>
#include <QListWidget>

class ITranslateWidgetTool;
class CMDEngineMgrDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CMDEngineMgrDialog(QWidget *parent = 0);
    ~CMDEngineMgrDialog();
    void initialUI();
    void showUI();
    void closeUI();
public:
    QListWidget *m_listWidget ;
    QStackedWidget *m_stackWidget ;
public:
    QPushButton *closeButton ;
public:
    ITranslateWidgetTool* m_twTool;

    void updateEngines();

/*!
\section 处理对 OK 和 Cancel 按钮的点击
*/
private slots:
    void on_closeButton_clicked();
    void on_engine_changed(int index);

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MDENGINEMGRDIALOG_H
