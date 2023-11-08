#ifndef CMENU_H
#define CMENU_H

#include <QWidget>
#include <QMenu>

class CDataSourceDialog;
class CFilterDialog;
class CExportDialog;
class CMDFilterTreeDataManager;
class CMenu : public QWidget
{
    Q_OBJECT
public:
    explicit CMenu(QWidget *parent = 0,bool isReport = false);
    ~CMenu();

    void setDataSourceInfor(const QMap<QString, QStringList> &dataSourceInfor);

    QMap<QString, QStringList> dataSourceInfor() const;
    QString hostName() const;
    QString driver() const;
    QString IP() const;
    QString port() const;
    QString userName() const;
    QString password() const;

    bool isDatabaseEnabled() const;

    int exportType() const;
    void createActionFlag(int num);
    int exec(const QPoint &);

    void setFilterPluginManager(CMDFilterTreeDataManager *manager);
private:
    enum ActionType{
        Data = 0,
        Filter,
        Export
    };
    void addActions();

    void createAction(const QString &name, ActionType type);
private:
    QMenu                       *m_menu;
    QWidget                     *m_parent;

    CFilterDialog               *m_filterDialog;
    CExportDialog               *m_exportDialog;
    CDataSourceDialog           *m_dataSourceDialog;
    CMDFilterTreeDataManager    *m_filterManager;

    QMap<QString, QStringList>  m_dataInfor;
    bool                                          m_isExport;
    bool                                          m_isQuery;
};
#endif // CMENU_H
