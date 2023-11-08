#ifndef MDDATAARCHIVESTABLEWIDGET_H
#define MDDATAARCHIVESTABLEWIDGET_H

/*!

\author bHVvY2hhbw==

\section 功能
1.数据归档表格界面。

*/

#define TAB_COL_ZERO                            0
#define TAB_COL_FIRST                           1
#define TAB_COL_SECOND                          2

#include <QTableWidget>
#include "IMDDataType.h"

class IMDDataGroup;
class IMDDataItem;

class CMDDataArchivesTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit CMDDataArchivesTableWidget(QWidget *parent = 0);
    ~CMDDataArchivesTableWidget();

    void initData(IMDDataGroup *pDataGrp); //! 初始化属性
    void setHorizontalHeaderText(const QStringList &titleList); //! 设置水平表头文本

signals:

public slots:

private:
    QString dataTypeToString(MDDataType type); //! 将枚举类型转换成字符串

private:
    int m_iRow; //! 行数
};

#endif // MDDATAARCHIVESTABLEWIDGET_H
