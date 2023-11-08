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
#ifndef NCRDPRINTWHENPANEL_H
#define NCRDPRINTWHENPANEL_H

#include <QWidget>
#include "ui_printwhenpanel.h"

class NCReportItem;
class QLineEdit;
class NCRDDocument;

/*!

\section 功能

高级选项管理类
【管理打印条件，Html选项等配置】

*/
class NCRDPrintWhenPanel : public QWidget, public Ui::NCRDPrintWhenPanelUI
{
  Q_OBJECT
public:
    NCRDPrintWhenPanel(QWidget* parent = 0 );
    ~NCRDPrintWhenPanel();

    void setOptions(NCReportItem* item , NCRDDocument *document);
    void getOptions( NCReportItem* item );
    void disableItemOptions();

public slots:
    void editPrintWhen();
    void editHtmlOpt();

private:
    NCReportItem* m_item;
    NCRDDocument* m_document;

    void openEditDialog( QLineEdit* editor );

};

#endif

