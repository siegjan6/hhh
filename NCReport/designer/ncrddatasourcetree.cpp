/****************************************************************************
*
*  Copyright (C) 2002-2013 Helta Kft. - NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: norbert@nocisoft.com, office@nocisoft.com
*  Web: www.nocisoft.com
*
*  Created: 2013.09.18. (nocisoft)
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#include "ncrddatasourcetree.h"
#include "ncreportdatasource.h"
#include "ncreportvariable.h"
#include "ncreportutils.h"
#include "ncreportdef.h"
#include "ncreportsection.h"

NCRDDataSourceTree::NCRDDataSourceTree(QWidget *parent) :
    QTreeWidget(parent), m_document(0), m_twi_DataSourceParent(0), m_twi_ParameterParent(0), m_twi_SysVarParent(0), m_twi_VariableParent(0), m_error(0)
{
    setDragEnabled(true);
    setDragDropMode(DragOnly);
    setSelectionMode(ExtendedSelection);

    QFont font;
    font.setBold(true);

    m_twi_DataSourceParent = new QTreeWidgetItem(this,DataSourceParent);
    m_twi_DataSourceParent->setText(0, tr("Data Sources"));
    m_twi_DataSourceParent->setIcon(0,QIcon(":/designer/images/datasource.png"));
    m_twi_DataSourceParent->setFont(0,font);

    m_twi_VariableParent = new QTreeWidgetItem(this,VariableParent);
    m_twi_VariableParent->setText(0, tr("Variables"));
    m_twi_VariableParent->setIcon(0,QIcon(":/designer/images/variable.png"));
    m_twi_VariableParent->setFont(0,font);

    m_twi_SysVarParent = new QTreeWidgetItem(this,SysVarParent);
    m_twi_SysVarParent->setText(0, tr("System Variables"));
    m_twi_SysVarParent->setIcon(0,QIcon(":/designer/images/variable.png"));
    m_twi_SysVarParent->setFont(0,font);

    m_twi_ParameterParent = new QTreeWidgetItem(this,ParameterParent);
    m_twi_ParameterParent->setText(0, tr("Parameters"));
    m_twi_ParameterParent->setIcon(0,QIcon(":/designer/images/tag.png"));
    m_twi_ParameterParent->setFont(0,font);
}

QStringList NCRDDataSourceTree::selectedFields() const
{
    return QStringList();
}

void NCRDDataSourceTree::setCurrentDocument(NCRDDocument *document)
{
    m_document = document;
}

NCRDDocument *NCRDDataSourceTree::currentDocument() const
{
    return m_document;
}

void NCRDDataSourceTree::addDataSource(NCReportDataSource *ds)
{
//  QTreeWidgetItem *twi = findChildItem( m_twi_DataSourceParent, ds->id());
//  if (!twi)
//  {
    QTreeWidgetItem*  twi = new QTreeWidgetItem(m_twi_DataSourceParent,DataSource);
    twi->setIcon(0,QIcon(":/designer/images/datasource.png"));
    twi->setText(0,ds->id());
//  }

//  deleteChildItems(twi);

    ds->setErrorHandler(m_error);

    if (ds->open(NCReportDataSource::OPEN_MODEL_DESIGN)) {
        for (int i=0; i<ds->columnCount(); i++)
        {
            QString columnName = ds->columnName(i);
            QString title = ds->columnTitle(i);

            bool hasTitle = (columnName!=title);
            QString c; // = QString("#%1: ").arg(i);
            if (hasTitle) {
                QTreeWidgetItem *labelItem = new QTreeWidgetItem(twi, Label);
                labelItem->setData(0,Qt::UserRole,i); // column index
                labelItem->setData(0,Qt::UserRole+1,columnName); // column index
                labelItem->setData(0,Qt::UserRole+2,title); // column title
                labelItem->setText(0,ds->columnTitle(i));
                labelItem->setIcon(0,QIcon(":/designer/images/label.png"));
                if (ds->columnToolTip(i)!=title)
                    labelItem->setToolTip(0, ds->columnToolTip(i) );

                // it does have a title
                c += title +" ("+columnName+")";
            } else {
                c += columnName;
            }

            QTreeWidgetItem *fieldItem = new QTreeWidgetItem(twi, DataSourceColumn);
            fieldItem->setData(0,Qt::UserRole,i); // column index
            fieldItem->setData(0,Qt::UserRole+1,columnName); // column index
            fieldItem->setData(0,Qt::UserRole+2,title); // column title
            fieldItem->setText(0,c);
            fieldItem->setIcon(0,QIcon(":/designer/images/table.png"));
            if (ds->columnToolTip(i)!=c)
                fieldItem->setToolTip(0, ds->columnToolTip(i) );
        }
        ds->close();
    }

    if (!m_twi_DataSourceParent->isExpanded())
        m_twi_DataSourceParent->setExpanded(true);
    if (!twi->isExpanded())
        twi->setExpanded(true);
}

void NCRDDataSourceTree::addVariable(NCReportVariable *var)
{
    QTreeWidgetItem*  twi = new QTreeWidgetItem(m_twi_VariableParent,Variable);
    twi->setIcon(0,QIcon(":/designer/images/variable.png"));
    twi->setText(0,var->id());
    if (!m_twi_VariableParent->isExpanded())
        m_twi_VariableParent->setExpanded(true);
}

void NCRDDataSourceTree::addSystemVariable(const QString &varID)
{
    QTreeWidgetItem*  twi = new QTreeWidgetItem(m_twi_SysVarParent,SysVar);
    twi->setIcon(0,QIcon(":/designer/images/variable.png"));
    twi->setText(0,varID);
}

void NCRDDataSourceTree::addParameter(const QString &parID)
{
    QTreeWidgetItem*  twi = new QTreeWidgetItem(m_twi_ParameterParent,Parameter);
    twi->setIcon(0,QIcon(":/designer/images/tag.png"));
    twi->setText(0,parID);
}

QTreeWidgetItem *NCRDDataSourceTree::findChildItem(QTreeWidgetItem *parent, const QString &id) const
{
    QTreeWidgetItem *childItem =0;
    for (int i=0; i<parent->childCount(); i++) {
        //parent->child(i)->type() ==
        if (parent->child(i)->text(0)==id)
            childItem = parent->child(i);
    }
    return childItem;
}

void NCRDDataSourceTree::deleteChildItems(QTreeWidgetItem *parent) const
{
    QList<QTreeWidgetItem *> items = parent->takeChildren();
    for (int i=0; i<items.count(); i++)
        delete items.at(i);
    items.clear();
}

void NCRDDataSourceTree::clearAll() const
{
    deleteChildItems(m_twi_DataSourceParent);
    deleteChildItems(m_twi_VariableParent);
    deleteChildItems(m_twi_SysVarParent);
}

void NCRDDataSourceTree::clearDataSources() const
{
    deleteChildItems(m_twi_DataSourceParent);
}

void NCRDDataSourceTree::clearVariables() const
{
    deleteChildItems(m_twi_VariableParent);
}

void NCRDDataSourceTree::clearParameters() const
{
    deleteChildItems(m_twi_ParameterParent);
}

void NCRDDataSourceTree::setErrorHandler(NCReportError *error)
{
    m_error = error;
}

void NCRDDataSourceTree::parseDataSourceColumn(const QString& expression, QString &columnID, QString &columnTitle) const
{
    QString id;
    id = expression.mid(expression.indexOf(':')+2);
    if (id.contains('(')) {
        id = NCReportUtils::parse(id, "(", ")");
        columnTitle = NCReportUtils::parse(expression,":","(").trimmed();
    }
    columnID = id;
}

void NCRDDataSourceTree::updateDataSources(NCReportDef *reportDef)
{
    clearDataSources();
    QHash<QString,NCReportDataSource*>::const_iterator i = reportDef->dataSources().constBegin();
    while ( i!= reportDef->dataSources().constEnd() ) {
        addDataSource(i.value());
        ++i;
    }
}

void NCRDDataSourceTree::updateVariables(NCReportDef *reportDef)
{
    clearVariables();
    QHash<QString,NCReportVariable*>::const_iterator i = reportDef->variables().constBegin();
    while ( i!= reportDef->variables().constEnd() ) {
        addVariable(i.value());
        ++i;
    }
}

void NCRDDataSourceTree::updateParameters(NCReportDef *reportDef)
{
    clearParameters();
    QStringList idList = reportDef->parameters().keys();
    idList.sort();
    for (int i=0;i<idList.count();++i) {
        addParameter(idList.at(i));
    }
}

NCRDDataSourceTree::ItemDescriptor NCRDDataSourceTree::parseItem(QTreeWidgetItem *twi, NCReportSection* section)
{
    ItemDescriptor d;
    d.itemSourceType = 0;
    d.objectType = 0;
    d.id = twi->text(0);

    switch (twi->type()) {
    case NCRDDataSourceTree::DataSourceColumn:
    {
        d.itemSourceType = NCReportItem::DataSource;
        d.objectType = NCReportItem::Field;
        d.id = twi->data(0,Qt::UserRole+1).toString();
        d.title = twi->data(0,Qt::UserRole+2).toString();
        break;
    }
    case NCRDDataSourceTree::Variable:
    {
        d.itemSourceType = NCReportItem::Variable;
        d.objectType = NCReportItem::Field;
        break;
    }
    case NCRDDataSourceTree::Parameter:
    {
        d.itemSourceType = NCReportItem::Parameter;
        d.objectType = NCReportItem::Field;
        break;
    }
    case NCRDDataSourceTree::SysVar:
    {
        d.itemSourceType = NCReportItem::SystemVariable;
        d.objectType = NCReportItem::Field;
        break;
    }
    case NCRDDataSourceTree::Label:
    {
#ifdef NCRD_DRAGDROP_LABEL_AS_FIELD
        d.itemSourceType = NCReportItem::DataSource;
        d.objectType = NCReportItem::Field;
        d.id = twi->data(0,Qt::UserRole+1).toString() + def_strMarkHeaderDisplayRole;
        //idx = twi->data(0,Qt::UserRole).toInt();
        d.title = twi->data(0,Qt::UserRole+2).toString();
        //source->parseDataSourceColumn(columnID, columnID, columnTitle);
#else
        d.itemSourceType = NCReportItem::Static;
        d.objectType = NCReportItem::Label;
#endif
        break;
    }
    }

    if (d.itemSourceType == NCReportItem::DataSource) {
        if ( section && section->type() == NCReportSection::Detail && section->dataSourceId() == twi->parent()->text(0)) {
            // nothing to do
        } else {
            d.id.prepend(twi->parent()->text(0)+".");
        }
    }

    return d;
}


//bool NCRDDataSourceTree::dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action)
//{
//    QList<QTreeWidgetItem *> selectList;
//    QTreeWidgetItem * myDraggedItem;
//    selectList = this->selectedItems();
//    myDraggedItem = selectList.at(0);


//    QModelIndex idx;
//    if (parent) idx = indexFromItem(parent);
//    bool isAccepted = model()->QAbstractItemModel::dropMimeData(data, action , index, 0, idx);
//    return isAccepted;
//}

