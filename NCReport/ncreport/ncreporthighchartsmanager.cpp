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

#include "ncreporthighchartsmanager.h"
#include "ncreportdef.h"
#include "ncreportdatasource.h"

#include <QFile>
#include <QScriptEngine>
#include <QScriptValueIterator>
#include <QDebug>
#include <QDomDocument>
#include <QTime>

#ifdef WEBENGINE_INTEGRATION
#include <QWebEnginePage>
#endif

#ifdef WEBKIT_INTEGRATION
#include <QWebPage>
#include <QWebFrame>
#endif

#include <QDir>
#include <QPainter>
#include <QMap>

//#include <QNetworkRequest>
//#include <QNetworkReply>
//#include <QNetworkAccessManager>

NCReportHighChartsManager::NCReportHighChartsManager(QObject *parent) :
    QObject(parent), m_error(false)
{
    m_outputFileName = QDir::tempPath()+"/chart.html";

#ifdef WEBENGINE_INTEGRATION
    webPage = new QWebEnginePage(this);
#else
    webPage = new QWebPage(this);
#endif
    connect(webPage, SIGNAL(loadFinished(bool)), this, SLOT(finishChart(bool)));
    //connect(webPage, SIGNAL(downloadRequested(QNetworkRequest)), this, SLOT(startDownload(QNetworkRequest)));

    //nam = new QNetworkAccessManager(this);
}

void NCReportHighChartsManager::setTemplateFile(const QString &fileName)
{
    QString tplFile(fileName);
    QFileInfo fi(tplFile);
    if (!fi.exists()) {
        while (true) {
            tplFile = fi.fileName();
            if (QFile::exists(tplFile))
                break;

            tplFile = m_reportDef->templateDir()+"/"+fi.fileName();
            if ( QFile::exists(tplFile) )
                break;

            tplFile = "templates/"+fi.fileName();
            if ( QFile::exists(tplFile) )
                break;

            tplFile = "../templates/"+fi.fileName();
            if ( QFile::exists(tplFile) )
                break;

            tplFile = QDir::homePath()+"/templates/"+fi.fileName();
            if ( QFile::exists(tplFile) )
                break;

            // not found
            tplFile = fileName;
            break;
        }
    }

    m_templateFileName = tplFile;
}

QString NCReportHighChartsManager::templateFile() const
{
    return m_templateFileName;
}

void NCReportHighChartsManager::setResultFile(const QString &fileName)
{
    m_outputFileName = fileName;
}
QString NCReportHighChartsManager::resultFile() const
{
    return m_outputFileName;
}

/*! Sets report XML definition representation of the chart */
void NCReportHighChartsManager::setReportData(const QString &reportDataScript)
{
    m_reportDataScript = reportDataScript;
}

void NCReportHighChartsManager::setReportDef(NCReportDef *rdef)
{
    m_reportDef = rdef;
}

bool NCReportHighChartsManager::loadTemplate(ProcessingMode mode, QString& reportData)
{
    QFile inFile(m_templateFileName);
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setError( tr("Invalid template file: %1").arg(m_templateFileName));
        return false;
    }

    QXmlStreamReader reader(&inFile);
    short scriptID = 0;

#ifdef NCREPORT_DEBUG_OUTPUT
    QTime t;
    t.start();
