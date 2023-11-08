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
#ifndef NCRDFIELDEXPWIDGET_H
#define NCRDFIELDEXPWIDGET_H

#include <QWidget>
#include "ui_fieldexp.h"

class NCReportItem;
class NCRDDocument;

QT_BEGIN_NAMESPACE
class QComboBox;
QT_END_NAMESPACE

/*!

\section 功能

字段表达式创建管理类
【管理设置到字段表达式内容编辑框中内容，提供设置字段表达式的相关接口】。

*/
class NCRDFieldExpWidget : public QWidget, public Ui::NCRDFieldExpWidgetUI
{
    Q_OBJECT
public:
    NCRDFieldExpWidget(QWidget* parent = 0 );
    ~NCRDFieldExpWidget();

    void setItem( NCReportItem* item );
    void setDocument( NCRDDocument* document );
    void setSourceTypeCombo( QComboBox* combo );

    void hideExpButton();

private slots:
    void openExpressionBuilder();

private:
    NCReportItem* m_item;
    NCRDDocument* m_document;
    QComboBox* m_stCombo;
};

#endif // NCRDFIELDEXPWIDGET_H

