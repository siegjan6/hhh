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
#ifndef NCRDFIELDPROPDIALOG_H
#define NCRDFIELDPROPDIALOG_H

#include <QDialog>
#include "ui_fieldpropdialog.h"

class QCompleter;
class NCRDDocument;
class NCReportItem;
class NCRDLanguage;

/*!
Field (report item) properties dialog class
*/

/*!

\section 功能

字段属性对话框管理类
【管理所要呈现字段的相关属性，如字段颜色，数据源类型，字段输出类型，对齐方式等，继承自Ui::NCRDFieldPropDialogU】。

*/
class NCRDFieldPropDialog : public QDialog, private Ui::NCRDFieldPropDialogUI
{
    Q_OBJECT
public:
    NCRDFieldPropDialog( NCReportItem *item, NCRDDocument* parent = 0 );
    ~NCRDFieldPropDialog();
    void applyChanges();
    void discardChanges();

public slots:
    void accept();
    void reject();

protected slots:
    void multiLangClicked(bool);
    void sourceTypeChanged(int);
    void dataSourceComboChanged(const QString&);
    void columnComboChanged(const QString&);
    void generateColumnName();

private:
    NCReportItem *m_item;
    NCRDDocument *m_doc;
    QCompleter *m_completer;
    NCRDLanguage *m_langManager;
    bool m_customizedMainWidget;
    bool m_addCustomTab;

    bool isCustomDataSource( const QString& dsID ) const;
    void parseColumnName();
    void setDefaultValues();
};

#endif

