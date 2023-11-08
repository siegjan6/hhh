#include "mailsender.h"
#include "mailsender_p.cpp"

#include <QString>
#include <QTextStream>
#include <QByteArray>
#include <QDateTime>
#include <QTextCodec>
#include <QFile>
#include <QFileInfo>
#include <QHostInfo>
#include <time.h>
#include <QCryptographicHash>
#include <QNetworkProxy>

/*!
LMailSender class provides the e-mail sending functions. Using this class you can send full featured e-mails from code.
*/

LMailSender::LMailSender(bool ssl, QObject* parent)
    : QThread(parent), d_ptr(new LMailSenderPrivate(this))
{
    Q_D(LMailSender);
    d->setup(SerialMode,ssl);
}

LMailSender::LMailSender(bool ssl, SendMode mode, QObject* parent /*= 0*/)
    : QThread(parent), d_ptr(new LMailSenderPrivate(this))
{
    Q_D(LMailSender);
    d->setup(mode,ssl);
}


LMailSender::~LMailSender()
{
}

void LMailSender::setFrom(const QString& from)
{
    Q_D(LMailSender);
    d->setFrom(from);
}
void LMailSender::setISO(ISO iso)
{
    Q_D(LMailSender);
    d->setISO(iso);
}
void LMailSender::setEncoding(Encoding encoding)
{
    Q_D(LMailSender);
    d->setEncoding(encoding);
}

void LMailSender::errorReceived(QAbstractSocket::SocketError socketError)
{
    //Q_D(LMailSender);
    QString msg;

    switch (socketError) {
    case QAbstractSocket::ConnectionRefusedError:
        msg = "ConnectionRefusedError";
        break;
    case QAbstractSocket::RemoteHostClosedError:
        msg = "RemoteHostClosedError";
        break;
    case QAbstractSocket::HostNotFoundError:
        msg = "HostNotFoundError";
        break;
    case QAbstractSocket::SocketAccessError:
        msg = "SocketAccessError";
        break;
    case QAbstractSocket::SocketResourceError:
        msg = "SocketResourceError";
        break;
    case QAbstractSocket::SocketTimeoutError:
        msg = "SocketTimeoutError";
        break;
    case QAbstractSocket::DatagramTooLargeError:
        msg = "DatagramTooLargeError";
        break;
    case QAbstractSocket::NetworkError:
        msg = "NetworkError";
        break;
    case QAbstractSocket::AddressInUseError:
        msg = "AddressInUseError";
        break;
    case QAbstractSocket::SocketAddressNotAvailableError:
        msg = "SocketAddressNotAvailableError";
        break;
    case QAbstractSocket::UnsupportedSocketOperationError:
        msg = "UnsupportedSocketOperationError";
        break;
    case QAbstractSocket::ProxyAuthenticationRequiredError:
        msg = "ProxyAuthenticationRequiredError";
        break;
    default:
        msg = "Unknown Error";
    }


    qDebug("Socket error [%s]",qPrintable(msg));
}

void LMailSender::run()
{
    Q_D(LMailSender);
    d->send();
    deleteLater();
}

bool LMailSender::send()
{
    Q_D(LMailSender);

    if (d->sendMode == ThreadMode) {
        start();
        return true;
    } else {
        return d->send();
    }
}

void LMailSender::proxyAuthentication(const QNetworkProxy& proxy, QAuthenticator* authenticator)
{
    Q_UNUSED(proxy)
    Q_D(LMailSender);
    *authenticator = d->authenticator;
}
void LMailSender::setProxyAuthenticator(const QAuthenticator& authenticator)
{
    Q_D(LMailSender);
    d->authenticator = authenticator;
}
QString LMailSender::lastError() {
    Q_D(LMailSender);
    return d->lastError;
}
QString LMailSender::lastCmd() {
    Q_D(LMailSender);
    return d->lastCmd;
}
QString LMailSender::lastResponse() {
    Q_D(LMailSender);
    return d->lastResponse;
}
QString LMailSender::lastMailData() {
    Q_D(LMailSender);
    return d->lastMailData;
}
void LMailSender::setSmtpServer(const QString& smtpServer) {
    Q_D(LMailSender);
    d->smtpServer = smtpServer;
}
void LMailSender::setPort(int port) {
    Q_D(LMailSender);
    d->port = port;
}
void LMailSender::setTimeout(int timeout) {
    Q_D(LMailSender);
    d->timeout = timeout;
}
void LMailSender::setLogin(const QString& login, const QString& passwd) {
    Q_D(LMailSender);
    d->login = login;
    d->password = passwd;
}
void LMailSender::setSsl(bool ssl) {
    Q_D(LMailSender);
    d->ssl = ssl;
}
void LMailSender::setCc(const QStringList& cc) {
    Q_D(LMailSender);
    d->cc = cc;
}
void LMailSender::setBcc(const QStringList& bcc) {
    Q_D(LMailSender);
    d->bcc = bcc;
}
void LMailSender::setAttachment(const QString& filename, const QByteArray& data)
{
    Q_D(LMailSender);
    AttachmentFile f;
    f.m_filename = filename;
    f.m_file = data;
    d->attachments.append(f);
}
void LMailSender::setReplyTo(const QString& replyTo) {
    Q_D(LMailSender);
    d->replyTo = replyTo;
}
void LMailSender::setPriority(Priority priority) {
    Q_D(LMailSender);
    d->priority = priority;
}
void LMailSender::setTo(const QStringList& to) {
    Q_D(LMailSender);
    d->to = to;
}
void LMailSender::setSubject(const QString& subject) {
    Q_D(LMailSender);
    d->subject = subject;
}
void LMailSender::setBody(const QString& body) {
    Q_D(LMailSender);
    d->body = body;
}
void LMailSender::setFromName(const QString& fromName) {
    Q_D(LMailSender);
    d->fromName = fromName;
}
void LMailSender::setContentType(ContentType contentType) {
    Q_D(LMailSender);
    d->contentType = contentType;
}

QStringList LMailSender::to() const {
    Q_D(const LMailSender);
    return d->to;
}
QString LMailSender::from() const {
    Q_D(const LMailSender);
    return d->from;
}
