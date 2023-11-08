/****************************************************************************
*
*  Copyright (C) 2007-2010 Helta Kft. / NociSoft Software Solutions
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
#ifndef NCRDPARAMETEREDITOR_H
#define NCRDPARAMETEREDITOR_H

#include "ui_parametereditor.h"
#include <QMap>
#include <QStringList>

class NCReportDef;

/*!

\section 功能

参数编辑器管理类
【用于编辑参数类型的数据源相关定义和属性】

*/
class NCRDParameterEditor : public QWidget, public Ui::NCRDParameterEditorUI
{
    Q_OBJECT
public:
    NCRDParameterEditor(NCReportDef *reportDef, QStringList& removedParameters, QWidget * parent = 0, Qt::WindowFlags f = 0 );
    //NCRDParameterEditor( QHash<QString, QVariant>& parameters, QStringList& removedParameters, QWidget * parent = 0, Qt::WindowFlags f = 0 );
    ~NCRDParameterEditor();

    void save();
    void load();
    void setCurrentParameter( const QString& paramID );

public slots:
    void add();
    void remove();
    void loadFromFile();

private:
    NCReportDef *m_rdef;
    QStringList& m_removedParameters;
    //QMap<QString, QVariant>& m_parameters;

    void addParam(const QString& name, const QString& value);

};

#endif // NCRDPARAMETEREDITOR_H
