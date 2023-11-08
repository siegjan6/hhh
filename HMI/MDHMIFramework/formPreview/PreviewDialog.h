/*!
\author aGRw 2016.03.14
\section 图形窗体预览，对话框
*/
#ifndef PREVIEWDIALOG_H
#define PREVIEWDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "PreviewNode.h"
#include "HMIFramework.h"
#include "FormObjectContainer.h"
#include "PreviewPropertyWidget.h"

namespace Ui {
class CPreviewDialog;
}

class CPreviewDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CPreviewDialog(QWidget *parent = 0);
    virtual ~CPreviewDialog();
private:
    Ui::CPreviewDialog *ui;
/*!
\section field
*/
private:
    CPreviewNode* m_node;//!文件管理节点
    CHMIFramework* m_framework;
    CFormObjectContainer m_objectWidget;//!窗体对象管理控件
    CPreviewPropertyWidget* m_property;//属性框
/*!
\section public function
*/
public:
    void init(CHMIFramework* framework);
    void loadForm(const QString& fullName);
    void saveForm(CFormObject *obj);
/*!
\section event
*/
protected:
    bool eventFilter(QObject *o, QEvent *e);
    void closeEvent(QCloseEvent *e);
/*!
\section test
*/
public:
    CFormObjectContainer* objectWidget(){return &m_objectWidget;}
    CPreviewPropertyWidget* property(){return m_property;}
};

#endif // PREVIEWDIALOG_H
