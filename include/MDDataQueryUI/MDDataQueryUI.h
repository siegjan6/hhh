#ifndef MDDATAQUERYUI_H
#define MDDATAQUERYUI_H


#include <QtCore/qglobal.h>
#include <QMap>
#include <QStringList>
#include <QMenu>

#if defined(MDDATAQUERYUI_LIBRARY)
#  define MDDATAQUERYUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDATAQUERYUISHARED_EXPORT Q_DECL_IMPORT
#endif

class CMenu;
class CMDFilterTreeDataManager;
class MDDATAQUERYUISHARED_EXPORT MDDataQueryUI
{

public:
    MDDataQueryUI(QWidget *parent = 0,bool isReport = false);
    ~MDDataQueryUI();

    void setDataSourceInfor(const QMap<QString, QStringList> &dataSourceInfor);

    QMap<QString, QStringList> dataSourceInfor() const;
    QString hostName() const;
    QString driver() const;
    QString IP() const;
    QString port() const;
    QString userName() const;
    QString password() const;

    bool isDatabaseEnabled() const;
    int exec(const QPoint &p) const;

    int exportType() const;
    void createActionFlag(int num);
    void setFilterPluginManager(CMDFilterTreeDataManager *manager);
private:
    CMenu *m_menu;
};

#endif // MDDATAQUERYUI_H
