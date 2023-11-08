/****************************************************************************
*
*  Copyright (C) 2002-2010 Helta Kft. / NociSoft Software Solutions
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
#ifndef NCRDFIELDTEXTEDIT_H
#define NCRDFIELDTEXTEDIT_H

#include <QTextEdit>

QT_BEGIN_NAMESPACE
class QCompleter;
QT_END_NAMESPACE

class NCRDFieldTextEdit : public QTextEdit
{
	Q_OBJECT
public:
	NCRDFieldTextEdit( QWidget* parent =0 );
	~NCRDFieldTextEdit();

	void setCompleter(QCompleter *c);
	QCompleter *completer() const;

signals:
	void ctrlEnter();

protected:
	void keyPressEvent(QKeyEvent *e);
	void focusInEvent(QFocusEvent *e);

 private slots:
	 void insertCompletion(const QString &completion);

 private:
	 QString textUnderCursor() const;

 private:
	 QCompleter *m_completer;
};

#endif // NCRDFIELDTEXTEDIT_H
