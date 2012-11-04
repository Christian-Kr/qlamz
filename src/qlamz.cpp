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

#include "qlamz.h"
#include "ui_qlamz.h"

#include "Settings.h"
#include "About.h"
#include "Error.h"
#include "Track.h"
#include "TrackModel.h"
#include "TrackDownloader.h"
#include "CheckboxDelegate.h"
#include "ProgressDelegate.h"

#include <QList>
#include <QStringList>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QTemporaryFile>
#include <QFileDialog>
#include <QSettings>
#include <QProcess>
#include <QXmlStreamReader>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkCookieJar>
#include <QDesktopServices>

#include <QDebug>

#include <amz.h>


qlamz::qlamz(QWidget *pParent)
    : QMainWindow(pParent),
    m_state(qlamz::Default),
    m_bCancel(false),
    m_iMaxDownloads(0),
    m_iLeftDownloads(0),
    m_pTrackModel(new TrackModel()),
    m_pUi(new Ui::MainWindow()),
    m_pstrAmazonFilePath(new QString()),
    m_pSettings(new Settings(this)),
    m_pSettingsData(new QSettings()),
    m_pAbout(new About(this)),
    m_pError(new Error(this)),
    m_pErrors(new QStringList()),
    m_pRecentFiles(new QStringList()),
    m_pstrDestination(new QString()),
    m_pstrXmlData(new QString()),
    m_pAmz(new amz::amz()),
    m_pNetAccessManager(new QNetworkAccessManager(this))
{
    m_pUi->setupUi(this);

    // Init network access manager
    m_pNetAccessManager->setCookieJar(new QNetworkCookieJar(m_pNetAccessManager));

    // Set some window configs.
    setWindowTitle(tr("qlamz"));

    // Set the settings data object.
    m_pSettings->setSettingsData(m_pSettingsData);

    // Set the model.
    m_pUi->tableViewTracks->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    m_pUi->tableViewTracks->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    m_pUi->tableViewTracks->verticalHeader()->setDefaultSectionSize(25);
    m_pUi->tableViewTracks->setItemDelegateForColumn(0, new CheckboxDelegate());
    m_pUi->tableViewTracks->setItemDelegateForColumn(1, new ProgressDelegate());
    m_pUi->tableViewTracks->setModel(m_pTrackModel);
    m_pUi->tableViewTracks->resizeColumnsToContents();
    m_pUi->tableViewTracks->hideColumn(1);

    connect(m_pSettings, SIGNAL(settingsSaved()), this, SLOT(loadSettings()));

    loadSettings();
}

qlamz::~qlamz()
{
    delete m_pstrXmlData;
    delete m_pTrackModel;
    delete m_pUi;
    delete m_pSettings;
    delete m_pSettingsData;
    delete m_pstrDestination;
    delete m_pError;
    delete m_pRecentFiles;
    delete m_pAbout;
    delete m_pAmz;
    delete m_pNetAccessManager;
}

void qlamz::downloadFinish(Track *pTrack, QNetworkReply *pNetworkReply,
    TrackDownloader *pTrackDownloader)
{
    // Create the path if it does not exist.
    QString strDestinationPath = destinationPath(pTrack);
    QDir dir;

    dir.mkpath(strDestinationPath);

    if (m_pSettingsData->value("destination.numberPrefix", true).toBool()) {
        int iNumber = pTrack->number();
        QString strNumber;
        if (iNumber < 10) {
            strNumber = "0";
        }

        strNumber = strNumber + QString::number(iNumber);

        strDestinationPath = strDestinationPath + strNumber + " - ";
    }

    // Create the file and write all data.
    QFile file(strDestinationPath + pTrack->title() + ".mp3");
    file.open(QIODevice::WriteOnly);
    file.write(pNetworkReply->readAll());
    file.close();

    // If there are files waiting for download, do so.
    if (m_trackList.size() > 0) {
        Track *pNextTrack = m_trackList.takeFirst();
        pTrackDownloader->startDownload(pNextTrack);
        m_pUi->tableViewTracks->update();
    } else {

        // Test if all downloader are not running.
        bool bDownloaderRunning = false;
        for (int i = 0; i < m_trackDownloaderList.size(); i++) {
            TrackDownloader *pTrackDownloaderTmp = m_trackDownloaderList.at(i);

            if (pTrackDownloader != pTrackDownloaderTmp &&
                pTrackDownloaderTmp->isRunning()) {
                bDownloaderRunning = true;
                break;
            }
        }

        // If no other downloader is running... reset the ui.
        if (!bDownloaderRunning) {
            m_state = qlamz::Default;
            updateUiState();
        }

        // Display the error dialog, if any errors occured.
        if (m_pErrors->size() > 0) {
            m_pError->exec(*m_pErrors);
        }
    }
}