#endif

    QFile outFile;

    if (mode==WriteMode) {
        outFile.setFileName(m_outputFileName);
        if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            setError( tr("Invalid output file: %1").arg(m_outputFileName));
            return false;
        }
        out.reset();
        out.setDevice(&outFile);
        out.setCodec("UTF-8");
    }

    while (!reader.atEnd()) {

        reader.readNext();
        if (reader.isStartElement()) {

            if (mode==WriteMode) {
                out << "<"+reader.name().toString();
                for (int i = 0; i < reader.attributes().size(); ++i) {
                    QXmlStreamAttribute a = reader.attributes().at(i);
                    out << " " + a.name().toString() << "=\"" << a.value().toString() << "\"";
                }
                out << ">";
            }
            if (reader.name() == "script") {
                // read script
                QStringRef r = reader.attributes().value("id");
                if ( r == "report-data") {

                    if (mode==WriteMode) {
                        //-----------------------------------------------
                        // REPLACE report data script to m_reportData
                        //-----------------------------------------------
                        reader.readElementText();	// dummy read
                        out << reportData;
                        writeEndElement(reader);
                    } else {
                        reportData = reader.readElementText();
                    }
                } else {

                    if (mode==WriteMode) {
                        out << reader.readElementText();
                        writeEndElement(reader);
                    } else {
                        reader.readElementText();
                    }
                }

                ++scriptID;
            } else if (reader.name() == "title") {

                if (mode==WriteMode) {
                    out << reader.readElementText();
                    writeEndElement(reader);
                } else {
                    reader.readElementText();
                }
            } else {
                //text = reader.readElementText();
            }

        } else if (reader.isEndElement()) {

            if (mode==WriteMode)
                writeEndElement(reader);
        }
    }


    if (reader.hasError()) {
        setError( tr("Template parse error: %1 Line:%2 Column:%3").arg(reader.errorString()).arg(reader.lineNumber()).arg(reader.columnNumber()));
        return false;
    }
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug() << "Done." << " Elapsed time:" << t.elapsed();
#endif

    return true;
}




bool NCReportHighChartsManager::loadContentFromFile(const QString &fileName, QString& content) const
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    content = file.readAll();
    return true;
}

bool NCReportHighChartsManager::saveContentToFile(const QString &fileName, const QString &content) const
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    QTextStream s(&file);
    s << content;
    file.close();
    return true;
}

bool NCReportHighChartsManager::parseScript( const QString &script, QScriptEngine &engine, QScriptValue &global )
{
    engine.evaluate(script);
    if (engine.hasUncaughtException()) {
        setError( tr("Script error in line %1\n%2").arg(engine.uncaughtExceptionLineNumber()).arg(engine.uncaughtException().toString()) );
        return false;
    }
    global = engine.globalObject();
    return true;
}


void NCReportHighChartsManager::setError(const QString &msg)
{
    m_error = !msg.isEmpty();
    m_errorMsg = msg;
}

bool NCReportHighChartsManager::load()
{
    //return loadTemplate(ReadMode);
    return true;
}

bool NCReportHighChartsManager::write()
{
    //return loadTemplate(WriteMode);
    return true;
}



QString NCReportHighChartsManager::objectToScript(const QScriptValue &value, int level, bool isLiteralArray, const QString &objectName ) const
{
    QString js;
    QString tab("\t");
    tab = tab.repeated(level);
    QScriptValueIterator it(value);
    while (it.hasNext()) {
        it.next();
        QScriptValue v = it.value();
        //qDebug() << it.name();

        if (v.isArray()) {
            js += tab + it.name()+":\t["+ objectToScript(v,level+1,true);
            js += "],\n";

        } else if (v.isObject()) {
            //recursive call
            if (isLiteralArray)
                js += tab + "\n";
            else
                js += tab + it.name()+":\n";
            js += tab + "{\n" + objectToScript(v,level+1);
            js += tab + "},\n";
        } else {
            QString quoteMark;
            if ( !(v.isBool() || v.isNumber() || v.isNull()) )
                quoteMark = '\"';

            if (isLiteralArray)
                js += quoteMark +v.toString() + quoteMark +",";
            else
                js += tab + it.name()+":\t" + quoteMark + v.toString() + quoteMark + ",\n";
        }
    }
    if (isLiteralArray) {
        js.chop(1);	//remove comma
        js.resize(js.lastIndexOf(','));		// remove last value which is = the length of array
    } else {
        js.chop(2);
        js.append("\n");
    }

    if (!objectName.isEmpty()) {
        js.prepend(objectName+" = {\n");
        js.append("};");
    }

    return js;
}

