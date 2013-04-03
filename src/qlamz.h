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

namespace amz {
    class amz;
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
     * Decrypt the given amazon file bytes.
     *
     * @param amazonEncryptedContent The content as a byte array. */
    QString decryptAmazonFile(const QByteArray &amazonEncryptedContent);

public slots:
    /**
     * Export the cookies created by the internal browser. */
    void exportCookies();

    /**
     * A new amz file was downloaded.
     *
     * @param strContent The content of the amz file. */
    void amzDownloaded(const QString &strContent);

    /**
     * Show the error log window. */
    void showErrorLog();

    /**
     * Show the xml content of the actual loaded amz file. */
    void showXMLContent();

    /**
     * Show the amazon store in a web browser.
     *
     * @param strUrl The url to open. */
    void openAmazonStore(const QString &strUrl = QString());

    /**
     * Show the about qt dialog. */
    void aboutQt();

    /**
     * Someone tries to load a recent file from the menu. */
    void recentFileTriggered();

    /**
     * Load settings. */
    void loadSettings();

    /**
     * Save settings. */
    void saveSettings();

    /**
     * Called when the download finished.
     *
     * @param pTrack The track which was downloaded. */
    void downloadFinish(Track *pTrack, QNetworkReply *pNetworkReply,
        TrackDownloader *pTrackDownloader);

    /**
     * Called when the bytes of the download updated.
     *
     * @param pTrack The track which is in download. */
    void downloadUpdate(Track *pTrack, qint64 iRecieved, qint64 iTotal, QNetworkReply *pNetworkReply);

    /**
     * Error while downloading.
     *
     * @param pTrack The track with the error.
     * @param iCode The error code.
     * @param strMessage An error message. */
    void downloadError(int iCode, const QString &strMessage, Track *pTrack);

    /**
     * Open the amazon file from content.
     *
     * @param strContent The xml content. */
    void openAmazonFileFromString(const QString &strContent);

    /**
     * Open the amazon file.
     *
     * @param strAmazonFileArg A amazon file path if u want to open a specific one. */
    void openAmazonFile(const QString &strAmazonFileArg = QString());

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

    /**
     * Open the Amazon.de url for setting a cookie, that AmazonDownloader is already installed. */
    void cookieAmazonDe();

    /**
     * Sets the value of the webview progressbar.
     *
     * @param iProgress The load of the progress from 0 to 100. */
    void setWebViewLoadProgress(int iProgress);

protected:
    /**
     * Overwriting from QMainWindow. */
    virtual void closeEvent(QCloseEvent *pEvent);

private:
    /**
     * Updates the ui state */
    void updateUiState();

    /**
     * Update the recent files menu. */
    void updateRecentFiles();

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

    enum State {Default, Download};

    qlamz::State m_state;

    bool m_bCancel;
    int m_iMaxDownloads;
    int m_iLeftDownloads;

    int m_iActualDownloadCount;
    int m_iTotalDownloadCount;

    TrackModel *m_pTrackModel;

    Ui::MainWindow *m_pUi;

    QString *m_pstrAmazonFilePath;

    Settings *m_pSettings;
    QSettings *m_pSettingsData;
    QSettings *m_pAmazonInfos;

    About *m_pAbout;
    Error *m_pError;

    QList<Track *> m_trackList;
    QList<TrackDownloader *> m_trackDownloaderList;

    QStringList *m_pErrors;
    QStringList *m_pRecentFiles;

    QString *m_pstrDestination;
    QString *m_pstrXmlData;
    QString *m_pstrOpenPath;

    amz::amz *m_pAmz;

    QNetworkAccessManager *m_pNetAccessManager;
};


#endif // QLAMZ_H
