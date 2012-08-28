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

#include "ProgressDelegate.h"

#include <QApplication>
#include <QCheckBox>

#include <QDebug>


ProgressDelegate::ProgressDelegate(QObject *pParent)
    : QStyledItemDelegate(pParent)
{
}

ProgressDelegate::~ProgressDelegate()
{
}

void ProgressDelegate::paint(QPainter *pPainter, const QStyleOptionViewItem &option,
    const QModelIndex &modelIndex) const
{
    if (modelIndex.column() == 1) {
        QStyleOptionProgressBarV2 progressBarOption;

        short progress = modelIndex.model()->data(modelIndex, Qt::DisplayRole).toInt();

        progressBarOption.rect = option.rect;
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.text = QString("%1%").arg(progress);
        progressBarOption.textVisible = true;
        progressBarOption.progress = progress;

        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, pPainter);
    } else {
        QStyledItemDelegate::paint(pPainter, option, modelIndex);
    }
}

QSize ProgressDelegate::sizeHint(const QStyleOptionViewItem &option,
    const QModelIndex &modelIndex) const
{
    if (modelIndex.data().type() == QVariant::Int) {
        return QSize(25, 25);
    } else {
        return QStyledItemDelegate::sizeHint(option, modelIndex);
    }
}
