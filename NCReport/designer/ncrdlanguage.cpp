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
#include "ncrdlanguage.h"
#include "ncrdfieldexpwidget.h"
#include "ncrdmainwindow.h"
#include "ncreportitem.h"

#include <QComboBox>
#include <QTabWidget>
#include <QMenu>
#include <QActionGroup>
#include <QDialog>

NCRDLanguage::NCRDLanguage(QObject* parent ) : QObject( parent )
{
	loadLanguages();
	m_multiLineFlag = false;
}

NCRDLanguage::~NCRDLanguage()
{}

/*
QString languageName( const QString& code ) const
{
	return QString();
}
*/

void NCRDLanguage::setLanguages( const QString& languageCodes )
{
	m_languages = languageCodes;
}
void NCRDLanguage::setCurrentLanguage( const QString& languageCode )
{
	m_currLang = languageCode;
}
void NCRDLanguage::setTabWidget( QTabWidget* tabwidget )
{
	m_tabwidget = tabwidget;
}

void NCRDLanguage::createTabs( NCRDDocument* document, NCReportItem* item, QComboBox* stcombo, const QString& label_tpl, bool showExpButton,
                               QHash<QString,QString>& translations, QCompleter* completer, QDialog* dialog )
{
	for ( int i=0; i<m_tabwidget->count(); ++i ) {
		QWidget *w = m_tabwidget->widget(i);
		m_tabwidget->removeTab(i);
		delete w;
	}
	buildLanguageCodeList( translations );

	for ( int i=0; i<m_languageCodeList.count(); ++i ) {
		QString langcode = m_languageCodeList.at(i);
		NCRDFieldExpWidget *tab = new NCRDFieldExpWidget();
		tab->setObjectName(QString::fromUtf8("tab_%1").arg(langcode));
        tab->setItem(item);
        tab->setDocument(document);
        tab->setSourceTypeCombo(stcombo);
        if (!showExpButton)
            tab->hideExpButton();

		if (completer)
			tab->editor->setCompleter( completer );
		connect( tab->editor, SIGNAL(ctrlEnter()), dialog, SLOT(accept()));
		m_tabwidget->addTab( tab, label_tpl+(langcode=="00" ? QString() : QString(" [%1]").arg(langcode) ) );
	}
}

void NCRDLanguage::setCompleter( QCompleter* completer )
{
	for ( int i=0; i<m_languageCodeList.count(); ++i ) {
		QString langcode = m_languageCodeList.at(i);
		NCRDFieldExpWidget *tab = m_tabwidget->findChild<NCRDFieldExpWidget*>( QString("tab_%1").arg(langcode) );
		if ( tab )
			tab->editor->setCompleter( completer );
	}
}

void NCRDLanguage::buildLanguageCodeList( QHash<QString,QString>& translations )
{
	m_languageCodeList.clear();
	m_languageCodeList = m_languages.split( ',', QString::SkipEmptyParts );
	// check existing translations
	QHash<QString, QString>::const_iterator iterator = translations.constBegin();
	while (iterator != translations.constEnd()) {
		if ( !m_languageCodeList.contains(iterator.key()) )
			m_languageCodeList << iterator.key();		// add missing
		++iterator;
	}
}

void NCRDLanguage::assignTranslations( const AssignMode mode, QHash<QString,QString>& translations )
{
	m_multiLineFlag = false;
	for ( int i=0; i<m_languageCodeList.count(); ++i ) {
		QString langcode = m_languageCodeList.at(i);
		NCRDFieldExpWidget *tab = m_tabwidget->findChild<NCRDFieldExpWidget*>( QString("tab_%1").arg(langcode) );
		bool isDefault = (langcode == m_currLang);
		if ( tab ) {
			if ( mode == TextsToTabs ) {
				//if ( isDefault )
				//	tab->editor->setPlainText(text);
				//else
					tab->editor->setPlainText(translations[langcode]);
			} else if ( mode == TabsToTexts ) {
				//if ( isDefault )
				//	text = tab->editor->toPlainText();
				//else
					translations[langcode] = tab->editor->toPlainText();
					if ( translations[langcode].contains('\n') )
						m_multiLineFlag = true;
			}
		}
		if (isDefault) {
			m_tabwidget->setCurrentWidget(tab);
			tab->editor->setFocus();
			tab->editor->selectAll();
		}
	}
}


