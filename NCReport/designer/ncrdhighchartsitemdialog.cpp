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
#ifdef HIGHCHARTS_INTEGRATION

#include "ncrdhighchartsitemdialog.h"
#include "ui_ncrdhighchartsitemdialog.h"
#include "ncreporthighchartsmanager.h"
#include "ncreportchartitem.h"
#include "ncrddocument.h"
#include "ncrdformgeometry.h"
#include "ncrdfileselector.h"
#include "ncrddef.h"

#include <QFormLayout>
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueIterator>
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QDebug>
#include <QFlags>
#include <QMessageBox>

NCRDHighChartsItemDialog::NCRDHighChartsItemDialog(NCReportItem *item, NCRDDocument* parent) : QDialog(parent),
	ui(new Ui::NCRDHighChartsItemDialog), m_doc(parent), /*hcm(0),*/ m_item((NCReportChartItem*)item)
{
	ui->setupUi(this);
	ui->chartPreview->setBackgroundRole(QPalette::Base);
	ui->scrollAreaWidgetContents->setBackgroundRole(QPalette::Base);
	ui->scriptEditor->installHighlighter();

	selectorTemplate = new NCRDFileSelector(this, ui->lineEditTemplateFile, ui->btnSelectTemplate );
	selectorTemplate->setFileTypes("*.html *.xhtml");
	selectorTemplate->setDescription( tr("XHTML Template files") );
	selectorTemplate->setEditMode(NCRDFileSelector::SelectOnly);
	selectorTemplate->setSelectMode(NCRDFileSelector::Open);

	selectorResult = new NCRDFileSelector(this, ui->lineEditResultFile, ui->btnSelectResult);
	selectorResult->setFileTypes("*.html");
	selectorResult->setDescription( tr("HTML Result files") );
	selectorResult->setEditMode(NCRDFileSelector::TypeAndSelect);
	selectorResult->setSelectMode(NCRDFileSelector::Save);

	ui->lineEditResultFile->setVisible(false);
	ui->btnSelectResult->setVisible(false);
	ui->lblResultFile->setVisible(false);

	containters << ui->w1;
	containters << ui->w2;
	containters << ui->w3;

	QFormLayout *l1 = new QFormLayout(ui->w1);
	l1->setSpacing(3);
	layouts << l1;
	QFormLayout *l2 = new QFormLayout(ui->w2);
	l2->setSpacing(3);
	layouts << l2;
	QFormLayout *l3 = new QFormLayout(ui->w3);
	l3->setSpacing(3);
	layouts << l3;

	selectorTemplate->setFileName(m_item->templateFile());
	selectorResult->setFileName(m_item->resultFile());

	manager = new NCReportHighChartsManager(this);
	manager->setReportDef(parent->reportDef());
	manager->setTemplateFile(m_item->templateFile());
	//manager->setResultFile(m_item->resultFile());
	ui->printWhenPanel->printWhen->setText( m_item->printWhen() );
	ui->printWhenPanel->zoneID->setValue( m_item->zoneID() );

	connect(manager,SIGNAL(chartIsReady()),this,SLOT(renderChart()));
	connect(ui->btnUpdate,SIGNAL(clicked()),this,SLOT(slotUpdateChart()));
	connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(slotTabChanged(int)));
	connect(selectorTemplate,SIGNAL(fileNameChanged(QString)),this,SLOT(templateFileChanged(QString)));
	//connect(selectorResult,SIGNAL(fileNameChanged(QString)),this,SLOT(resultFileChanged(QString)));

	templateFileChanged(m_item->templateFile());
	//buildUi(m_item->text());

	NCRDFormGeometry *fg = new NCRDFormGeometry(this);
	connect(this,SIGNAL(finished(int)),fg,SLOT(save()));
	fg->restore();
}

NCRDHighChartsItemDialog::~NCRDHighChartsItemDialog()
{
	delete ui;
}

void NCRDHighChartsItemDialog::accept()
{
	QString chartScript;
	if (updateChart(chartScript)) {
		m_item->setTemplateFile(selectorTemplate->fileName());
		//m_item->setResultFile(selectorResult->fileName());
		m_item->setText(chartScript);
		m_item->setPrintWhen( ui->printWhenPanel->printWhen->text() );
		m_item->setZoneID( ui->printWhenPanel->zoneID->value() );

		m_item->updateChart(chartScript, m_item->highChartsManager());
		m_doc->setDocumentModified();
		QDialog::accept();
	}
}

