/****************************************************************************
*
*  Copyright (C) 2002-2012 Helta Kft. / NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: norbert@nocisoft.com, office@nocisoft.com
*  Web: www.nocisoft.com
*
*  This file is part of the NCReport Report Generator System
*
*  Licensees holding a valid NCReport License Agreement may use this
*  file in accordance with the rights, responsibilities, and obligations
*  contained therein. Please consult your licensing agreement or contact
*  norbert@nocisoft.com if any conditions of this licensing are not clear
*  to you.
*
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/
#include "ncrdcondformatbutton.h"
#include "ncrdcodeeditdialog.h"

NCRDCondFormatButton::NCRDCondFormatButton(QWidget *parent) : QPushButton(parent)
{
	connect(this,SIGNAL(clicked()),this,SLOT(editCondFormatCode()));
	setText( tr("Conditional format...") );
}

void NCRDCondFormatButton::editCondFormatCode()
{
	NCRDCodeEditDialog dialog(parentWidget());
	dialog.setCodeText(m_cFormatCode);
	if (dialog.exec()==QDialog::Accepted)
		m_cFormatCode = dialog.codeText();
}

QString NCRDCondFormatButton::conditionalFormatCode() const
{
	return m_cFormatCode;
}

void NCRDCondFormatButton::setConditionalFormatCode(const QString &code)
{
	m_cFormatCode = code;
}

