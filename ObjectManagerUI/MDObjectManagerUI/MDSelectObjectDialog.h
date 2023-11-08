/*!

\author dGFuZ3p3

\section 功能

选择对象对话类。

*/

#ifndef MDSELECTOBJECTDIALOG_H
#define MDSELECTOBJECTDIALOG_H

#include <QDialog>

namespace Ui {
class CMDSelectObjectDialog;
}

class CMDObjectTreeModel;
class CMDObjectTreeWindow;
class CMDSelectObjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CMDSelectObjectDialog(QWidget *parent = 0);
    ~CMDSelectObjectDialog();

public:
    void init(const QString& title, CMDObjectTreeWindow *treeWindow, const QStringList& sourceList, bool addObj);
    bool selectedObjects_Name(QStringList& objectNameList, qint16 objectType);
    bool selectedObjects_Ptr(QList<void*>& objectList, qint16 objectType);
private slots:
    void on_OKButton_clicked();
    void on_CancelButton_clicked();

    void on_AddObjButton_clicked();

private:
    CMDObjectTreeModel *m_treeModel;
    CMDObjectTreeWindow *m_objectTreeWindow;
private:
    Ui::CMDSelectObjectDialog *ui;
};

#endif // MDSELECTOBJECTDIALOG_H
