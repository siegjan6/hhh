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

private:
    Ui::CMDDataBlockDialog *ui;
public:
    /*!
     * \brief setDataBlock:
        * 设置ui配置.
     */
    void setDataBlock(CMDDataBlock *dataBlock);
private:
    /*!
     * \brief promptMessageBox:
        * UI设置时提示信息.
     */
    void promptMessageBox(const QString &title,const QString &details);
private:
    CMDDataBlock *m_dataBlock;
    qint16 m_startAddress;
    qint16 m_endAddress;
};

#endif // MDDATABLOCKDIALOG_H
