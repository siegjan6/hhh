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
#ifndef NCRDCONDFORMATBUTTON_H
#define NCRDCONDFORMATBUTTON_H

#include <QPushButton>

/*!

\section 功能

报表条件格式按钮类
【侦听clicked信号，在槽函数中执行打开代码编辑器对话框操作】。

*/
class NCRDCondFormatButton : public QPushButton
{
	Q_OBJECT
public:
	explicit NCRDCondFormatButton(QWidget *parent = 0);
	QString conditionalFormatCode() const;
	void setConditionalFormatCode( const QString& code );

protected slots:
	void editCondFormatCode();

private:
	QString m_cFormatCode;

};

#endif // NCRDCONDFORMATBUTTON_H
