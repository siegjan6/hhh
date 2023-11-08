#ifndef MAILSENDER_P_H
#define MAILSENDER_P_H

#include "mailsender.h"

#include <QString>
#include <QStringList>
#include <QObject>
#include <QTcpSocket>
#include <QPointer>
#include <QAuthenticator>

 struct AttachmentFile {
    QString m_filename;
    QByteArray m_file;
 };

class  LMailSenderPrivate
{
public:
    LMailSenderPrivate(LMailSender* parent);
    virtual ~LMailSenderPrivate() {}

private:
    Q_DECLARE_PUBLIC(LMailSender);
    LMailSender* q_ptr;

public:
    LMailSender::SendMode sendMode;

    QString smtpServer;
    int port;
    int timeout;
    QString login;
    QString password;
    QTcpSocket* socket;
    bool ssl;
    QAuthenticator authenticator;
    QString lastError;
    QString lastCmd;
    QString lastResponse;
    QString lastMailData;

    QString from;
    QStringList to;
    QString subject;
    QString body;
    QStringList cc;
    QStringList bcc;
    QList<AttachmentFile> attachments;
    QString fromName;
    QString replyTo;
    LMailSender::Priority priority;
    LMailSender::ContentType contentType;
    QString encoding;
    QString iso;
    QString bodyCodec;
    QString confirmTo;

public:
    void setup(LMailSender::SendMode mode, bool i_ssl);

    bool send();

    QString getMimeType(QString ext) const;
    QString mailData();
    QString contentTypeToString() const;
    bool read(const QString& waitfor);
    bool sendCommand(const QString& cmd, const QString& waitfor);
    void setError(const QString& msg);

    void setFrom(const QString& from);
    void setISO(LMailSender::ISO iso);
    void setEncoding(LMailSender::Encoding encoding);
};

#endif // MAILSENDER_P_H
