#ifndef MDVARINFORMATION_H
#define MDVARINFORMATION_H

#include <QDialog>

class CMDDataBlock;
namespace Ui {
class CMDVarInformation;
}

class CMDVarInformation : public QDialog
{
    Q_OBJECT

public:
    explicit CMDVarInformation(QWidget *parent = 0);
    ~CMDVarInformation();

private:
    Ui::CMDVarInformation *ui;
private slots:
    void okButton_clicked();

   void cancelButton_clicked();
   /*!
     * \brief refreshDataBlock:
         * timer超时触发当前槽函数,当获取配置成功时,显示数据表号对应的变量属性.
     */
    void  refreshDataBlock();
    /*!
     * \brief on_dataTabNumberEdit_editingFinished:
        * 数据表号变化时,触发当前草函数,并显示数据表号对应的变量属性.
     */
    void dataTabNumberEdit_editingFinished();
    /*!
     * \brief on_refreshVarTabButton_clicked:
        * 当plc中变量有变化, 单击刷新按钮时,
        * 触发当前槽函数,重新获取变量属性,
        * 并刷新变量属性表,显示数据表号对应的变量属性.
     */
    void refreshVarTabButton_clicked();
public:
    /*!
     * \brief setDataBlock:
        * 设置ui配置.
     */
    void setDataBlock(CMDDataBlock *dataBlock);
    /*!
     * \brief refreshVarTab:
        * 根据数据表号,属性变量属性表.
     * \param currentDataTableNO:
        * 当前数据表号.
     */
    void refreshVarTab(qint16 currentDataTableNO);
    /*!
     * \brief varTypeReflect:
        * 根据plc返回的变量类型翻译成对应的字符串.
     */
    QString varTypeReflect(qint16 vartype);

private:
    CMDDataBlock *m_dataBlock;
    QTimer     *m_pTimer ;
};

#endif // MDVARINFORMATION_H
