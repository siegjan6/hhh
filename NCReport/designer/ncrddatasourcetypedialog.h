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
#ifndef NCRDDATASOURCETYPEDIALOG_H
#define NCRDDATASOURCETYPEDIALOG_H

#include <QDialog>
#include "ui_dstypedialog.h"
#include "ncreportdatasource.h"

/*!
Datasource type selector dialog class
*/

/*!

\section 功能

数据源类型选择对话框管理类
【枚举数据源类型，选择其中一种数据源类型】
＃＃＃在代码中暂时屏蔽了选择界面，并添加静态函数static NCReportDataSource::DataSourceType createDataSource()
默认选择SQL查询的数据源，暂时屏蔽其他 Text, Xml, StringList, ItemModel, Custom类型的DataSourceType＃＃＃

*/
class NCRDDataSourceTypeDialog : public QDialog, private Ui::NCRDDataSourceTypeDialogUI
{
  Q_OBJECT

public:
  NCRDDataSourceTypeDialog(QWidget* parent = 0 );
  ~NCRDDataSourceTypeDialog();
  /*$PUBLIC_FUNCTIONS$*/

    NCReportDataSource::DataSourceType selectedType() const;

public slots:
  /*$PUBLIC_SLOTS$*/

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/

};

#endif

