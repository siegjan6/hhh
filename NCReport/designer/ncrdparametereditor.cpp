/****************************************************************************
*
*  Copyright (C) 2007-2010 Helta Kft. / NociSoft Software Solutions
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
#include "ncrdparametereditor.h"
#include "ncreportdef.h"
#include "ncreportutils.h"

#include <QFileDialog>
#include <QTextStream>
#include <QFile>

NCRDParameterEditor::NCRDParameterEditor(NCReportDef* reportDef, QStringList &removedParameters, QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent,f), m_rdef(reportDef), m_removedParameters(removedParameters)
{
    setupUi(this);
    connect(btnAdd, SIGNAL(clicked()), this, SLOT(add()));
    connect(btnRemove, SIGNAL(clicked()), this, SLOT(remove()));
    connect(btnFileLoad, SIGNAL(clicked()), this, SLOT(loadFromFile()));
}

NCRDParameterEditor::~NCRDParameterEditor()
{
}

void NCRDParameterEditor::add()
{
    addParam("","");
    int rows = tableParams->rowCount();
    if ( rows>0 )
        tableParams->setCurrentCell( rows-1, 0 );
    tableParams->setFocus();
}

void NCRDParameterEditor::addParam( const QString& name, const QString& value )
{
    int rows = tableParams->rowCount();
    tableParams->setRowCount( rows+1 );

    QTableWidgetItem *newItem;

    newItem = new QTableWidgetItem(name);
    tableParams->setItem(rows, 0, newItem);

    newItem = new QTableWidgetItem(value);
    tableParams->setItem(rows, 1, newItem);
}

void NCRDParameterEditor::remove()
{
    if (tableParams->rowCount()==0)
        return;
    int i = tableParams->currentRow();

    QList<QTableWidgetItem*> items = tableParams->selectedItems();
    if (items.count()>0) {
        for (int i=0; i<items.count(); i++ ) {
            QTableWidgetItem* item = items.at(i);
            if (item->column()==0) {
                m_removedParameters << item->text();
                m_rdef->removeParameter(item->text());
                tableParams->removeRow(item->row());
            }
        }
    } else {
        m_removedParameters << tableParams->item(i,0)->text();
        m_rdef->removeParameter(tableParams->item(i,0)->text());
        tableParams->removeRow(i);
    }
}

void NCRDParameterEditor::loadFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open parameter file"), QString(), "*.txt *.ini" );
    if (!fileName.isEmpty()) {

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QTextStream ts(&file);
        while (!ts.atEnd()) {
            QString line = ts.readLine();
            if (!line.isEmpty() && line.contains('=')) {
                addParam( line.section('=',0,0).trimmed(), line.section('=',1,1).trimmed() );
            }
        }
    }
}

void NCRDParameterEditor::save()
{
    for (int i=0; i<tableParams->rowCount(); ++i ) {
        QString parName = tableParams->item(i,0)->text();
        if ( !parName.isEmpty() )
            m_rdef->addParameter(parName, tableParams->item(i,1)->text());
    }
}

void NCRDParameterEditor::load()
{
    QHash<QString, QVariant>::const_iterator i = m_rdef->parameters().constBegin();
    while (i != m_rdef->parameters().constEnd()) {
        addParam( i.key(), i.value().toString() );
        ++i;
    }
    tableParams->setFocus();
}

void NCRDParameterEditor::setCurrentParameter(const QString &paramID)
{
    if (paramID.isEmpty())
        return;
    QList<QTableWidgetItem*> itemList = tableParams->findItems(paramID, Qt::MatchExactly);
    if (itemList.count()>=1) {
        tableParams->setFocus();
        tableParams->setCurrentCell( tableParams->row(itemList.at(0)), 1);
    }
}
