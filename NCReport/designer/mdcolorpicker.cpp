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
#include "mdcolorpicker.h"

#include <QToolButton>
#include <QColorDialog>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QKeyEvent>
#include <QApplication>

MDColorPicker::MDColorPicker(QWidget *parent) :
    QWidget(parent)
{
	m_btnColor = new QToolButton(this);
	m_btnColor->setToolTip( tr("Choose color..."));
	connect(m_btnColor, SIGNAL(clicked()), this, SLOT(colorDia()));

	m_colorEdit = new QLineEdit(this);
	m_colorEdit->setInputMask(">\\#HHHHHH");
	//m_colorEdit->setMaximumWidth(120);
	connect(m_colorEdit,SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
	QHBoxLayout *lay = new QHBoxLayout;
	lay->addWidget(m_colorEdit);
	lay->addWidget(m_btnColor);
	lay->setMargin(0);
	lay->setSpacing(0);

	setLayout(lay);

	setColor(Qt::black);
}

MDColorPicker::~MDColorPicker()
{

}

QLineEdit * MDColorPicker::colorEdit()
{
	return m_colorEdit;
}

void MDColorPicker::colorDia()
{
	QColor col = QColorDialog::getColor(m_color, parentWidget() );
	if (!col.isValid())
		return;

	privateColorChanged(col,true);
	m_colorEdit->setModified(true);
	m_colorEdit->setFocus();
	m_colorEdit->selectAll();
	//QKeyEvent e(QEvent::KeyPress,Qt::Key_Space,Qt::NoModifier);
	//QApplication::sendEvent(m_colorEdit,&e);
}

const QColor & MDColorPicker::color() const
{
	return m_color;
}

void MDColorPicker::setColor(const QColor &color)
{
	if (color.isValid())
		privateColorChanged(color,true);
}

void MDColorPicker::textChanged(const QString &txt)
{
	if (txt.length()==4 || txt.length()==7 ) {
		QColor c;
		c.setNamedColor(txt);
		if (c.isValid())
			privateColorChanged(c,false);
	}
}

void MDColorPicker::privateColorChanged(const QColor &color, bool setColorName)
{
	QPixmap pix(14, 14);
	pix.fill(color);
	m_btnColor->setIcon(pix);
	m_color = color;

	if (setColorName) {
		m_colorEdit->blockSignals(true);
		m_colorEdit->setText(color.name().toUpper());
		m_colorEdit->blockSignals(false);
	}

	emit colorChanged(color);
}

