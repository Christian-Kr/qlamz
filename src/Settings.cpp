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

    // Initial amazon ending combo box.
    m_pUi->comboBoxAmazonUrl->addItem("de");
    m_pUi->comboBoxAmazonUrl->addItem("com");
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

    m_pUi->lineEditDestination->setText(m_pSettingsData
        ->value("destination.dir", QDir::homePath()).toString());

    m_pUi->lineEditDestinationFormat->setText(m_pSettingsData
        ->value("destination.format", QString()).toString());

    int iIndex = m_pUi->comboBoxAmazonUrl->findText(m_pSettingsData->value("amazon.tld",
        QString()).toString());
    m_pUi->comboBoxAmazonUrl->setCurrentIndex(iIndex);
    m_pUi->spinBoxMaxDownloads->setValue(m_pSettingsData->value("maxDownloads", 1).toInt());

    m_pUi->checkBoxNumberPrefix->setChecked(
        m_pSettingsData->value("destination.numberPrefix", true).toBool());

    m_pUi->checkBoxExternalBrowser->setChecked(
        m_pSettingsData->value("amazon.externalBrowser", false).toBool());

    m_pUi->buttonApply->setEnabled(false);
}

void Settings::setSettingsData(QSettings *pSettingsData)
{
    m_pSettingsData = pSettingsData;
    loadSettings();
}

void Settings::saveSettings()
{
    m_pSettingsData->setValue("amazon.externalBrowser",
        m_pUi->checkBoxExternalBrowser->isChecked());
    m_pSettingsData->setValue("destination.dir",
        m_pUi->lineEditDestination->text());
    m_pSettingsData->setValue("destination.format",
        m_pUi->lineEditDestinationFormat->text());
    m_pSettingsData->setValue("amazon.tld",
        m_pUi->comboBoxAmazonUrl->currentText());
    m_pSettingsData->setValue("destination.numberPrefix",
        m_pUi->checkBoxNumberPrefix->isChecked());
    m_pSettingsData->setValue("maxDownloads",
        m_pUi->spinBoxMaxDownloads->value());
}
