#ifndef MDDATABLOCKDIALOG_H
#define MDDATABLOCKDIALOG_H

#include <QDialog>
class CMDDataBlock;
namespace Ui {
class CMDDataBlockDialog;
}

class CMDDataBlockDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CMDDataBlockDialog(QWidget *parent = 0);
    ~CMDDataBlockDialog();
signals:
    void refreshStartLableStatus(int);
private slots:
    void okButton_clicked();

    void cancelButton_clicked();
    /*!
      * \brief on_startAddressEdit_editingFinished:
          * 开始地址编辑结束相应的槽函数.
      */
    void startAddressEdit_editingFinished();
    /*!
      * \brief on_endAddressEdit_editingFinished:
          * 结束地址编辑结束相应的槽函数.
      */
    void endAddressEdit_editingFinished();
    /*!
      * \brief registerComBox_currentIndexChanged:
          * 寄存器变化槽函数.
      */
    void registerComBox_currentIndexChanged(int registerIndex);

private:
    Ui::CMDDataBlockDialog *ui;
public:
    /*!
     * \brief setDataBlock:
        * 设置ui配置.
     */
    void setDataBlock(CMDDataBlock *dataBlock);
private:
    CMDDataBlock *m_dataBlock;
    QString m_startAddress;
    QString m_endAddress;
};

#endif // MDDATABLOCKDIALOG_H
