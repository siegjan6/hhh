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

#ifndef NCREPORTCONDITIONALFORMAT_H
#define NCREPORTCONDITIONALFORMAT_H

#include <QFont>
#include <QColor>

class NCReportEvaluator;

/*!

section 功能

条件格式管理类
【提供字体／颜色等条件格式接口】

*/
class NCReportConditionalFormat
{
public:
	NCReportConditionalFormat();

	void setFont( const QFont& font );
	QFont font() const;
	QFont& fontRef();
	bool evaluate( const QString &formatCode, NCReportEvaluator* evaluator );
	QString errorMsg() const;
	bool fontChanged() const;
	bool colorChanged() const;
	bool backColorChanged() const;
	QColor color() const;
	QColor backColor() const;

private:
	bool m_fontChanged;
	bool m_colorChanged;
	bool m_backColorChanged;
	QFont m_font;
	QColor m_backColor;
	QColor m_color;
	QString m_error;

	bool evaluateTag( const QString& tag, const QVariant& value );
	void setError( const QString& errorMsg );
};

#endif // NCREPORTCONDITIONALFORMAT_H
