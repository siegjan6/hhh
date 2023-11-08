/*!
author Y2hvdWps 2016.08.29
section 配方表格代理类
*/
#ifndef MDRECIPEGRIDDELEGATE_H
#define MDRECIPEGRIDDELEGATE_H

#include <QStyledItemDelegate>
#include <QSignalMapper>

class CGridBaseItem;
class CRecipeGridView;

class CRecipeGridDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CRecipeGridDelegate(QObject *parent = 0);
    virtual ~CRecipeGridDelegate(){}

    bool highLight() const {return m_highLight;}
    void setHighLight(bool highLight) {m_highLight = highLight;}
protected:
    virtual QWidget *createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void initStyleOption(QStyleOptionViewItem * option, const QModelIndex & index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
    void drawFrame(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    CRecipeGridView* m_mainView;
    bool m_highLight;
    QWidget* m_clickedEditor;   //! 记录被激活/点中的控件,用于判定脚本事件onEditFinish
signals:

public slots:
    /*!
    section 连接特殊型editor的提交数据的信号,在通过发送自身的commitData信号触发数据提交过程
    */
    void commitDataByComboBox(int);
    void commitDataByCheckBox(int);
    void commitDataByRadioButton(int);
    /*!
    section 用于判定脚本事件onEditFinish
    */
};

#endif // MDRECIPEGRIDDELEGATE_H
