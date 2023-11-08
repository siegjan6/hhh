#ifndef CFEATURETAGCONFIGDIALOG_H
#define CFEATURETAGCONFIGDIALOG_H

#include <QDialog>
#include "CTagPosition.h"
class QListWidgetItem;
class CFeatureTagPropertySettingDialog;
class CRealTagSettingDialog;
class CRealFTag;
class CFeatureTag;
class IMDRTDBManagerUI;
class CXAxis;
namespace Ui {
class CFeatureTagConfigDialog;
}

class CFeatureTagConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CFeatureTagConfigDialog(QWidget *parent = 0);
    ~CFeatureTagConfigDialog();

private:
    Ui::CFeatureTagConfigDialog *ui;

private:
    CFeatureTagPropertySettingDialog    *m_tagSetttingDialog;
    CRealTagSettingDialog               *m_itemPropertyDialog;

    enum class dialogType
    {
       itemPropertyDialog = 0,
       tagSetttingDialog
    };
public:
    void setTitle(const QString &title);
    void setData(CFeatureTag *featureTag, const CTagPos &tagPos);
    void setData(CRealFTag *realTag, const CTagPos &tagPos);

    CTagPos tagPos() const;
    void tagSettingInitialize();
    void applyChanges();

public:
    void setData(CFeatureTag *tag, const QList<CXAxis *> &xList);
    void setData(CRealFTag *tag, const QList<CXAxis *> &xList);
    void setVariantForm(IMDRTDBManagerUI *variantForm);//! 变量选择UI
    void setFields(QStringList fields);

/*!
\section 初始化
*/
private:
    void initialize();
    void addItems();
    void setItemsData(); //! 设置每个项的数据

/*!
\section 信号槽
*/
private:
    void createConnections();
    void breakConnections();
private slots:
    void slotShowWidget(QListWidgetItem *item);
    void slotConfirmButtonClicked();
    void slotCancelButtonClicked();

};

#endif // CFEATURETAGCONFIGDIALOG_H
