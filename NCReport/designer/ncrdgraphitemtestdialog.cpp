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

#include "ncrdgraphitemtestdialog.h"
#include "ui_ncrdgraphitemtestdialog.h"
#include "ncreportgraphitem.h"
#include "ncreportdef.h"
#include "ncreportgroup.h"
#include "ncreportdatasource.h"
#include <QPainter>

//! 多语言
#include "MDMultiLanguage.h"

NCRDGraphItemTestDialog::NCRDGraphItemTestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NCRDGraphItemTestDialog)
{
    ui->setupUi(this);

    //! 多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

NCRDGraphItemTestDialog::~NCRDGraphItemTestDialog()
{
    delete ui;
}


void NCRDTestGraphRenderer::paintItem(QPainter *painter, NCReportOutput *output, const QRectF &rect, const QString &itemdata)
{
    qDebug("NCRDTestGraphRenderer::paintItem(%p,%p,rect(%f,%f,%f,%f),'%s')",painter,output,rect.x(),rect.y(),rect.width(),rect.height(),qPrintable(itemdata));
    paintGraph(painter,rect,itemdata);
}

void NCRDTestGraphRenderer::paintEditorItem(QPainter *painter, const QRectF &rect, const QString &text)
{
    paintGraph(painter, rect, text);
}

void NCRDTestGraphRenderer::paintGraph(QPainter *painter, const QRectF &rect, const QString &text)
{
    qDebug("NCRDTestGraphRenderer::paintMyself...");

    int color1 = item()->parameter("color1").toString().toInt(0,16);
    int color2 = item()->parameter("color2").toString().toInt(0,16);
    int color3 = item()->parameter("color3").toString().toInt(0,16);
    int numcols = item()->parameter("numCols").toInt();

    QString firstValue;
    QString lastValue;
    if (item()->section() && item()->section()->relatedGroup()) {
        // THIS IS AVAILABLE IN RUNTIME ONLY
        firstValue = item()->section()->relatedGroup()->lastMetaInfo().firstValue;
        lastValue = item()->section()->relatedGroup()->lastMetaInfo().lastValue;
    }

    int number = 0;

    if (item()->reportDef()->variable("myVar1"))
        number = item()->reportDef()->variable("myVar1")->value().toInt();


    NCReportDataSource * ds = item()->reportDef()->dataSources().value("D2");
    if (ds && ds->isOpened()) {
//        while (ds->next()) {
            color1 = ds->value(0).toInt();
//        }
    }

    if (color1 == 0)
        color1 = 0x2B65EC;
    if (color2 == 0)
        color2 = 0x8D38C9;
    if (color3 == 0)
        color3 = 0x659EC7;

    if (numcols == 0)
        numcols = 6;

//    switch (item()->typeId()) {
//    case 750:
//    {
//        color1 = 0x2B65EC;
//        color2 = 0x8D38C9;
//        color3 = 0x659EC7;
//        numcols = 3;
//        break;
//    }
//    case 751:
//    {
//        color1 = 0xC25A7C;
//        color2 = 0x7E2217;
//        color3 = 0xE56717;
//        numcols = 6;
//        break;
//    }
//    }

    //item()->reportDef()->details().value(groupSections()

    const int cw = qRound(rect.width()/numcols);
    painter->setPen( Qt::NoPen );
    int ch=0;
    QColor color;
    color.setAlpha( 128 );
    for ( int i=0; i<numcols; ++i ) {
        if ( i%3 == 0 ) {
            color.setRgb(color1);
            ch = qRound(rect.height()*0.8);
        } else if ( i%2 == 0 ) {
            color.setRgb(color2);
            ch = qRound(rect.height()*0.4);
        } else {
            color.setRgb(color3);
            ch = qRound(rect.height()*0.6);
        }
        painter->setBrush( QBrush(color) );
        painter->drawRect( rect.x()+i*cw, rect.y()+qRound(rect.height())-ch , cw, ch );
    }

    painter->setPen( QPen(Qt::black) );
    painter->setBrush( Qt::NoBrush );

    painter->drawRect( rect );

    painter->setFont( QFont("Arial",8) );
    painter->drawText( rect, Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextWordWrap, text+QString("MyVar1: %1").arg(number) );

}


QDialog *NCRDTestDialogFactory::createDialog(NCReportItem *item, QWidget *parent)
{
    QDialog *dialog = new NCRDGraphItemTestDialog(parent);
    dialog->setWindowTitle( QString("Dialog Type: %1").arg(item->typeId()));

    switch (item->typeId()) {
    case 750:
    case 751:
        return dialog;
    default:
    {
        NCReportGraphItem *g = (NCReportGraphItem*)item;
        if (g->classID()=="GT01")
            return 0;
        else
            return new NCRDGraphItemTestDialog(parent);

    }
    }
}

void NCRDTestFunction::updateValue(NCReportItem *item, NCReportOutput *output, NCReportEvaluator *evaluator)
{
    Q_UNUSED(output)
    Q_UNUSED(evaluator)

    //item->setDisplayValue("This is a user coded value!");
    item->setText("This is a user coded value!");
}
