/*!
author Y2hvdWps 2016.10.19
section 数值映射表编辑对话框中的tableWidget使用的代理类
详细说明
主要用于绘制单元格填充内容无效时候的提示内容
*/
#ifndef VALUEMAPDELEGATE_H
#define VALUEMAPDELEGATE_H

#include <QStyledItemDelegate>
#include <QTableView>

class CValueMapDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CValueMapDelegate(QTableView *parent, QString placeholderText = "", QString validatorText = "");
public:
    enum itemtype{
        value = 0,
        desc,
        image
    };

protected:
    QString m_validatorText;
    QString m_placeholderText;
    QTableView* m_parent;

protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool isRowValid(const QModelIndex &index) const;
    void setValid(bool value);
signals:

public slots:

};

#endif // VALUEMAPDELEGATE_H
