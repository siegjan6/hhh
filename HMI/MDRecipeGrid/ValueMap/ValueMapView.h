/*!
author Y2hvdWps 2016.10.24
section 数值映射表表格类
*/
#ifndef VALUEMAPVIEW_H
#define VALUEMAPVIEW_H

#include <QTableView>
#include "IDrawEnvironment.h"
#include "MDMultiLanguage.h"
#include "ValueMapManager.h"


class CValueMapView : public QTableView
{
public:
    explicit CValueMapView(QWidget *parent = 0);
    virtual ~CValueMapView(){}

    void init(CValueMapManager::Values &data);

    void deleteSelectedRows();
    void selectedRows(QList<qint32>& rows);
    void setImageButtonItem(qint32 row);
    void setEnvironment(IDrawEnvrionment *environment);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
private:
    IDrawEnvrionment *m_environment;
    ITranslateWidgetTool* m_tool;
    qint32 m_imageButtonClickedRow;
private slots:
    void imageSelectButtonClickd();
};

#endif // VALUEMAPVIEW_H
