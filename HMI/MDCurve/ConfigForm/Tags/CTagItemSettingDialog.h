/*!

\author eHVr

\section 功能
    历史特征标签属性设置界面
*/
#ifndef CTAGITEMSETTINGDIALOG_H
#define CTAGITEMSETTINGDIALOG_H

#include <QDialog>

class IMDRTDBManagerUI;
class CFeatureTagItem;
class ITranslateWidgetTool;
namespace Ui {
class CTagItemSettingDialog;
}

class CTagItemSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CTagItemSettingDialog(QWidget *parent = 0);
    ~CTagItemSettingDialog();

public:
    //!静态函数.获取改变后的特征标签指针.
    static CFeatureTagItem *getItem(bool *ok, CFeatureTagItem *initial, QWidget *parent = 0);

    void setData(CFeatureTagItem *item);
    void setVariantForm(IMDRTDBManagerUI *variantForm);
    void setFields(const QStringList &fields);
/*!
\section 初始化
*/
private:
    void initialize();
    void initializeData();
    void initializeUI();
private:
    void applyChanges(); //!  应用修改

/*!
\section 信号槽
*/
private:
    void createConnections();
    void breakConnections();
private slots:
    void slotOkClicked();
    void slotCancelClicked();
    void slotSetFont();

private:
    Ui::CTagItemSettingDialog *ui;

    QFont                  m_font;         //! 特征标签字体
    CFeatureTagItem       *m_item;        //! 特征标签数据指针
    IMDRTDBManagerUI      *m_variantForm;
    QStringList            m_fields;
private:
    ITranslateWidgetTool  *m_transTool;
};

#endif // CTAGITEMSETTINGDIALOG_H
