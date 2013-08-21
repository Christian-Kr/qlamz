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

#include "CustomWebView.h"

#include "CustomWebPage.h"
#include "PersistentCookieJar.h"

#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QWebFrame>
#include <QWebPage>
#include <QByteArray>
#include <QDebug>


CustomWebView::CustomWebView(QWidget *pParent)
    : QWebView(pParent),
    m_bDownloadMode(false),
    m_pNetReply(NULL),
    m_pWebPage(new CustomWebPage()),
    m_pCookieJar(new PersistentCookieJar())
{
    // Set CustomWebPage object.
    setPage(m_pWebPage);

    m_pWebPage->setForwardUnsupportedContent(true);
    m_pWebPage->networkAccessManager()->setCookieJar(m_pCookieJar);
    m_pWebPage->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    // Build some connections.
    connect(m_pWebPage, SIGNAL(formSubmitted(const QNetworkRequest&)),
        this, SLOT(formSubmitted(const QNetworkRequest&)));

    connect(m_pWebPage, SIGNAL(unsupportedContent(QNetworkReply*)), this,
        SLOT(unsupportedContent(QNetworkReply*)));

    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
    connect(this, SIGNAL(linkClicked(const QUrl&)), this,
        SLOT(linkClicked(const QUrl&)));
}

CustomWebView::~CustomWebView()
{
    delete m_pWebPage;
    delete m_pCookieJar;
}

QByteArray CustomWebView::cookieData() const
{
    return m_pCookieJar->data();
}

void CustomWebView::load(const QUrl &url)
{
    createNetworkReplyFromUrl(url);
}

QNetworkReply* CustomWebView::createNetworkReplyFromUrl(const QUrl &url)
{
    QNetworkRequest netRequest;
    netRequest.setUrl(QUrl(url));
    netRequest.setRawHeader("User-Agent", "Amazon MP3 Downloader "
        "(Win32 1.0.17 en_US)");

    // Delete the last reply.
    if (m_pNetReply != NULL) {
        m_pNetReply->deleteLater();
    }

    m_pNetReply = m_pWebPage->networkAccessManager()->get(netRequest);

    buildNetReplyConnections(m_pNetReply);

    return m_pNetReply;
}

void CustomWebView::buildNetReplyConnections(const QNetworkReply *netReply)
{
    connect(netReply, SIGNAL(error(QNetworkReply::NetworkError)), this,
        SLOT(netReplyError(QNetworkReply::NetworkError)));

    connect(netReply, SIGNAL(finished()), this, SLOT(netReplyFinished()));
}

void CustomWebView::loadFinished(bool bOk)
{
    qDebug() << "CustomWebView::loadFinished";
    m_pCookieJar->save();
}

void CustomWebView::netReplyError(QNetworkReply::NetworkError netError)
{
    qDebug() << "Network Error Occured: " << netError;
    qDebug() << url();
}

void CustomWebView::netReplyFinished()
{
    qDebug() << "CustomWebView::netReplyFinished";

    if (m_pNetReply->rawHeader("Location").size() > 0) {
        qDebug() << ":: Location";

        createNetworkReplyFromUrl(QUrl::fromEncoded(
            m_pNetReply->rawHeader("Location"), QUrl::StrictMode));
    } else {
        // If we are in download mode we are ready.
        if (m_bDownloadMode) {
            m_bDownloadMode = false;

            qDebug() << "Read all: " << m_pNetReply->readAll();
            emit amzDownloaded(QString(m_pNetReply->readAll()));
        } else {
            qDebug() << ":: JavaScript";

            QString strTest("<script type='text/javascript'>"
                "var fake_navigator={};"
                "for(var i in navigator)"
                "{fake_navigator[i]=navigator[i];}"
                "fake_navigator.platform='Win32';"
                "fake_navigator.appCodeName='Mozilla';"
                "fake_navigator.appName='Microsoft Internet Explorer';"
                "fake_navigator.appVersion='4.0 (compatible; MSIE 8.0; Windows NT 6.1)';"
                "fake_navigator.userAgent='Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1)';"
                "navigator=fake_navigator;"
                "</script>");

            QString content = strTest + QString::fromUtf8(m_pNetReply->readAll());

            //m_pWebPage->noEmitFormSubmittedOnce();
            setHtml(content, m_pNetReply->url());
        }
    }
}

void CustomWebView::linkClicked(const QUrl &url)
{
    qDebug() << "CustomWebView::linkClicked: " << url.toString();

    m_pNetReply = createNetworkReplyFromUrl(url);
    buildNetReplyConnections(m_pNetReply);
}

void CustomWebView::formSubmitted(const QNetworkRequest &netRequest)
{
    qDebug() << "CustomWebView::formSubmitted: " << netRequest.header(QNetworkRequest::ContentTypeHeader).toString();

    if (m_pNetReply != NULL) {
        m_pNetReply->deleteLater();
    }

    m_pWebPage->noEmitFormSubmittedOnce();
    m_pNetReply = m_pWebPage->networkAccessManager()->get(netRequest);

    buildNetReplyConnections(m_pNetReply);
}

void CustomWebView::unsupportedContent(QNetworkReply *netReply)
{
    qDebug() << "Download Reply";

    m_bDownloadMode = true;

    if (m_pNetReply != NULL) {
        m_pNetReply->deleteLater();
    }

    m_pNetReply = netReply;

    buildNetReplyConnections(m_pNetReply);
}
