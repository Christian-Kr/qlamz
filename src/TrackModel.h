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

#ifndef TRACKMODEL_H
#define TRACKMODEL_H

#include <QAbstractTableModel>


class Track;
class QString;
template <class T> class QList;

class TrackModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    /**
     * Constructor */
    TrackModel(QObject *pParent = 0);

    /**
     * Destructor */
    ~TrackModel();

    /**
     * Appends a track list to this list.
     *
     * @param tracks A list of tracks to append. */
    void appendTracks(const QList<Track *> &tracks);

    /**
     * Get all the tracks.
     *
     * @return The tracks as a QList. */
    QList<Track *> tracks() const;

    /**
     * Remove and delete all tracks. */
    void removeTracks();

    /**
     * Override from QAbstractItemModel. */
    virtual bool setData(const QModelIndex &modelIndex, const QVariant &value,
        int iRole = Qt::EditRole);

    /**
     * Override from QAbstractItemModel. */
    virtual QVariant headerData(int iSection, Qt::Orientation orientation,
        int iRole = Qt::DisplayRole) const;

    /**
     * Override from QAbstractItemModel. */
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     * Override from QAbstractItemModel. */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     * Override from QAbstractItemModel. */
    virtual QVariant data(const QModelIndex &index, int iRole = Qt::DisplayRole) const;

    /**
     * Override from QAbstractItemModel. */
    virtual Qt::ItemFlags flags(const QModelIndex &modelIndex) const;

private:
    QList<Track *> *m_pTracks;
};


#endif // TRACKMODEL_H
