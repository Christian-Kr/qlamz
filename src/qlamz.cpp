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
#include <QDesktopServices>

#include <QDebug>


qlamz::qlamz(QWidget *pParent)
    : QMainWindow(pParent),
    m_state(qlamz::Default),
    m_pTrackModel(new TrackModel()),
    m_pTrackDownloader(new TrackDownloader(this)),
    m_pUi(new Ui::MainWindow()),
    m_pstrClamzPath(new QString()),
    m_pstrAmazonFilePath(new QString()),
    m_pSettings(new Settings(this)),
    m_pSettingsData(new QSettings("Christian Krippendorf", "qlamz")),
    m_pAbout(new About(this)),
    m_pError(new Error(this)),
    m_pProcess(new QProcess(this)),
    m_pNetworkAccessManager(new QNetworkAccessManager(this)),
    m_pNetworkReply(NULL),
    m_pErrors(new QStringList()),
    m_pstrDestination(new QString())
{
    m_pUi->setupUi(this);

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

    // Set configs for processes.
    m_pProcess->setProcessChannelMode(QProcess::MergedChannels);

    // Build some connections.
    connect(m_pTrackDownloader, SIGNAL(finished(Track *)), this, SLOT(downloadFinished(Track *)));
    connect(m_pTrackDownloader, SIGNAL(updated(Track *)), this, SLOT(downloadUpdated(Track *)));
    connect(m_pTrackDownloader, SIGNAL(error(int, const QString &, Track *)), this,
        SLOT(downloadError(int, const QString &, Track *)));
    connect(m_pSettings, SIGNAL(settingsSaved()), this, SLOT(loadSettings()));

    loadSettings();
}

qlamz::~qlamz()
{
    delete m_pTrackModel;
    delete m_pTrackDownloader;
    delete m_pUi;
    delete m_pstrClamzPath;
    delete m_pSettings;
    delete m_pSettingsData;
    delete m_pNetworkAccessManager;
    delete m_pNetworkReply;
    delete m_pstrDestination;
    delete m_pError;
    delete m_pAbout;
}

void qlamz::downloadFinished(Track *pTrack)
{
    if (m_trackList.size() > 0) {
        Track *pNextTrack = m_trackList.takeFirst();
        m_pTrackDownloader->startDownload(pNextTrack, destinationPath(pTrack));

        m_pUi->tableViewTracks->update();
    } else {
        m_state = qlamz::Default;
        updateUiState();

        // Display the error dialog, if any errors occured.
        if (m_pErrors->size() > 0) {
            m_pError->exec(*m_pErrors);
            m_pErrors->clear();
        }
    }
}

void qlamz::downloadUpdated(Track *pTrack)
{
    m_pUi->tableViewTracks->reset();
}

