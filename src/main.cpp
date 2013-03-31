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

#include <QApplication>
#include <QTranslator>
#include <QLocale>

#include <iostream>

#include "qlamz.h"

#define VERSION 0.1


int main(int argc, char *argv[])
{
    std::cout << "qlamz Version: " << VERSION
        << " - A frontend for downloading with amz files in C++/Qt4."
        <<  std::endl;

    // Create the application object
    QApplication::setOrganizationName("qlamz");
    QApplication::setApplicationName("qlamz");
    QApplication application(argc, argv);

    // Internationalization
    QString locale = QLocale::system().name();

    QTranslator translator;
    translator.load(QString("qlamz_") + locale);
    application.installTranslator(&translator);

    qlamz * pqlamz = new qlamz();
    pqlamz->setVisible(true);

    // If we got a argument with the fiel name, we will try to open it.
    if (argc > 1) {
        pqlamz->openAmazonFile(argv[1]);
    }

    // Go!
    return application.exec();
}
