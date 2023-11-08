/*!
\author emhhbmdsZQ==
\section  配置主界面，在该界面上完成对仪表的配置。
*/

#ifndef DESIGNDIALOG_H
#define DESIGNDIALOG_H
#include <QDialog>
#include <QTreeWidget>
#include "DrawData.h"

class CDrawGauge;
namespace Ui {
class CDesignDialog;
}

class CDesignDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CDesignDialog(QWidget *parent = 0);
    ~CDesignDialog();

private:
    Ui::CDesignDialog *ui;

public:
    QList<CDataLayer*> m_layers;   //!  控制层列表
    CDrawGauge *m_gauge;   //!  表盘指针
private:
    QRect m_gaugeRect ;   //!  绘制所有控制层所有仪表的区域
    QRect m_itemRect;    //!  绘制仪表的单一部件的区域
    CData* m_itemData;
    QDialog* m_itemDialog;      //! 仪表各组件配置界面的基类指针

    QStringList m_names;     //!  组件名称列表    
    QList<int>m_layerIndexList;//!  控制层索引

public:
    void onInitDialog();

private:
    QTreeWidgetItem* createTreeItem(CData* value);   //!创建树
    CData* createData(drawType type);
    void getDataItem(QTreeWidgetItem *item);
    void addItem(drawType type);

public:
    void paintEvent(QPaintEvent*);    //!在配置界面上绘制当前配置的仪表
private slots:
     void on_addBtn_clicked();      //!  增加仪表某一组件
     void on_deleteBtn_clicked();  //!   删除仪表某一组件
     void on_onOkBtn_clicked();    //!  配置仪表完成确认按钮
     void on_cancleBtn_clicked();  //!   取消仪表配置
     void on_upBtn_clicked();       //!  将仪表部件对应树的节点进行上移
     void on_downBtn_clicked();   //!  将仪表部件对应树的节点进行下移
     void on_applyBtn_clicked();

};

#endif // DESIGNDIALOG_H
