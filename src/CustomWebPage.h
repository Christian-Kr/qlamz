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

#ifndef CUSTOMWEBPAGE_H
#define CUSTOMWEBPAGE_H

#include <QWebPage>


class QUrl;

class CustomWebPage : public QWebPage
{
    Q_OBJECT

public:
    /**
     * Constructor */
    CustomWebPage(QObject *pParent = 0);

    /**
     * Destructor */
    ~CustomWebPage();

    void noEmitFormSubmittedOnce() { bNoEmit = true; }
public slots:

protected:

    QString userAgentForUrl(const QUrl &url);

    bool acceptNavigationRequest(QWebFrame *frame,
        const QNetworkRequest &request, NavigationType type);

    QWebPage* createWindow(WebWindowType type) { return this; }

signals:

    void formSubmitted(const QNetworkRequest &netRequest);

private:

    bool bNoEmit;
};


#endif // CUSTOMWEBPAGE_H
