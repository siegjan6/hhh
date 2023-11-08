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
#include "ncreporttextoutput.h"
#include "ncreportsection.h"
#include "ncreportevaluator.h"
#include "ncreportgroup.h"
//#include "ncreportitem.h"
#include "ncreportfielditem.h"

#include <QFile>
#include <QTextStream>

#define TEXT_OUTPUT_BEGIN_LEFT_ID	"<!-- BEGIN {"
#define TEXT_OUTPUT_BEGIN_LEFT_ID_LENGTH	sizeof(TEXT_OUTPUT_BEGIN_LEFT_ID)

#define TEXT_OUTPUT_BEGIN_RIGHT_ID	"} -->"
#define TEXT_OUTPUT_BEGIN_RIGHT_ID_LENGTH	sizeof(TEXT_OUTPUT_BEGIN_RIGHT_ID)

#define TEXT_OUTPUT_END_ID		"<!-- END -->"

//static const char * const p_sections[] = {"DH","DF","PH","PF","RH","RF", "D.","GH.","GF.",0};

NCReportTextOutput::NCReportTextOutput(QObject *parent) : NCReportOutput(parent), m_ts(0), m_file(0), m_insideSection(false)
{
    setOutput( Text );
    setDocumentTemplateBased(true);
}

NCReportTextOutput::~NCReportTextOutput()
{
}

bool NCReportTextOutput::setup()
{
    // processes template file
    if (m_template.isEmpty() && m_templateFile.isEmpty()) {
        return false;
    }

    m_textSections.clear();
    bool ok=false;

    if (!m_templateFile.isEmpty()) {
        QFile file(m_templateFile);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return false;

        QTextStream in(&file);
        while (!in.atEnd()) {
            ok = processTemplateLine(in.readLine());
            if (!ok)
                break;
        }
    } else {
        QTextStream in(&m_template);
        while (!in.atEnd()) {
            ok = processTemplateLine(in.readLine());
            if (!ok)
                break;
        }
    }
    return ok;
}

void NCReportTextOutput::cleanup()
{
}

void NCReportTextOutput::begin()
{
    m_file = new QFile(fileName());
    if (!m_file->open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    m_ts = new QTextStream(m_file);

    if (m_textSections.contains("DH"))
        *m_ts << m_textSections.value("DH");
}

void NCReportTextOutput::end()
{
    if (m_textSections.contains("DF"))
        *m_ts << m_textSections.value("DF");

    if (m_file->isOpen())
        m_file->close();

    if (m_ts) {
        delete m_ts;
        m_ts = 0;
    }
    if (m_file)	{
        delete m_file;
        m_file = 0;
    }
}

void NCReportTextOutput::newPage()
{
    //nothing to do
}

bool NCReportTextOutput::renderSection(NCReportSection *section, NCReportEvaluator *evaluator)
{
    QString textSection;
    switch (section->type()) {
    case NCReportSection::PageHeader:
        textSection = m_textSections.value("PH");
        break;
    case NCReportSection::PageFooter:
        textSection = m_textSections.value("PF");
        break;
    case NCReportSection::GroupHeader:
        textSection = m_textSections.value( QString("GH.")+section->relatedGroup()->detail()->id()+"."+section->relatedGroup()->groupId() );
        break;
    case NCReportSection::GroupFooter:
        textSection = m_textSections.value( QString("GF.")+section->relatedGroup()->detail()->id()+"."+section->relatedGroup()->groupId() );
        break;
    case NCReportSection::Detail:
        textSection = m_textSections.value(QString("D.")+section->id());
        break;
    case NCReportSection::ReportHeader:
        textSection = m_textSections.value("RH");
        break;
    case NCReportSection::ReportFooter:
        textSection = m_textSections.value("RF");
        break;
    case NCReportSection::Custom:
        break;
    }

    if (textSection.isEmpty())
        return true;

    if (textSection.contains("$F{")) {
        // field reference
        QListIterator<NCReportItem*> i(section->reportItems());
        while ( i.hasNext() ) {
            NCReportItem *item = i.next(); //mSection->reportItems().at(i);
            if (item->objectType() == NCReportItem::Field) {
                item->updateValue(this, evaluator);
                //NCReportFieldItem* field = ((NCReportFieldItem*)item);
                //qDebug("Before Field: %s DisplayValue: %s", qPrintable(item->id()), qPrintable(field->displayValue()) );
            }
        }
    }
    evaluator->setSection(section);
    bool ok = evaluator->evaluate(textSection);

    if (ok)
        *m_ts << textSection;

    return ok;
}

void NCReportTextOutput::setTemplate(const QString &tpl)
{
    m_template = tpl;
}

void NCReportTextOutput::setTemplateFile(const QString &fileName)
{
    m_templateFile = fileName;
}

bool NCReportTextOutput::processTemplateLine(const QString &line)
{
    if (line.startsWith(TEXT_OUTPUT_BEGIN_LEFT_ID)) {
        if (m_insideSection)
            return false;	// end tag missing
        m_insideSection = true;
        m_currentID = line.trimmed();
        m_currentID.chop(TEXT_OUTPUT_BEGIN_RIGHT_ID_LENGTH-1);
        m_currentID = m_currentID.mid(TEXT_OUTPUT_BEGIN_LEFT_ID_LENGTH-1);
    } else if (line.startsWith(TEXT_OUTPUT_END_ID)) {
        if (!m_insideSection)
            return false;	// begin tag missing
        m_insideSection = false;
        m_textSections.insert(m_currentID,m_currentTextSection);
        m_currentTextSection.clear();
        m_currentID.clear();
    } else {
        if (m_insideSection) {
            m_currentTextSection += line + "\n";
        }
    }
    return true;
}
