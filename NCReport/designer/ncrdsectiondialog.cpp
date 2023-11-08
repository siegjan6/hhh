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
#include "ncrdsectiondialog.h"
#include "ncrddocument.h"
#include "ncrddesignsection.h"
#include "ncrdsectionscene.h"
#include "ncrdformgeometry.h"

//! 多语言
#include "MDMultiLanguage.h"

NCRDSectionDialog::NCRDSectionDialog(NCRDDocument *doc, NCRDDesignSection* dsect, QWidget* parent)
    : QDialog( parent ), Ui::NCRDSectionDialogUI(),
      m_document(doc),
      m_dsect(dsect),
      m_prevHeight(0)
{
    setupUi(this);

    NCReportSection *section = dsect->sectionScene();
    leType->setText( section->caption() );
    leID->setText( section->id() );
    spHeight->setValue( section->metricHeight() );
    m_prevHeight = section->metricHeight();
    cbAutoHeight->setChecked( section->isAutoHeight() );
    cbHideIfBlank->setChecked( section->hideIfBlank() );
    cbNotAlone->setChecked( section->isNotAlone() );
    cbStartsOnNewPage->setChecked(section->startsOnNewPage());
    spOrphan->setValue(section->orphanedTolaranceMM());
    cbAnchor->setChecked(section->isAnchorToPageFooter());

    printWhenPanel->printWhen->setText( section->printWhenExp() );
    printWhenPanel->htmlOptions->setText(section->htmlOptions());
    printWhenPanel->disableItemOptions();
    printWhenPanel->setOptions(0,doc);

    cbNotAlone->setEnabled(false);
    cbStartsOnNewPage->setEnabled(false);
    cbAnchor->setEnabled(false);

    switch(section->type()) {
    case NCReportSection::Detail:
    {
        leID->setReadOnly(true);
        break;
    }
    case NCReportSection::PageFooter:
    {
        // no autoHeigh is valid for pagefooter		cbAutoHeight->setChecked(false);
        cbAutoHeight->setChecked(false);
        cbAutoHeight->setEnabled(false);
        cbHideIfBlank->setChecked(false);
        cbHideIfBlank->setEnabled(false);
        break;
    }
    case NCReportSection::GroupHeader:
    {
        cbNotAlone->setEnabled(true);
        break;
    }
    case NCReportSection::GroupFooter:
    {
        cbAnchor->setEnabled(true);
        break;
    }
    case NCReportSection::ReportFooter:
    {
        cbStartsOnNewPage->setEnabled(true);
        cbAnchor->setEnabled(true);
        break;
    }
    case NCReportSection::ReportHeader:
    case NCReportSection::PageHeader:
    case NCReportSection::Custom:
        break;
    }

    NCRDFormGeometry *fg = new NCRDFormGeometry(this);
    connect(this,SIGNAL(finished(int)),fg,SLOT(save()));
    fg->restore();
    spHeight->setFocus();
    spHeight->selectAll();

    //! 多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

NCRDSectionDialog::~NCRDSectionDialog()
{
}

/*$SPECIALIZATION$*/

void NCRDSectionDialog::applyChanges()
{
    NCReportSection *section = m_dsect->sectionScene();

    section->setID(leID->text());
    section->setMetricHeight( spHeight->value() );
    section->setAutoHeight( cbAutoHeight->isChecked() );
    section->setHideIfBlank( cbHideIfBlank->isChecked() );
    section->setPrintWhenExp( printWhenPanel->printWhen->text() );
    section->setStartsOnNewPage(cbStartsOnNewPage->isChecked());
    section->setAnchorToPageFooter(cbAnchor->isChecked());
    section->setHtmlOptions(printWhenPanel->htmlOptions->text());

//    if ( cbNotAlone->isChecked() )
//        section->setCheckRole( NCReportSection::SectionPlusDetail );
//    else
//        section->setCheckRole( NCReportSection::SectionOnly );


    section->setNotAlone( cbNotAlone->isChecked() );
    section->setOrphanedTolaranceMM( spOrphan->value() );

    if ( spOrphan->value() > 0 )
        section->setCheckRole( NCReportSection::SectionPlusOrphanedTolerance );

    if ( spHeight->value() != m_prevHeight )  {
        m_dsect->setHeightMM( spHeight->value() );
        m_document->adjustContent();
    }

    m_document->setDocumentModified();
}

void NCRDSectionDialog::discardChanges()
{
}


