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
#ifndef NCRDHIGHCHARTSITEMDIALOG_H
#define NCRDHIGHCHARTSITEMDIALOG_H

#ifdef HIGHCHARTS_INTEGRATION

#include <QDialog>
#include <QList>
#include <QString>

QT_BEGIN_NAMESPACE
class QFormLayout;
class QComboBox;
class QScriptValue;
namespace Ui {
    class NCRDHighChartsItemDialog;
}
QT_END_NAMESPACE

class NCReportHighChartsManager;
class NCRDDocument;
class NCReportItem;
class NCReportChartItem;
class NCRDFileSelector;

/*!

\section 功能

报表高亮某一控件对话框配置类

*/
class NCRDHighChartsItemDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit NCRDHighChartsItemDialog(NCReportItem *item, NCRDDocument* parent = 0);
	~NCRDHighChartsItemDialog();
	enum DataType { String=0, Number, Boolean };

	struct WidgetInfo
	{
		WidgetInfo( const QString& wID, DataType dt, QWidget* w) : id(wID), dataType(dt), widget(w) {}

		QString id;
		DataType dataType;
		QWidget* widget;
	};


public slots:
	void accept();
	void reject();

	void slotUpdateChart();

private slots:
	void renderChart();
	void slotTabChanged(int);
	void templateFileChanged(const QString& fileName);
	//void resultFileChanged(const QString& fileName);

private:
	Ui::NCRDHighChartsItemDialog *ui;
	NCRDDocument *m_doc;
	NCReportHighChartsManager* manager;
	NCReportChartItem* m_item;
	NCRDFileSelector *selectorTemplate;
	NCRDFileSelector *selectorResult;

	QList<WidgetInfo> widgets;
	QList<QWidget*> containters;
	QList<QFormLayout*> layouts;

	bool buildUi(const QString &reportData);
	void loadChartTypeCombo(QComboBox* combo, const QScriptValue& value, const QString &currentValue);
	bool generateChartScript(QString &output);
	void deleteContainters();
	bool updateChart(QString &chartScript);
};

#endif // HIGHCHARTS_INTEGRATION
#endif // NCRDHIGHCHARTSITEMDIALOG_H