void NCReportHighChartsManager::overViewScriptValue(const QScriptValue &value)
{
    QScriptValueIterator it(value);
    while (it.hasNext()) {
        it.next();
        qDebug() << it.name() << ": " << it.value().toString();
    }
}

void NCReportHighChartsManager::writeEndElement(const QXmlStreamReader &reader)
{
    out << "</" << reader.name().toString() << ">";
    endl(out);
}

QString NCReportHighChartsManager::slicedByUpperPos(const QString &str) const
{
    /*
    QStringList l = str.split(QRegExp("[A-Z]"));
    QString value = l.join(" ");
    return value;
    */
    //str.contains(section()
    int pos=-1;
    const QChar *data = str.constData();
    QString value;
    while (!data->isNull()) {
        pos++;
        if (pos==0) {
            value.append(data->toUpper());
        } else {
            if (data->isUpper()) {
                value.append(' ');
            }
            value.append(*data);
        }
        ++data;
    }

    return value;
}

void NCReportHighChartsManager::startToLoadChart()
{
    m_chartSvg.clear();
    QUrl url("file:///" +m_outputFileName);

#ifdef WEBENGINE_INTEGRATION
    webPage->load(url);
#endif
#ifdef WEBKIT_INTEGRATION
    webPage->mainFrame()->load(url);
#endif
}

void NCReportHighChartsManager::finishChart(bool ok)
{
    Q_UNUSED(ok)

#ifdef WEBENGINE_INTEGRATION
    //    struct SetPlainTextFunctor {
//        QByteArray *text;
//        SetPlainTextFunctor(QByteArray *text) : text(text) { }
//        void operator()(const QByteArray &result) {
//            text->clear();
//            text->append(result);
//        }
//    };

//    QWebEnginePage *page = new QWebEnginePage;
//    QByteArray *ba = &m_chartSvg;
//    page->toPlainText(SetPlainTextFunctor(ba));
#endif
#ifdef WEBKIT_INTEGRATION
    m_chartSvg = webPage->mainFrame()->toPlainText().toUtf8();
#endif
    emit chartIsReady();
}


void NCReportHighChartsManager::renderAsImage()
{
#ifdef WEBKIT_INTEGRATION
    webPage->setViewportSize(webPage->mainFrame()->contentsSize());
    QImage image(webPage->viewportSize(), QImage::Format_ARGB32);
    QPainter painter(&image);
    webPage->mainFrame()->render(&painter);
    painter.end();
    image.save("/tmp/chart232100.svg");
#endif
}

QByteArray NCReportHighChartsManager::chartSvg() const
{
    return m_chartSvg;
}

QPixmap &NCReportHighChartsManager::chartImage()
{
    return m_chartImage;
}

QString NCReportHighChartsManager::errorMsg() const
{
    return m_errorMsg;
}

bool NCReportHighChartsManager::error() const
{
    return m_error;
}

void NCReportHighChartsManager::timeoutError()
{
    setError( tr("Timeout error when generating chart."));
}

bool NCReportHighChartsManager::evaluateDataSource(QScriptValue &seriesList)
{
    m_dataSourcesToClose.clear();
    int count = seriesList.property("length").toInteger();
    for (int i=0; i<count; ++i) {

        QScriptValue ds = seriesList.property(i).property("datasource");
        if (ds.isValid())
            loadData(i, ds.toString(), "data", seriesList, QVariant::Double);

        ds = seriesList.property(i).property("categorysource");
        if (ds.isValid())
            loadData(i, ds.toString(), "category", seriesList, QVariant::String);
    }

    for (int i=0; i<m_dataSourcesToClose.count(); ++i)
        m_dataSourcesToClose.at(i)->close();

    return true;
}


