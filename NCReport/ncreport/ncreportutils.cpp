/****************************************************************************
*
*  Copyright (C) 2002-2008 Helta Kft. / NociSoft Software Solutions
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
#include "ncreportutils.h"
#include <math.h>

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QProcessEnvironment>
#include <QApplication>
#include <QVariant>

#if defined(Q_WS_MAC) || defined(Q_OS_MAC)
//#define FONT_SIZE_FACTOR	1.2
#define FONT_SIZE_FACTOR	1.0
#else
#define FONT_SIZE_FACTOR	1.0
#endif


NCReportUtils::NCReportUtils()
{
}

bool NCReportUtils::fileToString( const QString& filename, QString& target )
{
    QString m_filename(filename.trimmed());
    if (m_filename.isEmpty())
        return false;
    evaluatedFileName(m_filename);
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    target.clear();
    QTextStream ts(&file);
    while (!ts.atEnd()) {
        QString line = ts.readLine();
        if ( !line.isEmpty() )
            target += line + "\n";
    }
    return true;
}

bool NCReportUtils::fileToStringAll( const QString& filename, QString& target )
{
    QString m_filename(filename);
    evaluatedFileName(m_filename);
    QFile file(m_filename);
    if ( file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        target = in.readAll();
        return true;
    }
    return false;
}

void NCReportUtils::evaluateFileName(QString &filename)
{
    if (filename.startsWith("http"))
        return;

    filename = filename.trimmed();
    filename.replace("%TEMP%",QDir::tempPath());
    filename.replace("{TEMP}",QDir::tempPath());
    filename.replace("%HOME%",QDir::homePath());
    filename.replace("{HOME}",QDir::homePath());
    filename.replace("{CURR}",QDir::currentPath());

    if (filename.contains("{NCREPORT_DIR}")) {
        filename.replace("{NCREPORT_DIR}", QProcessEnvironment::systemEnvironment().value("NCREPORT_DIR") );
    }
    if (filename.contains("{ROOT}")) {
        filename.replace("{ROOT}", qApp->property("NCREPORT_ROOT_DIR").toString() );
    }

    if (!filename.contains("$")) {
        QFileInfo fi(filename);
        if (fi.isRelative() && !fi.exists()) {
            // saved report path
            filename = qApp->property("REPORT_PATH").toString()+"/"+filename;
        }
    }
}

QString NCReportUtils::evaluatedFileName(const QString &filename)
{
    QString fn(filename);
    evaluateFileName(fn);
    return fn;
}

qreal NCReportUtils::fontSizeConvert( qreal pointSize )
{
    return pointSize*FONT_SIZE_FACTOR;
}

qreal NCReportUtils::fontSizeRestore( qreal pointSize )
{
    return pointSize/FONT_SIZE_FACTOR;
}

bool NCReportUtils::stringToFile(const QString &filename, const QString &content, const char *codecName)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
         return false;

    QTextStream out(&file);
    out.setCodec(codecName);
    out << content;
    return true;
}

int NCReportUtils::verticalAlignmentToIndex( const Qt::Alignment a )
{
    QFlags<Qt::AlignmentFlag> flags(a);
    int f=0;
    if ( flags.testFlag(Qt::AlignTop) )
        f=0;
    else if ( flags.testFlag(Qt::AlignVCenter))
        f=1;
    else if ( flags.testFlag(Qt::AlignBottom) )
        f=2;

    return f;
}

int NCReportUtils::horizontalAlignmentToIndex( const Qt::Alignment a )
{
    QFlags<Qt::AlignmentFlag> flags(a);
    int f=0;
    if ( flags.testFlag(Qt::AlignLeft) )
        f=0;
    else if ( flags.testFlag(Qt::AlignHCenter))
        f=1;
    else if ( flags.testFlag(Qt::AlignRight) )
        f=2;

    return f;
}

Qt::Alignment NCReportUtils::indexToVerticalAlignment(int idx)
{
    switch (idx) {
    case 0: return Qt::AlignTop;
    case 1: return Qt::AlignVCenter;
    case 2: return Qt::AlignBottom;
    }
    return Qt::AlignTop;
}

Qt::Alignment NCReportUtils::indexToHorizontalAlignment(int idx)
{
    switch (idx) {
    case 0: return Qt::AlignLeft;
    case 1: return Qt::AlignHCenter;
    case 2: return Qt::AlignRight;
    }
    return Qt::AlignLeft;
}

Qt::Alignment NCReportUtils::verticalAlignment( const Qt::Alignment alignment )
{
    QFlags<Qt::AlignmentFlag> flags(alignment);
    Qt::Alignment vAlign = Qt::AlignTop;
    if ( flags.testFlag(Qt::AlignTop) )
        vAlign = Qt::AlignTop;
    else if ( flags.testFlag(Qt::AlignVCenter))
        vAlign = Qt::AlignVCenter;
    else if ( flags.testFlag(Qt::AlignBottom) )
        vAlign = Qt::AlignBottom;

    return vAlign;
}

Qt::Alignment NCReportUtils::horizontalAlignment(const Qt::Alignment alignment)
{
    QFlags<Qt::AlignmentFlag> flags(alignment);
    Qt::Alignment hAlign = Qt::AlignLeft;
    if ( flags.testFlag(Qt::AlignLeft) )
        hAlign = Qt::AlignLeft;
    else if ( flags.testFlag(Qt::AlignHCenter))
        hAlign = Qt::AlignHCenter;
    else if ( flags.testFlag(Qt::AlignRight) )
        hAlign = Qt::AlignRight;

    return hAlign;
}

QString NCReportUtils::randomString(const int len)
{
    static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    QString string(len,'0');
    for (int i = 0; i < len; ++i) {
        string[i] = QChar(alphanum[qrand() % (sizeof(alphanum) - 1)]);
    }
    return string;
}

QString NCReportUtils::parse(const QString &string, const QString &startTag, const QString &endTag)
{
    int i1 = string.indexOf(startTag);
    if (i1<0)
        return QString();
    int i2 = string.indexOf(endTag);
    if (i2<0)
        return QString();

    return string.mid(i1+1,i2-i1);
}

void NCReportUtils::saveFilePath(const char* name, const QString &fileName)
{
    QFileInfo fi(fileName);
#ifndef Q_OS_MAC
    qApp->setProperty(name, fi.path() );
#endif
}

NCReportUtils::id2D::id2D(short x, short y) : m_x(x), m_y(y)
{
}

NCReportUtils::id2D::id2D(int id)
{
    setX( x(id) );
    setY( y(id) );
}

short NCReportUtils::id2D::x() const
{
    return m_x;
}

void NCReportUtils::id2D::setX(short x)
{
    m_x = x;
}

short NCReportUtils::id2D::x(int id)
{
    return id-(id << 16);
}

short NCReportUtils::id2D::y() const
{
    return m_y;
}

void NCReportUtils::id2D::setY(short y)
{
    m_y = y;
}

short NCReportUtils::id2D::y(int id)
{
    return (id >> 16);
}

int NCReportUtils::id2D::id(short x, short y)
{
    return x + (y << 16);
}

int NCReportUtils::id2D::id() const
{
    return m_x + (m_y << 16);
}


NCReportUtils::id2D NCReportUtils::id2D::fromId(int id)
{
    NCReportUtils::id2D key(id);
    return key;
}

qreal NCReportUtils::fround(qreal n, int d)
{
    return floor(n * pow(10., d) + .5) / pow(10., d);
}

double NCReportUtils::standardDeviation(const QList<double> &numbers)
{
    double total=0;
    double difftotal=0;
    double avg=0;
    for (int i=0; numbers.size(); ++i)
        total += numbers.at(i);
    avg = total/numbers.count();
    for (int i=0; numbers.size(); ++i)
        difftotal += pow(numbers.at(i)-avg,2);

    return sqrt(difftotal/numbers.count());
}

QString NCReportUtils::intListToString(const QList<int> &list)
{
    QString str;
    for (int i=0; i<list.count();i++)
        str += QString::number(list.at(i))+",";
    if (!str.isEmpty())
        str.chop(1);
    return str;
}

void NCReportUtils::stringToIntList(const QString &str, QList<int> &intlist)
{
    intlist.clear();
    QStringList strlist = str.split(',', QString::SkipEmptyParts);
    for (int i=0;i<strlist.count();i++)
        intlist << strlist.at(i).toInt();
}

