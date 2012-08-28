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

#ifndef PROGRESSDELEGATE_H
#define PROGRESSDELEGATE_H

#include <QStyledItemDelegate>


class ProgressDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    /**
     * Constructor */
    ProgressDelegate(QObject *pParent = 0);

    /**
     * Destructor */
    ~ProgressDelegate();

    /**
     * Override from QStyledItemDelegate. */
    virtual void paint(QPainter *pPainter, const QStyleOptionViewItem &option,
        const QModelIndex &modelIndex) const;

    /**
     * Override from QAbstractItemModel. */
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &modelIndex) const;
};


#endif // PROGRESSDELEGATE_H
