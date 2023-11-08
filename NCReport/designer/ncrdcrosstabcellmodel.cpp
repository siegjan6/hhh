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

#include "ncrdcrosstabcellmodel.h"
#include "ncrdcrosstabdialog.h"
#include "ncrdenumdata.h"

#include <QComboBox>

NCRDCrossTabCellModel::NCRDCrossTabCellModel(QObject *parent) : QAbstractItemModel(parent)
{
	for ( int celltype =0; celltype < NCREPORTCROSSTABITEMCELL_COUNT; ++celltype )
		m_cells[celltype].setType((NCReportCrossTabItemCell::CellType)celltype);
}

NCRDCrossTabCellModel::~NCRDCrossTabCellModel()
{

}

QModelIndex NCRDCrossTabCellModel::index(int row, int column, const QModelIndex &) const
{
	//return QModelIndex();
	return createIndex(row,column,1550);
}

QModelIndex NCRDCrossTabCellModel::parent(const QModelIndex &) const
{
	return QModelIndex();
}

int NCRDCrossTabCellModel::rowCount(const QModelIndex &) const
{
	return NCREPORTCROSSTABITEMCELL_COUNT;
}

int NCRDCrossTabCellModel::columnCount(const QModelIndex &) const
{
	return CELL_DATA_COUNT;
}

