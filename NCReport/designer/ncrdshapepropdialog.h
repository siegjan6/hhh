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
#ifndef NCRDSHAPEPROPDIALOG_H
#define NCRDSHAPEPROPDIALOG_H

#include <QDialog>
#include "ui_shapepropdialog.h"

class NCRDDocument;
class NCReportItem;

/*!
Property setting dialog class for report items such as: Line, Rectangle, Ellipse
*/

/*!

\section 功能

报表项目属性设置对话框类，如：线，矩形，椭圆

*/
class NCRDShapePropDialog : public QDialog, private Ui::NCRDShapePropDialogUI
{
  Q_OBJECT

public:
  NCRDShapePropDialog( NCReportItem *item, NCRDDocument* parent = 0 );
  ~NCRDShapePropDialog();
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
    void enableForm();
private:
    NCReportItem *m_item;
    NCRDDocument *m_doc;

};

#endif

