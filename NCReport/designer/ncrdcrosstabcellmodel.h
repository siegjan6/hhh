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

#ifndef NCRDCROSSTABCELLMODEL_H
#define NCRDCROSSTABCELLMODEL_H

#include <QAbstractItemModel>
#include <QItemDelegate>
#include "ncreportcrosstabitem.h"
#include "ncreportcrosstabitemcell.h"

#define CELL_DATA_CELLNAME		0
#define CELL_DATA_ALIGNH		1
#define CELL_DATA_ALIGNV		2
#define CELL_DATA_FONTFAMILY	3
#define CELL_DATA_FONTSIZE		4
#define CELL_DATA_BOLD			5
#define CELL_DATA_ITALIC		6
#define CELL_DATA_UNDERLINE		7
#define CELL_DATA_FORECOLOR		8
#define CELL_DATA_BACKCOLOR		9
#define CELL_DATA_FILLSTYLE		10
#define CELL_DATA_BGMODE		11
#define CELL_DATA_FORMATSTRING	12
#define CELL_DATA_WORDBREAK		13
#define CELL_DATA_FIELDWIDTH	14
#define CELL_DATA_FORMATCODE	15
#define CELL_DATA_PRECISION		16
#define CELL_DATA_BLANKIFZERO	17
#define CELL_DATA_LOCALIZED		18
#define CELL_DATA_FORMATTED		19
#define CELL_DATA_FILLCHAR		20
#define CELL_DATA_DATATYPE		21
#define CELL_DATA_TEXT			22

#define CELL_BORDER_TOP_LINESTYLE		23
#define CELL_BORDER_TOP_LINEWIDTH		24
#define CELL_BORDER_TOP_LINECOLOR		25
#define CELL_BORDER_TOP_ENABLED			26

#define CELL_BORDER_RIGHT_LINESTYLE		27
#define CELL_BORDER_RIGHT_LINEWIDTH		28
#define CELL_BORDER_RIGHT_LINECOLOR		29
#define CELL_BORDER_RIGHT_ENABLED		30

#define CELL_BORDER_BOTTOM_LINESTYLE	31
#define CELL_BORDER_BOTTOM_LINEWIDTH	32
#define CELL_BORDER_BOTTOM_LINECOLOR	33
#define CELL_BORDER_BOTTOM_ENABLED		34

#define CELL_BORDER_LEFT_LINESTYLE		35
#define CELL_BORDER_LEFT_LINEWIDTH		36
#define CELL_BORDER_LEFT_LINECOLOR		37
#define CELL_BORDER_LEFT_ENABLED		38

#define CELL_DATA_ROTATION				39

#define CELL_DATA_COUNT					40

QT_BEGIN_NAMESPACE
class QComboBox;
QT_END_NAMESPACE

/*!

\section 功能

交叉表控件数据模型类
【继承自QAbstractItemModel类，用于管理交叉表控件中呈现的相关数据】。
＃＃＃暂时屏蔽了交叉表控件数据模型类＃＃＃

*/
class NCRDCrossTabCellModel : public QAbstractItemModel
{
    Q_OBJECT
public:
	explicit NCRDCrossTabCellModel(QObject *parent = 0);
	~NCRDCrossTabCellModel();

	QModelIndex index(int row, int column, const QModelIndex &parent) const;
	QModelIndex parent(const QModelIndex &child) const;
	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;
	bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;

	QString cellName(int row) const;
	Qt::Alignment comboValueHAlignment(int index) const;

	void setCell( const NCReportCrossTabItemCell& cell, int cellType );
	const NCReportCrossTabItemCell& cell(int cellType) const;

private:
	NCReportCrossTabItemCell m_cells[NCREPORTCROSSTABITEMCELL_COUNT];
};


/*!
Item delegate class for handling data widget delegation in cross tab dialog
*/
/*
class NCRDCrossTabCellModelDelegate : public QItemDelegate
{
public:
	NCRDCrossTabCellModelDelegate( QObject *parent );
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:

	bool isEnumComboBox( const QModelIndex & index );
};
*/

#endif // NCRDCROSSTABCELLMODEL_H
