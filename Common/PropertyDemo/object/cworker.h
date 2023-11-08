#ifndef CWORKER_H
#define CWORKER_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QString>

class CWorkerPrivate;
class CWorker : public QObject
{
    Q_OBJECT
    Q_ENUMS(Sex)
    Q_PROPERTY(QMap propertyMap READ propertyMap)
    Q_PROPERTY(QMap groupMap READ groupMap)

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(Sex sex READ sex WRITE setSex NOTIFY sexChanged)
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString complexion READ complexion WRITE setComplexion  NOTIFY complexionChanged)

public:
    explicit CWorker(QObject *parent = 0);

    enum class Sex{Man,Woman};
    ~CWorker();

    void setSex(Sex sex);
    Sex sex() const;

    QString name() const;
    void setName(QString name);

    int id() const ;
    void setId(int id);

    QString complexion() const ;
    void setComplexion(QString complexion);


    QMap<QString, QVariant> propertyMap() const ;
    QMap<QString, QVariant> groupMap() const;

    void refreshLanguage();

Q_SIGNALS:
    void sexChanged(Sex);
    void nameChanged(QString);
    void idChanged(int);
    void complexionChanged(QString);

private:

    CWorkerPrivate * d_ptr;
    Q_DECLARE_PRIVATE(CWorker)
    Q_DISABLE_COPY(CWorker)
};

#endif // CWORKER_H
