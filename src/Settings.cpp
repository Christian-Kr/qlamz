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

#include "Settings.h"
#include "ui_Settings.h"

#include <QFileDialog>
#include <QDir>
#include <QSettings>


Settings::Settings(QWidget *pParent)
    : QDialog(pParent),
    m_pUi(new Ui::SettingsDialog()),
    m_pSettingsData(NULL)
{
    m_pUi->setupUi(this);

    // Initial the combo box.
    m_pUi->comboBoxFormat->addItem("No");
    m_pUi->comboBoxFormat->addItem("interpret/album");
}

Settings::~Settings()
{
    delete m_pUi;
}

void Settings::openDestination()
{
    QString strDir = QFileDialog::getExistingDirectory(this, tr("Open Destination"),
        QDir::homePath());

    if (strDir.size() > 0) {
        m_pUi->lineEditDestination->setText(strDir);
    }
}

void Settings::updateButtons()
{
    m_pUi->buttonApply->setEnabled(true);
}

void Settings::apply()
{
    saveSettings();

    emit settingsSaved();
    m_pUi->buttonApply->setEnabled(false);
}

void Settings::ok()
{
    saveSettings();

    emit settingsSaved();
    close();
}

void Settings::loadSettings()
{
    if (m_pSettingsData == NULL) {
        return;
    }

    m_pUi->lineEditDestination->setText(m_pSettingsData->value("destination.dir",
        QDir::homePath()).toString());
    m_pUi->comboBoxFormat->setCurrentIndex(m_pSettingsData->value("destination.format", 0).toInt());

    m_pUi->buttonApply->setEnabled(false);
}

void Settings::setSettingsData(QSettings *pSettingsData)
{
    m_pSettingsData = pSettingsData;
    loadSettings();
}

void Settings::saveSettings()
{
    m_pSettingsData->setValue("destination.dir", m_pUi->lineEditDestination->text());
    m_pSettingsData->setValue("destination.format", m_pUi->comboBoxFormat->currentIndex());
}
