#ifndef CFILTERDIALOG_H
#define CFILTERDIALOG_H

#include <QDialog>
#include <QMap>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QStyledItemDelegate>
#include <QLabel>
#include <QPlainTextEdit>
#include <QLayout>

class QTableWidgetItem;
class CMDFilterTreeDataItem;
class CMDFilterTreeDataManager;
namespace Ui {
class CFilterDialog;
}

class CFilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CFilterDialog(QWidget *parent = 0);
    ~CFilterDialog();

    QMap<QString, QStringList> dataInfor() const;
    void setDataInfor(const QMap<QString, QStringList> &dataInfor);

    void setFilterPluginManager(CMDFilterTreeDataManager *manager);

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();
    void on_applyAllButton_clicked();
private:
    QTableWidgetItem *createTableItem(const QString &text) const;

    enum class TableColumn{
        Name = 0,
        Value,
        Tip
    };

    QWidget *createTab(const QList<CMDFilterTreeDataItem *> &items, const QString &title) const;
    void setTableInfor(const QMap<QString,QStringList> &datas);
    void setApplyButtonStatus();
    QString itemName(CMDFilterTreeDataItem *) const;
private:
    Ui::CFilterDialog *ui;
    QMap<QString,QStringList> m_dataInfor;
    CMDFilterTreeDataManager *m_filterManager;
    QMap<QString ,QList<CMDFilterTreeDataItem *>> m_filters;
};


class ItemDelegate: public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ItemDelegate(QObject *parent = 0)
        : QStyledItemDelegate(parent)
    {
    }
private:
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const;
};

class CTableWidget: public QTableWidget
{
    Q_OBJECT
public:
    explicit CTableWidget(QWidget *parent = 0);

    virtual ~CTableWidget(){};
};
#endif // CFILTERDIALOG_H
