/****************************************************************************
*
*  Copyright (C) 2002-2014 Helta Kft. - NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: norbert@nocisoft.com, office@nocisoft.com
*  Web: www.nocisoft.com
*
*  Created: 2014.01.21. (nocisoft)
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#ifndef NCRDGRAPHITEMTESTDIALOG_H
#define NCRDGRAPHITEMTESTDIALOG_H

#include <QDialog>

#include "ncrdgraphitemdialogfactory.h"
#include "ncreportgraphrenderer.h"
#include "ncrdwidgetmapper.h"
#include "ncreportgraphitem.h"
#include "ncreportuserfunction.h"

namespace Ui {
class NCRDGraphItemTestDialog;
}

/*!

\section 功能

报表图表控件测试对话框类
＃＃＃在代码中暂时屏蔽了此项功能＃＃＃

*/
class NCREPORTDESIGNER_SHARED_EXPORT NCRDGraphItemTestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NCRDGraphItemTestDialog(QWidget *parent = 0);
    ~NCRDGraphItemTestDialog();

private:
    Ui::NCRDGraphItemTestDialog *ui;
};

/*!

\section 功能

派生自报表图表控件管理对话框工厂类

*/
class NCREPORTDESIGNER_SHARED_EXPORT NCRDTestDialogFactory : public NCRDGraphItemDialogFactory
{
public:
    NCRDTestDialogFactory() {}

    QDialog* createDialog(NCReportItem *item, QWidget *parent);
};

class NCRDTestGraphRenderer : public NCReportGraphRenderer
{
public:
    NCRDTestGraphRenderer()  {}
    ~NCRDTestGraphRenderer() {}

    void paintItem( QPainter* painter, NCReportOutput* output, const QRectF& rect, const QString& itemdata );
    void paintEditorItem(QPainter* painter, const QRectF& rect , const QString &text);

private:
    void paintGraph(QPainter* painter, const QRectF& rect, const QString &text);
};

class NCRDTestFunction : public NCReportUserFunction
{
public:
    NCRDTestFunction() {}
    ~NCRDTestFunction() {}

    void updateValue(NCReportItem *item, NCReportOutput *output, NCReportEvaluator *evaluator);



};

#endif // NCRDGRAPHITEMTESTDIALOG_H
