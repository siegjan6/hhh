#ifndef CDEMOCLASS_H
#define CDEMOCLASS_H

#include <QObject>
#include <QString>
#include <QPixmap>
#include <QVariant>

class CDemoClass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap NOTIFY pixmapChanged)
    Q_PROPERTY(QString other READ other WRITE setOther NOTIFY otherChanged)
public:
    CDemoClass(QObject *parent = 0);
    ~CDemoClass(){}

    QString name() const;
    void setName(QString name);

    QPixmap pixmap() const;
    void setPixmap(const QPixmap &pixmap);

    QString other() const;
    void setOther(const QString &other);
private Q_SLOTS:
    void slotInforChanged(QVariant);
Q_SIGNALS:
    void nameChanged(QVariant);
    void otherChanged(QVariant);
    void pixmapChanged(QPixmap);
    void inforChanged(QVariant);
private:
    QString m_name;
    QString m_other;
    QPixmap m_pixmap;

};

#endif // CDEMOCLASS_H
