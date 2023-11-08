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
#ifndef NCRDLABELPROPDIA_H
#define NCRDLABELPROPDIA_H

#include <QDialog>
#include "ui_labelpropdialog.h"

class NCRDDocument;
class NCReportItem;
class NCRDLanguage;

/*!
Label item properties dialog class
*/

/*!

\section 功能

标签控件属性管理类
【继承自Ui::NCRDLabelPropDialogUI，用于管理标签控件的相关属性】

*/
class NCRDLabelPropDialog : public QDialog, private Ui::NCRDLabelPropDialogUI
{
  Q_OBJECT

public:
  NCRDLabelPropDialog( NCReportItem *item, NCRDDocument* parent = 0 );
  ~NCRDLabelPropDialog();
  /*$PUBLIC_FUNCTIONS$*/
    void applyChanges();
    void discardChanges();

public slots:
  /*$PUBLIC_SLOTS$*/
    void accept();
    void reject();
protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/
private:
    NCReportItem *m_item;
    NCRDDocument *m_doc;
    NCRDLanguage *m_langManager;
};

#endif

