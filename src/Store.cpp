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

#include "Store.h"
#include "ui_Store.h"

#include "CustomWebPage.h"
#include "PersistentCookieJar.h"

#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QWebFrame>
#include <QWebPage>
#include <QByteArray>
#include <QDebug>


Store::Store(QWidget *pParent)
    : QWidget(pParent),
    m_bDownloadMode(false),
    m_pUi(new Ui::StoreWidget()),
    m_pNetReply(NULL),
    m_pWebPage(new CustomWebPage()),
    m_pCookieJar(new PersistentCookieJar())
{
    m_pUi->setupUi(this);

    m_pUi->webViewStore->setPage(m_pWebPage);

    m_pWebPage->setLinkDelegationPolicy(QWebPage::DontDelegateLinks);
    m_pWebPage->setForwardUnsupportedContent(true);

    m_pCookieJar->load();
    m_pWebPage->networkAccessManager()->setCookieJar(m_pCookieJar);

    // Build some connections.
    connect(m_pWebPage, SIGNAL(formSubmitted(const QNetworkRequest&)),
        this, SLOT(formSubmitted(const QNetworkRequest&)));
    connect(m_pWebPage, SIGNAL(unsupportedContent(QNetworkReply*)), this,
        SLOT(unsupportedContent(QNetworkReply*)));

    connect(m_pUi->webViewStore, SIGNAL(loadFinished(bool)), this,
        SLOT(loadFinished(bool)));
}

Store::~Store()
{
    delete m_pUi;
    delete m_pWebPage;
    delete m_pCookieJar;
}

QByteArray Store::cookieData() const
{
    return m_pCookieJar->data();
}

void Store::load(const QUrl &url)
{
    createNetworkReplyFromUrl(url);
}

QNetworkReply* Store::createNetworkReplyFromUrl(const QUrl &url)
{
    QNetworkRequest netRequest;
    netRequest.setUrl(QUrl(url));
    netRequest.setRawHeader("User-Agent", "Mozilla/4.0 (compatible; MSIE 8.0; "
        "Windows NT 6.1)");

    // Delete the last reply.
    if (m_pNetReply != NULL) {
        m_pNetReply->deleteLater();
    }

    m_pNetReply = m_pWebPage->networkAccessManager()->get(netRequest);

    buildNetReplyConnections(m_pNetReply);

    return m_pNetReply;
}

void Store::buildNetReplyConnections(const QNetworkReply *netReply)
{
    connect(netReply, SIGNAL(error(QNetworkReply::NetworkError)), this,
        SLOT(netReplyError(QNetworkReply::NetworkError)));

    connect(netReply, SIGNAL(finished()), this, SLOT(netReplyFinished()));
}

void Store::loadFinished(bool bOk)
{
    qDebug() << "Load finished";
    m_pCookieJar->save();
}

void Store::netReplyError(QNetworkReply::NetworkError netError)
{
    qDebug() << "Network Error Occured: " << netError;
    qDebug() << m_pUi->webViewStore->url();
}

void Store::netReplyFinished()
{
    qDebug() << "Finished";

    if (m_pNetReply->rawHeader("Location").size() > 0) {
        createNetworkReplyFromUrl(QUrl::fromEncoded(
            m_pNetReply->rawHeader("Location"), QUrl::StrictMode));
    } else {
        // If we are in download mode we are ready.
        if (m_bDownloadMode) {
            m_bDownloadMode = false;

            emit amzDownloaded(QString(m_pNetReply->readAll()));
        } else {
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

            QString content = strTest + QString::fromUtf8(m_pNetReply->readAll()).toAscii();

            //m_pWebPage->noEmitFormSubmittedOnce();
            m_pUi->webViewStore->setHtml(content, m_pNetReply->url());
        }
    }
}

void Store::formSubmitted(const QNetworkRequest &netRequest)
{
    qDebug() << "Submitted";

    if (m_pNetReply != NULL) {
        m_pNetReply->deleteLater();
    }

    m_pWebPage->noEmitFormSubmittedOnce();
    m_pNetReply = m_pWebPage->networkAccessManager()->get(netRequest);

    buildNetReplyConnections(m_pNetReply);
}

void Store::unsupportedContent(QNetworkReply *netReply)
{
    qDebug() << "Download Reply";

    m_bDownloadMode = true;

    if (m_pNetReply != NULL) {
        m_pNetReply->deleteLater();
    }

    m_pNetReply = netReply;

    buildNetReplyConnections(m_pNetReply);
}
