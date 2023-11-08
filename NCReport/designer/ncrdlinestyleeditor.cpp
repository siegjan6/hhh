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

#include "ncrdlinestyleeditor.h"
#include "ncrdlinestylecombo.h"
#include "mdcolorpicker.h"

#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QHBoxLayout>

NCRDLineStyleEditor::NCRDLineStyleEditor(QWidget *parent) : QWidget(parent)
{
	m_lineStyleCombo = new NCRDLineStyleCombo(this);

	m_cbEnabled = new QCheckBox(this);
	m_cbEnabled->setText( tr("Enabled"));
	m_cbEnabled->setEnabled(true);

	m_spLineWidth = new QDoubleSpinBox(this);
	m_spLineWidth->setMinimum(0);
	m_spLineWidth->setMaximum(10);
	m_spLineWidth->setDecimals(2);
	m_spLineWidth->setSingleStep(0.1);
	m_spLineWidth->setSuffix( tr(" mm"));

    m_colorPicker = new MDColorPicker(this);

	QHBoxLayout* lay = new QHBoxLayout;
	lay->addWidget(m_cbEnabled);
	lay->addWidget(m_lineStyleCombo);
	lay->addWidget(m_spLineWidth);
	lay->addWidget(m_colorPicker);
	lay->setMargin(0);
	lay->setSpacing(3);

	setLayout(lay);
}

QComboBox * NCRDLineStyleEditor::lineStyleCombo()
{
	return m_lineStyleCombo;
}

QDoubleSpinBox* NCRDLineStyleEditor::lineWidthSpinBox()
{
	return m_spLineWidth;
}

MDColorPicker * NCRDLineStyleEditor::colorPicker()
{
	return m_colorPicker;
}

QCheckBox * NCRDLineStyleEditor::enabledCheckBox()
{
	return m_cbEnabled;
}
