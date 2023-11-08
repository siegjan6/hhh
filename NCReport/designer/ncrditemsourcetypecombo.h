/****************************************************************************
*
*  Copyright (C) 2002-2013 Helta Kft. - NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: norbert@nocisoft.com, office@nocisoft.com
*  Web: www.nocisoft.com
*
*  Created: 2013.11.12. (nocisoft)
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#ifndef NCRDITEMSOURCETYPECOMBO_H
#define NCRDITEMSOURCETYPECOMBO_H

#include <QComboBox>
#include <QList>
#include "ncreportitem.h"
#include "ncrd_global.h"

/*!

\section 功能

数据源类型下拉框管理类的导出类

*/
class NCREPORTDESIGNER_SHARED_EXPORT NCRDItemSourceTypeCombo : public QComboBox
{
    Q_OBJECT
public:
    explicit NCRDItemSourceTypeCombo(QWidget *parent = 0);

    void setValue(NCReportItem::SourceType st);
    NCReportItem::SourceType value() const;
    void setAvailableItems( const QList<int>& values);

    QString title() const;
};

#endif // NCRDITEMSOURCETYPECOMBO_H
