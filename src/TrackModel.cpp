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

#include "TrackModel.h"
#include "Track.h"

#include <QList>

#include <QDebug>


TrackModel::TrackModel(QObject *pParent)
    : QAbstractTableModel(pParent),
    m_pTracks(new QList<Track *>())
{
}

TrackModel::~TrackModel()
{
    delete m_pTracks;
}

void TrackModel::appendTracks(const QList<Track *> &tracks)
{
    int iBegin = (m_pTracks->size() < 1) ? 0 : m_pTracks->size() - 1;
    beginInsertRows(QModelIndex(), iBegin, iBegin + tracks.size() - 1);

    m_pTracks->append(tracks);

    endInsertRows();
}

QList<Track *> TrackModel::tracks() const
{
    return *m_pTracks;
}

void TrackModel::removeTracks()
{
    while (m_pTracks->size() > 0) {
        delete m_pTracks->takeFirst();
    }
}

int TrackModel::columnCount(const QModelIndex &) const
{
    return 8;
}

int TrackModel::rowCount(const QModelIndex &) const
{
    return m_pTracks->size();
}

QVariant TrackModel::data(const QModelIndex &modelIndex, int iRole) const
{
    if (iRole == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }

    if (iRole == Qt::DisplayRole) {
        switch (modelIndex.column()) {
        case 0:
            return m_pTracks->at(modelIndex.row())->download();
        case 1:
            return m_pTracks->at(modelIndex.row())->downloadPercentage();
        case 2:
            return m_pTracks->at(modelIndex.row())->number();
        case 3:
            return m_pTracks->at(modelIndex.row())->title();
        case 4:
            return m_pTracks->at(modelIndex.row())->creator();
        case 5:
            return m_pTracks->at(modelIndex.row())->album();
        case 6:
            return m_pTracks->at(modelIndex.row())->primaryGenre();
        case 7:
            return m_pTracks->at(modelIndex.row())->trackType();
        default:
            qDebug() << __func__ << ": Data not available on row: " << modelIndex.row();

            return QVariant();
        }
    }

    return QVariant();
}

bool TrackModel::setData(const QModelIndex &modelIndex, const QVariant &value, int iRole)
{
    if (!modelIndex.isValid()) {
        return false;
    }

    if (iRole == Qt::EditRole) {
        m_pTracks->at(modelIndex.row())->setDownload(value.toBool());

        emit dataChanged(modelIndex, modelIndex);

        return true;
    }

    return false;
}

Qt::ItemFlags TrackModel::flags(const QModelIndex &modelIndex) const
{
    if (modelIndex.isValid()) {
        switch (modelIndex.column()) {
        case 0:
            return QAbstractTableModel::flags(modelIndex) | Qt::ItemIsEditable;
        default:
            return QAbstractTableModel::flags(modelIndex);
        }
    }

    return Qt::NoItemFlags;
}

QVariant TrackModel::headerData(int iSection, Qt::Orientation orientation, int iRole) const
{
    if (iRole == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (iSection) {
            case 0:
                return QVariant();
            case 1:
                return tr("Download");
            case 2:
                return tr("Number");
            case 3:
                return tr("Title");
            case 4:
                return tr("Creator");
            case 5:
                return tr("Album");
            case 6:
                return tr("Genre");
            case 7:
                return tr("Media Type");
            default:
                return QVariant();
            }
        } else {
            return QVariant(iSection + 1);
        }
    }

    return QVariant();
}