void qlamz::downloadError(int iCode, const QString &strMessage, Track *pTrack)
{
    m_pErrors->append("Error downloading pTrack: " + pTrack->title() + "\n" + strMessage + "\n");
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

QString qlamz::clamzVersion()
{
    m_pProcess->start(*m_pstrClamzPath, QStringList() << "--version");

    if (!m_pProcess->waitForFinished()) {
        return QString();
    }

    QString strOutput = m_pProcess->readLine();

    // Get the version number from the output.
    QStringList outputList = strOutput.split(' ');

    if (outputList.size() < 2) {
        return QString();
    }

    QString strVersion = outputList[1].split('\n')[0];

    qDebug() << __func__ << ": Found Version: " << strVersion;

    return strVersion;
}

void qlamz::loadSettings()
{
    // If this function gets called, we have to read in the custom paths cause maybe they changed.
    QStringList customPaths = m_pSettingsData->value("clamz.customPaths", QStringList())
        .toStringList();
    QString strClamzPath = clamzAvailable(customPaths);

    // Test if we found clamz.
    delete m_pstrClamzPath;

    if (strClamzPath.size() > 0) {
        m_pstrClamzPath = new QString(strClamzPath);
    } else {
        m_pstrClamzPath = new QString();
        QMessageBox::information(this, tr("Information"), tr("We cannot find clamz. If u have "
            "clamz already installed, please set a custom search path in settings."),
            QMessageBox::Ok);
    }

    updateClamzStatus();
}

void qlamz::settings()
{
    m_pSettings->exec();
}

void qlamz::openAmazonFile()
{
    QString strAmazonFile = QFileDialog::getOpenFileName(this, tr("Open Amazon File"),
        QDir::homePath(), tr("Amazon (*.amz)"));

    if (strAmazonFile.size() > 0) {
        *m_pstrAmazonFilePath = strAmazonFile;
        setWindowTitle("qlamz - " + strAmazonFile);
    } else {
        return;
    }

    // Create a temporary file from the xml output of the amazon file.
    QString strXmlData = getXmlFromFile(*m_pstrAmazonFilePath);

    QList<Track *> tracks = readTracksFromXml(strXmlData);

    qDebug() << __func__ << ": Number of Tracks: " << tracks.size();

    m_pTrackModel->removeTracks();
    m_pTrackModel->appendTracks(tracks);
    m_pUi->tableViewTracks->resizeColumnsToContents();

    updateUiState();

    m_pUi->tableViewTracks->hideColumn(1);
    m_pUi->actionDeselectAll->setEnabled(true);
    m_pUi->actionSelectAll->setEnabled(true);
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

void qlamz::updateClamzStatus()
{
    QString strStatusText;
    QString strColor;

    if (QFile::exists(*m_pstrClamzPath)) {
        strStatusText = tr("Found!");
        strColor = "green";
    } else {
        strStatusText = tr("Not Found!");
        strColor = "red";
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

        break;
    case qlamz::Download:
        m_pUi->buttonQuit->setEnabled(false);
        m_pUi->buttonCancel->setEnabled(true);
        m_pUi->buttonDownload->setEnabled(false);

        m_pUi->tableViewTracks->setEnabled(false);

        m_pUi->actionDeselectAll->setEnabled(false);
        m_pUi->actionSelectAll->setEnabled(false);

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

    switch (m_pSettingsData->value("destination.format", 0).toInt()) {
    case 0:
        break;
    case 1:
        strFormat = pTrack->creator() + "/" + pTrack->album();
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

    // Set the actual state.
    m_state = qlamz::Download;
    updateUiState();

    // Display the download progress column.
    m_pUi->tableViewTracks->showColumn(1);

    // Go throught all file objects.
    m_trackList = m_pTrackModel->tracks();

    if (m_trackList.size() > 0) {
        Track *pTrack = m_trackList.takeFirst();
        m_pTrackDownloader->startDownload(pTrack, destinationPath(pTrack));
    }
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

void qlamz::cancelDownload()
{
    m_state = qlamz::Default;
    updateUiState();
}

QString qlamz::clamzAvailable(const QStringList &customPaths) const
{
    // Search for clamz in a linux system.
    QStringList searchPaths;

    // Add custom paths.
    searchPaths.append(customPaths);

    // Add static paths.
    searchPaths.append("/usr/bin");
    searchPaths.append("/usr/local/bin");

    for (int i = 0; i < searchPaths.size(); ++i) {
        QString strPath = QString("%1/clamz").arg(searchPaths.at(i));

        if (QFile::exists(strPath)) {
            qDebug() << __func__ << ": Found path: " << strPath;

            return strPath;
        }
    }

    return QString();
}

QString qlamz::getXmlFromFile(const QString &strAmazonFilePath)
{
    m_pProcess->start(*m_pstrClamzPath + " -x " + strAmazonFilePath);

    if (!m_pProcess->waitForFinished()) {
        qDebug() << __func__ << ": m_pProcess->waitForFinished() failed! ";

        return QString();
    }

    return m_pProcess->readAll();
}

