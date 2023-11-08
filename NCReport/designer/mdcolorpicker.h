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

#ifndef MDCOLORPICKER_H
#define MDCOLORPICKER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QToolButton;
class QLineEdit;
QT_END_NAMESPACE

/*!

section 功能

【颜色选择器】

*/
class MDColorPicker : public QWidget
{
    Q_OBJECT
	Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged )
public:
    explicit MDColorPicker(QWidget *parent = 0);
    ~MDColorPicker();

	QLineEdit* colorEdit();
	const QColor& color() const;

public Q_SLOTS:
	void setColor(const QColor& color);

Q_SIGNALS:
	void colorChanged( const QColor& );

private slots:
	void colorDia();
	void textChanged(const QString& txt);

private:
	QLineEdit* m_colorEdit;
	QToolButton* m_btnColor;
	QColor m_color;

	void privateColorChanged(const QColor& color, bool setColorName);

};

#endif // MDCOLORPICKER_H
