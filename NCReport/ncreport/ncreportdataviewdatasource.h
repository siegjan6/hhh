#ifndef NCREPORTDATAVIEWDATASOURCE_2016_11_16_H
#define NCREPORTDATAVIEWDATASOURCE_2016_11_16_H
#include "ncreportdatasource.h"

namespace DBQ
{
    struct View;
}
class IMDDBQueryEngine;
class CMDDatabaseRecordset;

/*!
 * \brief 编写此类是为了接入MindSCADA中的数据库查询模块
 * 一个源对应一个数据视图
 */
//TODO:重命名NCReportDataViewDataSource
class NCReportDataViewDataSource : public NCReportDataSource
{
Q_OBJECT
public:
    NCReportDataViewDataSource(QObject * parent);
    ~NCReportDataViewDataSource();
    //<实现纯虚接口>
    bool open(OPEN_MODEL model);
    bool close();
    bool first();
    bool last();
    bool next(bool isSamplingPageBreak=false);
    bool previous();
    bool hasNext();
    bool seek( int index );
    int size() const;
    QVariant value( const QString& columnname, bool* ok=0, int nQTDataRole = -1 ) const;
    QVariant value( int column, bool* ok=0, int nQTDataRole = -1 ) const;
    bool isValid() const;
    //</实现纯虚接口>
    //-------------------------------
    bool update();
    virtual int rowCount() const;
    virtual int columnCount() const;
    virtual bool read( NCReportXMLReader* reader );
    virtual bool write( NCReportXMLWriter* writer );
    virtual void evaluate( NCReportEvaluator* evaluator);
    virtual QStringList keywordList();
    virtual QStringList columnNames();
    virtual QString columnName(int column) const;
    virtual bool hasColumn(int column) const;
    virtual bool hasColumn(const QString& columnname) const;
    virtual int columnIndexByName(const QString& columnname) const;
    //------------------------------------
    QString getViewName();
    void setViewName(const QString & viewName);
    void setViewPath(const QString & viewPath);
    void setQueryEngine(IMDDBQueryEngine * pDBQueryEngine);
    //函数功能:获得此视图对应的所有视图字段
    //QList<DBQ::ViewField *> getFields();
 protected:
    //如果没有加载View 则加载View
    bool TestAndLoadView();
 protected:
    QString m_ViewName;
    DBQ::View * m_pView;
    QString m_QueryID;
    CMDDatabaseRecordset * m_pRecordset;
    QString m_ViewPath;//View所在路径
    IMDDBQueryEngine * m_pDBQueryEngine;

};

#endif // NCREPORTDATAVIEWDATASOURCE_2016_11_16_H
