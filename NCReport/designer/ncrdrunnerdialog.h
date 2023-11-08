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
#ifndef NCRDRUNNERDIALOG_H
#define NCRDRUNNERDIALOG_H

#include <QDialog>
#include "ui_starterdialog.h"

class NCRDMainWindow;
class NCRDDocument;
class NCRDParameterEditor;

/*!
Dialog class for running reports from inside of designer
*/

/*!

\section 功能

运行报表对话框管理类
【从设计器内部运行报表的对话框类，选择报表输出类型，运行报表】

*/
class NCRDRunnerDialog : public QDialog, private Ui::NCRDRunnerDialogUI
{
    Q_OBJECT
public:
    NCRDRunnerDialog( NCRDMainWindow *mw, NCRDDocument *doc, QWidget* parent = 0 );
    ~NCRDRunnerDialog();

    void runReport();

private slots:
    void extend(bool);
    void enableBatchMode(bool enable);

private:
    NCRDMainWindow *mainwindow;
    NCRDDocument *defaultDocument;
    NCRDParameterEditor *pEditor;
    QPushButton *btnParam;
    QList<NCRDDocument*> documents;
};

#endif

