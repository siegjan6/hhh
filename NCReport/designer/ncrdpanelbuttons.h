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
#ifndef NCRDPANELBUTTONS_H
#define NCRDPANELBUTTONS_H

#include <QWidget>
#include "ui_panelbuttons.h"

/*!

\section 功能

添加／删除／上移／下移按钮组管理类
【在细节和分组中使用次按钮组】

*/
class NCRDPanelButtons : public QWidget, public Ui::NCRDPanelButtonsUI
{
    Q_OBJECT
public:
    NCRDPanelButtons(QWidget *parent = 0 );
    ~NCRDPanelButtons();
signals:
public slots:
};

#endif // NCRDPANELBUTTONS_H