void qlamz::downloadUpdate(Track *pTrack, qint64 iRecieved, qint64 iTotal,
    QNetworkReply *pNetworkReply)
{
    pTrack->setDownloadPercentage((short) ((double) iRecieved / (double) iTotal * 100));
    m_pUi->tableViewTracks->reset();
}

void qlamz::downloadError(int iCode, const QString &strMessage, Track *pTrack)
{
    m_pErrors->append("Error downloading pTrack: " + pTrack->title() + "\n" + strMessage + "\n");
    qDebug() << "Error downloading pTrack: " + pTrack->title() + "\n" + strMessage + "\n";
}

void qlamz::about()
{
    m_pAbout->exec();
}

void qlamz::cookieAmazonDe()
{
    // Load the cookie link from the settings.
    QDesktopServices::openUrl(m_pSettingsData->value("amazon.cookie.url.de", QString()).toString());
}

void qlamz::closeEvent(QCloseEvent *pEvent)
{
    saveSettings();

    QMainWindow::closeEvent(pEvent);
}

void qlamz::loadSettings()
{
    // Read in recent files.
    *m_pRecentFiles = m_pSettingsData->value("recentfiles", QStringList()).toStringList();

    m_iMaxDownloads = m_pSettingsData->value("maxDownloads", 1).toInt();

    // Init the TrackDownloader list.
    int iMaxDownloadDiff = m_iMaxDownloads - m_trackDownloaderList.size();

    if (iMaxDownloadDiff != 0) {
        if (iMaxDownloadDiff > 0) {
            // Create new TrackDownlader object, until we got the maximum number.
            while (iMaxDownloadDiff-- > 0) {
                TrackDownloader *pTrackDownloader = new TrackDownloader(m_pNetAccessManager, this);
                m_trackDownloaderList.append(pTrackDownloader);

                qDebug() << "Create TrackDownloader";

                // Build some connections.
                connect(pTrackDownloader,
                    SIGNAL(finish(Track *, QNetworkReply *, TrackDownloader *)), this,
                    SLOT(downloadFinish(Track *, QNetworkReply *, TrackDownloader *)));
                connect(pTrackDownloader, SIGNAL(update(Track *, qint64, qint64, QNetworkReply *)), this,
                    SLOT(downloadUpdate(Track *, qint64, qint64, QNetworkReply *)));
                connect(pTrackDownloader, SIGNAL(error(int, const QString &, Track *)), this,
                    SLOT(downloadError(int, const QString &, Track *)));
            }
        } else {
            // Delete TrackDownloader objects, until we got the maximum number.
            while (m_trackDownloaderList.size() > m_iMaxDownloads) {
                delete m_trackDownloaderList.takeFirst();
            }
        }
    }

    updateRecentFiles();
}

void qlamz::saveSettings()
{
    m_pSettingsData->setValue("recentFiles", *m_pRecentFiles);
    m_pSettingsData->setValue("maxDownloads", m_iMaxDownloads);
}

void qlamz::updateRecentFiles()
{
    // Delete all old actions.
    m_pUi->menuRecentFiles->clear();

    for (int i = 0; i < m_pRecentFiles->size(); i++) {
        QAction *action = new QAction(m_pRecentFiles->at(i), m_pUi->menuRecentFiles);
        m_pUi->menuRecentFiles->addAction(action);

        connect(action, SIGNAL(triggered()), this, SLOT(recentFileTriggered()));
    }
}

void qlamz::recentFileTriggered()
{
    QAction *action = (QAction *) sender();
    qDebug() << action->text();
    openAmazonFile(action->text());
}

void qlamz::settings()
{
    m_pSettings->exec();
}

void qlamz::aboutQt()
{
    QApplication::aboutQt();
}

QString qlamz::decryptAmazonFile(const QByteArray &amazonEncryptedContent)
{
    return QString::fromUtf8(reinterpret_cast<const char *>(m_pAmz->decryptAmzData(
        const_cast<char *>(amazonEncryptedContent.data()), amazonEncryptedContent.size())));
}

