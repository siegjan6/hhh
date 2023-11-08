/*!

\author eHVr

\section 功能
    实时标签设置窗体
*/
#ifndef CREALTAGSETTINGDIALOG_H
#define CREALTAGSETTINGDIALOG_H

#include <QDialog>
#include <QList>
#include <QMessageBox>
#include <QMenu>

class CRealFTag;
class CFeatureTag;
class CFeatureTagItem;

class QTableWidgetItem;
class IMDRTDBManagerUI;
class ITranslateWidgetTool;
class CXAxis;
class QComboBox;

namespace Ui {
class CRealTagSettingDialog;
}

class CRealTagSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CRealTagSettingDialog(QWidget *parent = 0);
    ~CRealTagSettingDialog();
private:
    Ui::CRealTagSettingDialog *ui;

public:
    void setData(CFeatureTag *tag, const QList<CXAxis *> &xList);
    void setData(CRealFTag *tag, const QList<CXAxis *> &xList);
    void setVariantForm(IMDRTDBManagerUI *variantForm);//! 变量选择UI
    void setFields(QStringList fields);

    void initialize();
    void applyChanges();

/*!
\section 信号槽
*/
private:
    void createConnections();
    void breakConnections();
private slots:
    void slotAddItem();
    void slotDeleteItem();
    void slotMoveUpRow();                       //! 选中行上移
    void slotMoveDownRow();                     //! 选中行下移
    void slotSetItemProperty(QTableWidgetItem*);//! 设置选中行
    void slotSetItemVisible(QTableWidgetItem *item);
    void slotSetTimeVariant();
    void slotCustomMenu(const QPoint &p);

private:
    void parseItemsInformation();                     //! 解析
    void swapTableItem(int preIndex,int currentIndex);//! 交换选中项
    QTableWidgetItem *createNameItem(const QString &name);
    QTableWidgetItem *createVisibleItem(bool visible);
    QTableWidgetItem *createCommentItem(const QString &comment);
    QTableWidgetItem *createFormatItem(const QString &format);
    void insertTableItem(int row,int column,QTableWidgetItem* item);

    int showMessage(const QString & comment,
           QMessageBox::StandardButtons buttons = QMessageBox::Ok);//! Tips

    void initializeAxisComboBox(QList<CXAxis *> axisList, QComboBox *comboBox);

private:
    CRealFTag               *m_tag;
    CFeatureTag             *m_historyTag;
    QStringList              m_fields;

    QList<CFeatureTagItem*>  m_itemList;
    IMDRTDBManagerUI        *m_variantForm;
    QList<CXAxis *>          m_xList;
    QMenu                   *m_menu;
private:
    ITranslateWidgetTool    *m_transTool; //! 多语言


private:
    void createMenu();
    bool isTagRepeat(const QString &str);
};

#endif // CREALTAGSETTINGDIALOG_H
