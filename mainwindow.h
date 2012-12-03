/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Qt/qapplication.h>
#include <Qt/qmainwindow.h>
#include <Qt/qplaintextedit.h>
#include <Qt/qfiledialog.h>
#include <Qt/qmessagebox.h>
#include <Qt/qaction.h>
#include <Qt/qmenubar.h>
#include <Qt/qmenu.h>
#include <Qt/qtoolbar.h>
#include <Qt/qstatusbar.h>

#include <QtGui>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef Q_OS_LINUX
#include <unistd.h>
#include <fcntl.h>
#endif
#ifdef Q_OS_WIN32
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#define open _open
#define close _close
#define lseek _lseek
#define read _read
#define ssize_t size_t
#define sleep(x) Sleep(x*1000)
#define O_RDONLY _O_RDONLY
#endif
QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event);


private slots:
//    void newFile();
    void fileOpen();
//    bool save();
//    bool saveAs();
    void about();
//    void documentWasModified();
    void reloadFile();
private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
//    bool maybeSave();
    void loadFile(const QString &fileName);
    //bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QPlainTextEdit *textEdit;
    QString curFile;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
//    QAction *newAct;
    QAction *openAct;
//    QAction *saveAct;
//    QAction *saveAsAct;
    QAction *exitAct;
//    QAction *cutAct;
    QAction *copyAct;
//    QAction *pasteAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
};
//! [0]

#endif
