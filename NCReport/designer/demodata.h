/****************************************************************************
*
*  Copyright (C) 2002-2008 Helta Kft. / NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: nszabo@helta.hu, info@nocisoft.com
*  Web: www.nocisoft.com
*
*  This file is part of the NCReport reporting software
*
*  Licensees holding a valid NCReport License Agreement may use this
*  file in accordance with the rights, responsibilities, and obligations
*  contained therein. Please consult your licensing agreement or contact
*  nszabo@helta.hu if any conditions of this licensing are not clear
*  to you.
*
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/
#ifndef NCRD_DEMODATA_H
#define NCRD_DEMODATA_H

#include <QStringList>
#include <QStandardItemModel>

#ifdef NCRD_CUSTOM_DATASOURCE_DEMO

#include "../runner/testdatasource.h"

NCReportDataSource* createCustomTestData(QObject* parent)
{
    //-------------------------------------------
    // TEST CUSTOM DATASOURCE DEMO
    //-------------------------------------------
    TestDataSource *ds = new TestDataSource(parent);
    ds->setID("cds0");

    TestData d1;
    d1.id = 123;
    d1.name = "Alexander Henry";
    d1.address = "HOT SPRINGS VILLAGE, AR";
    d1.valid = true;
    d1.date = QDate(2008,01,10);
    ds->addData( d1 );

    TestData d2;
    d2.id = 157;
    d2.name = "Julius Coleman";
    d2.address = "Coronado, CA";
    d2.valid = false;
    d2.date = QDate(2008,01,12);
    ds->addData( d2 );

    TestData d3;
    d3.id = 157;
    d3.name = "Peter Moulding";
    d3.address = "San francisco, CA";
    d3.valid = true;
    d3.date = QDate(2008,01,07);

    ds->addData( d3 );

    return ds;
}
#endif

#ifdef NCRD_ITEMMODEL_DEMO

QAbstractItemModel* createDemoItemModel()
{
    //-------------------------------------------
    // TEST ITEM MODEL DATASOURCE
    //-------------------------------------------
    QStandardItemModel *model = new QStandardItemModel( 6, 4 );
    model->setHeaderData(0,Qt::Horizontal,"item_number",Qt::EditRole);
    //model->setHeaderData(0,Qt::Horizontal,"Item#",Qt::DisplayRole);

    qDebug("E:%s D:%s",qPrintable(model->headerData(0,Qt::Horizontal,Qt::EditRole).toString()), qPrintable(model->headerData(0,Qt::Horizontal,Qt::DisplayRole).toString()));

    model->setHeaderData(1,Qt::Horizontal,"description",Qt::EditRole);
    //model->setHeaderData(1,Qt::Horizontal,"Description",Qt::DisplayRole);
    model->setHeaderData(2,Qt::Horizontal,"qty",Qt::EditRole);
    //model->setHeaderData(2,Qt::Horizontal,"Quantity",Qt::DisplayRole);
    model->setHeaderData(3,Qt::Horizontal,"price",Qt::EditRole);
    //model->setHeaderData(3,Qt::Horizontal,"Unit Price",Qt::DisplayRole);

    QStandardItem *item =0;

    // -----------------------------------
    item = new QStandardItem();
    item->setData( 1, Qt::EditRole );
    model->setItem( 0, 0, item);

    item = new QStandardItem();
    item->setData( "Chai", Qt::EditRole );
    model->setItem( 0, 1, item);

    item = new QStandardItem();
    item->setData( 16.0, Qt::EditRole );
    model->setItem( 0, 2, item);

    item = new QStandardItem();
    item->setData( 1540.0, Qt::EditRole );
    model->setItem( 0, 3, item);

    // -----------------------------------
    item = new QStandardItem();
    item->setData( 1, Qt::EditRole );
    model->setItem( 1, 0, item);

    item = new QStandardItem();
    item->setData( "Chef Anton's Cajun Seasoning", Qt::EditRole );
    QFont f;
    f.setBold(true);
    f.setItalic(true);
    f.setUnderline(true);
    item->setData( f, Qt::FontRole );
    model->setItem( 1, 1, item);

    item = new QStandardItem();
    item->setData( 20.0, Qt::EditRole );
    item->setData( Qt::AlignCenter, Qt::TextAlignmentRole );
    model->setItem( 1, 2, item);

    item = new QStandardItem();
    item->setData( 1230.0, Qt::EditRole );
    model->setItem( 1, 3, item);

    // -----------------------------------
    item = new QStandardItem();
    item->setData( 1, Qt::EditRole );
    model->setItem( 2, 0, item);

    item = new QStandardItem();
    item->setData( "Grandma's Boysenberry Spread", Qt::EditRole );
    model->setItem( 2, 1, item);

    item = new QStandardItem();
    item->setData( 21.0, Qt::EditRole );
    model->setItem( 2, 2, item);

    item = new QStandardItem();
    item->setData( 520.2, Qt::EditRole );
    model->setItem( 2, 3, item);

    // -----------------------------------
    item = new QStandardItem();
    item->setData( 1, Qt::EditRole );
    model->setItem( 3, 0, item);

    item = new QStandardItem();
    item->setData( "Uncle Bob's Organic Dried Pears", Qt::EditRole );
    model->setItem( 3, 1, item);

    item = new QStandardItem();
    item->setData( 25.6, Qt::EditRole );
    model->setItem( 3, 2, item);

    item = new QStandardItem();
    item->setData( 593.0, Qt::EditRole );
    model->setItem( 3, 3, item);

    // -----------------------------------
    item = new QStandardItem();
    item->setData( 5, Qt::EditRole );
    model->setItem( 4, 0, item);

    item = new QStandardItem();
    item->setData( "Mishi Kobe Niku", Qt::EditRole );
    model->setItem( 4, 1, item);

    item = new QStandardItem();
    item->setData( 72.0, Qt::EditRole );
    model->setItem( 4, 2, item);

    item = new QStandardItem();
    item->setData( 130.0, Qt::EditRole );
    model->setItem( 4, 3, item);

    // -----------------------------------
    item = new QStandardItem();
    item->setData( 6, Qt::EditRole );
    model->setItem( 5, 0, item);

    item = new QStandardItem();
    item->setData( "Queso Manchego La Pastora", Qt::EditRole );
    model->setItem( 5, 1, item);

    item = new QStandardItem();
    item->setData( 32.0, Qt::EditRole );
    model->setItem( 5, 2, item);

    item = new QStandardItem();
    item->setData( 985.5, Qt::EditRole );
    model->setItem( 5, 3, item);

    // -----------------------------------
    item = new QStandardItem();
    item->setData( 7, Qt::EditRole );
    model->setItem( 6, 0, item);

    item = new QStandardItem();
    item->setData( "Genen Shouyu", Qt::EditRole );
    model->setItem( 6, 1, item);

    item = new QStandardItem();
    item->setData( 14.2, Qt::EditRole );
    model->setItem( 6, 2, item);

    item = new QStandardItem();
    item->setData( 1005.0, Qt::EditRole );
    model->setItem( 6, 3, item);

    return model;
}
#endif

#ifdef NCRD_STRINGLIST_DEMO

class DemoData {
public:
    static QStringList createStringList1()
    {
        //-------------------------------------------
        // TEST STRINGLIST DATASOURCE
        //-------------------------------------------
        QStringList list;
        list << "24\tRenate Moulding\tDesert Hot Springs,CA\t1\t2008-01-01";
        list << "78\tAlfred Muller\tMiami Beach, FL\t1\t2008-01-03";
        list << "140\tAngela Merkel\tMunchen, Germany\t1\t2008-01-07";
        list << "139\tBob Larson\tDallas, TX\t0\t2008-01-20";

        return list;
    }

    static QStringList createStringList2()
    {
        QStringList list;
        list << "0000000014|Abkrzung: 005|Ein -->>|Zeitgruppe|<<-- Karenzzeit|";
        list << "0000000015|Abkrzung: 010|Zwei -->>|Zeitgruppe|2254112|";
        list << "0000000016|Abkrzung: 015|Drei -->>|Zeitgruppe|6998714|";

        return list;
    }

    static QStringList createStringListPB()
    {
        QStringList list;
        return list;
    }


};

#endif

#endif  // NCRD_DEMODATA_H
