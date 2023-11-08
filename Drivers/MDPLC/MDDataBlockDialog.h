#ifndef MDDATABLOCKDIALOG_H
#define MDDATABLOCKDIALOG_H

#include <QDialog>
#include <QEvent>
#include <QKeyEvent>
#include <QComboBox>
#include <QLineEdit>
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

    void tableWidget_cellDoubleClicked(int row, int column);

    void setColString(QString str);

    void addressEditFinish();

    void selectOtherItem();
//!事件
protected:
    bool eventFilter(QObject *,QEvent *);
    void mousePressEvent(QMouseEvent *);

private:
    Ui::CMDDataBlockDialog *ui;
public:
    /*!
     * \brief setDataBlock:
        * 设置ui配置.
     */
    void setDataBlock(CMDDataBlock *dataBlock);
    /*!
     * \brief refreshBytesCount:
        * 刷新bytesCount.
     */
    bool refreshBytesCount();
private:
    CMDDataBlock *m_dataBlock;
    int m_comboRow ;
    int m_comboCol ;

    QLineEdit *m_pLineEdit;

};

#endif // MDDATABLOCKDIALOG_H
