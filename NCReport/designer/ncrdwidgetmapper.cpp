/****************************************************************************
*
*  Copyright (C) 2002-2014 Helta Kft. / NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: office@nocisoft.com
*  Web: www.nocisoft.com
*
*  This file is part of the NCReport reporting software
*
*  Licensees holding a valid NCReport License Agreement may use this
*  file in accordance with the rights, responsibilities, and obligations
*  contained therein. Please consult your licensing agreement or contact
*  office@nocisoft.com if any conditions of this licensing are not clear
*  to you.
*
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/
#include "ncrdwidgetmapper.h"

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDateEdit>
#include <QDateTimeEdit>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QSlider>
#include <QDial>
#include <QRadioButton>

NCRDWidgetMapper::NCRDWidgetMapper(QObject *parent) : QObject(parent), m_widget(0), m_container(0)
{
}

void NCRDWidgetMapper::setWidget(QWidget *widget)
{
    m_widget = widget;
}

QWidget *NCRDWidgetMapper::widget() const
{
    return m_widget;
}

void NCRDWidgetMapper::setDataContainer(QHash<QString, QVariant> *container)
{
    m_container = container;
}

QHash<QString, QVariant> *NCRDWidgetMapper::dataContainer() const
{
    return m_container;
}


void NCRDWidgetMapper::updateWidget()
{
    mapWidgets(DataToWidget);
}


void NCRDWidgetMapper::updateData()
{
    mapWidgets(WidgetToData);
}


void NCRDWidgetMapper::mapWidgets(NCRDWidgetMapper::MapDirection direction)
{
    if (!m_widget || !m_container)
        return;

    QList<QWidget*> widgets = m_widget->findChildren<QWidget*>();
    for (int i=0; i<widgets.count(); i++) {
        mapping(direction,widgets.at(i),widgets.at(i)->objectName());
    }
}


void NCRDWidgetMapper::mapping(MapDirection direction, QWidget *w, const QString& id )
{
    if ( qstrcmp(w->metaObject()->className(),"QLineEdit")==0) {
        QLineEdit *dataWidget = qobject_cast<QLineEdit *>(w);
        if (qobject_cast<QAbstractSpinBox*>(dataWidget->parent()))
            return; // recursive line edit protection for spinbox widgets

        if (direction==DataToWidget)
            dataWidget->setText(m_container->value(id).toString());
        else
            m_container->insert(id, dataWidget->text());
    }
    else if ( qstrcmp(w->metaObject()->className(),"QComboBox")==0) {
        QComboBox *combo = qobject_cast<QComboBox*>(w);
        if (direction==DataToWidget)
            combo->setCurrentIndex(m_container->value(id).toInt());
        else
            m_container->insert(id, combo->currentIndex());
    }
    else if ( qstrcmp(w->metaObject()->className(),"QSpinBox")==0) {
        QSpinBox *dataWidget = qobject_cast<QSpinBox *>(w);
        if (direction==DataToWidget)
            dataWidget->setValue(m_container->value(id).toInt());
        else
            m_container->insert(id, dataWidget->value());
    }
    else if ( qstrcmp(w->metaObject()->className(),"QDoubleSpinBox")==0) {
        QDoubleSpinBox *dataWidget = qobject_cast<QDoubleSpinBox *>(w);
        if (direction==DataToWidget)
            dataWidget->setValue(m_container->value(id).toDouble());
        else
            m_container->insert(id, dataWidget->value());
    }
    else if ( qstrcmp(w->metaObject()->className(),"QDateTimeEdit")==0) {
        QDateTimeEdit *dataWidget = qobject_cast<QDateTimeEdit *>(w);
        if (direction==DataToWidget)
            dataWidget->setDateTime(m_container->value(id).toDateTime());
        else
            m_container->insert(id, dataWidget->dateTime());
    }
    else if ( qstrcmp(w->metaObject()->className(),"QDateEdit")==0) {
        QDateEdit *dataWidget = qobject_cast<QDateEdit*>(w);
        if (direction==DataToWidget)
            dataWidget->setDate(m_container->value(id).toDate());
        else
            m_container->insert(id, dataWidget->date());
    }
    else if ( qstrcmp(w->metaObject()->className(),"QPlainTextEdit")==0) {
        QPlainTextEdit *dataWidget = qobject_cast<QPlainTextEdit*>(w);
        if (direction==DataToWidget)
            dataWidget->setPlainText(m_container->value(id).toString());
        else
            m_container->insert(id, dataWidget->toPlainText());
    }
    else if ( qstrcmp(w->metaObject()->className(),"QTextEdit")==0) {
        QTextEdit *dataWidget = qobject_cast<QTextEdit*>(w);
        if (direction==DataToWidget)
            dataWidget->setHtml(m_container->value(id).toString());
        else
            m_container->insert(id, dataWidget->toHtml());
    }
    else if ( qstrcmp(w->metaObject()->className(),"QCheckBox")==0) {
        QCheckBox *dataWidget = qobject_cast<QCheckBox*>(w);
        if (direction==DataToWidget)
            dataWidget->setChecked(m_container->value(id).toBool());
        else
            m_container->insert(id, dataWidget->isChecked());
    }
    else if ( qstrcmp(w->metaObject()->className(),"QRadioButton")==0) {
        QRadioButton *radio = qobject_cast<QRadioButton*>(w);
        if (direction==DataToWidget)
            radio->setChecked(m_container->value(id).toBool());
        else
            m_container->insert(id, radio->isChecked());
    }
    else if ( qstrcmp(w->metaObject()->className(),"QSlider")==0) {
        QSlider *slider = qobject_cast<QSlider*>(w);
        if (direction==DataToWidget)
            slider->setValue(m_container->value(id).toInt());
        else
            m_container->insert(id, slider->value());
    }
    else if ( qstrcmp(w->metaObject()->className(),"QDial")==0) {
        QDial *dial = qobject_cast<QDial*>(w);
        if (direction==DataToWidget)
            dial->setValue(m_container->value(id).toInt());
        else
            m_container->insert(id, dial->value());
    }
}