QVariant NCRDCrossTabCellModel::data(const QModelIndex &index, int role) const
{
	int i=index.row();

	switch(role) {
	case Qt::DisplayRole:
	case Qt::EditRole:
		break;
	default:
		return QVariant();
	}

	switch (index.column()) {
	case CELL_DATA_CELLNAME:
		return cellName(i);
	case CELL_DATA_ALIGNH: {
			QFlags<Qt::AlignmentFlag> f(m_cells[i].alignment());
			return NCRDEnumData::alignToIndex(f & Qt::AlignHorizontal_Mask);
		}
	case CELL_DATA_ALIGNV: {
			QFlags<Qt::AlignmentFlag> f(m_cells[i].alignment());
			return NCRDEnumData::alignToIndex(f & Qt::AlignVertical_Mask);
		}
	case CELL_DATA_FONTFAMILY:
		return ((NCRDCrossTabCellModel*)this)->m_cells[i].font().family();
		//return ((NCRDCrossTabCellModel*)this)->m_cells[i].font();
	case CELL_DATA_FONTSIZE:
		return ((NCRDCrossTabCellModel*)this)->m_cells[i].font().pointSize();
	case CELL_DATA_BOLD:
		return ((NCRDCrossTabCellModel*)this)->m_cells[i].font().bold();
	case CELL_DATA_ITALIC:
		return ((NCRDCrossTabCellModel*)this)->m_cells[i].font().italic();
	case CELL_DATA_UNDERLINE:
		return ((NCRDCrossTabCellModel*)this)->m_cells[i].font().underline();
	case CELL_DATA_FORECOLOR:
		return m_cells[i].foreColor().name();
	case CELL_DATA_BACKCOLOR:
		return((NCRDCrossTabCellModel*)this)->m_cells[i].brush().color().name();
		//return((NCRDCrossTabCellModel*)this)->m_cells[i].brush().color();
	case CELL_DATA_FILLSTYLE:
        return (int)((NCRDCrossTabCellModel*)this)->m_cells[i].brush().style();
	case CELL_DATA_BGMODE:
		return m_cells[i].BGMode();
	case CELL_DATA_FORMATSTRING:
		return m_cells[i].formatString();
	case CELL_DATA_FIELDWIDTH:
		return m_cells[i].fieldWidth();
	case CELL_DATA_FORMATCODE:
		return QString(m_cells[i].formatCode());
	case CELL_DATA_PRECISION:
		return m_cells[i].precision();
	case CELL_DATA_BLANKIFZERO:
		return m_cells[i].blankIfZero();
	case CELL_DATA_LOCALIZED:
		return m_cells[i].localized();
	case CELL_DATA_FORMATTED:
		return m_cells[i].formatted();
	case CELL_DATA_FILLCHAR:
		return QString(m_cells[i].fillChar());
	case CELL_DATA_DATATYPE:
		return m_cells[i].dataType();
	case CELL_DATA_TEXT:
		return m_cells[i].text();
	case CELL_DATA_ROTATION:
		return m_cells[i].rotation();

	case CELL_BORDER_TOP_LINESTYLE:
        return (int)((NCRDCrossTabCellModel*)this)->m_cells[i].pen(NCReportCrossTabItemCell::TopBorder).style();
	case CELL_BORDER_TOP_LINEWIDTH:
		return ((NCRDCrossTabCellModel*)this)->m_cells[i].lineWidth(NCReportCrossTabItemCell::TopBorder);
	case CELL_BORDER_TOP_LINECOLOR:
		return ((NCRDCrossTabCellModel*)this)->m_cells[i].pen(NCReportCrossTabItemCell::TopBorder).color().name();
	case CELL_BORDER_TOP_ENABLED:
		return ((NCRDCrossTabCellModel*)this)->m_cells[i].enabled(NCReportCrossTabItemCell::TopBorder);

	case CELL_BORDER_RIGHT_LINESTYLE:
        return (int)((NCRDCrossTabCellModel*)this)->m_cells[i].pen(NCReportCrossTabItemCell::RightBorder).style();
	case CELL_BORDER_RIGHT_LINEWIDTH:
		return ((NCRDCrossTabCellModel*)this)->m_cells[i].lineWidth(NCReportCrossTabItemCell::RightBorder);
	case CELL_BORDER_RIGHT_LINECOLOR:
		return ((NCRDCrossTabCellModel*)this)->m_cells[i].pen(NCReportCrossTabItemCell::RightBorder).color().name();
	case CELL_BORDER_RIGHT_ENABLED:
		return ((NCRDCrossTabCellModel*)this)->m_cells[i].enabled(NCReportCrossTabItemCell::RightBorder);

	case CELL_BORDER_BOTTOM_LINESTYLE:
        return (int)((NCRDCrossTabCellModel*)this)->m_cells[i].pen(NCReportCrossTabItemCell::BottomBorder).style();
	case CELL_BORDER_BOTTOM_LINEWIDTH:
		return ((NCRDCrossTabCellModel*)this)->m_cells[i].lineWidth(NCReportCrossTabItemCell::BottomBorder);
	case CELL_BORDER_BOTTOM_LINECOLOR:
		return ((NCRDCrossTabCellModel*)this)->m_cells[i].pen(NCReportCrossTabItemCell::BottomBorder).color().name();
	case CELL_BORDER_BOTTOM_ENABLED:
		return ((NCRDCrossTabCellModel*)this)->m_cells[i].enabled(NCReportCrossTabItemCell::BottomBorder);

	case CELL_BORDER_LEFT_LINESTYLE:
        return (int)((NCRDCrossTabCellModel*)this)->m_cells[i].pen(NCReportCrossTabItemCell::LeftBorder).style();
	case CELL_BORDER_LEFT_LINEWIDTH:
		return ((NCRDCrossTabCellModel*)this)->m_cells[i].lineWidth(NCReportCrossTabItemCell::LeftBorder);
	case CELL_BORDER_LEFT_LINECOLOR:
		return ((NCRDCrossTabCellModel*)this)->m_cells[i].pen(NCReportCrossTabItemCell::LeftBorder).color().name();
	case CELL_BORDER_LEFT_ENABLED:
		return ((NCRDCrossTabCellModel*)this)->m_cells[i].enabled(NCReportCrossTabItemCell::LeftBorder);

	}

	return QVariant();
}

