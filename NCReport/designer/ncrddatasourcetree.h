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

#ifndef NCRDDATASOURCETREE_H
#define NCRDDATASOURCETREE_H

#include <QTreeWidget>

class NCReportDataSource;
class NCReportVariable;
class NCReportError;
class NCRDDocument;
class NCReportDef;
class NCReportSection;

/*!

\section 功能

数据源类型树形结构管理类
【管理数据源／变量／系统变量／参数四种类型的数据源树形结构的组织】

*/
class NCRDDataSourceTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit NCRDDataSourceTree(QWidget *parent = 0);

    enum ItemType { DataSourceParent=0, DataSource, DataSourceColumn, Label, VariableParent, Variable, SysVarParent, SysVar, ParameterParent, Parameter };

    struct ItemDescriptor {
        int itemSourceType;
        int objectType;
        QString id;
        QString title;
    };

    QStringList selectedFields() const;

    void setCurrentDocument( NCRDDocument* document );
    NCRDDocument *currentDocument() const;

    void addDataSource( NCReportDataSource* ds );
    void addVariable( NCReportVariable* var );
    void addSystemVariable( const QString& varID );
    void addParameter( const QString& parID );
    QTreeWidgetItem *findChildItem( QTreeWidgetItem* parent, const QString &id ) const;
    void deleteChildItems( QTreeWidgetItem* parent ) const;
    void clearAll() const;
    void clearDataSources() const;
    void clearVariables() const;
    void clearParameters() const;

    void setErrorHandler(NCReportError *error);

    void parseDataSourceColumn(const QString &expression, QString& columnID, QString &columnTitle) const;

    void updateDataSources( NCReportDef* reportDef );
    void updateVariables( NCReportDef* reportDef );
    void updateParameters( NCReportDef* reportDef );

    NCRDDataSourceTree::ItemDescriptor parseItem(QTreeWidgetItem* twi , NCReportSection *section = 0);


protected:
    //bool dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action);

public slots:

private:
    NCRDDocument* m_document;
    QTreeWidgetItem *m_twi_DataSourceParent;
    QTreeWidgetItem *m_twi_ParameterParent;
    QTreeWidgetItem *m_twi_SysVarParent;
    QTreeWidgetItem *m_twi_VariableParent;

    NCReportError* m_error;
};

#endif // NCRDDATASOURCETREE_H
