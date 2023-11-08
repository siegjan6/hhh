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

#ifndef NCRDFILESELECTOR_H
#define NCRDFILESELECTOR_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QWidget;
class QLineEdit;
class QAbstractButton;
QT_END_NAMESPACE

/*!

\section 功能

报表文件选择管理类
【提供报表文件选择的相关接口，如文件名称，文件描述。文件类型等】。

*/
class NCRDFileSelector : public QObject
{
	Q_OBJECT
public:
	explicit NCRDFileSelector(QWidget *parent, QLineEdit *le, QAbstractButton *btn);
	enum EditMode { TypeAndSelect=0, SelectOnly };
	enum SelectMode { Open=0, Save };
	/*
	void setButton( QAbstractButton* btn );
	void setLineEdit( QLineEdit* le );
	void setWidgets( QLineEdit *le, QAbstractButton *btn );
	*/
	void setEditMode( EditMode mode);
	void setSelectMode( SelectMode mode);
	void setFileName( const QString& fileName );
	QString fileName() const;
	void setDescription(const QString& desc);
	void setFileTypes(const QString& type);

signals:
	void fileNameChanged(const QString&);

public slots:
	void selectFile();

private slots:
	void slotFileNameChanged(const QString&);

private:
	QLineEdit* m_lineEditFileName;
	QAbstractButton* m_btnSelect;
	QWidget* m_parentWidget;
	SelectMode m_selectMode;
	QString m_desc;
	QString m_type;
};

#endif // NCRDFILESELECTOR_H
