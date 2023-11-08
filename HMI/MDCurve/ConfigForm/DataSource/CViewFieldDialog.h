#ifndef CVIEWFIELDDIALOG_H
#define CVIEWFIELDDIALOG_H
#include "MDDBQueryUI.h"
//#include "MDDBQueryUI\MDDBQueryUI.h"

#include <QDialog>

namespace Ui {
class CViewFieldDialog;
}

class QTreeWidgetItem;
class CViewFieldDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CViewFieldDialog(QWidget *parent = 0);
    ~CViewFieldDialog();

private:
    Ui::CViewFieldDialog *ui;

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
    static QStringList getField(bool *ok , const QStringList &viewList, QMap<QString,QList<DBQ::ViewField *>>map, QWidget *parent = 0);

private:
    void setHeaderList(const QStringList &viewList);
    void setViewVariantMap(QMap<QString,QList<DBQ::ViewField *>>map);
    void initialize();
    QStringList selectField() const;

private:
   QString typeToString(DBQ::VIEW_FIELD_TYPE type);

private slots:
    void slotOk();
    void slotCancel();
    void slotSetChecked(QTreeWidgetItem *item, int column);


private:
    QStringList m_viewList;
    QMap<QString,QList<DBQ::ViewField *>>m_map;


};

#endif // CVIEWFIELDDIALOG_H
