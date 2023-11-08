#include "mailsender.h"
#include "mailsender_p.h"

#include <QString>
#include <QTextStream>
#include <QByteArray>
#include <QDateTime>
#include <QTextCodec>
#include <QFile>
#include <QFileInfo>
#include <QHostInfo>
#include <QCryptographicHash>
#undef QT_NO_SSL
#ifndef NCREPORT_NO_SSL
    #include <QSslSocket>
#endif

#ifndef Q_OS_WINCE
#   include <ctime>
#endif

//#include <LCore/LException>

static int dateswap(QString form, uint unixtime)
{
    QDateTime fromunix;
    fromunix.setTime_t(unixtime);
    QString numeric = fromunix.toString((const QString)form);
    bool ok;
    return (int)numeric.toFloat(&ok);
}

static QString encodeBase64(QString xml)
{
    QByteArray text;
    text.append(xml);
    return text.toBase64();
}

static QString TimeStampMail()
{
#ifndef Q_OS_WINCE
    /* mail rtf Date format! http://www.faqs.org/rfcs/rfc788.html */
    uint unixtime = (uint)std::time(NULL);
    QDateTime fromunix;
    fromunix.setTime_t(unixtime);
    QStringList RTFdays = QStringList() << "giorno_NULL" << "Mon" << "Tue" << "Wed" << "Thu" << "Fri" << "Sat" << "Sun";
    QStringList RTFmonth = QStringList() << "mese_NULL" << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun" << "Jul" << "Aug" << "Sep" << "Oct" << "Nov" << "Dec";
    QDate timeroad(dateswap("yyyy", unixtime), dateswap("M", unixtime), dateswap("d", unixtime));
    QStringList rtfd_line;
    rtfd_line.clear();
    rtfd_line.append("Date: ");
    rtfd_line.append(RTFdays.at(timeroad.dayOfWeek()));
    rtfd_line.append(", ");
    rtfd_line.append(QString::number(dateswap("d", unixtime)));
    rtfd_line.append(" ");
    rtfd_line.append(RTFmonth.at(dateswap("M", unixtime)));
    rtfd_line.append(" ");
    rtfd_line.append(QString::number(dateswap("yyyy", unixtime)));
    rtfd_line.append(" ");
    rtfd_line.append(fromunix.toString("hh:mm:ss"));
    rtfd_line.append("");

    return QString(rtfd_line.join(""));
#else
    throw StandardExceptionTr("Unter WindowsCe k�nnen keine Emails verschickt werden");
#endif
}

static QString createBoundary()
{
    QByteArray hash = QCryptographicHash::hash(QString(QString::number(qrand())).toUtf8(), QCryptographicHash::Md5);
    QString boundary = hash.toHex();
    boundary.truncate(26);
    boundary.prepend("_part_");
    return boundary;
}

LMailSenderPrivate::LMailSenderPrivate(LMailSender* parent)
    : q_ptr(parent)
{

}

void LMailSenderPrivate::setup(LMailSender::SendMode mode, bool i_ssl)
{
    Q_Q(LMailSender);
    sendMode = mode;
    port = 25;
    timeout = 30000;
    priority = LMailSender::Normal;
    contentType = LMailSender::Text;
    setEncoding(LMailSender::_8bit);
    setISO(LMailSender::Utf8);
    ssl = i_ssl;

#ifndef NCREPORT_NO_SSL
    if (i_ssl)
        socket = new QSslSocket(q);
    else
#endif
        socket = new QTcpSocket(q);


    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
                     q, SLOT(errorReceived(QAbstractSocket::SocketError)));
    QObject::connect(socket, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy& , QAuthenticator*)),
                     q, SLOT(proxyAuthentication(const QNetworkProxy&, QAuthenticator*)));

}

bool LMailSenderPrivate::read(const QString& waitfor)
{
    if (! socket->waitForReadyRead()) {
        setError("Read timeout");
        return false;
    }

    if (!socket->canReadLine()) {
        setError("Can't read");
        return false;
    }

    QString responseLine;

    do {
        responseLine = socket->readLine();
    } while (socket->canReadLine() && responseLine[3] != ' ');

    lastResponse = responseLine;

    QString prefix = responseLine.left(3);
    bool isOk = (prefix == waitfor);
    if (!isOk) {
        setError("waiting for " + waitfor + ", received " + prefix);
    }

    return isOk;
}

bool LMailSenderPrivate::sendCommand(const QString& cmd, const QString& waitfor)
{
    QTextStream t(socket);
    t.setCodec(QTextCodec::codecForName(bodyCodec.toLatin1()));
    t << cmd + "\r\n";
    t.flush();
    lastCmd = cmd;
    return read(waitfor);
}

void LMailSenderPrivate::setError(const QString& msg)
{
    lastError = msg;
}

