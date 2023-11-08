/*!
\author enli 2015.01.08
\section 选择变量对话框，用于中间变量+tang：selectVariables
*/
#ifndef SELECTVARIABLEWIDGET_H
#define SELECTVARIABLEWIDGET_H
#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QTreeView>

#include "IMDVariable.h"
#include "IMDTreeModel.h"
#include "IMDRTDBManager.h"

#if defined(MDSELECTVARIABLEWIDGETEXPERT_LIBRARY)
#  define MDSELECTVARIABLEWIDGETEXPERT_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDSELECTVARIABLEWIDGETEXPERT_SHARED_EXPORT Q_DECL_IMPORT
#endif

namespace Ui {
class CMDSelectVariableWidget;
}

class MDSELECTVARIABLEWIDGETEXPERT_SHARED_EXPORT CMDSelectVariableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CMDSelectVariableWidget(QWidget *parent = 0);
    ~CMDSelectVariableWidget();

private:
    Ui::CMDSelectVariableWidget *ui;
public:
    void setRTDBManager(IMDRTDBManager*nsRTDBManager);
    //!获取选中变量链表
    bool getSelectedVariables(QList<IMDVariable*>&varList);

signals:
    void selectVariableName(const QString& retStr);
private:
    void doubleClickTree(const QModelIndex &index);

private:
    IMDRTDBManager *m_rtdbManager;
    IMDTreeModel *m_variableTreeModel;
    QHBoxLayout *m_mainLayout;
    QTreeView *m_treeView;
};

#endif // SELECTVARIABLEWIDGET_H
