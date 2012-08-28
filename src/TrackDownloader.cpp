/* Copyright (c) 2012 Christian Krippendorf <Coding@Christian-Krippendorf.de>
 *
 * This file is part of qlamz.
 *
 * qlamz is free software: you can redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * qlamz is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with qlamz.  If not, see
 * <http://www.gnu.org/licenses/>. */

#include "TrackDownloader.h"
#include "Track.h"

#include <QFile>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkCookieJar>
#include <QDir>
#include <QDebug>


TrackDownloader::TrackDownloader(QObject *pParent)
    : QObject(pParent),
    m_pTrack(NULL),
    m_strPath(new QString()),
    m_pNetAccessManager(new QNetworkAccessManager(this)),
    m_pNetworkReply(NULL)
{
    m_pNetAccessManager->setCookieJar(new QNetworkCookieJar(m_pNetAccessManager));
}

TrackDownloader::~TrackDownloader()
{
    delete m_pTrack;
    delete m_strPath;
    delete m_pNetAccessManager;
    delete m_pNetworkReply;
}

void TrackDownloader::startDownload(Track *pTrack, const QString &strPath)
{
    m_pTrack = pTrack;
    *m_strPath = strPath;

    m_pNetworkReply = createNetworkReply(m_pTrack->location());

    // Build some connections.
    connect(m_pNetworkReply, SIGNAL(error(QNetworkReply::NetworkError)), this,
        SLOT(networkReplyError(QNetworkReply::NetworkError)));
    connect(m_pNetworkReply, SIGNAL(finished()), this, SLOT(finish()));
    connect(m_pNetworkReply, SIGNAL(downloadProgress(qint64, qint64)), this,
        SLOT(update(qint64, qint64)));
}

QNetworkReply * TrackDownloader::createNetworkReply(const QString &strUrl)
{
    QUrl location = QUrl::fromEncoded(strUrl.toAscii(), QUrl::StrictMode);

    qDebug() << "Url is valid: " << location.isValid();

    QNetworkRequest request;
    request.setUrl(location);
    request.setRawHeader("User-Agent", "Amazon MP3 Downloader (qlamz 0.1)");

    return m_pNetAccessManager->get(request);
}

void TrackDownloader::networkReplyError(QNetworkReply::NetworkError code)
{
    m_pTrack->setDownloadPercentage(100);

    emit error(code, m_pNetworkReply->errorString(), m_pTrack);
}

void TrackDownloader::finish()
{
    qDebug() << "Finish";

    if (m_pNetworkReply->rawHeader("Location").size() > 0) {
        qDebug() << m_pNetworkReply->rawHeader("Location");

        QByteArray byLocation = m_pNetworkReply->rawHeader("Location");

        m_pNetworkReply->deleteLater();

        m_pNetworkReply = createNetworkReply(byLocation);

            // Build some connections.
        connect(m_pNetworkReply, SIGNAL(error(QNetworkReply::NetworkError)), this,
            SLOT(networkReplyError(QNetworkReply::NetworkError)));
        connect(m_pNetworkReply, SIGNAL(finished()), this, SLOT(finish()));
        connect(m_pNetworkReply, SIGNAL(downloadProgress(qint64, qint64)), this,
            SLOT(update(qint64, qint64)));
    } else {
        // Create the path if it does not exist.
        QDir dir;
        dir.mkpath(*m_strPath);

        QFile file(*m_strPath + "/" + m_pTrack->title() + ".mp3");
        file.open(QIODevice::WriteOnly);
        file.write(m_pNetworkReply->readAll());
        file.close();

        m_pNetworkReply->close();
        m_pNetworkReply->deleteLater();
        m_pNetworkReply = NULL;

        emit finished(m_pTrack);
    }
}

void TrackDownloader::update(qint64 iRecieved, qint64 iTotal)
{
    m_pTrack->setDownloadPercentage((short) ((double) iRecieved / (double) iTotal * 100));
    emit updated(m_pTrack);
}
