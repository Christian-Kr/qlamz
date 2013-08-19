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

#ifndef TRACK_H
#define TRACK_H

#include <QObject>


class QString;

class Track : public QObject
{
    Q_OBJECT

public:
    /**
     * Constructor */
    Track(QObject *pParent = 0);

    /**
     * Destructor */
    ~Track();

    /**
     * Get whether the item should be donwloaded or not.
     *
     * @return The actual download value. */
    bool download() const;

    /**
     * Set the download value as it tells whether the track should be downloaded or not.
     *
     * @param bDownload The download value to set up. */
    void setDownload(bool bDownload);

    /**
     * Get a number as a percentage of how many bytes of the file is actually downloaded.
     *
     * @return The percentage number. */
    short downloadPercentage() const;

    /**
     * Set the download value as it tells whether the track should be downloaded or not.
     *
     * @param sDownloadPercentage The download value to set up. */
    void setDownloadPercentage(short sDownloadPercentage);

    /**
     * Get the creator.
     *
     * @return The creator actually set up. */
    QString creator() const;

    /**
     * Set the creator.
     *
     * @param strCreator The creator to set up. */
    void setCreator(const QString &strCreator);

    /**
     * Get the album primary artist.
     *
     * @return The album primary artist actually set up. */
    QString albumPrimaryArtist() const;

    /**
     * Set the album primary artist.
     *
     * @param strAlbumPrimaryArtist The album primary artist to set up. */
    void setAlbumPrimaryArtist(const QString &strAlbumPrimaryArtist);

    /**
     * Get the album.
     *
     * @return The album actually set up. */
    QString album() const;

    /**
     * Set the album.
     *
     * @param strAlbum The album to set up. */
    void setAlbum(const QString &strAlbum);

    /**
     * Get the location.
     *
     * @return The location actually set up. */
    QString location() const;

    /**
     * Set the location.
     *
     * @param setLocation The location to set up. */
    void setLocation(const QString &strLocation);

    /**
     * Get the title.
     *
     * @return The title actually set up. */
    QString title() const;

    /**
     * Set the title.
     *
     * @param setTitle The title to set up. */
    void setTitle(const QString &strTitle);

    /**
     * Get the track number.
     *
     * @return The track number. */
    int number() const;

    /**
     * Set the track number.
     *
     * @param iNumber The number to set up. */
    void setNumber(const int iNumber);

    /**
     * Get the track type.
     *
     * @return The track type actually set up. */
    QString trackType() const;

    /**
     * Set the track type.
     *
     * @param strTrackType The track type to set up. */
    void setTrackType(const QString &strTrackType);

    /**
     * Get the primary genre.
     *
     * @return The primary genre actually set up. */
    QString primaryGenre() const;

    /**
     * Set the primary genre.
     *
     * @param strPrimaryGenre The primary genre to set up. */
    void setPrimaryGenre(const QString &strPrimaryGenre);


private:
    QString *m_pstrCreator;
    QString *m_pstrAlbum;
    QString *m_pstrAlbumPrimaryArtist;
    QString *m_pstrTitle;
    QString *m_pstrDuration;
    QString *m_pstrTrackType;
    QString *m_pstrPrimaryGenre;

    QString *m_pstrImageLink;
    QString *m_pstrLocation;

    bool *m_pbDownload;
    int *m_piNumber;
    short *m_psDownloadPercentage;
};


#endif // TRACK_H
