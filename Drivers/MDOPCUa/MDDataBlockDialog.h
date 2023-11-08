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
};

#endif // MDDATABLOCKDIALOG_H
