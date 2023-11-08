/*!

\author eHVr

\section 功能
    工具设置界面，
    \a 放大，缩小，移动，选中等按钮的设置
    \a 按钮的排列顺序设置，是否可见
*/
#ifndef CTOOLSSETTINGFORM_H
#define CTOOLSSETTINGFORM_H

#include <QWidget>
#include <QMessageBox>

#include "CToolButton.h"

class CTools;
class QTableWidgetItem;
namespace Ui {
class CToolsSettingForm;
}

class CToolsSettingForm : public QWidget
{
    Q_OBJECT

public:
    explicit CToolsSettingForm(QWidget *parent = 0);
    ~CToolsSettingForm();
    void setData(CTools *tools);
    void applyChanges();
/*!
\section 窗体内容是否修改
*/
public:
    void resetModified();
private slots:
    void slotHandleModified();
signals:
    void itemModified();
/*!
\section  初始化窗体数据
*/
private:
    void initialize();
    void initializeData();
    void initializeUI();
private:
    void createConnections();
    void breakConnections();
private slots:
    void slotSetItemVisible(QTableWidgetItem*);
    void slotMoveUp();
    void slotMoveDown();
private:
     void parseButtons(const QButtonGroup &buttonGroup);
     void setToolButtonChanges(int rowIndex);
     QTableWidgetItem *createNameItem(const QString &name);
     QTableWidgetItem *createVisibleItem(bool visible);
     QTableWidgetItem *createIconItem(const QIcon &icon);
     void insertTableItem(int row,int column,QTableWidgetItem* item);

     int showMessage(const QString & comment,
            QMessageBox::StandardButtons buttons = QMessageBox::Ok);//! Tips
    void swapTableItem(int preIndex,int currentIndex);              //! 交换选中行信息
private:
    Ui::CToolsSettingForm *ui;

    bool   m_isModified;
    CTools *m_tools ;

};

#endif // CTOOLSSETTINGFORM_H
