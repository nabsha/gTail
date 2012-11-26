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

//! [0]
#include <QtGui>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

#include "mainwindow.h"
//! [0]
QFile *logFile;
QTextStream *inReader;
QFileSystemWatcher *logger;
qint64 cursorLoc = 0;

MainWindow::MainWindow()
{
    QFont newFont("Courier", 10, QFont::Normal, false);
    textEdit = new QPlainTextEdit;
    QPalette p = textEdit->palette();
    textEdit->setReadOnly(true);
    p.setColor(QPalette::Active, QPalette::Base, Qt::black);
    p.setColor(QPalette::Inactive, QPalette::Base, Qt::black);
    p.setColor(QPalette::Active, QPalette::Text,Qt::white);
    p.setColor(QPalette::Inactive, QPalette::Text,Qt::white);

    textEdit->setPalette(p);

    setCentralWidget(textEdit);
    textEdit->setFont(newFont);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    readSettings();
    setCurrentFile("");
    setUnifiedTitleAndToolBarOnMac(true);
}



void MainWindow::closeEvent(QCloseEvent *event)
{
}
void MainWindow::fileOpen()
{
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty()) {
            loadFile(fileName);
        }
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About gTail"),
            tr("The <b>gTail</b> is open source GUI extension of GNU tail program written in Qt"));
}
void MainWindow::createActions()
{
    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(fileOpen()));
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));
    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    copyAct->setEnabled(false);
    connect(textEdit, SIGNAL(copyAvailable(bool)),
            copyAct, SLOT(setEnabled(bool)));
}
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(copyAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(openAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(copyAct);
}
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings("Trolltech", "Application Example");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}

void MainWindow::writeSettings()
{
    QSettings settings("Trolltech", "Application Example");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

#define BUF_SIZE 8192
void MainWindow::reloadFile(){
    int input_fd;
    ssize_t ret_in;
    char buffer[BUF_SIZE];
    struct stat st;
    off_t oldSize, newSize;
    input_fd = open (curFile.toAscii(), O_RDONLY);
    if (input_fd == -1) {
            perror ("open");
            return;
    }
    errno = 0;
    if(fstat(input_fd, &st))
    {
        printf("\nfstat error: [%s]\n",strerror(errno));
        ::close(input_fd);
        return;
    }
    oldSize = st.st_size;

    forever {
        QCoreApplication::processEvents();
        if(fstat(input_fd, &st))
        {
            printf("\nfstat error: [%s]\n",strerror(errno));
            ::close(input_fd);
            return;
        }
        newSize = st.st_size;

        if (oldSize == newSize)
            continue;

        lseek(input_fd, oldSize, SEEK_SET);
        while((ret_in = read (input_fd, &buffer, newSize-oldSize)) > 0){
            QString qBuf(buffer);
            QCoreApplication::processEvents();
            textEdit->appendPlainText(qBuf.trimmed());
        }
        memset(buffer,0,sizeof(buffer));
        oldSize = newSize;
        sleep(1);
    }
    ::close (input_fd);

}


void MainWindow::loadFile(const QString &fileName)
{
    logFile  = new QFile(fileName);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
    reloadFile();
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
