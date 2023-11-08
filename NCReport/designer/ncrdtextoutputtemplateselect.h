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

#ifndef NCRDTEXTOUTPUTTEMPLATESELECT_H
#define NCRDTEXTOUTPUTTEMPLATESELECT_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class NCRDTextOutputTemplateSelect;
}
QT_END_NAMESPACE

class NCRDFileSelector;

class NCRDTextOutputTemplateSelect : public QDialog
{
	Q_OBJECT

/*!

section 功能

【输出模板选择】

*/
public:
	explicit NCRDTextOutputTemplateSelect(QWidget *parent = 0);
	~NCRDTextOutputTemplateSelect();

	QString fileName() const;
	
private:
	Ui::NCRDTextOutputTemplateSelect *ui;
	NCRDFileSelector *selector;
};

#endif // NCRDTEXTOUTPUTTEMPLATESELECT_H
