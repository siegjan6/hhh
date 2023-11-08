/***************************************************************************
 *   Copyright (C) 2010 by NociSoft Software Solutions   
 *   support@nocisoft.com   
 ***************************************************************************/
#include <QtNetwork>

#include "ncreporthttpclient.h"

NCReportHttpClient::NCReportHttpClient(QObject *parent) :  QObject(parent)
{
	httpRequestAborted = false;
	ready = true;

	connect(&qnam, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),
			this, SLOT(slotAuthenticationRequired(QNetworkReply*,QAuthenticator*)));
#ifndef QT_NO_OPENSSL
	connect(&qnam, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
			this, SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
#endif
	errorCode =0;
}

void NCReportHttpClient::clear()
{
	data.clear();
	httpRequestAborted = true;
	ready = true;
}

void NCReportHttpClient::startRequest(QUrl url)
{
	setError("",0);
	reply = qnam.get(QNetworkRequest(url));
	connect(reply, SIGNAL(finished()), this, SLOT(httpFinished()));
	connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
	connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateDataReadProgress(qint64,qint64)));
}

void NCReportHttpClient::downloadContent( const QString& urlText )
{
	if (!ready)
		cancelDownload();
	data.clear();
	url = urlText;
	// schedule the request
	httpRequestAborted = false;
	ready = false;
	startRequest(url);
}

/*!
  Start download with wait event loop until the download is completed.

  */
void NCReportHttpClient::waitForDownloadContent( const QString& urlText )
{
	downloadContent(urlText);
	while(!ready)
		QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents);
}

void NCReportHttpClient::cancelDownload()
{
	setError( tr("Download canceled."), -2 );
	httpRequestAborted = true;
	ready = true;
	reply->abort();
}

void NCReportHttpClient::httpFinished()
{
	if (httpRequestAborted) {
		data.clear();
		reply->deleteLater();
		return;
	}

	QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
	if (reply->error()) {
		data.clear();
		setError( tr("Download failed: %1.").arg(reply->errorString()), -1 );
	} else if (!redirectionTarget.isNull()) {
		QUrl newUrl = url.resolved(redirectionTarget.toUrl());
		if (true) {
			// handle redirection
			url = newUrl;
			reply->deleteLater();
			data.clear();
			startRequest(url);
			return;
		}
	} else {
	}

	reply->deleteLater();
	reply = 0;
	ready = true;
	emit finished();
}

void NCReportHttpClient::setError(const QString &msg, int code)
{
	errorMsg = msg;
	errorCode = code;
}

void NCReportHttpClient::httpReadyRead()
{
	// this slot gets called everytime the QNetworkReply has new data.
	// We read all of its new data and write it into the file.
	// That way we use less RAM than when reading it at the finished()
	// signal of the QNetworkReply
	data.append(reply->readAll());
}

void NCReportHttpClient::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
	if (httpRequestAborted)
		return;
	emit readBytes(bytesRead,totalBytes);
}

void NCReportHttpClient::slotAuthenticationRequired(QNetworkReply*,QAuthenticator *authenticator)
{
	setError( tr("Authentication required: %1 at %2").arg(authenticator->realm()).arg(url.host()), -3 );
	httpRequestAborted = true;
	reply->abort();
}

QString NCReportHttpClient::errorMessage() const
{
	return errorMsg;
}

int NCReportHttpClient::error() const
{
	return errorCode;
}

#ifndef QT_NO_OPENSSL
void NCReportHttpClient::sslErrors(QNetworkReply*,const QList<QSslError> &errors)
{
	QString errorString;
	foreach (const QSslError &error, errors) {
		if (!errorString.isEmpty())
			errorString += ", ";
		errorString += error.errorString();
	}
	setError( tr("One or more SSL errors has occurred: %1").arg(errorString), -4 );
}
#endif

const QByteArray& NCReportHttpClient::content() const
{
	return data;
}

