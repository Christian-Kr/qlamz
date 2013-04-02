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

#include "PersistentCookieJar.h"

#include <QSettings>
#include <QByteArray>
#include <QList>
#include <QDebug>


PersistentCookieJar::PersistentCookieJar(QObject *parent)
    : QNetworkCookieJar(parent)
{
    // Load the cookies.
    load();
}

PersistentCookieJar::~PersistentCookieJar()
{
    // Save the cookies.
    save();
}

QByteArray PersistentCookieJar::data() const
{
    // Create a new data object.
    QByteArray data;

    foreach (QNetworkCookie cookie, allCookies()) {
        if (!cookie.isSessionCookie()) {
            data.append(cookie.toRawForm());
            data.append("\n");
        }
    }

    return data;
}

void PersistentCookieJar::save()
{
    qDebug() << "Speichern";

    QSettings settings("qlamz", "amazon_cookie");
    settings.setValue("Cookies", data());
    settings.sync();
}

void PersistentCookieJar::load()
{
    qDebug() << "Laden";

    QSettings settings("qlamz", "amazon_cookie");
    QByteArray data = settings.value("Cookies").toByteArray();
    setAllCookies(QNetworkCookie::parseCookies(data));
}