QString LMailSenderPrivate::mailData() // EMailnachricht vorbereiten
{
    QString data;
    QString boundary1 = createBoundary();
    //QString boundary2 = createBoundary();

    QByteArray hash = QCryptographicHash::hash(QString(QString::number(qrand())).toUtf8(), QCryptographicHash::Md5);
    QString id = hash.toHex();
    if (smtpServer.isEmpty()) {
        data.append("Message-ID: " + id + "@" + QHostInfo::localHostName() + "\n");
    } else {
        data.append("Message-ID: " + id + "@" + smtpServer + "\n");
    }

    data.append("From: \"" + fromName + "\"");
    data.append(" <" + from + ">\n");

    if (to.count() > 0) {
        data.append("To: ");
        for (int i = 0; i < to.count(); i++) {
            data.append("<" + to.at(i) + ">" + ",");
        }

        data.append("\n");
    }
    if (cc.count() > 0) {
        data.append("Cc: ");
        for (int i = 0; i < cc.count(); i++) {
            data.append(cc.at(i) + ",");
            if (i < cc.count() - 1) {
                data.append(",");
            }
        }
        data.append("\n");
    }
    data.append("Subject: " + subject + "\n");
    data.append(TimeStampMail() + "\n");

    // Mails generell als Multipart Mails versenden.
    data.append("MIME-Version: 1.0\n");
    data.append("Content-Type: Multipart/Mixed; boundary=\"" + boundary1 + "\"\n");

    data.append("\n");

    data.append("This is a multi-part message in MIME format.\n\n");
    data.append("--" + boundary1 + "\n");

    //========================================================================
    // Erster Part f�r den Text bzw. das HTML
    //========================================================================
    data.append("Content-Type: " + contentTypeToString() + ";\n");
    data.append("  charset=" + iso + "\n");
    data.append("Content-Transfer-Encoding: " + encoding + "\n");
    data.append("\n");

    QByteArray encodedBody(body.toUtf8()); // = array;
    QTextCodec* codec = QTextCodec::codecForName(bodyCodec.toLatin1());
    data.append(codec->toUnicode(encodedBody) + "\n");
    //========================================================================
    // Zweiter Part f�r die Anh�nge
    //========================================================================
    if (attachments.count() > 0) {
        for (int i = 0; i < attachments.count(); i++) {
            data.append("--" + boundary1 + "\n");

            QString fileName = attachments.at(i).m_filename;
            QString type = getMimeType(fileName.split(".").last());
            QString contentTransferEnc;
            QByteArray fileDataRaw = attachments.at(i).m_file;
            int fileSize = fileDataRaw.length();

//            if (type.startsWith("text")) {
//                 contentTransferEnc = "quoted-printable";
//            } else {
                 fileDataRaw = fileDataRaw.toBase64();
                 contentTransferEnc = "base64";
//            }

            QString fileData;
            QStringList fileDataLines;

            int extLines = 0;
            if ((fileDataRaw.length() % 72) > 0) {
                extLines = 1;
            }
            for (int i = 0; i < (fileDataRaw.length() / 72) + extLines; i++) {
                fileDataLines.append(QString(fileDataRaw.mid(i * 72, 72)));
            }

            fileData = fileDataLines.join("\n");

            data.append("Content-Type: " + type + ";");
            data.append("  name=\"" + fileName + "\"\n");

            data.append("Content-Transfer-Encoding: " + contentTransferEnc + "\n");
            data.append("Content-Disposition: attachment;");
            data.append("  filename=\"" + fileName + "\";\n");
            data.append("  size=\"" + QString::number(fileSize) + "\";\n\n");
            data.append(fileData);
            data.append("\n");
        }
    }
    data.append("--" + boundary1 + "--\n\n");
    lastMailData = data;
    return data;
}

QString LMailSenderPrivate::contentTypeToString() const
{
    switch (this->contentType) {
    case LMailSender::Html:
        return "text/html";
    case LMailSender::MultiPartMixed:
        return "multipart/mixed";
    case LMailSender::Text:
    default:
        return "text/plain";
    }
}

