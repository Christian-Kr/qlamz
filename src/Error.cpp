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

#include "Error.h"
#include "ui_Error.h"

#include <QDir>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>


Error::Error(QWidget *pParent)
    : QDialog(pParent),
    m_pUi(new Ui::ErrorDialog())
{
    m_pUi->setupUi(this);
}

Error::~Error()
{
    delete m_pUi;
}

void Error::exec(const QStringList &errors)
{
    m_pUi->textEditErrorLog->clear();

    for (int i = 0; i < errors.size(); i++) {
        m_pUi->textEditErrorLog->append(errors.at(i));
    }

    QTextCursor textCursor = m_pUi->textEditErrorLog->textCursor();
    textCursor.movePosition(QTextCursor::Start);
    m_pUi->textEditErrorLog->setTextCursor(textCursor);

    QDialog::exec();
}

void Error::exec(const QString &error)
{
    m_pUi->textEditErrorLog->clear();
    m_pUi->textEditErrorLog->append(error);

    QTextCursor textCursor = m_pUi->textEditErrorLog->textCursor();
    textCursor.movePosition(QTextCursor::Start);
    m_pUi->textEditErrorLog->setTextCursor(textCursor);

    QDialog::exec();
}

void Error::save()
{
    QString strPath = QFileDialog::getSaveFileName(this, tr("Save"), QDir::homePath());

    if (strPath.size() < 1) {
        return;
    }

    QFile file(strPath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream stream(&file);
    stream << m_pUi->textEditErrorLog->toPlainText();
    stream.flush();

    file.close();
}
