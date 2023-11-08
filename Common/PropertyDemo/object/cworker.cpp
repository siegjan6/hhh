#include "cworker.h"


class CWorkerPrivate
{
    CWorker *q_ptr;
    Q_DECLARE_PUBLIC(CWorker)

private:
    CWorker::Sex m_sex;
    QString m_name;
    int m_id;
    QString m_complexion;
    QMap<QString, QVariant> m_map;
    QList<QStringList> m_group;
    QMap <QString,QVariant> m_groupMap;
private:    
     void  init();
     void  update();
};

CWorker::CWorker(QObject *parent) :
    QObject(parent)
{
    d_ptr = new CWorkerPrivate;
    d_ptr->q_ptr = this;
    d_ptr->init();
}

CWorker::~CWorker()
{
    delete d_ptr;
}

void CWorker::setSex(CWorker::Sex sex)
{
    d_ptr->m_sex = sex;
    emit sexChanged(sex);
}

CWorker::Sex CWorker::sex() const
{
    return d_ptr->m_sex;
}

QString CWorker::name() const
{
    return d_ptr->m_name;
}

void CWorker::setName(QString name)
{
    d_ptr->m_name = name;
    emit nameChanged(name);
}

int CWorker::id() const
{
    return d_ptr->m_id;
}

void CWorker::setId(int id)
{
    d_ptr->m_id = id;
    emit idChanged(id);
}

QString CWorker::complexion() const
{
    return d_ptr->m_complexion;
}

void CWorker::setComplexion(QString complexion)
{
    d_ptr->m_complexion = complexion;
    emit complexionChanged(complexion);
}

QMap<QString, QVariant> CWorker::propertyMap() const
{
    return d_ptr->m_map;
}

QMap<QString, QVariant> CWorker::groupMap() const
{
    return d_ptr->m_groupMap;
}

void CWorker::refreshLanguage()
{
    d_ptr->update();
}

void CWorkerPrivate::init()
{
    m_map["sex"]        = "性别";
    m_map["name"]       = "姓名";
    m_map["id"]         = "工号";
    m_map["complexion"] = "肤色";
    m_map["Man"]        = "男";
    m_map["Woman"]      = "女";
    m_map["className"]  = "工人";

    m_id = 0;

    QStringList m_base;
    m_base << "基本属性"<< "id"<< "sex";

    QStringList m_pro;
    m_pro  << "高级属性"<<"name"<<"complexion";

    m_groupMap["1"] = m_base;
    m_groupMap["0"] = m_pro;
}

void CWorkerPrivate::update()
{
    m_map["sex"]        = "sex";
    m_map["name"]       = "name";
    m_map["id"]         = "id";
    m_map["complexion"] = "complexion";
    m_map["Man"]        = "Man";
    m_map["Woman"]      = "Woman";
    m_map["className"]  = "CWoker";

    m_id = 0;

    QStringList m_base;
    m_base << "Base"<< "id"<< "sex";

    QStringList m_pro;
    m_pro  << "Pro"<<"name"<<"complexion";

    m_groupMap["1"] = m_base;
    m_groupMap["0"] = m_pro;
}