void NCRDLanguage::setValueToAllTabs( const QString& value )
{
	for ( int i=0; i<m_languageCodeList.count(); ++i ) {
		QString langcode = m_languageCodeList.at(i);
		NCRDFieldExpWidget *tab = m_tabwidget->findChild<NCRDFieldExpWidget*>( QString("tab_%1").arg(langcode) );
		if ( tab )
			tab->editor->setPlainText(value);

	}
}

//bool NCRDLanguage::isMultiLanguage() const
//{
//	return m_languages.count(',')>0;
//	//QStringList languageList = m_languages.split( ',', QString::SkipEmptyParts );
//	//return languageList.count()	> 1;
//}

bool NCRDLanguage::isMultiEdited() const
{
	QStringList languageList = m_languages.split( ',', QString::SkipEmptyParts );
	for ( int i=0; i<languageList.count(); ++i ) {
		QString langcode = languageList.at(i);
		NCRDFieldExpWidget *tab = m_tabwidget->findChild<NCRDFieldExpWidget*>( QString("tab_%1").arg(langcode) );
		if (tab && !tab->editor->toPlainText().isEmpty() && langcode != m_currLang )
			return true;
	}
	return false;
}

bool NCRDLanguage::hasMultiLine() const
{
	return m_multiLineFlag;
}

void NCRDLanguage::buildLanguageMenu( QMenu* menu, QActionGroup* actionGroup, QWidget* parent )
{
    Q_UNUSED(menu);
    Q_UNUSED(actionGroup);
    Q_UNUSED(parent);
//	QStringList languageList = m_languages.split( ',', QString::SkipEmptyParts );

//	if (languageList.count()==0)
//		return;

//	if (actionGroup)
//		delete actionGroup;

//	actionGroup = new QActionGroup(parent);
//    NCRDMainWindow *mw = qobject_cast<NCRDMainWindow*>(parent);
//    if (mw) {
//        connect(actionGroup, SIGNAL(triggered(QAction*)), mw, SLOT(languageChangeByAction(QAction*)));
//    }
//	menu->clear();


//	for ( int i=0; i<languageList.count(); ++i ) {
//		QString langcode = languageList.at(i);
//		QAction *action  = new QAction( languageName(langcode), actionGroup );
//		action->setCheckable(true);
//		action->setData( langcode );
//		action->setChecked( langcode == m_currLang );
//		menu->addAction( action );
//	}
}

QString NCRDLanguage::languageName( const QString& langcode ) const
{
	int i = m_codes.indexOf(langcode);
	if ( i>=0 )
		return m_names.at(i);

	return QString();
}


void NCRDLanguage::fillComboBox( QComboBox* cb )
{
	for (int i = 0; i < m_codes.size(); ++i)
		cb->addItem( m_names.at(i), m_codes.at(i) );

//	QHash<QString, QString>::const_iterator i = lhash.constBegin();
//	while (i != lhash.constEnd()) {
//		cb->addItem(i.value(), i.key());
//		++i;
//	}

	/*
	int i=0;
	while ( ncrd_languages[i] ) {
		QLocale::Language language = (QLocale::Language)ncrd_languages[i];
		QLocale locale( language, QLocale::AnyCountry );
		qDebug( "Language: %s %s", qPrintable(locale.languageToString(language)), qPrintable(locale.name()) );
		cb->addItem(locale.languageToString(language), locale.name().left(2));
		++i;
	}
	return;
	*/
}