QString LMailSenderPrivate::getMimeType(QString ext) const
{
    //texte
    if (ext == "txt") {
        return "text/plain";
    }
    if (ext == "htm" || ext == "html") {
        return "text/html";
    }
    if (ext == "css") {
        return "text/css";
    }
    //Images
    if (ext == "png") {
        return "image/png";
    }
    if (ext == "gif") {
        return "image/gif";
    }
    if (ext == "jpg" || ext == "jpeg") {
        return "image/jpeg";
    }
    if (ext == "bmp") {
        return "image/bmp";
    }
    if (ext == "tif") {
        return "image/tiff";
    }
    //Archives
    if (ext == "bz2") {
        return "application/x-bzip";
    }
    if (ext == "gz") {
        return "application/x-gzip";
    }
    if (ext == "tar") {
        return "application/x-tar";
    }
    if (ext == "zip") {
        return "application/zip";
    }
    //Audio
    if (ext == "aif" || ext == "aiff") {
        return "audio/aiff";
    }
    if (ext == "mid" || ext == "midi") {
        return "audio/mid";
    }
    if (ext == "mp3") {
        return "audio/mpeg";
    }
    if (ext == "ogg") {
        return "audio/ogg";
    }
    if (ext == "wav") {
        return "audio/wav";
    }
    if (ext == "wma") {
        return "audio/x-ms-wma";
    }
    //Video
    if (ext == "asf" || ext == "asx") {
        return "video/x-ms-asf";
    }
    if (ext == "avi") {
        return "video/avi";
    }
    if (ext == "mpg" || ext == "mpeg") {
        return "video/mpeg";
    }
    if (ext == "wmv") {
        return "video/x-ms-wmv";
    }
    if (ext == "wmx") {
        return "video/x-ms-wmx";
    }
    //XML
    if (ext == "xml") {
        return "text/xml";
    }
    if (ext == "xsl") {
        return "text/xsl";
    }
    //Microsoft
    if (ext == "doc" || ext == "rtf" || ext == "docx") {
        return "application/msword";
    }
    if (ext == "xls" || ext == "xlsx") {
        return "application/excel";
    }
    if (ext == "ppt" || ext == "pps" || ext == "pptx") {
        return "application/vnd.ms-powerpoint";
    }
    //Adobe
    if (ext == "pdf")
        //return "application/octet-stream";
    {
        return "application/pdf";
    }
    if (ext == "ai" || ext == "eps") {
        return "application/postscript";
    }
    if (ext == "psd") {
        return "image/psd";
    }
    //Macromedia
    if (ext == "swf") {
        return "application/x-shockwave-flash";
    }
    //Real
    if (ext == "ra") {
        return "audio/vnd.rn-realaudio";
    }
    if (ext == "ram") {
        return "audio/x-pn-realaudio";
    }
    if (ext == "rm") {
        return "application/vnd.rn-realmedia";
    }
    if (ext == "rv") {
        return "video/vnd.rn-realvideo";
    }
    //Other
    if (ext == "exe") {
        return "application/x-msdownload";
    }
    if (ext == "pls") {
        return "audio/scpls";
    }
    if (ext == "m3u") {
        return "audio/x-mpegurl";
    }

    return "text/plain"; // default
}

bool LMailSenderPrivate::send()
{
    QString currentMailData = mailData();
    lastError = "";
    bool auth = !login.isEmpty();

#ifndef NCREPORT_NO_SSL
    if (this->ssl) {
        QSslSocket *sslSocket = (QSslSocket*)socket;

        sslSocket->connectToHostEncrypted(smtpServer, port);
        if (!sslSocket->waitForEncrypted()) {
            //sslSocket->ignoreSslErrors();

            setError(sslSocket->errorString());
            return false;
        }
    } else {
#else
    {
#endif
        socket->connectToHost(smtpServer, port);
    }

    if (!socket->waitForConnected(timeout)) {
        setError("Time out connecting host");
        return false;
    }

    if (!read("220")) {
        return false;
    }

    if (!sendCommand("EHLO there", "250")) {
        if (!sendCommand("HELO there", "250")) {
            return false;
        }
    }

    if (auth) {
        if (!sendCommand("AUTH LOGIN", "334")) {
            return false;
        }
        if (!sendCommand(encodeBase64(login), "334")) {
            return false;
        }
        if (!sendCommand(encodeBase64(password), "235")) {
            return false;
        }
    }

    if (!sendCommand(QString::fromLatin1("MAIL FROM:<") + from + QString::fromLatin1(">"), "250")) {
        return false;
    }

    QStringList recipients = to + cc + bcc;
    for (int i = 0; i < recipients.count(); i++) {
        if (!sendCommand(QString::fromLatin1("RCPT TO:<") + recipients.at(i) + QString::fromLatin1(">"), "250")) {
            return false;
        }
    }

    if (!sendCommand(QString::fromLatin1("DATA"), "354")) {
        return false;
    }
    if (!sendCommand(currentMailData + QString::fromLatin1("\r\n."), "250")) {
        return false;
    }
    if (!sendCommand(QString::fromLatin1("QUIT"), "221")) {
        return false;
    }

    socket->disconnectFromHost();
    return true;
}

void LMailSenderPrivate::setFrom(const QString& from)
{
    this->from = from;
    this->fromName = from;
    this->replyTo = from;
}

void LMailSenderPrivate::setISO(LMailSender::ISO iso)
{
    switch (iso) {
    case LMailSender::ISO88591:
        this->iso = "iso-8859-1";
        this->bodyCodec = "ISO 8859-1";
        break;
    case LMailSender::Utf8:
        this->iso = "utf-8";
        this->bodyCodec = "UTF-8";
        break;
    }
}

void LMailSenderPrivate::setEncoding(LMailSender::Encoding encoding)
{
    switch (encoding) {
    case LMailSender::_7bit:
        this->encoding = "7bit";
        break;
    case LMailSender::_8bit:
        this->encoding = "8bit";
        break;
    case LMailSender::base64:
        this->encoding = "base64";
        break;
    }
}