void NCRDHighChartsItemDialog::reject()
{
	QDialog::reject();
}


void NCRDHighChartsItemDialog::slotUpdateChart()
{
	QString chartScript;
	if (updateChart(chartScript))
		ui->tabWidget->setCurrentWidget(ui->tabPreview);

	if (manager->error())

		QMessageBox::warning(this, tr("Chart error"), manager->errorMsg());
}

bool NCRDHighChartsItemDialog::updateChart(QString& chartScript)
{
	manager->setError("");
	if (generateChartScript(chartScript)) {
		m_item->updateChart(chartScript, manager);
	} else {
		return false;
	}
	return true;
}

void NCRDHighChartsItemDialog::slotTabChanged(int idx)
{
	if (idx==2 && manager->chartSvg().isEmpty()) {
		slotUpdateChart();
	}
}

void NCRDHighChartsItemDialog::templateFileChanged(const QString &fileName)
{
	manager->setTemplateFile(fileName);
	if (!buildUi(m_item->text()))
		QMessageBox::warning(this, tr("Chart error"), manager->errorMsg());
}

/*
void NCRDHighChartsItemDialog::resultFileChanged(const QString &fileName)
{
	manager->setResultFile(fileName);
}
*/

bool NCRDHighChartsItemDialog::buildUi( const QString& reportData )
{
	if (manager->templateFile().isEmpty())
		return true;

	const short maxItemsPerLayout = 12;
	const short maxLayouts = 3;

	QScriptEngine engine;
	QScriptValue global;
	if (!manager->parseTemplateMetaData(engine, global))
		return false;

	if (!reportData.isEmpty())	{
		if (!manager->parseReportData(engine, global, reportData))
			return false;
	}

	QScriptValue tplParameters = global.property("templateMetaData").property("parameters");
	QScriptValue rptParameters = global.property("reportData").property("parameters");

	ui->lineEditChartTitle->setText(global.property("reportData").property("title").toString());

	QScriptValueIterator it(rptParameters);
	int itemCount =0;
	int currentLayout=0;

	deleteContainters();
	widgets.clear();

	while (it.hasNext()) {
		it.next();

		QScriptValue v = it.value();
		QWidget* uiWidget=0;
		QString propertyName;
		QString toolTipDesc;
		DataType dataType = String;

		if (it.name()!="chartType" && tplParameters.property(it.name()).isValid()) {
			// There is representation in templateMetaData
			propertyName = tplParameters.property(it.name()).property("name").toString();
			toolTipDesc = tplParameters.property(it.name()).property("description").toString();
		} else {
			propertyName = it.name();
			propertyName = manager->slicedByUpperPos(propertyName);
		}

		if (it.name()=="chartType")	{
			uiWidget = new QComboBox(containters.at(currentLayout));
			loadChartTypeCombo( ((QComboBox*)uiWidget), tplParameters.property("chartType"), v.toString() );

		} else if (v.isBool()) {
			uiWidget = new QCheckBox(containters.at(currentLayout));
			((QCheckBox*)uiWidget)->setText(propertyName);
			((QCheckBox*)uiWidget)->setChecked(v.toBool());
			propertyName = "";
			dataType = Boolean;

		} else if (v.isString()) {
			uiWidget = new QLineEdit(containters.at(currentLayout));
			((QLineEdit*)uiWidget)->setText(v.toString());
		} else if (v.isNumber()) {
			//uiWidget = new QSpinBox(widgets.at(currentLayout));
			uiWidget = new QLineEdit(containters.at(currentLayout));
			((QLineEdit*)uiWidget)->setAlignment(Qt::AlignRight);
			uiWidget->setMinimumWidth(100);
			uiWidget->setMaximumWidth(100);
			((QLineEdit*)uiWidget)->setText(v.toString());
			dataType = Number;
		}

		if (uiWidget) {
			uiWidget->setToolTip(toolTipDesc);

			itemCount++;
			if (itemCount>((currentLayout+1)*maxItemsPerLayout))
				currentLayout++;

			if (currentLayout<maxLayouts) {
				if (propertyName.isEmpty())
					layouts.at(currentLayout)->addRow(uiWidget);
				else
					layouts.at(currentLayout)->addRow(propertyName, uiWidget);

				widgets.append( WidgetInfo(it.name(),dataType,uiWidget) );
			} else {
				delete uiWidget;
				break;
			}

		}

	}

	ui->scriptEditor->clear();
    ui->scriptEditor->appendPlainText("script:\n{");
    ui->scriptEditor->appendPlainText( manager->objectToScript(global.property("reportData").property("script") ) );
    ui->scriptEditor->appendPlainText("\n},\n");

    ui->scriptEditor->appendPlainText("resultSet:\n{\n");
    ui->scriptEditor->appendPlainText(manager->objectToScript(global.property("reportData").property("resultSet") ) );
    ui->scriptEditor->appendPlainText("\n}\n");

	return true;
}

