/****************************************************************************
*
*  Copyright (C) 2002-2012 Helta Kft. / NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: norbert@nocisoft.com, office@nocisoft.com
*  Web: www.nocisoft.com
*
*  This file is part of the NCReport Report Generator System
*
*  Licensees holding a valid NCReport License Agreement may use this
*  file in accordance with the rights, responsibilities, and obligations
*  contained therein. Please consult your licensing agreement or contact
*  norbert@nocisoft.com if any conditions of this licensing are not clear
*  to you.
*
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/
#include "ncrdfileselector.h"

#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>

NCRDFileSelector::NCRDFileSelector(QWidget *parent, QLineEdit *le, QAbstractButton *btn ) : QObject(parent),
	m_lineEditFileName(le), m_btnSelect(btn), m_parentWidget(parent), m_selectMode(Open)
{
	connect(m_btnSelect,SIGNAL(clicked()),this,SLOT(selectFile()));
	connect(m_lineEditFileName,SIGNAL(textChanged(QString)),this,SLOT(slotFileNameChanged(QString)));
}

/*
void NCRDFileSelector::setWidgets(QLineEdit *le, QAbstractButton *btn)
{
	m_btnSelect = btn;
	m_lineEditFileName = le;
}

void NCRDFileSelector::setButton(QAbstractButton *btn)
{
	m_btnSelect = btn;
}

void NCRDFileSelector::setLineEdit(QLineEdit *le)
{
	m_lineEditFileName = le;
}
*/
void NCRDFileSelector::setEditMode(NCRDFileSelector::EditMode mode)
{
	m_lineEditFileName->setReadOnly(mode == SelectOnly);
}

void NCRDFileSelector::setSelectMode(NCRDFileSelector::SelectMode mode)
{
	m_selectMode = mode;
}

void NCRDFileSelector::setFileName(const QString & fileName)
{
	m_lineEditFileName->setText(fileName);
}

QString NCRDFileSelector::fileName() const
{
	return m_lineEditFileName->text();
}

void NCRDFileSelector::setDescription(const QString &desc)
{
	m_desc = desc;
}

void NCRDFileSelector::setFileTypes(const QString &type)
{
	m_type = type;
}

void NCRDFileSelector::selectFile()
{
	QString fname;

	if (m_selectMode==Open)
		fname = QFileDialog::getOpenFileName(m_parentWidget, m_desc, m_lineEditFileName->text(), m_type );
	else if (m_selectMode==Save)
		fname = QFileDialog::getSaveFileName(m_parentWidget, m_desc, m_lineEditFileName->text(), m_type );

	if (!fname.isEmpty())
		m_lineEditFileName->setText(fname);

}

void NCRDFileSelector::slotFileNameChanged(const QString &fname)
{
	emit fileNameChanged(fname);
}
