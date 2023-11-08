/****************************************************************************
*
*  Copyright (C) 2002-2013 Helta Kft. - NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: norbert@nocisoft.com, office@nocisoft.com
*  Web: www.nocisoft.com
*
*  Created: 2013.11.12. (nocisoft)
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#include "ncrditemsourcetypecombo.h"

NCRDItemSourceTypeCombo::NCRDItemSourceTypeCombo(QWidget *parent) :
    QComboBox(parent)
{
    clear();
    insertItem(NCReportItem::Static, tr("Static"));
    insertItem(NCReportItem::DataSource, tr("Data source"));
    insertItem(NCReportItem::File, tr("File"));
    insertItem(NCReportItem::FileNameFromDataSource, tr("File name from data source"));
    insertItem(NCReportItem::Parameter, tr("Parameter"));
    insertItem(NCReportItem::Url, tr("URL"));
    insertItem(NCReportItem::UrlFromDataSource, tr("URL from data source"));
    insertItem(NCReportItem::InternalDocument, tr("Internal document"));
    insertItem(NCReportItem::Variable, tr("Variable"));
    insertItem(NCReportItem::SystemVariable, tr("System variable"));
    insertItem(NCReportItem::ScriptExpression, tr("Script expression"));
    insertItem(NCReportItem::Template, tr("Template"));

    setValue(NCReportItem::Static);
}

void NCRDItemSourceTypeCombo::setValue(NCReportItem::SourceType st)
{
    setCurrentIndex( (int)st );
}

NCReportItem::SourceType NCRDItemSourceTypeCombo::value() const
{
    return (NCReportItem::SourceType)currentIndex();
}

void NCRDItemSourceTypeCombo::setAvailableItems(const QList<int> &values)
{
    for (int i=0; i<count(); i++) {
        int enabled = 0;
        if (values.contains(i))
            enabled = 33;
        setItemData(i, enabled, Qt::UserRole-1);
    }
}

QString NCRDItemSourceTypeCombo::title() const
{
    QString ttl;
    switch (value()) {
    case NCReportItem::Static:
    case NCReportItem::File:
        ttl = tr("File name");
        break;
    case NCReportItem::DataSource:
    case NCReportItem::FileNameFromDataSource:
    case NCReportItem::UrlFromDataSource:
        ttl = tr("Data source column");
        break;
    case NCReportItem::Url:
        ttl = tr("Source URL");
        break;
    case NCReportItem::Parameter:
        ttl = tr("Parameter ID");
        break;
    case NCReportItem::InternalDocument:
        ttl = tr("Internal content ID:");
        break;
    case NCReportItem::Variable:
        ttl = tr("Variable ID");
        break;
    case NCReportItem::SystemVariable:
        ttl = tr("System Var.ID");
        break;
    case NCReportItem::ScriptExpression:
        ttl = tr("Script expression");
        break;
    case NCReportItem::Template:
        ttl = tr("Template expression");
        break;
    case NCReportItem::FieldDisplay:
        ttl = tr("Field display value");
        break;
    }
    return ttl;
}
