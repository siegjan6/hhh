/***************************************************************************
 *   Copyright (C) 2010 by NociSoft Software Solutions
 *   support@nocisoft.com
 ***************************************************************************/

#ifndef NCRDITEMSOURCEPANEL_H
#define NCRDITEMSOURCEPANEL_H

#include <QGroupBox>
#include "ncreportitem.h"

QT_BEGIN_NAMESPACE
class QAbstractButton;
namespace Ui {
    class NCRDItemSourcePanel;
}
QT_END_NAMESPACE

class NCRDDocument;

/*!

\section 功能

控件数据源选择组合框管理类
＃＃＃在代码中暂时屏蔽了此项功能＃＃＃

*/
class NCRDItemSourcePanel : public QGroupBox
{
    Q_OBJECT
public:
    NCRDItemSourcePanel(QWidget *parent = 0);
    ~NCRDItemSourcePanel();

    void setItem( NCReportItem* item );
    void setDocument( NCRDDocument* document );
    void setSourceType( NCReportItem::SourceType );
    void setSourceTypeEnabled( NCReportItem::SourceType, bool enable );
    NCReportItem::SourceType sourceType() const;
    void setSourceText( const QString& );
    QString sourceText() const;
    QAbstractButton* buttonSelect();
    QAbstractButton* buttonPreview();
    QAbstractButton* buttonExpBuild();
    QWidget *sourceEdit();

signals:
    void sourceTypeChanged(int);

protected:
    void changeEvent(QEvent *e);

protected slots:
    void typeChanged(int);
    void openExpressionBuilder();

private:
    Ui::NCRDItemSourcePanel *ui;
    NCReportItem* m_item;
    NCRDDocument* m_document;

};

#endif // NCRDITEMSOURCEPANEL_H
