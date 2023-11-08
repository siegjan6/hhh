#include "defaulttransporter.h"
#include "qredisclient/connection.h"
#include "qredisclient/connectionconfig.h"
#include "qredisclient/utils/sync.h"

RedisClient::DefaultTransporter::DefaultTransporter(RedisClient::Connection *c)
    : RedisClient::AbstractTransporter(c), m_socket(nullptr), m_errorOccurred(false)
{
}

void RedisClient::DefaultTransporter::initSocket()
{
    using namespace RedisClient;

    m_socket = QSharedPointer<QSslSocket>(new QSslSocket());
    m_socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

    connect(m_socket.data(), (void (QSslSocket::*)(QAbstractSocket::SocketError))&QSslSocket::error,
            this, &DefaultTransporter::error);
    connect(m_socket.data(), (void (QSslSocket::*)(const QList<QSslError> &errors))&QSslSocket::sslErrors,
            this, &DefaultTransporter::sslError);
    connect(m_socket.data(), &QAbstractSocket::readyRead, this, &AbstractTransporter::readyRead);
    connect(m_socket.data(), &QSslSocket::encrypted, this, [this]() { emit logEvent("SSL encryption: OK"); });
    //增加连接状态信号
    //connect(m_socket.data(), SIGNAL(stateChanged(QAbstractSocket::SocketState state)),this,SLOT(socketStateChanged(QAbstractSocket::SocketState)));//chp
    //connect(m_socket.data(), &QAbstractSocket::stateChanged,this,&AbstractTransporter::abc);
}

void RedisClient::DefaultTransporter::disconnectFromHost()
{
    if (m_socket.isNull())
        return;

    m_executionTimer.clear();
    m_loopTimer.clear();
    m_socket->abort();
    m_socket.clear();
}

bool RedisClient::DefaultTransporter::isInitialized() const
{
    return !m_socket.isNull();
}

bool RedisClient::DefaultTransporter::isSocketReconnectRequired() const
{
    return m_socket->state() == QAbstractSocket::UnconnectedState;
}

bool RedisClient::DefaultTransporter::canReadFromSocket()
{
    return m_socket->bytesAvailable() > 0;
}

QByteArray RedisClient::DefaultTransporter::readFromSocket()
{
    return m_socket->readAll();
}

bool RedisClient::DefaultTransporter::connectToHost()
{
    m_errorOccurred = false;

        auto conf = m_connection->getConfig();

    bool connectionResult = false;

    if (conf.useSsl()) {
        QList<QSslCertificate> trustedCas = conf.sslCaCertificates(); // Required

        if (trustedCas.empty()) {
            emit errorOccurred("SSL Error: no trusted Cas");
            return false;
        }

        m_socket->addCaCertificates(trustedCas);

        QString privateKey = conf.sslPrivateKeyPath();
        if (!privateKey.isEmpty()) {
            m_socket->setPrivateKey(privateKey);
        }

        QString localCert = conf.sslLocalCertPath();
        if (!localCert.isEmpty()) {
            m_socket->setLocalCertificate(localCert);
        }

        m_socket->connectToHostEncrypted(conf.host(), conf.port());
        connectionResult = m_socket->waitForEncrypted(conf.connectionTimeout())
                && m_socket->waitForConnected(conf.connectionTimeout());

    } else {
        m_socket->connectToHost(conf.host(), conf.port());
        connectionResult = m_socket->waitForConnected(conf.connectionTimeout());
    }

    if (connectionResult)
    {
        emit connected();
        emit logEvent(QString("%1 > connected").arg(conf.name()));
        return true;
    }

    if (!m_errorOccurred)
        emit errorOccurred("Connection timeout");

    emit logEvent(QString("%1 > connection failed").arg(conf.name()));
    return false;
}

void RedisClient::DefaultTransporter::sendCommand(const QByteArray& cmd)
{
    m_socket->write(cmd);
    m_socket->flush();
}

void RedisClient::DefaultTransporter::error(QAbstractSocket::SocketError error)
{
    if (error == QAbstractSocket::UnknownSocketError
            && connectToHost()
            && m_runningCommand) {
        return runCommand(m_runningCommand->cmd);
    }

    m_errorOccurred = true;

    emit errorOccurred(
        QString("Connection error: %1").arg(m_socket->errorString())
        );

    return sendResponse(m_response);
}

void RedisClient::DefaultTransporter::sslError(const QList<QSslError> &errors)
{
    m_errorOccurred = true;
    for (QSslError err : errors)
        emit errorOccurred(QString("SSL error: %1").arg(err.errorString()));
}

void RedisClient::DefaultTransporter::reconnect()
{
    emit logEvent("Reconnect to host");
    //m_loopTimer->stop();//chp
    m_socket->abort();    
    connectToHost();
}
