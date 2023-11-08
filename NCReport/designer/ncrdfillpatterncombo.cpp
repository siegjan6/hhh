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
#include "ncrdfillpatterncombo.h"

NCRDFillPatternCombo::NCRDFillPatternCombo(QWidget *parent) : QComboBox(parent)
{
	insertItem( 0, tr("No fill") );
	insertItem( 1, tr("Solid") );
	insertItem( 2, tr("Dense1") );
	insertItem( 3, tr("Dense2") );
	insertItem( 4, tr("Dense3") );
	insertItem( 5, tr("Dense4") );
	insertItem( 6, tr("Dense5") );
	insertItem( 7, tr("Dense6") );
	insertItem( 8, tr("Dense7") );
	insertItem( 9, tr("Horizontal") );
	insertItem( 10, tr("Vertical") );
	insertItem( 11, tr("Cross") );
	insertItem( 12, tr("B.diagonal") );
	insertItem( 13, tr("F.diagonal") );
	insertItem( 14, tr("Diagonal cross") );
	insertItem( 15, tr("Linear gradient") );
	insertItem( 16, tr("Concial gradient") );
	insertItem( 17, tr("Radial gradient") );
	insertItem( 18, tr("Texture") );
}