/*!
  Dedicated data source with named columns prepared for chart data.
  Structure: category, series, data, groupname, category_key
  Required columns: category, data
 */
bool NCReportHighChartsManager::evaluateDedicatedDataSource( const QString& dsID, QScriptEngine &engine, QScriptValue &seriesList,
                                                             QScriptValue &newSeriesList)
{
    Q_UNUSED(seriesList);
    NCReportDataSource *ds = m_reportDef->dataSource(dsID);
    if (!ds) {
        setError(tr("Invalid data source ID: %1").arg(dsID));
        return false;
    }

    ds->setErrorHandler( m_reportDef->error() );
    bool dsWasOpened = ds->isOpened();
    if (!dsWasOpened) {

        //ds->evaluate( evaluator );

        if (!ds->open()) {
            setError(tr("Data source open error: %1").arg(m_reportDef->error()->errorMsg()));
            return false;
        }
    }
    bool hasSeries = ds->hasColumn("series");
    bool hasData = ds->hasColumn("data");
    bool hasCategory = ds->hasColumn("category");
    bool hasCategoryKey = ds->hasColumn("category_key");

    if (hasCategoryKey)	{
        loadUnPivotedDataWithCategoryKey(ds,engine,newSeriesList);
    } else {
        if (hasSeries && hasData && hasCategory)
            loadUnPivotedData(ds,engine,newSeriesList);
        else
            loadPivotedData(ds,engine,newSeriesList);
    }
    if (!dsWasOpened)
        ds->close();

    return true;
}

bool NCReportHighChartsManager::loadUnPivotedData(NCReportDataSource *ds, QScriptEngine &engine, QScriptValue &newSeriesList)
{
    bool hasSeries = ds->hasColumn("series");

    QString groupColumn;
    bool hasGroup = false;
    if (ds->hasColumn("group")) {
        hasGroup = true;
        groupColumn = "group";
    } else if ( ds->hasColumn("groupname") ) {
        hasGroup = true;
        groupColumn = "groupname";
    }
    //bool hasCategoryItem = (seriesList.property(0).property("name").toString()=="category");	// category item was defined

    ds->first();
    int dataIdx=0;
    int seriesIdx=-1;
    bool initMode = true;
    QString seriesID;
    QScriptValue currentSeriesItem;
    QStringList seriesIDList;

    do {
//		if (initMode && hasCategoryItem) {
//			newSeriesList.setProperty(0,seriesList.property(0));	// copy category item
//			seriesIDList << "category";
//		}
        if (hasSeries)
            seriesID = ds->value("series").toString();
        else
            seriesID = "series";

        seriesIdx = seriesIDList.indexOf(seriesID);

        if (seriesIdx<0) {
            // this is a new series item
            seriesIdx = seriesIDList.count();
            dataIdx = 0;	// reset data index
            currentSeriesItem = engine.newObject();
            currentSeriesItem.setProperty("name", seriesID );
            currentSeriesItem.setProperty("category",engine.newArray());
            currentSeriesItem.setProperty("data",engine.newArray());
            if (hasGroup)
                currentSeriesItem.setProperty("group", ds->value(groupColumn).toString() );
            newSeriesList.setProperty(seriesIdx, currentSeriesItem);

            seriesIDList << seriesID;
        } else {
            currentSeriesItem = newSeriesList.property(seriesIdx);
        }

        currentSeriesItem.property("data").setProperty(dataIdx, ds->value("data").toDouble());
        currentSeriesItem.property("category").setProperty(dataIdx, ds->value("category").toString());
        dataIdx++;
        initMode = false;
    } while (ds->next());

    return true;
}

