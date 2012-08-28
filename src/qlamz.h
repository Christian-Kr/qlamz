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

#ifndef QLAMZ_H
#define QLAMZ_H

#include <QMainWindow>
#include <QProcess>


namespace Ui {
    class MainWindow;
}

class QStringList;
class Settings;
class About;
class Error;
class Track;
class TrackModel;
class TrackDownloader;
class QSettings;
class QStringList;
class QFile;
class QTemporaryFile;
class QNetworkAccessManager;
class QNetworkReply;

class qlamz : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * Constructor */
    qlamz(QWidget *pParent = 0);

    /**
     * Destructor */
    ~qlamz();

    /**
     * Search for clamz on linux.
     *
     * @param customPaths If setting here some paths, this function will also search in this paths.
     * @return Empty string if nothing found, else the path of clamz. */
    QString clamzAvailable(const QStringList &customPaths = QStringList()) const;

    /**
     * Get the clamz version. */
    QString clamzVersion();

public slots:
    /**
     * Load settings. */
    void loadSettings();

    /**
     * Called when the download finished.
     *
     * @param pTrack The track which was downloaded. */
    void downloadFinished(Track *pTrack);

    /**
     * Called when the bytes of the download updated.
     *
     * @param pTrack The track which is in download. */
    void downloadUpdated(Track *pTrack);

    /**
     * Error while downloading.
     *
     * @param pTrack The track with the error.
     * @param iCode The error code.
     * @param strMessage An error message. */
    void downloadError(int iCode, const QString &strMessage, Track *pTrack);

    /**
     * Open the amazon file. */
    void openAmazonFile();

    /**
     * Updates the download button if it could be enabled or not. */
    void updateDownloadButton();

    /**
     * Start donwloading the file with clamz. */
    void startDownload();

    /**
     * Cancel an active download. */
    void cancelDownload();

    /**
     * Open the settings dialog. */
    void settings();

    /**
     * Open the about dialog. */
    void about();

    /**
     * Select all items. */
    void selectAll();

    /**
     * Deselect all items. */
    void deselectAll();

private:
    /**
     * Update the status text of the clamzStatus label. */
    void updateClamzStatus();

    /**
     * Updates the ui state */
    void updateUiState();

    /**
     * Read all Track objects from the xml document.
     *
     * @param strData The xml data.
     * @return A list of the Track objects. The reciever takes care about deleting the Track
     *     objects! */
    QList<Track *> readTracksFromXml(const QString &strData);

    /**
     * Create a xml of the given amazon file with the help of clamz. The object has to be deleted by
     * the user.
     *
     * @param strAmazonFile The path name of the amazon file.
     * @return A string with the xml data. */
    QString getXmlFromFile(const QString &strAmazonFile);

    /**
     * Get the destination path for the given track.
     *
     * @return The destination path as a string. */
    QString destinationPath(const Track * const pTrack) const;

    /**
     * Show the error log dialog. */
    void showErrorLog();

    enum State {Default, Download};

    qlamz::State m_state;

    TrackModel *m_pTrackModel;
    TrackDownloader *m_pTrackDownloader;

    Ui::MainWindow *m_pUi;

    QString *m_pstrClamzPath;
    QString *m_pstrAmazonFilePath;

    Settings *m_pSettings;
    QSettings *m_pSettingsData;

    About *m_pAbout;

    Error *m_pError;

    QProcess *m_pProcess;

    QNetworkAccessManager *m_pNetworkAccessManager;
    QNetworkReply *m_pNetworkReply;

    QList<Track *> m_trackList;
    QStringList *m_pErrors;

    QString *m_pstrDestination;
};


#endif // QLAMZ_H
