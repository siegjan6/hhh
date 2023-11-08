/****************************************************************************
*
*  Copyright (C) 2002-2014 Helta Kft. - NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: norbert@nocisoft.com, office@nocisoft.com
*  Web: www.nocisoft.com
*
*  Created: 2014. 08. 25. (nszabo)
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#ifndef NCRDEXPRESSIONBUILDERDIALOG_H
#define NCRDEXPRESSIONBUILDERDIALOG_H

#include <QDialog>

namespace Ui {
class NCRDExpressionBuilderDialog;
}

class NCReportItem;
class NCRDDocument;

QT_BEGIN_NAMESPACE
class QTreeWidgetItem;
class QButtonGroup;
class QAbstractButton;
QT_END_NAMESPACE

/*!

\section 功能

表达式创建器对话框管理类
【用于创建表达式，如所要输出呈现的数据源中相关字段／系统变量等】。

*/
class NCRDExpressionBuilderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NCRDExpressionBuilderDialog(NCRDDocument *document, const QList<int>& validTypes, int sourceType, QWidget *parent = 0);
    ~NCRDExpressionBuilderDialog();

    QString expression() const;
    void setExpression( const QString& exp );
    void setSourceType( int stype );
    int sourceType() const;

protected slots:
    void updateDataSourceColumns();
    void updateSourceType();

    void addDataSource();
    void addDataSourceColumn();
    void addDataSourceFunction();
    void addValueFunction();
    void addVariable();
    void addParameter();
    void addTreeItem(QTreeWidgetItem *item, int column);

    void onButtonClicked(QAbstractButton* btn);

/*
    void buttonAnd();
    void buttonOr();
    void buttonNot();
    void buttonXor();
    void buttonPlus();
    void buttonMinus();
    void buttonDiv();
    void buttonMulti();
    void buttonEqual();
    void buttonLessThan();
    void buttonGreatherThan();
    void buttonLessEqual();
    void buttonGreatEqual();
    void button();
    void button();
*/

protected:
    bool isComplexExpression() const;

private:
    Ui::NCRDExpressionBuilderDialog *ui;
    /*NCReportItem* m_item;*/
    NCRDDocument* m_document;
    QButtonGroup* m_bg;

    void addSymbol( const QString& exp, int dstype);
};

#endif // NCRDEXPRESSIONBUILDERDIALOG_H
