/* Copyright (c) 2012 Christian Krippendorf <Coding@Christian-Krippendorf.de>
 *
 * This file is part of qlamz.
 *
 * qlamz is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * qlamz is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * qlamz.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef CUSTOMWEBVIEW_H
#define CUSTOMWEBVIEW_H

#include <QWebView>
#include <QNetworkReply>


class QByteArray;
class CustomWebPage;
class PersistentCookieJar;
class QUrl;
class QNetworkAccessManager;

class CustomWebView : public QWebView
{
    Q_OBJECT

public:
    /**
     * Constructor */
    CustomWebView(QWidget *pParent = 0);

    /**
     * Destructor */
    ~CustomWebView();

    /**
     * Return the actual cookie data.
     *
     * @return a QByteArray object with the data. */
    QByteArray cookieData() const;

public slots:
    /**
     * Called when a download was requested.
     *
     * @param netRequest The netRequest. */
    void unsupportedContent(QNetworkReply *netReply);

    /**
     * Load an url.
     *
     * @param url The url that should be load. */
    void load(const QUrl &url);

    /**
     * Will be called, when loading finished.
     *
     * @param bOk Everthing was ok while loading. */
    void loadFinished(bool bOk);

    /**
     * Will be called, when getting teh network reply has finished. */
    void netReplyFinished();

    /**
     * Will be called, when a network error occured.
     *
     * @param netError The network error code. */
    void netReplyError(QNetworkReply::NetworkError netError);

    /**
     * Will be called, when a a form was submitted.
     *
     * @param netRequest The network request. */
    void formSubmitted(const QNetworkRequest &netRequest);

    void linkClicked(const QUrl &url);

signals:
    /**
     * Emits when an amz file will be downloaded. */
    void amzDownloaded(const QString &content);

private:
    bool m_bDownloadMode;

    QNetworkReply *m_pNetReply;
    CustomWebPage *m_pWebPage;
    PersistentCookieJar *m_pCookieJar;

    /**
     * Build connections to the given netReply.
     *
     * @param netReply Reply to build connections with. */
    void buildNetReplyConnections(const QNetworkReply *netReply);

    /**
     * Create a netReply from the url.
     *
     * @param url The url to create a network reply from.
     * @return The new netReply object. */
    QNetworkReply* createNetworkReplyFromUrl(const QUrl &url);
};


#endif // CUSTOMWEBVIEW_H
