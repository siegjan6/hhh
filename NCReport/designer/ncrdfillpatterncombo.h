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

#ifndef NCRDFILLPATTERNCOMBO_H
#define NCRDFILLPATTERNCOMBO_H

#include <QComboBox>

/*!

\section 功能

填充模式下拉框选择管理类
【在构造函数中默认提供了相关填充模式】。

*/
class NCRDFillPatternCombo : public QComboBox
{
    Q_OBJECT
public:
	explicit NCRDFillPatternCombo(QWidget *parent = 0);
};

#endif // NCRDFILLPATTERNCOMBO_H
