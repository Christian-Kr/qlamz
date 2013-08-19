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

#include "Track.h"


Track::Track(QObject *pParent)
    : QObject(pParent),
    m_pstrCreator(new QString()),
    m_pstrAlbum(new QString()),
    m_pstrAlbumPrimaryArtist(new QString()),
    m_pstrTitle(new QString()),
    m_pstrDuration(new QString()),
    m_pstrTrackType(new QString()),
    m_pstrPrimaryGenre(new QString()),
    m_pstrImageLink(new QString()),
    m_pstrLocation(new QString()),
    m_pbDownload(new bool),
    m_piNumber(new int),
    m_psDownloadPercentage(new short)
{
    *m_pbDownload = true;
    *m_psDownloadPercentage = 0;
    *m_piNumber = 0;
}

Track::~Track()
{
    delete m_pstrCreator;
    delete m_pstrAlbum;
    delete m_pstrAlbumPrimaryArtist;
    delete m_pstrTitle;
    delete m_pstrDuration;
    delete m_pstrImageLink;
    delete m_piNumber;
    delete m_pbDownload;
}

bool Track::download() const
{
    return *m_pbDownload;
}

void Track::setDownload(bool bDownload)
{
    *m_pbDownload = bDownload;
}

short Track::downloadPercentage() const
{
    return *m_psDownloadPercentage;
}

void Track::setDownloadPercentage(short sDownloadPercentage)
{
    *m_psDownloadPercentage = sDownloadPercentage;
}

QString Track::creator() const
{
    return *m_pstrCreator;
}

void Track::setCreator(const QString &strCreator)
{
    *m_pstrCreator = strCreator;
}

QString Track::album() const
{
    return *m_pstrAlbum;
}

void Track::setAlbum(const QString &strAlbum)
{
    *m_pstrAlbum = strAlbum;
}

QString Track::albumPrimaryArtist() const
{
    return *m_pstrAlbumPrimaryArtist;
}

void Track::setAlbumPrimaryArtist(const QString &strAlbumPrimaryArtist)
{
    *m_pstrAlbumPrimaryArtist = strAlbumPrimaryArtist;
}

QString Track::location() const
{
    return *m_pstrLocation;
}

void Track::setLocation(const QString &strLocation)
{
    *m_pstrLocation = strLocation;
}

QString Track::title() const
{
    return *m_pstrTitle;
}

void Track::setTitle(const QString &strTitle)
{
    *m_pstrTitle = strTitle;
}

QString Track::trackType() const
{
    return *m_pstrTrackType;
}

void Track::setTrackType(const QString &strTrackType)
{
    *m_pstrTrackType = strTrackType;
}

QString Track::primaryGenre() const
{
    return *m_pstrPrimaryGenre;
}

void Track::setPrimaryGenre(const QString &strPrimaryGenre)
{
    *m_pstrPrimaryGenre = strPrimaryGenre;
}

int Track::number() const
{
    return *m_piNumber;
}

void Track::setNumber(const int iNumber)
{
    *m_piNumber = iNumber;
}