bool NCReportHighChartsManager::loadUnPivotedDataWithCategoryKey(NCReportDataSource *ds, QScriptEngine &engine, QScriptValue &newSeriesList)
{
    QString groupColumn;
    bool hasGroup = false;
    if (ds->hasColumn("group")) {
        hasGroup = true;
        groupColumn = "group";
    } else if ( ds->hasColumn("groupname") ) {
        hasGroup = true;
        groupColumn = "groupname";
    }
    //generate sorted result set
    typedef QHash<QString,QVariant> DataRecord;
    typedef QMultiMap<QString, DataRecord> ResultSet;
    QMap<QString,int> categoryKeyDict;
    ResultSet resultSet;

    ds->first();
    do {
        QString keyValue = ds->value("category_key").toString();
        DataRecord dataRecord;
        for (int column=0; column<ds->columnCount(); ++column) {
            dataRecord[ds->columnName(column)] = ds->value(column);
        }
        resultSet.insert(keyValue, dataRecord);
        categoryKeyDict[keyValue]=0;
    } while (ds->next());

    int idx=0;
    QMap<QString, int>::const_iterator dict_i = categoryKeyDict.constBegin();
    while (dict_i != categoryKeyDict.constEnd()) {
        categoryKeyDict[dict_i.key()]=idx++;
        ++dict_i;
    }

    int seriesIdx=-1;
    int dataIdx=0;
    bool initMode = true;
    double data =0;
    QString category_key;
    QString category;
    QString series;
    QString group;
    QScriptValue currentSeriesItem;
    QStringList seriesIDList;

    QMultiMap<QString, DataRecord>::const_iterator i = resultSet.constBegin();
    while (i != resultSet.constEnd()) {
        if (initMode) {
            //category series item
            currentSeriesItem = engine.newObject();
            currentSeriesItem.setProperty("name", "category" );
            currentSeriesItem.setProperty("data",engine.newArray());
            newSeriesList.setProperty(0, currentSeriesItem);
            seriesIDList << series;
        }
        DataRecord record = i.value();
        category = record.value("category").toString();
        data = record.value("data").toDouble();
        category_key = i.key();
        series = record.value("series").toString();
        if (hasGroup) group = record.value(groupColumn).toString();
        dataIdx = categoryKeyDict.value(category_key);

        newSeriesList.property(0).property("data").setProperty(dataIdx, category );	// add category data to 1st series

        seriesIdx = seriesIDList.indexOf(series);

        if (seriesIdx<0) {
            // this is a new series item
            seriesIdx = seriesIDList.count();
            currentSeriesItem = engine.newObject();
            currentSeriesItem.setProperty("name", series );
            if (hasGroup) currentSeriesItem.setProperty("group", group );

            QScriptValue data = engine.newArray();
            QMap<QString, int>::const_iterator i = categoryKeyDict.constBegin();
            while (i != categoryKeyDict.constEnd()) {
                data.setProperty(i.value(), QScriptValue::NullValue );
                ++i;
            }
            currentSeriesItem.setProperty("data", data);
            newSeriesList.setProperty(seriesIdx, currentSeriesItem);
            seriesIDList << series;
        } else {
            currentSeriesItem = newSeriesList.property(seriesIdx);
        }
        currentSeriesItem.property("data").setProperty(dataIdx, data);
        initMode = false;
        ++i;
    }
    return true;
}


bool NCReportHighChartsManager::loadPivotedData(NCReportDataSource *ds, QScriptEngine &engine, QScriptValue &newSeriesList)
{
    for (int column=0; column<ds->columnCount(); ++column) {
        QScriptValue seriesItem = engine.newObject();
        seriesItem.setProperty("name", ds->columnName(column) );
        seriesItem.setProperty("data",engine.newArray() );
        newSeriesList.setProperty(column, seriesItem);
        int dataIdx=0;
        ds->first();
        do {
            QVariant value = ds->value(column);
            bool isNumeric;
            double d = value.toDouble(&isNumeric);

            //bool isString = (value.type() == QVariant::String || value.type()==QVariant::Date || value.type()==QVariant::DateTime);
            seriesItem.property("data").setProperty(dataIdx++, isNumeric ? QScriptValue(d) : QScriptValue(value.toString()) );
        } while (ds->next());

    }
    return true;
}

