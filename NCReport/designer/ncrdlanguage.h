/****************************************************************************
*
*  Copyright (C) 2002-2009 Helta Kft. / NociSoft Software Solutions
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
#ifndef NCRDLANGUAGE_H
#define NCRDLANGUAGE_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QStringList>
#include <QLocale>

QT_BEGIN_NAMESPACE
class QComboBox;
class QTabWidget;
class QMenu;
class QActionGroup;
class QCompleter;
class QDialog;
QT_END_NAMESPACE

class NCReportItem;
class NCRDDocument;

/*!

\section 功能

语言管理类

*/
class NCRDLanguage : public QObject
{
	Q_OBJECT
public:
	NCRDLanguage( QObject* parent =0 );
	~NCRDLanguage();

	enum AssignMode { TextsToTabs=0, TabsToTexts };
	struct LanguageData {
		LanguageData() : name( QString() ), code( QString() ), localeID( QLocale::C ) {}
		LanguageData( const QString& n, const QString& c, const QLocale::Language lid ) : name(n), code( c ), localeID(lid) {}
		QString name, code;
		QLocale::Language localeID;
	};

	void setLanguages( const QString& languageCodes );
	void setCurrentLanguage( const QString& languageCode );
	void setTabWidget( QTabWidget* tabwidget );

	//bool isMultiLanguage() const;
	bool isMultiEdited() const;
	void fillComboBox( QComboBox* );
    void createTabs(NCRDDocument *document, NCReportItem* item, QComboBox *stcombo, const QString& label_tpl, bool showExpButton,
                    QHash<QString,QString>& translations, QCompleter* completer, QDialog* dialog );
	/*static QString languageName( const QString& code ) const;*/
	void assignTranslations( const AssignMode mode, QHash<QString,QString>& translations );
	void buildLanguageMenu( QMenu* menu, QActionGroup* actionGroup, QWidget* parent );
	QString languageName(const QString& langcode ) const;
	bool hasMultiLine() const;
	void setValueToAllTabs( const QString& value );
	void setCompleter( QCompleter* completer );

signals:
    void itemSourceTypeChanged(int);

private:
	QString m_currLang;
	QString m_languages;
	QTabWidget* m_tabwidget;
	//QHash<QString,QString> lhash;
	QStringList m_codes, m_names;
	QStringList m_languageCodeList;
	bool m_multiLineFlag;

	void loadLanguages();
	void buildLanguageCodeList( QHash<QString,QString>& translations );

};

#endif // NCRDLANGUAGE_H
