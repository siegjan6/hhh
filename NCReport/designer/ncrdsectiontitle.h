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
#ifndef NCRDSECTIONTITLE_H
#define NCRDSECTIONTITLE_H

#include <QWidget>
#include <QIcon>

/*!
Title bar widget of a report section
*/

/*!

\section 功能

标题栏部件管理类

*/
class NCRDSectionTitle : public QWidget
{
public:
    NCRDSectionTitle( QWidget * parent );
    ~NCRDSectionTitle();

    void setText(const QString& t);
    void setIcon( const QIcon& icon );
    QString text() const { return m_text; }
protected:
    virtual void paintEvent( QPaintEvent* );
private:
    int topMargin;
    QIcon m_icon;
    QString m_text;
};

#endif
