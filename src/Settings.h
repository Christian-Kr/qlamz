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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>


class QSettings;
namespace Ui {
    class SettingsDialog;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    /**
     * Constructor */
    Settings(QWidget *pParent = 0);

    /**
     * Destructor */
    ~Settings();

    /**
     * Load the settings from the settings object. */
    void loadSettings();

    /**
     * Save the settings. */
    void saveSettings();

    /**
     * Set the settings object.
     *
     * @param pSettings The settings object. */
    void setSettingsData(QSettings *pSettingsData);

public slots:
    /**
     * Opens the destination folder. */
    void openDestination();

    /**
     * Update the buttons. */
    void updateButtons();

    /**
     * Apply settings. */
    void apply();

    /**
     * Apply settings and close. */
    void ok();

signals:
    /**
     * Emited when settings saved. */
    void settingsSaved();

private:
    Ui::SettingsDialog *m_pUi;

    QSettings *m_pSettingsData;
};


#endif // SETTINGS_H
