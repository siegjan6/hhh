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

#ifndef NCRDLINESTYLEEDITOR_H
#define NCRDLINESTYLEEDITOR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
class QDoubleSpinBox;
QT_END_NAMESPACE

class MDColorPicker;

/*!

\section 功能

线条控件风格配置编辑类

*/
class NCRDLineStyleEditor : public QWidget
{
    Q_OBJECT
public:
    explicit NCRDLineStyleEditor(QWidget *parent = 0);

	QCheckBox* enabledCheckBox();
	QComboBox* lineStyleCombo();
	QDoubleSpinBox* lineWidthSpinBox();
    MDColorPicker* colorPicker();

private:
	QCheckBox* m_cbEnabled;
	QComboBox* m_lineStyleCombo;
	QDoubleSpinBox* m_spLineWidth;
    MDColorPicker* m_colorPicker;
};

#endif // NCRDLINESTYLEEDITOR_H