bool NCReportHighChartsManager::loadData( int seriesIdx, const QString &dataRef, const QString &target,
                                          QScriptValue &seriesList, QVariant::Type type)
{
    if (dataRef.isEmpty() || !dataRef.contains("."))
        return true;

    QString dsID = dataRef.section(".",0,0);
    QString columnID = dataRef.section(".",1,1);
    NCReportDataSource *dataSource = m_reportDef->dataSource(dsID);

    if (dataSource) {
        dataSource->setErrorHandler( m_reportDef->error() );
        if (!dataSource->isOpened()) {
            if (dataSource->open()) {
                if (m_dataSourcesToClose.contains(dataSource))
                    m_dataSourcesToClose.append(dataSource);
            } else {
                return false;
            }
        }
        dataSource->first();
        int row=0;
        do {
            QVariant dsValue = dataSource->value(columnID);
            QScriptValue scriptValue;
            switch (type) {
            case QVariant::String: scriptValue = dsValue.toString(); break;
            default: scriptValue = dsValue.toDouble(); break;
            }
            seriesList.property(seriesIdx).property(target).setProperty(row, scriptValue );
            row++;
        } while (dataSource->next());
    } else {
        setError(tr("Invalid data source ID: %1").arg(dsID));
        return false;
    }

    return true;
}

/*! Parses template and report data using m_reportItemScript content */

bool NCReportHighChartsManager::parseTemplateMetaData(QScriptEngine &engine, QScriptValue &global)
{
    QString templateReportData;
    if (!loadTemplate(NCReportHighChartsManager::ReadMode, templateReportData))
        return false;

    if ( !parseScript(templateReportData,engine,global) )
        return false;

    return true;
}

bool NCReportHighChartsManager::parseReportData(QScriptEngine &engine, QScriptValue &global, const QString& reportData )
{
    if (reportData.isEmpty())
        return false;

    if ( !parseScript(reportData,engine,global) )
        return false;

    return true;
}


//void NCReportHighChartsManager::runChartTest()
//{
//	QScriptEngine engine;
//	QString exporting_js, test_chart;
//	loadContentFromFile("exporting.js", exporting_js );
//	loadContentFromFile("testchart.js", test_chart );
//	engine.evaluate(test_chart);

//	QScriptValue global = engine.globalObject();
//	overViewScriptValue(global);

//	QScriptValue chart = global.property("chart");
//	overViewScriptValue(chart);
//}

//void NCReportHighChartsManager::uniteTemplate(QString &templateData, const QString &reportChartData)
//{
//	if (reportChartData.isEmpty())
//		return;

//	const QString s1("/*NCR_BEGIN*/");
//	const QString s2("/*NCR_END*/");
//	int pos1 = templateData.indexOf(s1);
//	int pos2 = templateData.indexOf(s2);

//	if (pos1<0 || pos2<0)
//		return;

//	templateData.replace(pos1+s1.size(),pos2-pos1-s1.size(),reportChartData);

//}

//bool NCReportHighChartsManager::loadTemplateDOM()
//{
//	return false;	// this was a test only

//	QDomDocument doc("template");
//	QFile file(m_templateFileName);
//	if (!file.open(QIODevice::ReadOnly))
//		return false;
//	if (!doc.setContent(&file)) {
//		file.close();
//		return false;
//	}
//	file.close();

//	QDomElement docElem = doc.documentElement();	//root
//	QDomNode n = docElem.firstChild();
//	while(!n.isNull()) {
//		QDomElement e = n.toElement(); // try to convert the node to an element.
//		if(!e.isNull()) {
//			qDebug() << e.tagName() << endl; // the node really is an element.
//		}
//		n = n.nextSibling();
//	}

//	QDomElement e = doc.elementById("report-data");
//	qDebug() << e.tagName() ;