void NCRDLanguage::loadLanguages()
{
	m_codes << "00"; m_names << tr("Default");
	m_codes << "ab"; m_names << tr("Abkhazian");
	m_codes << "aa"; m_names << tr("Afar");
	m_codes << "af"; m_names << tr("Afrikaans");
	m_codes << "ak"; m_names << tr("Akan");
	m_codes << "sq"; m_names << tr("Albanian");
	m_codes << "am"; m_names << tr("Amharic");
	m_codes << "ar"; m_names << tr("Arabic");
	m_codes << "an"; m_names << tr("Aragonese");
	m_codes << "hy"; m_names << tr("Armenian");
	m_codes << "as"; m_names << tr("Assamese");
	m_codes << "av"; m_names << tr("Avaric");
	m_codes << "ae"; m_names << tr("Avestan");
	m_codes << "ay"; m_names << tr("Aymara");
	m_codes << "az"; m_names << tr("Azerbaijani");
	m_codes << "bm"; m_names << tr("Bambara");
	m_codes << "ba"; m_names << tr("Bashkir");
	m_codes << "eu"; m_names << tr("Basque");
	m_codes << "be"; m_names << tr("Belarusian");
	m_codes << "bn"; m_names << tr("Bengali");
	m_codes << "bh"; m_names << tr("Bihari");
	m_codes << "bi"; m_names << tr("Bislama");
	m_codes << "bs"; m_names << tr("Bosnian");
	m_codes << "br"; m_names << tr("Breton");
	m_codes << "bg"; m_names << tr("Bulgarian");
	m_codes << "my"; m_names << tr("Burmese");
	m_codes << "ca"; m_names << tr("Catalan, Valencian");
	m_codes << "km"; m_names << tr("Central Khmer");
	m_codes << "ch"; m_names << tr("Chamorro");
	m_codes << "ce"; m_names << tr("Chechen");
	m_codes << "ny"; m_names << tr("Chichewa, Chewa, Nyanja");
	m_codes << "zh"; m_names << tr("Chinese");
	m_codes << "cv"; m_names << tr("Chuvash");
	m_codes << "kw"; m_names << tr("Cornish");
	m_codes << "co"; m_names << tr("Corsican");
	m_codes << "cr"; m_names << tr("Cree");
	m_codes << "hr"; m_names << tr("Croatian");
	m_codes << "cs"; m_names << tr("Czech");
	m_codes << "da"; m_names << tr("Danish");
	m_codes << "dv"; m_names << tr("Divehi, Dhivehi, Maldivian");
	m_codes << "nl"; m_names << tr("Dutch, Flemish");
	m_codes << "dz"; m_names << tr("Dzongkha");
	m_codes << "en"; m_names << tr("English");
	m_codes << "eo"; m_names << tr("Esperanto");
	m_codes << "et"; m_names << tr("Estonian");
	m_codes << "ee"; m_names << tr("Ewe");
	m_codes << "fo"; m_names << tr("Faroese");
	m_codes << "fj"; m_names << tr("Fijian");
	m_codes << "fi"; m_names << tr("Finnish");
	m_codes << "fr"; m_names << tr("French");
	m_codes << "ff"; m_names << tr("Fulah");
	m_codes << "gd"; m_names << tr("Gaelic, Scottish Gaelic");
	m_codes << "gl"; m_names << tr("Galician");
	m_codes << "lg"; m_names << tr("Ganda");
	m_codes << "ka"; m_names << tr("Georgian");
	m_codes << "de"; m_names << tr("German");
	m_codes << "gn"; m_names << tr("Guaraní");
	m_codes << "gu"; m_names << tr("Gujarati");
	m_codes << "ht"; m_names << tr("Haitian, Haitian Creole");
	m_codes << "ha"; m_names << tr("Hausa");
    m_codes << "he"; m_names << tr("Hebrew");
	m_codes << "hi"; m_names << tr("Hindi");
	m_codes << "ho"; m_names << tr("Hiri Motu");
	m_codes << "hu"; m_names << tr("Hungarian");
	m_codes << "is"; m_names << tr("Icelandic");
	m_codes << "io"; m_names << tr("Ido");
	m_codes << "ig"; m_names << tr("Igbo");
	m_codes << "id"; m_names << tr("Indonesian");
	m_codes << "ia"; m_names << tr("Interlingua (IALA)");
	m_codes << "ie"; m_names << tr("Interlingue, Occidental");
	m_codes << "iu"; m_names << tr("Inuktitut");
	m_codes << "ik"; m_names << tr("Inupiaq");
	m_codes << "ga"; m_names << tr("Irish");
	m_codes << "it"; m_names << tr("Italian");
	m_codes << "ja"; m_names << tr("Japanese");
	m_codes << "jv"; m_names << tr("Javanese");
	m_codes << "kl"; m_names << tr("Kalaallisut, Greenlandic");
	m_codes << "kn"; m_names << tr("Kannada");
	m_codes << "kr"; m_names << tr("Kanuri");
	m_codes << "ks"; m_names << tr("Kashmiri");
	m_codes << "kk"; m_names << tr("Kazakh");
	m_codes << "ki"; m_names << tr("Kikuyu, Gikuyu");
	m_codes << "rw"; m_names << tr("Kinyarwanda");
	m_codes << "ky"; m_names << tr("Kirghiz, Kyrgyz");
	m_codes << "kv"; m_names << tr("Komi");
	m_codes << "kg"; m_names << tr("Kongo");
	m_codes << "ko"; m_names << tr("Korean");
	m_codes << "ku"; m_names << tr("Kurdish");
	m_codes << "kj"; m_names << tr("Kwanyama, Kuanyama");
	m_codes << "lo"; m_names << tr("Lao");
	m_codes << "la"; m_names << tr("Latin");
	m_codes << "lv"; m_names << tr("Latvian");
	m_codes << "li"; m_names << tr("Limburgish, Limburgan, Limburger");
	m_codes << "ln"; m_names << tr("Lingala");
	m_codes << "lt"; m_names << tr("Lithuanian");
	m_codes << "lu"; m_names << tr("Luba-Katanga");
	m_codes << "lb"; m_names << tr("Luxembourgish, Letzeburgesch");
	m_codes << "mk"; m_names << tr("Macedonian");
	m_codes << "mg"; m_names << tr("Malagasy");
	m_codes << "ms"; m_names << tr("Malay");
	m_codes << "ml"; m_names << tr("Malayalam");
	m_codes << "mt"; m_names << tr("Maltese");
	m_codes << "gv"; m_names << tr("Manx");
	m_codes << "mi"; m_names << tr("Māori");
	m_codes << "mr"; m_names << tr("Marathi");
	m_codes << "mh"; m_names << tr("Marshallese");
	m_codes << "el"; m_names << tr("Modern Greek");
	m_codes << "he"; m_names << tr("Modern Hebrew");
	m_codes << "mn"; m_names << tr("Mongolian");
	m_codes << "na"; m_names << tr("Nauru");
	m_codes << "nv"; m_names << tr("Navajo, Navaho");
	m_codes << "ng"; m_names << tr("Ndonga");
	m_codes << "ne"; m_names << tr("Nepali");
	m_codes << "nd"; m_names << tr("North Ndebele");
	m_codes << "se"; m_names << tr("Northern Sami");
	m_codes << "no"; m_names << tr("Norwegian");
	m_codes << "nb"; m_names << tr("Norwegian Bokmål");
	m_codes << "nn"; m_names << tr("Norwegian Nynorsk");
	m_codes << "oc"; m_names << tr("Occitan (after 1500)");
	m_codes << "oj"; m_names << tr("Ojibwa");
	m_codes << "or"; m_names << tr("Oriya");
	m_codes << "om"; m_names << tr("Oromo");
	m_codes << "os"; m_names << tr("Ossetian, Ossetic");
	m_codes << "pi"; m_names << tr("Pāli");
	m_codes << "pa"; m_names << tr("Panjabi, Punjabi");
	m_codes << "ps"; m_names << tr("Pashto, Pushto");
	m_codes << "fa"; m_names << tr("Persian");
	m_codes << "pl"; m_names << tr("Polish");
	m_codes << "pt"; m_names << tr("Portuguese");
	m_codes << "qu"; m_names << tr("Quechua");
	m_codes << "ro"; m_names << tr("Romanian, Moldavian, Moldovan");
	m_codes << "rm"; m_names << tr("Romansh");
	m_codes << "rn"; m_names << tr("Rundi");
	m_codes << "ru"; m_names << tr("Russian");
	m_codes << "sm"; m_names << tr("Samoan");
	m_codes << "sg"; m_names << tr("Sango");
	m_codes << "sa"; m_names << tr("Sanskrit");
	m_codes << "sc"; m_names << tr("Sardinian");
	m_codes << "sr"; m_names << tr("Serbian");
	m_codes << "sn"; m_names << tr("Shona");
	m_codes << "ii"; m_names << tr("Sichuan Yi, Nuosu");
	m_codes << "sd"; m_names << tr("Sindhi");
	m_codes << "si"; m_names << tr("Sinhala, Sinhalese");
	m_codes << "cu"; m_names << tr("Slavic, Old Slavonic");
	m_codes << "sk"; m_names << tr("Slovak");
	m_codes << "sl"; m_names << tr("Slovene");
	m_codes << "so"; m_names << tr("Somali");
	m_codes << "nr"; m_names << tr("South Ndebele");
	m_codes << "st"; m_names << tr("Southern Sotho");
	m_codes << "es"; m_names << tr("Spanish, Castilian");
	m_codes << "su"; m_names << tr("Sundanese");
	m_codes << "sw"; m_names << tr("Swahili");
	m_codes << "ss"; m_names << tr("Swati");
	m_codes << "sv"; m_names << tr("Swedish");
	m_codes << "tl"; m_names << tr("Tagalog");
	m_codes << "ty"; m_names << tr("Tahitian");
	m_codes << "tg"; m_names << tr("Tajik");
	m_codes << "ta"; m_names << tr("Tamil");
	m_codes << "tt"; m_names << tr("Tatar");
	m_codes << "te"; m_names << tr("Telugu");
	m_codes << "th"; m_names << tr("Thai");
	m_codes << "bo"; m_names << tr("Tibetan");
	m_codes << "ti"; m_names << tr("Tigrinya");
	m_codes << "to"; m_names << tr("Tonga (Tonga Islands)");
	m_codes << "ts"; m_names << tr("Tsonga");
	m_codes << "tn"; m_names << tr("Tswana");
	m_codes << "tr"; m_names << tr("Turkish");
	m_codes << "tk"; m_names << tr("Turkmen");
	m_codes << "tw"; m_names << tr("Twi");
	m_codes << "ug"; m_names << tr("Uighur, Uyghur");
	m_codes << "uk"; m_names << tr("Ukrainian");
	m_codes << "ur"; m_names << tr("Urdu");
	m_codes << "uz"; m_names << tr("Uzbek");
	m_codes << "ve"; m_names << tr("Venda");
	m_codes << "vi"; m_names << tr("Vietnamese");
	m_codes << "vo"; m_names << tr("Volapük");
	m_codes << "wa"; m_names << tr("Walloon");
	m_codes << "cy"; m_names << tr("Welsh");
	m_codes << "fy"; m_names << tr("Western Frisian");
	m_codes << "wo"; m_names << tr("Wolof");
	m_codes << "xh"; m_names << tr("Xhosa");
	m_codes << "yi"; m_names << tr("Yiddish");
	m_codes << "yo"; m_names << tr("Yoruba");
	m_codes << "za"; m_names << tr("Zhuang, Chuang");
	m_codes << "zu"; m_names << tr("Zulu");

}
