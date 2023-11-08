/****************************************************************************
*
*  Copyright (C) 2002-2011 Helta Kft. / NociSoft Software Solutions
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

#ifndef NCRDCROSSTABDIALOG_H
#define NCRDCROSSTABDIALOG_H


#include <QDialog>
#include <QModelIndex>

class NCReportItem;
class NCRDDocument;
class NCRDCrossTabCellModel;
class NCReportCrossTabItem;

QT_BEGIN_NAMESPACE
class QDataWidgetMapper;

namespace Ui {
    class NCRDCrossTabDialog;
}
QT_END_NAMESPACE

/*!

\section 功能

交叉表控件属性配置类
【管理交叉表控件相关参数和属性】。
＃＃＃暂时屏蔽了交叉表控件属性配置类＃＃＃

*/
class NCRDCrossTabDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NCRDCrossTabDialog(NCReportItem *item, NCRDDocument* parent = 0);
    ~NCRDCrossTabDialog();

    void applyChanges();
    void discardChanges();
    Ui::NCRDCrossTabDialog *ui;
    QDataWidgetMapper* mapper() const { return m_mapper; }


public slots:
  /*$PUBLIC_SLOTS$*/
    void accept();
    void reject();

protected slots:
    void slotItemChanged( const QModelIndex & current, const QModelIndex & previous );

private:

    NCRDCrossTabCellModel* m_cellModel;
    NCReportCrossTabItem* m_item;
    QDataWidgetMapper *m_mapper;

    void setupDialog();
    void setupMapper();
};

#endif // NCRDCROSSTABDIALOG_H
