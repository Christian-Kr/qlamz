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

#ifndef ERROR_H
#define ERROR_H

#include <QDialog>


namespace Ui {
    class ErrorDialog;
}

class Error : public QDialog
{
    Q_OBJECT

public:
    /**
     * Constructor */
    Error(QWidget *pParent = 0);

    /**
     * Destructor */
    ~Error();

    /**
     * Exec the dialog with a StringList message.
     *
     * @param strErrors The list of errors. */
    void exec(const QStringList &errors);

    /**
     * Exec the dialog with the string message.
     *
     * @param strError The error message. */
    void exec(const QString &errors);

public slots:
    /**
     * Save error log to file. */
    void save();

private:
    Ui::ErrorDialog *m_pUi;
};


#endif // ERROR_H
