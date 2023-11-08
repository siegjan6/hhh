/****************************************************************************
*
*  Copyright (C) 2002-2013 Helta Kft. - NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: norbert@nocisoft.com, office@nocisoft.com
*  Web: www.nocisoft.com
*
*  Created: 7 Mar 2013 (nszabo)
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#ifndef NCRDTABLEITEMDIALOG_H
#define NCRDTABLEITEMDIALOG_H

#include <QDialog>

namespace Ui {
class NCRDTableItemDialog;
}
class NCReportItem;
class NCRDDocument;

/*!

\section 功能

报表表格控件对话框管理类
＃＃＃暂时屏蔽此表格控件＃＃＃

*/
class NCRDTableItemDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NCRDTableItemDialog( NCReportItem *item, NCRDDocument* parent = 0);
    ~NCRDTableItemDialog();
    void applyChanges();
    void discardChanges();

public slots:
    void accept();
    void reject();

private:
    Ui::NCRDTableItemDialog *ui;
    NCReportItem *m_item;
    NCRDDocument *m_doc;
};

#endif // NCRDTABLEITEMDIALOG_H
