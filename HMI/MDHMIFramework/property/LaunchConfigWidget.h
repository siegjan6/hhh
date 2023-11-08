/*!
\author aGRw 2015.08.27
\section 窗体启动配置
*/
#ifndef LAUNCHCONFIGWIDGET_H
#define LAUNCHCONFIGWIDGET_H

#include <QDialog>
#include <QListWidgetItem>
#include "FileNode.h"
#include "IHMIWidget.h"

class CHMIFramework;

namespace Ui {
class CLaunchConfigWidget;
}

class CLaunchConfigWidget : public QDialog, public IHMIWidget
{
    Q_OBJECT
public:
    explicit CLaunchConfigWidget(QWidget *parent = 0);
    virtual ~CLaunchConfigWidget();
private slots:
    void on_listWidgetSelected_itemChanged(QListWidgetItem *item);
    void on_treeWidgetForm_itemChanged(QTreeWidgetItem *item, int column);
    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();
private:
    Ui::CLaunchConfigWidget *ui;
//////////////////////////////////////////////////////////////////////////
private:
    QStringList m_selectForms;//!打开的窗体
    bool m_fullScreen;//!全屏
    bool m_canEvent;//!事件触发是否有效
    CHMIFramework* m_framework;
    CFileNode* m_file;
    bool m_isSelect;
//////////////////////////////////////////////////////////////////////////
public:
    //!确定
    bool onOk();
    //!启动窗体初始化
    void init(IHMIFramework* framework);
    //!选择窗体初始化
    void initSelect(IHMIFramework* framework, const QStringList& selectForms);
    QStringList selectForms(){return m_selectForms;}
private:
    void addToList(QTreeWidgetItem *node, const QString &path);
    void delFromList(QTreeWidgetItem *node);
    void initTree(CFileNode *file);
    void resetNode(QTreeWidgetItem* node);
    void init(IHMIFramework* framework, const QStringList &selectForms, bool isSelect = false);
    void saveData();
};

#endif // LAUNCHCONFIGWIDGET_H
