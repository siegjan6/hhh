/*!

\author eHVr

\section 功能

    数据变量设置对话框。
    针对历史曲线的字段所设计。
    使用：当历史曲线配置绑定字段时调用
*/
#ifndef CVARIANTDIALOG_H
#define CVARIANTDIALOG_H

#include <QDialog>
#include <QListWidget>

namespace Ui {
class CVariantDialog;
}
class QListWidgetItem;
class ITranslateWidgetTool;
class CVariantDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CVariantDialog(QWidget *parent = 0);
    ~CVariantDialog();

public:

    /*!
     \brief
        调用此函数即可获取选定的字段
     \param ok      状态
     \param initial 初始化字段
     \param fields  字段链表
     \param parent  父窗体
     \return 选中的字段字符串
    */
    static QStringList getField(bool *ok , QString &initial, const QStringList &fields, QWidget *parent = 0);
    static QStringList getField(bool *ok,const QStringList &fields, QWidget *parent = 0);
public:
    QStringList fields() const;
    void setFields(const QStringList &fields);

    QStringList selectField();
    void setSelectField(const QString &selectField);

    void initialize();

private:
    void addItems(const QStringList &itemNames);
    QListWidgetItem *createListItem(const QString &name);
    int findItem(const QListWidget &widget, const QString &text);
private slots:
    void slotUnchecked();
    void slotCheckAll();
private:
    Ui::CVariantDialog *ui;

    QStringList          m_fields;
    QString              m_selectField;
private:
    ITranslateWidgetTool *m_translatorTool;
};

#endif // CVARIANTDIALOG_H
