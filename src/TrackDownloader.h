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

#ifndef TRACKDOWNLOADER_H
#define TRACKDOWNLOADER_H

#include <QObject>
#include <QNetworkReply>


// Forward declaration
class Track;
class QString;
class QNetworkAccessManager;

class TrackDownloader : public QObject
{
    Q_OBJECT

public:
    /**
     * Constructor */
    TrackDownloader(QObject *pParent = 0);

    /**
     * Destructor */
    ~TrackDownloader();

    /**
     * Download the given track. Please do not delete the given object until the finish signal
     * emited.
     *
     * @param strPath The path of the file downloaded to.
     * @param pTrack The track to download. */
    void startDownload(Track *pTrack, const QString &strPath);

signals:
    /**
     * Emited when the whole downloading process is finish. */
    void finished(Track *pTrack);

    /**
     * Emited when the track number of the download progress updated. */
    void updated(Track *pTrack);

    /**
     * Emited when an error occured. */
    void error(int iCode, const QString &strMessage, Track *pTrack);

private slots:
    /**
     * Finish the download progress. */
    void finish();

    /**
     * Error occured.
     *
     * @param code The error code. */
    void networkReplyError(QNetworkReply::NetworkError code);

    /**
     * Update the download progress.
     *
     * @param iRecieved The actual number of recieved bytes.
     * @param iTotal The total number of reieving bytes. */
    void update(qint64 iRecieved, qint64 iTotal);

private:
    /**
     * Create and start a new network reply to the given url.
     *
     * @param strUrl The given url for the network reply.
     * @return The created netowrk reply object. */
    QNetworkReply * createNetworkReply(const QString &strUrl);

    Track *m_pTrack;

    QString *m_strPath;

    QNetworkAccessManager *m_pNetAccessManager;
    QNetworkReply *m_pNetworkReply;
};


#endif // TRACKDOWNLOADER_H