//	// Here we append a new element to the end of the document
//	//QDomElement elem = doc.createElement("img");
//	//elem.setAttribute("src", "myimage.png");
//	//docElem.appendChild(elem);
//	return true;
//}

/*
bool NCReportHighChartsManager::evaluateDedicatedDataSource_Old( const QString& dsID, QScriptEngine &engine, QScriptValue &seriesList,
                                                             QScriptValue &newSeriesList, NCReportDef *rdef)
{
    NCReportDataSource *ds = rdef->dataSource(dsID);
    if (!ds) {
        setError(tr("Invalid data source ID: %1").arg(dsID));
        return false;
    }

    ds->setErrorHandler( rdef->error() );
    bool dsWasOpened = ds->isOpened();
    if (!dsWasOpened) {

        //ds->evaluate( evaluator );

        if (!ds->open()) {
            setError(tr("Data source open error: %1").arg(rdef->error()->errorMsg()));
            return false;
        }
    }

    bool hasSeries = ds->hasColumn("series");
    bool hasGroup = ds->hasColumn("groupname");
    int seriesCounter=0;
    QString seriesID;
    QScriptValue currentSeriesItem;
    QHash<QString,QScriptValue> seriesContainer;

    ds->first();
    int row=0;
    do {
        if (hasSeries)
            seriesID = ds->value("series").toString();
        else
            seriesID = "series";

        if (seriesContainer.contains(seriesID)) {
            currentSeriesItem = seriesContainer[seriesID];
        } else {
            // Create new series item
            seriesCounter++;
            QScriptValue svSeriesItem = engine.newObject();
            svSeriesItem.setProperty("data",engine.newArray());
            svSeriesItem.setProperty("category",engine.newArray());

            seriesContainer[seriesID]= svSeriesItem;
            currentSeriesItem = svSeriesItem;
        }

        int currentDataIdx = currentSeriesItem.property("data").property("length").toInteger();
        currentSeriesItem.setProperty("name", seriesID );
        if (hasGroup)
            currentSeriesItem.setProperty("group", ds->value("groupname").toString() );
        currentSeriesItem.property("data").setProperty(currentDataIdx, ds->value("data").toDouble());

        QString category = ds->value("category").toString();
        //category.replace("<","&lt;");
        currentSeriesItem.property("category").setProperty(currentDataIdx, category);
        row++;
    } while (ds->next());

    int idx=0;
    if ( seriesList.property(0).property("name").toString()=="category") {	// category item was defined
        newSeriesList.setProperty(idx,seriesList.property(0));	// copy item
        ++idx;
    }

    // load series
    QHash<QString, QScriptValue>::const_iterator i = seriesContainer.constBegin();

    while (i != seriesContainer.constEnd()) {
        newSeriesList.setProperty(idx, i.value());
        ++i;
        ++idx;
    }

    if (!dsWasOpened)
        ds->close();

    return true;
}
*/


//	//TEST
//	QScriptValue sv = global.property("reportData").property("resultSet").property("seriesList");
//	//bool ar = sv.isArray();
//	sv.property(1).property("data").setProperty(0,99);
//	sv.property(1).property("data").setProperty(1,99);
//	sv.property(1).property("data").setProperty(2,99);
//	sv.property(1).property("data").setProperty(3,99);
    //	sv.property(1).property("data").setProperty(4,99);


//void NCReportHighChartsManager::startDownload(const QNetworkRequest &request)
//{
//	QNetworkReply *reply = webPage->networkAccessManager()->get(request);
//	connect(reply, SIGNAL(finished()), this, SLOT(finishDownload()));
//}

//void NCReportHighChartsManager::finishDownload()
//{
//	QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
//	m_chartSvg = reply->readAll();
//	reply->deleteLater();
//	emit chartIsReady();
//}

#endif // HIGHCHARTS_INTEGRATION
