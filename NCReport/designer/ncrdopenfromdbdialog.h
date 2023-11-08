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
#ifndef NCRDOPENFROMDBDIALOG_H
#define NCRDOPENFROMDBDIALOG_H

#include <QDialog>
#include "ui_openfromdbdialog.h"
#include "ncreportsource.h"

class NCRDMainWindow;

/*!
Dialog class for opening report from database
*/

/*!

\section 功能

数据库打开数据源配置管理对话框类
＃＃＃在代码中暂时屏蔽了此项功能＃＃＃

*/
class NCRDOpenFromDBDialog : public QDialog, private Ui::NCRDOpenFromDBDialogUI
{
  Q_OBJECT

public:
    NCRDOpenFromDBDialog( NCRDMainWindow* parent = 0 );
    ~NCRDOpenFromDBDialog();
    /*$PUBLIC_FUNCTIONS$*/

    NCReportSource reportSource() const;

public slots:
    /*$PUBLIC_SLOTS$*/

protected:
    /*$PROTECTED_FUNCTIONS$*/

protected slots:
    /*$PROTECTED_SLOTS$*/
    virtual void reject();
    virtual void accept();

private:
    NCRDMainWindow* mwindow;
};

#endif