bool NCRDCrossTabCellModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

	switch(role) {
	case Qt::EditRole:
	case Qt::DisplayRole:
		break;
	default:
		return false;
	}

	int i=index.row();

	switch (index.column()) {
	case CELL_DATA_ALIGNH:
	case CELL_DATA_ALIGNV: {
			QFlags<Qt::AlignmentFlag> f(m_cells[i].alignment());
			Qt::Alignment alignH = f & Qt::AlignHorizontal_Mask;
			Qt::Alignment alignV = f & Qt::AlignVertical_Mask;
			if (index.column()==CELL_DATA_ALIGNH)
				m_cells[i].setAlignment( NCRDEnumData::indexToAlignH(value.toInt()) | alignV );
			else
				m_cells[i].setAlignment( alignH | NCRDEnumData::indexToAlignV(value.toInt()) );
			break;
		}
	case CELL_DATA_FONTFAMILY:
	{
		m_cells[i].font().setFamily(value.toString());
		//m_cells[i].setFont(value);
		QString ff = m_cells[i].font().family();
		break;
	}
	case CELL_DATA_FONTSIZE:
		m_cells[i].font().setPointSize(value.toInt()); break;
	case CELL_DATA_BOLD:
		m_cells[i].font().setBold(value.toBool()); break;
	case CELL_DATA_ITALIC:
		m_cells[i].font().setItalic(value.toBool()); break;
	case CELL_DATA_UNDERLINE:
		m_cells[i].font().setUnderline(value.toBool()); break;
	case CELL_DATA_FORECOLOR:
		m_cells[i].setForeColor( QColor(value.toString()) ); break;
	case CELL_DATA_BACKCOLOR:
		m_cells[i].brush().setColor( QColor(value.toString()) ); break;
	case CELL_DATA_FILLSTYLE:
		m_cells[i].brush().setStyle( NCRDEnumData::indexToBrushStyle(value.toInt()) ); break;
	case CELL_DATA_BGMODE:
		m_cells[i].setBGMode( NCRDEnumData::indexToBGMode(value.toInt()) ); break;
	case CELL_DATA_FORMATSTRING:
		m_cells[i].setFormatString(value.toString()); break;
	case CELL_DATA_FIELDWIDTH:
		m_cells[i].setFieldWidth(value.toInt()); break;
	case CELL_DATA_FORMATCODE:
		m_cells[i].setFormatCode(value.toString().at(0).toLatin1()); break;
	case CELL_DATA_PRECISION:
		m_cells[i].setPrecision(value.toInt()); break;
	case CELL_DATA_BLANKIFZERO:
		m_cells[i].setBlankIfZero(value.toBool()); break;
	case CELL_DATA_LOCALIZED:
		m_cells[i].setLocalized(value.toBool()); break;
	case CELL_DATA_FORMATTED:
		m_cells[i].setFormatted(value.toBool()); break;
	case CELL_DATA_FILLCHAR:
		m_cells[i].setFillChar(value.toString().at(0)); break;
	case CELL_DATA_DATATYPE:
		m_cells[i].setDataType( NCRDEnumData::indexToDataType(value.toInt()) ); break;
	case CELL_DATA_TEXT:
		m_cells[i].setText(value.toString()); break;
	case CELL_DATA_ROTATION:
		m_cells[i].setRotation(value.toDouble()); break;

	case CELL_BORDER_TOP_LINESTYLE:
		m_cells[i].pen(NCReportCrossTabItemCell::TopBorder).setStyle(NCRDEnumData::indexToPenStyle(value.toInt()));
		break;
	case CELL_BORDER_TOP_LINEWIDTH:
		m_cells[i].setLineWidth(value.toDouble(), NCReportCrossTabItemCell::TopBorder);
		break;
	case CELL_BORDER_TOP_LINECOLOR:
		m_cells[i].pen(NCReportCrossTabItemCell::TopBorder).setColor( value.toString() );
		break;
	case CELL_BORDER_TOP_ENABLED:
		m_cells[i].setEnabled(value.toBool(), NCReportCrossTabItemCell::TopBorder);
		break;

	case CELL_BORDER_RIGHT_LINESTYLE:
		m_cells[i].pen(NCReportCrossTabItemCell::RightBorder).setStyle(NCRDEnumData::indexToPenStyle(value.toInt()));
		break;
	case CELL_BORDER_RIGHT_LINEWIDTH:
		m_cells[i].setLineWidth(value.toDouble(), NCReportCrossTabItemCell::RightBorder);
		break;
	case CELL_BORDER_RIGHT_LINECOLOR:
		m_cells[i].pen(NCReportCrossTabItemCell::RightBorder).setColor( value.toString() );
		break;
	case CELL_BORDER_RIGHT_ENABLED:
		m_cells[i].setEnabled(value.toBool(), NCReportCrossTabItemCell::RightBorder);
		break;

	case CELL_BORDER_BOTTOM_LINESTYLE:
		m_cells[i].pen(NCReportCrossTabItemCell::BottomBorder).setStyle(NCRDEnumData::indexToPenStyle(value.toInt()));
		break;
	case CELL_BORDER_BOTTOM_LINEWIDTH:
		m_cells[i].setLineWidth(value.toDouble(), NCReportCrossTabItemCell::BottomBorder);
		break;
	case CELL_BORDER_BOTTOM_LINECOLOR:
		m_cells[i].pen(NCReportCrossTabItemCell::BottomBorder).setColor( value.toString() );
		break;
	case CELL_BORDER_BOTTOM_ENABLED:
		m_cells[i].setEnabled(value.toBool(), NCReportCrossTabItemCell::BottomBorder);
		break;

	case CELL_BORDER_LEFT_LINESTYLE:
		m_cells[i].pen(NCReportCrossTabItemCell::LeftBorder).setStyle(NCRDEnumData::indexToPenStyle(value.toInt()));
		break;
	case CELL_BORDER_LEFT_LINEWIDTH:
		m_cells[i].setLineWidth(value.toDouble(), NCReportCrossTabItemCell::LeftBorder);
		break;
	case CELL_BORDER_LEFT_LINECOLOR:
		m_cells[i].pen(NCReportCrossTabItemCell::LeftBorder).setColor( value.toString() );
		break;
	case CELL_BORDER_LEFT_ENABLED:
		m_cells[i].setEnabled(value.toBool(), NCReportCrossTabItemCell::LeftBorder);
		break;

	}

	return true;
}

