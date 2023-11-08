/****************************************************************************
*
*  Copyright (C) 2002-2011 Helta Kft. / NociSoft Software Solutions
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

#ifndef NCRDCODEEDITDIALOG_H
#define NCRDCODEEDITDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class NCRDCodeEditDialog;
}
QT_END_NAMESPACE

class NCRDCodeEditDialog : public QDialog
{
	Q_OBJECT
	
/*!

\section 功能

代码编辑器类
【管理代码编辑器对话框】。

    */
public:
	explicit NCRDCodeEditDialog(QWidget *parent = 0);
	~NCRDCodeEditDialog();

	QString codeText() const;
	void setCodeText( const QString& code );

private:
	Ui::NCRDCodeEditDialog *ui;

};

#endif // NCRDCODEEDITDIALOG_H
