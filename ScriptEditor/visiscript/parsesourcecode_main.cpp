/*
    Test program for source code parser (used to navigate to symbols in editor)

    Started: 18.12.2009

*/

#include <QtCore/QCoreApplication>
#include <QStringList>
#include <QFile>
#include <QTextStream>

#include "parsesourcecode.h"

#include <iostream>

using namespace std;

void usage()
{
    cout << "usage: parsesourcecode filename" << endl;
}

void process( const QString & sFileName )
{
    QFile aFile(sFileName);

    if( aFile.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        cout << "processing" << (const char *)sFileName.toAscii() << " ..." << endl;

        QTextStream in(&aFile);
        QString sSourceCode = in.readAll();
        ParseSourceCode aParser;
        aParser.parse(sSourceCode);
    }
    else
    {
        cerr << "Error: can not open file " << (const char *)sFileName.toAscii() << endl;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    process("D:\\svn_work\\1_Sources\\Electronics\\Analysis\\Python\\pyfaro\\test_fcn.py");

    if( a.arguments().length()!=2 )
    {
        usage();
    }
    else
    {
        process(a.arguments()[1]);
    }

    //return a.exec();
    return 0;
}

