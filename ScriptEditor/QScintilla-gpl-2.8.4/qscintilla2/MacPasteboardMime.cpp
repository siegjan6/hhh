// This module implements part of the support for rectangular selections on
// OS/X.  It is a separate file to avoid clashes between OS/X and Scintilla
// data types.
//
// Copyright (c) 2014 Riverbank Computing Limited <info@riverbankcomputing.com>
// 
// This file is part of QScintilla.
// 
// This file may be used under the terms of the GNU General Public
// License versions 2.0 or 3.0 as published by the Free Software
// Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
// included in the packaging of this file.  Alternatively you may (at
// your option) use any later version of the GNU General Public
// License if such license has been publicly approved by Riverbank
// Computing Limited (or its successors, if any) and the KDE Free Qt
// Foundation. In addition, as a special exception, Riverbank gives you
// certain additional rights. These rights are described in the Riverbank
// GPL Exception version 1.1, which can be found in the file
// GPL_EXCEPTION.txt in this package.
// 
// If you are unsure which license is appropriate for your use, please
// contact the sales department at sales@riverbankcomputing.com.
// 
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#include <qglobal.h>

#if (QT_VERSION >= 0x040200 && QT_VERSION < 0x050000 && defined(Q_OS_MAC)) || (QT_VERSION >= 0x050200 && defined(Q_OS_OSX))

#include <QByteArray>
#include <QLatin1String>
#include <QList>
#include <QString>
#include <QStringList>
#include <QVariant>

#include <QMacPasteboardMime>


static const QLatin1String mimeRectangular("text/x-qscintilla-rectangular");
static const QLatin1String utiRectangularMac("com.scintilla.utf16-plain-text.rectangular");


class RectangularPasteboardMime : public QMacPasteboardMime
{
public:
    RectangularPasteboardMime() : QMacPasteboardMime(MIME_ALL)
    {
    }

    bool canConvert(const QString &mime, QString flav)
    {
        return mime == mimeRectangular && flav == utiRectangularMac;
    }

    QList<QByteArray> convertFromMime(const QString &, QVariant data, QString)
    {
        QList<QByteArray> converted;

        converted.append(data.toByteArray());

        return converted;
    }

    QVariant convertToMime(const QString &, QList<QByteArray> data, QString)
    {
        QByteArray converted;
        for(int i = 0 ;i < data.count();i++)
        {
            QByteArray byteary = data[i];
            converted += byteary;
        }

        return QVariant(converted);
    }

    QString convertorName()
    {
        return QString("QScintillaRectangular");
    }

    QString flavorFor(const QString &mime)
    {
        if (mime == mimeRectangular)
            return QString(utiRectangularMac);

        return QString();
    }

    QString mimeFor(QString flav)
    {
        if (flav == utiRectangularMac)
            return QString(mimeRectangular);

        return QString();
    }
};


// Initialise the singleton instance.
void initialiseRectangularPasteboardMime()
{
    static RectangularPasteboardMime *instance = 0;

    if (!instance)
    {
        instance = new RectangularPasteboardMime();

        qRegisterDraggedTypes(QStringList(utiRectangularMac));
    }
}


#endif