void qlamz::openAmazonFile(const QString &strAmazonFileArg)
{
    QString strAmazonFile;

    if (strAmazonFileArg.size() > 0) {
        strAmazonFile = strAmazonFileArg;
    } else {
        strAmazonFile = QFileDialog::getOpenFileName(this, tr("Open Amazon File"), QDir::homePath(),
            tr("Amazon (*.amz)"));
    }

    if (strAmazonFile.size() > 0) {
            *m_pstrAmazonFilePath = strAmazonFile;
            setWindowTitle("qlamz - " + strAmazonFile);
        } else {
            return;
    }

    // Create a temporary file from the xml output of the amazon file.
    QString strXmlData = getXmlFromFile(*m_pstrAmazonFilePath);

    *m_pstrXmlData = strXmlData;

    QList<Track *> tracks = readTracksFromXml(strXmlData);

    qDebug() << __func__ << ": Number of Tracks: " << tracks.size();

    m_pTrackModel->removeTracks();
    m_pTrackModel->appendTracks(tracks);
    m_pUi->tableViewTracks->resizeColumnsToContents();

    updateUiState();

    m_pUi->tableViewTracks->hideColumn(1);
    m_pUi->actionDeselectAll->setEnabled(true);
    m_pUi->actionSelectAll->setEnabled(true);

    // Add the opened file to the recent files menu.
    while (m_pRecentFiles->indexOf(strAmazonFile) > -1) {
        m_pRecentFiles->takeAt(m_pRecentFiles->indexOf(strAmazonFile));
    }

    m_pRecentFiles->insert(0, strAmazonFile);

    // Remove the last entry, when tere are more than 10.
    if (m_pRecentFiles->size() > 10) {
        m_pRecentFiles->takeLast();
    }

    updateRecentFiles();
}

void qlamz::selectAll()
{
    // Close any opened editor.
    m_pUi->tableViewTracks->closePersistentEditor(m_pUi->tableViewTracks->currentIndex());

    QList<Track *> tracks = m_pTrackModel->tracks();

    for (int i = 0; i < tracks.size(); i++) {
        tracks.at(i)->setDownload(true);
    }

    m_pUi->tableViewTracks->reset();
}

void qlamz::deselectAll()
{
    // Close any opened editor.
    m_pUi->tableViewTracks->closePersistentEditor(m_pUi->tableViewTracks->currentIndex());

    QList<Track *> tracks = m_pTrackModel->tracks();

    for (int i = 0; i < tracks.size(); i++) {
        tracks.at(i)->setDownload(false);
    }

    m_pUi->tableViewTracks->reset();
}

void qlamz::updateDownloadButton()
{
    if (m_pstrAmazonFilePath->length() > 0) {
        m_pUi->buttonDownload->setEnabled(true);
    } else {
        m_pUi->buttonDownload->setEnabled(false);
    }
}

void qlamz::updateUiState()
{
    switch (m_state) {
    case qlamz::Default:
        m_pUi->buttonQuit->setEnabled(true);
        m_pUi->buttonCancel->setEnabled(false);

        // Sepcial handling for the download button.
        updateDownloadButton();

        m_pUi->tableViewTracks->setEnabled(true);

        m_pUi->actionDeselectAll->setEnabled(true);
        m_pUi->actionSelectAll->setEnabled(true);
        m_pUi->actionSettings->setEnabled(true);

        break;
    case qlamz::Download:
        m_pUi->buttonQuit->setEnabled(false);
        m_pUi->buttonCancel->setEnabled(true);
        m_pUi->buttonDownload->setEnabled(false);

        m_pUi->tableViewTracks->setEnabled(false);

        m_pUi->actionDeselectAll->setEnabled(false);
        m_pUi->actionSelectAll->setEnabled(false);
        m_pUi->actionSettings->setEnabled(false);

        break;
    default:
        qDebug() << __func__ << ": Unknown state";
    }
}

