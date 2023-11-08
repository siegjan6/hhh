#ifndef MDREGISTERANDADDRESSCONFIGTAB_H
#define MDREGISTERANDADDRESSCONFIGTAB_H

#include <QDialog>
#include <QEvent>
#include <QKeyEvent>
#include <QComboBox>
#include <QLineEdit>
class CMDDataBlock;
namespace Ui {
class CMDRegisterAndAddressConfigTab;
}

class CMDRegisterAndAddressConfigTab : public QDialog
{
    Q_OBJECT

public:
    explicit CMDRegisterAndAddressConfigTab(QWidget *parent = 0);
    ~CMDRegisterAndAddressConfigTab();

private:
    Ui::CMDRegisterAndAddressConfigTab *ui;
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

#endif // MDREGISTERANDADDRESSCONFIGTAB_H
