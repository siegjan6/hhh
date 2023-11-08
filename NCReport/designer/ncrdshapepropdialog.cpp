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
#include "ncrdshapepropdialog.h"
#include "ncrddocument.h"
#include "ncreportshapeitem.h"
#include "ncreportrectitem.h"
#include "ncrdformgeometry.h"

//! 多语言
#include "MDMultiLanguage.h"

NCRDShapePropDialog::NCRDShapePropDialog(NCReportItem * item, NCRDDocument * parent)
: QDialog( parent ), Ui::NCRDShapePropDialogUI(), m_item(item), m_doc(parent)
{
    setupUi(this);
    connect( cbAutoHeight, SIGNAL(clicked()), this, SLOT(enableForm()) );
    //resize( QSize(width(), sizeHint().height() ) );

    NCReportShapeItem *si = (NCReportShapeItem*)item;
    QString formName;

    gbRound->setVisible(false);

    switch(si->objectType()) {
    case NCReportItem::Rectangle: {
        NCReportRectItem *ri = (NCReportRectItem*)item;
        spRoundX->setValue(ri->xRound() );
        spRoundY->setValue(ri->yRound());
        gbRound->setVisible(true);
        formName = "RectPropDia";
        break;
    }
    default:
        formName = "ShapePropDia";
        break;
    }
    printWhenPanel->setOptions(si,parent);
    cbAutoHeight->setChecked( si->isAutoHeight() );
    optExpandToSection->setChecked( si->fitRole() == NCReportItem::FitHeightToSection );
    optExpandToZone->setChecked( si->fitRole() == NCReportItem::FitHeightToZone );
    cbPinToRight->setChecked( si->pinToRight() );

    enableForm();
    NCRDFormGeometry *fg = new NCRDFormGeometry(this,0,formName);
    connect(this,SIGNAL(finished(int)),fg,SLOT(save()));
    fg->restore();

    //! 多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

NCRDShapePropDialog::~NCRDShapePropDialog()
{
}

/*$SPECIALIZATION$*/

void NCRDShapePropDialog::applyChanges()
{
    NCReportShapeItem *si = (NCReportShapeItem*)m_item;
    if ( si->objectType() == NCReportItem::Rectangle ) {
        NCReportRectItem *ri = (NCReportRectItem*)m_item;
        ri->setXRound( spRoundX->value() );
        ri->setYRound( spRoundY->value() );
    }

    printWhenPanel->getOptions(si);

    si->setAutoHeight( cbAutoHeight->isChecked() );
    if ( cbAutoHeight->isChecked() ) {
        si->setFitRole( optExpandToSection->isChecked() ? NCReportItem::FitHeightToSection : NCReportItem::FitHeightToZone );
    }
    si->setPinToRight( cbPinToRight->isChecked() );
    m_doc->setDocumentModified();
}

void NCRDShapePropDialog::discardChanges()
{
}

void NCRDShapePropDialog::enableForm()
{
    optExpandToSection->setEnabled( cbAutoHeight->isChecked() );
    optExpandToZone->setEnabled( cbAutoHeight->isChecked() );
    if ( !cbAutoHeight->isChecked() ) {
        optExpandToSection->setChecked(false);
        optExpandToZone->setChecked(false);
    }
    //adjustSize();
}


void NCRDShapePropDialog::accept()
{
    applyChanges();
    QDialog::accept();
}

void NCRDShapePropDialog::reject()
{
    discardChanges();
    QDialog::reject();
}



