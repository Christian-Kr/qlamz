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

#include "CheckboxDelegate.h"

#include <QApplication>
#include <QCheckBox>

#include <QDebug>


CheckboxDelegate::CheckboxDelegate(QObject *pParent)
    : QStyledItemDelegate(pParent)
{
}

CheckboxDelegate::~CheckboxDelegate()
{
}

void CheckboxDelegate::paint(QPainter *pPainter, const QStyleOptionViewItem &option,
    const QModelIndex &modelIndex) const
{
    if (modelIndex.column() == 0) {
        QStyleOptionButton checkboxOption;

        if (modelIndex.model()->data(modelIndex, Qt::DisplayRole).toBool()) {
            checkboxOption.state = QStyle::State_On;
        } else {
            checkboxOption.state = QStyle::State_Off;
        }

        checkboxOption.rect = option.rect;

        QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkboxOption, pPainter);
    } else {
        QStyledItemDelegate::paint(pPainter, option, modelIndex);
    }
}

QWidget * CheckboxDelegate::createEditor(QWidget *pParent, const QStyleOptionViewItem &option,
    const QModelIndex &modelIndex) const
{
    if (modelIndex.data().type() == QVariant::Bool) {
        QCheckBox *pCheckBox = new QCheckBox(pParent);

        return pCheckBox;
    } else {
        return QStyledItemDelegate::createEditor(pParent, option, modelIndex);
    }
}

bool CheckboxDelegate::editorEvent(QEvent *pEvent, QAbstractItemModel *pModel,
    const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (pEvent->type() == QEvent::MouseButtonPress) {
        emit editorClicked(index.row());
    }

    return QStyledItemDelegate::editorEvent(pEvent, pModel, option, index);
}

void CheckboxDelegate::setEditorData(QWidget *pEditor, const QModelIndex &modelIndex) const
{
    if (modelIndex.data().type() == QVariant::Bool) {
        QCheckBox *pCheckBox = qobject_cast<QCheckBox *>(pEditor);
        pCheckBox->setChecked(modelIndex.data().toBool());
    } else {
        QStyledItemDelegate::setEditorData(pEditor, modelIndex);
    }
}

QSize CheckboxDelegate::sizeHint(const QStyleOptionViewItem &option,
    const QModelIndex &modelIndex) const
{
    if (modelIndex.data().type() == QVariant::Bool) {
        return QSize(25, 25);
    } else {
        return QStyledItemDelegate::sizeHint(option, modelIndex);
    }
}
