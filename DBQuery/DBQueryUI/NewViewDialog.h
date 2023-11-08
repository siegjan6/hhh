/*!

\author bWVpeGw=

\section 功能

管理【数据查询模块，新建视图对话框管理类，
    用来创建视图】。

*/

#ifndef NEWVIEWDIALOG_H
#define NEWVIEWDIALOG_H

#include <QDialog>
#include <MDDBQueryConfigure.h>
#include "MDProjectConfiger.h"
#include "IStudioInterface.h"

namespace Ui {
class NewViewDialog;
}

class NewViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewViewDialog(QString title,QWidget *parent = 0);
    ~NewViewDialog();

public:
    QString getViewName();  //! 获取视图变量名称

    DBQ::View *getView() const;
    void setView(DBQ::View *view);

    IStudioInterface *studioInterface() const;
    void setStudioInterface(IStudioInterface *studioInterface);

private slots:
    //! 确认/取消操作
    void on_pbt_Confirm_clicked();
    void on_pbt_Cancel_clicked();

private:
private:
    /*!
     判断当前文件目录下是否含有与所要新建的视图文件名相同的文件

     \param    1. QString xmlName 需要新建的视图文件名
               2. QString Path  报表文件的存储目录
     \returns   bool
     */
    bool hascurrentXMLFile(QString xmlName,QString Path);

private:
    Ui::NewViewDialog *ui;
    QString m_windowTitle;
    QString m_viewName;     //! 数据查询视图名称
    DBQ::View *m_view;      //! 数据查询视图指针
    IStudioInterface* m_studioInterface;
    bool isexists_currentfile;
};

#endif // NEWVIEWDIALOG_H
