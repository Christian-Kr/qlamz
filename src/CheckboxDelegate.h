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

#ifndef CHECKBOXDELEGATE_H
#define CHECKBOXDELEGATE_H

#include <QStyledItemDelegate>


class CheckboxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    /**
     * Constructor */
    CheckboxDelegate(QObject *pParent = 0);

    /**
     * Destructor */
    ~CheckboxDelegate();

    /**
     * Override from QStyledItemDelegate. */
    virtual void paint(QPainter *pPainter, const QStyleOptionViewItem &option,
        const QModelIndex &modelIndex) const;

    /**
     * Override from QStyledItemDelegate. */
    virtual QWidget * createEditor(QWidget *pParent, const QStyleOptionViewItem &option,
        const QModelIndex &modelIndex) const;
    /**
     * Override from QStyledItemDelegate. */
    virtual void setEditorData(QWidget *pEditor, const QModelIndex &modelIndex) const;

    /**
     * Override from QStyledItemDelegate. */
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &modelIndex) const;

    /**
     * Override from QStyledItemDelegate. */
    virtual bool editorEvent(QEvent *pEvent, QAbstractItemModel *pModel,
        const QStyleOptionViewItem &option, const QModelIndex &index);

signals:
    /**
     * Emited when the editor was clicked.
     *
     * @param iRow The row clicked. */
    void editorClicked(int iRow);
};


#endif // CHECKBOXDELEGATE_H
