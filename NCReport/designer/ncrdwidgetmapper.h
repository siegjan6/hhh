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
#ifndef NCRDWIDGETMAPPER_H
#define NCRDWIDGETMAPPER_H

#include "ncrd_global.h"

#include <QObject>
#include <QString>
#include <QVariant>
#include <QHash>

class QWidget;

/*!
 * \brief The NCRDWidgetMapper class
 * This is a class for mapping data widgets onto a data container hash and vica verse.
 * The general purpose of this class to manage a dialog's data widgets setter/getter function.
 * For special input widgets you can subclass it, but it handles the general input widgets by default.
 */

/*!

section 功能

【映射数据控件类到数据容器的类，主要目的时管理对话框的数据控件的setter/getter函数
对于特定的输入小部件，您可以将它的子类化，但它默认的处理一般的输入窗口小部件】

*/
class NCREPORTDESIGNER_SHARED_EXPORT NCRDWidgetMapper : public QObject
{
    Q_OBJECT
public:
    NCRDWidgetMapper(QObject *parent = 0);

    enum MapDirection { DataToWidget=0, WidgetToData };

    /*!
     * \brief setWidget sets the container widget.
     * \param widget container/parent widget
     */
    void setWidget( QWidget* widget );
    /*!
     * \brief widget
     * \return Returns the container widget.
     */
    QWidget *widget() const;
    /*!
     * \brief setDataContainer sets the data container pointer
     * \param container
     */
    void setDataContainer( QHash<QString,QVariant>* container );
    /*!
     * \brief dataContainer returns the data container pointer
     * \return
     */
    QHash<QString,QVariant>* dataContainer() const;

    void updateWidget();
    void updateData();

protected:
    virtual void mapWidgets( MapDirection direction );
    /*!
     * \brief mapping
     * \param direction
     * \param widget
     * \param id
     * Virtual method for mapping widget to data hash and vica verse depending on the direction flag.
     * The default behaviour iterates over the parent widget children and sets the basic widget types.
     * id parameter represents the object name: objectName()
     */
    virtual void mapping(MapDirection direction, QWidget* widget, const QString &id);

private:
    QWidget *m_widget;
    QHash<QString,QVariant>* m_container;

};

#endif
