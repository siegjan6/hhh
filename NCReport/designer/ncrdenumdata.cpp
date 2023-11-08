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

#include "ncrdenumdata.h"

#include <QObject>

NCRDEnumData::NCRDEnumData()
{
}

Qt::Alignment NCRDEnumData::indexToAlignH(int index)
{
	switch (index) {
	case 0: return Qt::AlignLeft;
	case 1: return Qt::AlignRight;
	case 2: return Qt::AlignCenter;
	case 3: return Qt::AlignJustify;
	}
	return Qt::AlignLeft;
}

Qt::Alignment NCRDEnumData::indexToAlignV(int index)
{
	switch (index) {
	case 0: return Qt::AlignTop;
	case 1: return Qt::AlignBottom;
	case 2: return Qt::AlignVCenter;
	}
	return Qt::AlignTop;
}

int NCRDEnumData::alignToIndex(const Qt::Alignment align)
{
	switch(align) {
	case Qt::AlignLeft: return 0;
	case Qt::AlignRight: return 1;
	case Qt::AlignHCenter: return 2;
	case Qt::AlignJustify: return 3;
	case Qt::AlignTop: return 0;
	case Qt::AlignBottom: return 1;
	case Qt::AlignVCenter: return 2;
	}
	return 0;
}

QString NCRDEnumData::alignToText(const Qt::Alignment align)
{
	switch(align) {
	case Qt::AlignLeft: return QObject::tr("Left");
	case Qt::AlignRight: return QObject::tr("Right");
	case Qt::AlignHCenter: return QObject::tr("H.Center");
	case Qt::AlignJustify: return QObject::tr("Justify");
	case Qt::AlignTop: return QObject::tr("Top");
	case Qt::AlignBottom: return QObject::tr("Bottom");
	case Qt::AlignVCenter: return QObject::tr("V.Center");
	}
	return QString();
}

Qt::BrushStyle NCRDEnumData::indexToBrushStyle(int index)
{
	if (index>=0 && index <=17)
		return Qt::BrushStyle(index);

	return Qt::NoBrush;
}

Qt::BGMode NCRDEnumData::indexToBGMode(int index)
{
	return (Qt::BGMode)index;
}

NCReportData::DataType NCRDEnumData::indexToDataType(int index)
{
	return (NCReportData::DataType)index;
}

Qt::PenStyle NCRDEnumData::indexToPenStyle(int index)
{
	return (Qt::PenStyle)index;
}



