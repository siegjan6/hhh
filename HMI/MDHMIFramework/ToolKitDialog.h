/*!
\author aGRw 2015.12.02
\section 工具箱窗体
*/
#ifndef TOOLKITDIALOG_H
#define TOOLKITDIALOG_H

#include <QWidget>
#include <QTreeWidgetItem>
#include "DrawObjDefine.h"
#include "MDMultiLanguage.h"

class CHMIStudio;

namespace Ui {
class CToolKitDialog;
}

class CToolKitDialog : public QWidget
{
    Q_OBJECT
public:
    explicit CToolKitDialog(QWidget *parent = 0);
    ~CToolKitDialog();
private:
    Ui::CToolKitDialog *ui;
private:
    void currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
public:
    void init(CHMIStudio* container);
    //!当前项目回到指针
    void resetToolKit();
private:
    CHMIStudio* m_container;
    //!是否是鼠标操作状态
    bool m_isMouseOperate;
    //!不要置位到指针
    bool m_notReset;
/*!
\section language
*/
public:
    void changeLanguage();
private:
    QList<QStringList*> m_itemTexts;
    ITranslateWidgetTool* m_transTool;
};

#endif // TOOLKITDIALOG_H
