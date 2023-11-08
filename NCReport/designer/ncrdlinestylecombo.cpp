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

#include "ncrdlinestylecombo.h"

NCRDLineStyleCombo::NCRDLineStyleCombo(QWidget *parent) : QComboBox(parent)
{
	insertItem( 0, tr("No pen") );
	insertItem( 1, tr("Solid") );
	insertItem( 2, tr("Dash") );
	insertItem( 3, tr("Dot") );
	insertItem( 4, tr("Dash dot") );
	insertItem( 5, tr("Dash dot dot") );
	insertItem( 6, tr("Custom dash") );
}
