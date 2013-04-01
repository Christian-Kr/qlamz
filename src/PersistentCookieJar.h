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

#ifndef PERSISTENTCOOKIEJAR_H
#define PERSISTENTCOOKIEJAR_H

#include <QNetworkCookieJar>


class PersistentCookieJar : public QNetworkCookieJar
{
    Q_OBJECT

public:
    /**
     * Constructor */
    PersistentCookieJar(QObject *parent = 0);

    /**
     * Destructor */
    ~PersistentCookieJar();

public:
    /**
     * Save the cookies. */
    void save();

    /**
     * Load the cookies. */
    void load();
};


#endif // PERSISTENTCOOKIEJAR_H
