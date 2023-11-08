/****************************************************************************
*
*  Copyright (C) 2002-2013 Helta Kft. - NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: norbert@nocisoft.com, office@nocisoft.com
*  Web: www.nocisoft.com
*
*  Created: 2013.11.13. (nocisoft)
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#ifndef NCRDITEMSOURCESELECTOR_H
#define NCRDITEMSOURCESELECTOR_H

#include <QWidget>

namespace Ui {
class NCRDItemSourceSelector;
}

/*!

\section 功能

此类在代码中未使用

*/
class NCRDItemSourceSelector : public QWidget
{
    Q_OBJECT
public:
    explicit NCRDItemSourceSelector(QWidget *parent = 0);
    ~NCRDItemSourceSelector();

private:
    Ui::NCRDItemSourceSelector *ui;
};

#endif // NCRDITEMSOURCESELECTOR_H
