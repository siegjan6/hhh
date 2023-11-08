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
#include "ncrdfieldexpwidget.h"
#include "ncrdexpressionbuilderdialog.h"
#include "ncreportitem.h"

#include <QComboBox>

NCRDFieldExpWidget::NCRDFieldExpWidget(QWidget* parent) : QWidget( parent ), Ui::NCRDFieldExpWidgetUI(), m_item(0), m_document(0)
{
    setupUi(this);
    connect(btnBuildExpr,SIGNAL(clicked()),this,SLOT(openExpressionBuilder()));
}

NCRDFieldExpWidget::~NCRDFieldExpWidget()
{
}

void NCRDFieldExpWidget::setItem(NCReportItem *item)
{
    m_item = item;
}

void NCRDFieldExpWidget::setDocument(NCRDDocument *document)
{
    m_document = document;
}

void NCRDFieldExpWidget::setSourceTypeCombo(QComboBox *combo)
{
    m_stCombo = combo;
}

void NCRDFieldExpWidget::hideExpButton()
{
    btnBuildExpr->hide();
}

void NCRDFieldExpWidget::openExpressionBuilder()
{
    if (!m_item)
        return;

    NCRDExpressionBuilderDialog dialog(m_document, m_item->availableSourceTypes(), m_stCombo->currentIndex(), parentWidget());
    dialog.setExpression(editor->toPlainText());
    //dialog.setSourceType(m_stCombo->currentIndex());

    if (dialog.exec()==QDialog::Accepted) {
        editor->setPlainText( dialog.expression() );
        m_stCombo->setCurrentIndex(dialog.sourceType());
    }
}