QString qlamz::destinationPath(const Track * const pTrack) const
{
    // Load the information about the destination and build the destination path.
    QString strDestination = m_pSettingsData->value("destination.dir", QDir::homePath()).toString();
    QString strFormat;

    QString strCreator = pTrack->creator();
    QString strAlbum = pTrack->album();

    if (strCreator.startsWith('.')) {
        strCreator.insert(0, ' ');
    }

    if (strAlbum.startsWith('.')) {
        strAlbum.insert(0, ' ');
    }

    switch (m_pSettingsData->value("destination.format", 0).toInt()) {
    case 0:
        break;
    case 1:
        strFormat = strCreator + "/" + strAlbum + "/";
        break;
    default:
        break;
    }

    return strDestination + "/" + strFormat;
}

void qlamz::startDownload()
{
    // Set the actual state.
    m_state = qlamz::Download;
    updateUiState();

    // Display the download progress column.
    m_pUi->tableViewTracks->showColumn(1);

    // Add all tracks that are checked to the m_trackList.
    QList<Track *> trackList = m_pTrackModel->tracks();
    m_trackList.clear();

    for (int i = 0; i < trackList.size(); i++) {
        if (trackList.at(i)->download()) {
            m_trackList.append(trackList.at(i));
        }
    }

    // Start all TrackDownloader with downloading a file.
    for (int i = 0; i < m_trackDownloaderList.size(); i++) {
        if (m_trackList.size() <= 0) {
            break;
        }

        Track *pTrack = m_trackList.takeFirst();
        m_trackDownloaderList.at(i)->startDownload(pTrack);
    }

    m_pErrors->clear();
}

QList<Track *> qlamz::readTracksFromXml(const QString &strData)
{
    QList<Track *> tracks;

    QXmlStreamReader xmlReader(strData);

    while (!xmlReader.atEnd()) {
        xmlReader.readNextStartElement();

        // If finding <track> get the properties.
        if (xmlReader.name() == "track") {
            Track *pTrack = new Track();

            while (!(xmlReader.tokenType() == QXmlStreamReader::EndElement &&
                xmlReader.name() == "track")) {

                xmlReader.readNext();

                // We just go one, when we get a starting element.
                if (xmlReader.tokenType() == QXmlStreamReader::StartElement) {
                    if (xmlReader.name() == "location") {
                        pTrack->setLocation(xmlReader.readElementText());
                    } else if (xmlReader.name() == "creator") {
                        pTrack->setCreator(xmlReader.readElementText());
                    } else if (xmlReader.name() == "album") {
                        pTrack->setAlbum(xmlReader.readElementText());
                    } else if (xmlReader.name() == "title") {
                        pTrack->setTitle(xmlReader.readElementText());
                    } else if (xmlReader.name() == "trackNum") {
                        pTrack->setNumber(xmlReader.readElementText().toInt());
                    } else {
                        xmlReader.skipCurrentElement();
                    }
                }
            }

            // Add the track to the track list.
            tracks.append(pTrack);
        }
    }

    if (xmlReader.hasError()) {
        qDebug() << __func__ << ": " << xmlReader.errorString();
    }

    return tracks;
}

void qlamz::showErrorLog()
{
    m_pError->exec(*m_pErrors);
}

void qlamz::showXMLContent()
{
    m_pError->exec(*m_pstrXmlData);
}

void qlamz::openAmazonStore()
{
    QString strAmazonTld = m_pSettingsData->value(QString("amazon.tld"),
        QString()).toString();
    QString strUrl = m_pSettingsData->value("amazon.store.url." + strAmazonTld,
        QString()).toString();

    if (strUrl.size() < 1) {
        QMessageBox::warning(this, tr("Warning"), tr("Cannot find a url matching the tld. Sorry!"),
            QMessageBox::Ok);

        return;
    }

    QDesktopServices::openUrl(strUrl);
}

void qlamz::cancelDownload()
{
    int iReturn = QMessageBox::question(this, tr("Cancel download"), tr("Are you sure canceling the"
        " download progress?"), QMessageBox::Yes | QMessageBox::No);

    if (iReturn != QMessageBox::Yes) {
        return;
    }

    m_bCancel = true;

    for (int i = 0; i < m_trackDownloaderList.size(); i++) {
        m_trackDownloaderList.at(i)->abort();
    }

    m_state = qlamz::Default;
    updateUiState();
}

QString qlamz::getXmlFromFile(const QString &strAmazonFilePath)
{
    QFile file(strAmazonFilePath);
    file.open(QIODevice::ReadOnly);

    QByteArray tmpData = file.readAll().replace('\n', "");
    file.close();

    return decryptAmazonFile(tmpData);
}

