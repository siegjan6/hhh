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

#include "ncreportpivottablemodel.h"
#include <math.h>

NCReportPivotTableModel::NCReportPivotTableModel(QObject *parent) :
	QAbstractItemModel(parent), m_aggregation(NCReportVariable::Sum)
{
}

NCReportPivotTableModel::~NCReportPivotTableModel()
{
	m_rows.clear();
	m_columns.clear();
	m_data.clear();
}

QModelIndex NCReportPivotTableModel::index(int row, int column, const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return createIndex(row,column);
}

QModelIndex NCReportPivotTableModel::parent(const QModelIndex &child) const
{
	Q_UNUSED(child);
	return QModelIndex();
}

int NCReportPivotTableModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return m_rows.count();
}

int NCReportPivotTableModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return m_columns.count();
}

QVariant NCReportPivotTableModel::data(const QModelIndex &index, int role) const
{
	switch(role) {
	case Qt::DisplayRole:
	case Qt::EditRole:
		break;
	default:
		return QVariant();
	}

	if (!isValidIndex(index))
		return QVariant();

	PivotTableData d = m_data.value(index);

	switch (m_aggregation) {
	case NCReportVariable::Count: return d.counter;
	case NCReportVariable::Average: return average(d);
	case NCReportVariable::Std: return stDeviation(d);
	case NCReportVariable::Sum:
	case NCReportVariable::Min:
	case NCReportVariable::Max:
	case NCReportVariable::System: return d.value;
	}


	return m_data.value(index).value;
}

bool NCReportPivotTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	switch(role) {
	case Qt::DisplayRole:
	case Qt::EditRole:
		break;
	default:
		return false;
	}

	if (!isValidIndex(index))
		return false;

	m_data[index] = PivotTableData(value.toDouble(),0);
	return true;
}

/*!
  Appends pivot table
  */
bool NCReportPivotTableModel::addData(const QVariant &rowData, const QVariant &columnData, double value)
{
	int row = m_rows.indexOf(rowData);
	if (row<0) {
		m_rows.append(rowData);
		row = m_rows.count()-1;
		//qSort(m_rows.begin(), m_rows.end());
		//row = m_rows.indexOf(rowData);
	}
	int column = m_columns.indexOf(columnData);
	if (column<0) {
		m_columns.append(columnData);
		column = m_columns.count()-1;
		//qSort(m_columns.begin(), m_columns.end());
		//column = m_columns.indexOf(columnData);
	}
	QModelIndex currentIndex = index(row,column,QModelIndex());
	if (m_data.contains(currentIndex)) {

		m_data[currentIndex].counter++;

		switch (m_aggregation) {
		case NCReportVariable::Min:
			m_data[currentIndex].value = qMin(value,m_data[currentIndex].value);
			break;
		case NCReportVariable::Max:
			m_data[currentIndex].value = qMax(value,m_data[currentIndex].value);
			break;
		default:
			m_data[currentIndex].value += value;
			break;
		}

	} else {
		m_data[currentIndex] = PivotTableData(value,1);
	}

	return true;
}

void NCReportPivotTableModel::setAggregation(int function)
{
	m_aggregation = (NCReportVariable::FunctionType)function;
}

double NCReportPivotTableModel::stDeviation(const NCReportPivotTableModel::PivotTableData &data) const
{
	//Formula 1:
	//sqrt(((pow(d.value,2.0)) -(( 1.0/d.counter) * (pow(d.value,2.0))))/ (d.counter -1.0));

	//s = sqrt ( { sum (x^2) - [sum (x)]^2 / n } / (n - 1) )
	double S;
	double U;
	U =  (data.value - average(data) )*ceil (2.0);
	S = (sqrt(U / data.counter));
	return S;
}

double NCReportPivotTableModel::average(const NCReportPivotTableModel::PivotTableData &data) const
{
	return (data.counter>0 ? data.value/data.counter : 0.0 );
}


QVariant NCReportPivotTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	switch(role) {
	case Qt::DisplayRole:
	case Qt::EditRole:
		break;
	default:
		return QVariant();
	}

	switch (orientation) {
	case Qt::Horizontal: return m_columns.at(section);
	case Qt::Vertical: return m_rows.at(section);
	}

	return QVariant();
}

bool NCReportPivotTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
	Q_UNUSED(section);

	switch(role) {
	case Qt::DisplayRole:
	case Qt::EditRole:
		break;
	default:
		return false;
	}

	QList<QVariant>* headerList=0;
	switch (orientation) {
	case Qt::Horizontal: headerList = &m_columns;
	case Qt::Vertical: headerList = &m_rows;
	}

	if (!headerList->contains(value)) {
		headerList->append(value);
	}

	return true;

}


bool NCReportPivotTableModel::isValidIndex(const QModelIndex &index) const
{
	if (index.row()>m_rows.count()-1)
		return false;

	if (index.column()>m_columns.count()-1)
		return false;

	return true;
}





