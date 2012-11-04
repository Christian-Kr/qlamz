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
#include <QDir>
#include <QDebug>


TrackDownloader::TrackDownloader(QNetworkAccessManager *pNetAccessManager, QObject *pParent)
    : QObject(pParent),
    m_pTrack(NULL),
    m_pNetworkReply(NULL),
    m_pNetAccessManager(pNetAccessManager)
{
}

TrackDownloader::~TrackDownloader()
{
}

bool TrackDownloader::isRunning()
{
    if (m_pNetworkReply == NULL) {
        return false;
    }

    return m_pNetworkReply->isRunning();
}

void TrackDownloader::startDownload(Track *pTrack)
{
    m_pTrack = pTrack;

    if (m_pNetworkReply != NULL) {
        m_pNetworkReply->close();
        m_pNetworkReply->deleteLater();
    }

    m_pNetworkReply = createNetworkReply(m_pTrack->location());

    // Build some connections.
    connect(m_pNetworkReply, SIGNAL(error(QNetworkReply::NetworkError)), this,
        SLOT(networkReplyError(QNetworkReply::NetworkError)));
    connect(m_pNetworkReply, SIGNAL(finished()), this, SLOT(networkReplyFinish()));
    connect(m_pNetworkReply, SIGNAL(downloadProgress(qint64, qint64)), this,
        SLOT(networkReplyUpdate(qint64, qint64)));
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

void TrackDownloader::networkReplyFinish()
{
    if (m_pNetworkReply->rawHeader("Location").size() > 0) {
        QByteArray byLocation = m_pNetworkReply->rawHeader("Location");

        m_pNetworkReply->close();
        m_pNetworkReply->deleteLater();
        m_pNetworkReply = createNetworkReply(byLocation);

        // Build some connections.
        connect(m_pNetworkReply, SIGNAL(error(QNetworkReply::NetworkError)), this,
            SLOT(networkReplyError(QNetworkReply::NetworkError)));
        connect(m_pNetworkReply, SIGNAL(finished()), this, SLOT(networkReplyFinish()));
        connect(m_pNetworkReply, SIGNAL(downloadProgress(qint64, qint64)), this,
            SLOT(networkReplyUpdate(qint64, qint64)));
    } else {
        emit finish(m_pTrack, m_pNetworkReply, this);
    }
}

void TrackDownloader::networkReplyUpdate(qint64 iRecieved, qint64 iTotal)
{
    emit update(m_pTrack, iRecieved, iTotal, m_pNetworkReply);
}

void TrackDownloader::abort()
{
    m_pNetworkReply->abort();
    m_pNetworkReply->deleteLater();
    m_pNetworkReply = NULL;
}
