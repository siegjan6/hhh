/****************************************************************************
**
** Copyright (C) 2007-2007 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** Licensees holding a valid Qt License Agreement may use this file in
** accordance with the rights, responsibilities and obligations
** contained therein.  Please consult your licensing agreement or
** contact sales@trolltech.com if any conditions of this licensing
** agreement are not clear to you.
**
** Further information about Qt licensing is available at:
** http://www.trolltech.com/products/qt/licensing.html or by
** contacting info@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef NCRDCOLORCOMBO_H
#define NCRDCOLORCOMBO_H

#include <QComboBox>
QT_BEGIN_NAMESPACE
class QColor;
class QWidget;
QT_END_NAMESPACE

/*!

\section 功能

颜色选择下拉框类
【管理颜色选择中的下拉框相关属性，继承自QComboBox基类】。

*/
class NCRDColorCombo : public QComboBox
{
	Q_OBJECT
	Q_PROPERTY(QColor color READ color WRITE setColor USER true)

public:
	NCRDColorCombo(QWidget *widget = 0);

public:
	QColor color() const;
	void setColor(QColor c);

private:
	void populateList();
};

#endif 