QString NCRDCrossTabCellModel::cellName(int row) const
{
	NCReportCrossTabItemCell::CellType type = (NCReportCrossTabItemCell::CellType)row;
	QString n;
	switch(type) {
	case NCReportCrossTabItemCell::CornerHeader: n=tr("Corner header"); break;
	case NCReportCrossTabItemCell::ColumnHeader: n=tr("Column header"); break;
	case NCReportCrossTabItemCell::SideSummaryHeader: n=tr("Side summary header"); break;
	case NCReportCrossTabItemCell::RowHeader: n=tr("Row header"); break;
	case NCReportCrossTabItemCell::Data: n=tr("Data"); break;
	case NCReportCrossTabItemCell::SideSummaryData: n=tr("Side summary data"); break;
	case NCReportCrossTabItemCell::BottomSummaryHeader: n=tr("Bottom summary header"); break;
	case NCReportCrossTabItemCell::BottomSummaryData: n=tr("Bottom summary data"); break;
	case NCReportCrossTabItemCell::CrossSummaryData: n=tr("Cross summary data"); break;
	}
	return n;
}

QVariant NCRDCrossTabCellModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	Q_UNUSED(section);
	Q_UNUSED(orientation);
	Q_UNUSED(role);

	return QVariant();
}

void NCRDCrossTabCellModel::setCell(const NCReportCrossTabItemCell &cell, int cellType)
{
	m_cells[cellType] = cell;
}

const NCReportCrossTabItemCell & NCRDCrossTabCellModel::cell(int cellType) const
{
	return m_cells[cellType];
}





/*!
  Model item delegate class for editing cross tab cell properties
  */
/*
NCRDCrossTabCellModelDelegate::NCRDCrossTabCellModelDelegate( QObject *parent ) : QItemDelegate( parent )
{
}

QWidget * NCRDCrossTabCellModelDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	return QItemDelegate::createEditor( parent, option, index);
}

void NCRDCrossTabCellModelDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{

	if ( ((NCRDCrossTabCellModelDelegate*)this)->isEnumComboBox(index) ) {
		QComboBox *combo = qobject_cast<QComboBox*>(editor);
		if ( combo ) {
			int value = index.data(Qt::EditRole).toInt();
			combo->setCurrentIndex( value );
			//combo->setCurrentIndex( combo->findData(value,Qt::EditRole) );
		}
	} else {

		QItemDelegate::setEditorData( editor, index);
	}
}

void NCRDCrossTabCellModelDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{

	if ( ((NCRDCrossTabCellModelDelegate*)this)->isEnumComboBox(index) ) {
		QComboBox *combo = qobject_cast<QComboBox*>(editor);
		if ( combo ) {
			model->setData( index, combo->currentIndex() );
			//model->setData( index, combo->itemData( combo->currentIndex(), Qt::EditRole) );
		}
	} else {

		QItemDelegate::setModelData( editor, model, index);
	}
}

void NCRDCrossTabCellModelDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	QItemDelegate::updateEditorGeometry( editor, option, index );
}

bool NCRDCrossTabCellModelDelegate::isEnumComboBox(const QModelIndex & index)
{
	switch ( index.column() ) {
	case CELL_DATA_ALIGNH:
	case CELL_DATA_ALIGNV:
	case CELL_DATA_FILLSTYLE:
	case CELL_DATA_BGMODE:
	case CELL_DATA_DATATYPE:
		return true;
	default:
		break;
	}
	return false;
}



*/