void NCRDHighChartsItemDialog::loadChartTypeCombo(QComboBox *combo, const QScriptValue &value, const QString& currentValue )
{
	QScriptValueIterator it(value);
	combo->clear();
	int i=0;
	while (it.hasNext()) {
		it.next();
		if (it.name()=="length")
			continue;
		QScriptValue v = value.property(QString::number(i));

		QPixmap pm(v.property("icon").toString());
		QVariant userData = v.property("identifier").toVariant();

		if (pm.isNull())
			combo->addItem(v.property("name").toString(), userData );
		else
			combo->addItem( QIcon(pm), v.property("name").toString(), userData );

		if (userData.toString()==currentValue)
			combo->setCurrentIndex(i);

		i++;
	}
}

bool NCRDHighChartsItemDialog::generateChartScript(QString& output)
{
	QString userScript(ui->scriptEditor->toPlainText());
	userScript.prepend("var test={");
	userScript.append("}");
	QScriptEngine engine;
	QScriptValue global;
	// TEST USER SCRIPT VALIDATION
	if (!manager->parseReportData(engine, global, userScript)) {
		QMessageBox::warning(this, tr("Script error"), manager->errorMsg());
		return false;
	}
	userScript = ui->scriptEditor->toPlainText();

	QTextStream in(&output);
	const char qm = '\"';

	in << "\nvar reportData =\n{\n";
	in << "title:\t\"" << ui->lineEditChartTitle->text() << "\",\n";
	in << "parameters:\n{\n";

	for (int i = 0; i < widgets.size(); ++i) {
		WidgetInfo wi = widgets.at(i);

		QString value;
		if (wi.widget->metaObject()->className()== QLatin1String("QComboBox")) {
			QComboBox *combo = qobject_cast<QComboBox*>(wi.widget);
			value = combo->itemData(combo->currentIndex()).toString();
		}
		else if (wi.widget->metaObject()->className()== QLatin1String("QLineEdit")) {
			QLineEdit *lineEdit = qobject_cast<QLineEdit*>(wi.widget);
			value = lineEdit->text();
			lineEdit->alignment();

		}
		else if (wi.widget->metaObject()->className()== QLatin1String("QCheckBox")) {
			QCheckBox *cb = qobject_cast<QCheckBox*>(wi.widget);
			value = (cb->isChecked() ? "true" : "false");
		}

		if ( wi.dataType == String )
			value = qm + value +qm;

		in << "\t" << wi.id << ":\t" << value << ",\n";

	}
	in << flush;
	output.chop(2);

	in << "},\n";

	in << userScript;

	in << "\n};\n";

	return true;
}

void NCRDHighChartsItemDialog::deleteContainters()
{
	for (int i=0; i<containters.count(); ++i) {
		QWidget *w = containters.at(i);
		for (int j=0; j<w->children().count(); ++j) {
			QObject* obj = w->children().at(j);
			if (obj->isWidgetType())
				obj->deleteLater();
		}
	}
}

void NCRDHighChartsItemDialog::renderChart()
{
	QPixmap pm;
	pm.loadFromData(manager->chartSvg());
	ui->chartPreview->clear();
	if (pm.isNull())
		ui->chartPreview->setText(manager->chartSvg());
	else {
		ui->chartPreview->setPixmap(pm);
	}
}

#endif //HIGHCHARTS_INTEGRATION
