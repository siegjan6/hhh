/*!
author Y2hvdWps 2016.12.20
section 编辑器模态窗口
详细说明:
exec()函数返回QDialog::Accepted时候content是最新值.
如果返回QDialog::Rejected，代表放弃修改。content不变.
exec()只可能有这两个返回值.
*/
#ifndef EDITORDIALOG_H
#define EDITORDIALOG_H

#include <QDialog>
#include "WebContainer.h"


namespace Ui {
class CEditorDialog;
}

class CEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CEditorDialog(QString content, const QList< QPair<QString, QString> >& objs, QWidget *parent = 0);
    ~CEditorDialog();

    QString m_content;
protected:
    void showEvent(QShowEvent *e);
    virtual void done(int res);

private:
    Ui::CEditorDialog *ui;
    CWebContainer* m_webview;
    bool m_modify;
    bool m_willQuit;
    QString m_srcContent;
signals:

public slots:
    void onSetModify(bool modify);
    void onSaveFileFinished();
};

#endif // EDITORDIALOG_H
