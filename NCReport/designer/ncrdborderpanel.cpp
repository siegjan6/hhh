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
#include "ncrdborderpanel.h"
#include "ncrdcodeeditdialog.h"
#include "ncreportitem.h"
#include "ncreportlabelitem.h"
#include "ncrdexpressionbuilderdialog.h"

NCRDBorderPanel::NCRDBorderPanel(QWidget* parent)
: QWidget( parent ), Ui::NCRDBorderPanelUI(), m_item(0), m_document(0)
{
    setupUi(this);

    connect(borderColorButton,&QPushButton::clicked,this,&NCRDBorderPanel::slotSetBorderColor);
}

NCRDBorderPanel::~NCRDBorderPanel()
{
}

void NCRDBorderPanel::slotSetBorderColor()
{
    QColor c = m_item->borderPen().color();
    selectColor(c, *borderColorButton, this);
}

void NCRDBorderPanel::setOptions(NCReportLabelItem *item, NCRDDocument *document)
{
    m_item = item;
    m_document = document;
    if (item) {
        cbBorderVisible->setChecked(item->borderVisible());
        int index = 0;
        switch ( item->borderPen().style() ) {
            case Qt::SolidLine: index=1; break;
            case Qt::DashLine: index=2; break;
            case Qt::DotLine: index=3; break;
            case Qt::DashDotDotLine: index=4; break;
            case Qt::DashDotLine: index=5; break;
            case Qt::NoPen: index=0; break;
            default: break;
        }
        comboLineStyle->setCurrentIndex(index);
        setButtonColor(item->borderPen().color(), *borderColorButton);
        spinLineWidth->setValue(item->lineWidth());
    }
}

void NCRDBorderPanel::getOptions(NCReportLabelItem *item)
{
    item->setBorderVisible(cbBorderVisible->isChecked());
    QPen borderpen;
    borderpen.setColor(m_borderColor);
    Qt::PenStyle ps = Qt::SolidLine;
    QString ls = comboLineStyle->currentText().toLower();
    if (ls == "solid")
        ps = Qt::SolidLine;
    else if (ls == "dash")
        ps = Qt::DashLine;
    else if (ls == "dot")
        ps = Qt::DotLine;
    else if (ls == "dash dot dot")
        ps = Qt::DashDotDotLine;
    else if (ls == "dash dot")
        ps = Qt::DashDotLine;
    else if (ls == "nopen" || ls=="no" || ls=="none" )
        ps = Qt::NoPen;
    borderpen.setStyle( ps );
    item->setBorderPen(borderpen);
    item->setLineWidth(spinLineWidth->value());
}


