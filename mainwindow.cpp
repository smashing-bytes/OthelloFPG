#include "mainwindow.h"
#include "historydialog.h"
#include "optionsdialog.h"
#include <QMenu>
#include <QToolBar>
#include <QMenuBar>
#include <QLabel>
#include <QLCDNumber>
#include <QDebug>
#include <QAction>
#include <QStatusBar>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QBoxLayout>
#include <QDockWidget>
#include <QPushButton>
#include <QSettings>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFileDialog>
#include <QStringList>
#include <QDateTime>
#include <QPlastiqueStyle>
#include <QApplication>

MainWindow::MainWindow() {
    createGui();
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    readSettings();

    isSaved = false;
    p1->startTimers();
    updateStatusBar(p1->getName() + "'s turn");

    QApplication::setStyle(new QPlastiqueStyle);
}


void MainWindow::createGui() {

    p1 = new Player(true, "Player1");
    p2 = new Player(false, "Player2");

    boardwidget = new BoardWidget(p1,p2,this);
    setCentralWidget(boardwidget);

    scoreWidgetP1 = new ScoreWidget(this);
    scoreWidgetP2 = new ScoreWidget(this);

    p1->setScoreWidget(scoreWidgetP1);
    p2->setScoreWidget(scoreWidgetP2);

    dockWidgetP1 = new QDockWidget("Player1", this);
    dockWidgetP1->setAllowedAreas(Qt::BottomDockWidgetArea);
    dockWidgetP1->setWidget(scoreWidgetP1);
    dockWidgetP1->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::BottomDockWidgetArea, dockWidgetP1);

    dockWidgetP2 = new QDockWidget("Player2",this);
    dockWidgetP2->setAllowedAreas(Qt::BottomDockWidgetArea);
    dockWidgetP2->setWidget(scoreWidgetP2);
    dockWidgetP2->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::BottomDockWidgetArea, dockWidgetP2);

    setFixedSize(B_WIDTH * 8,695);
}

void MainWindow::createActions() {
    newAction = new QAction("&New Game", this);
    newAction->setIcon(QIcon(":/images/new-icon.png"));
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, SIGNAL(triggered()), this, SLOT(newGame()));

    loadAction = new QAction("&Load Game...", this);
    loadAction->setIcon(QIcon(":/images/open-icon.png"));
    loadAction->setShortcut(tr("Ctrl+L"));
    connect(loadAction, SIGNAL(triggered()), this,SLOT(loadGame()));

    saveAction = new QAction("&Save Game As...", this);
    saveAction->setIcon(QIcon(":/images/save-icon.png"));
    saveAction->setShortcut(QKeySequence::SaveAs);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveGame()));

    exitAction = new QAction("E&xit", this);
    exitAction->setIcon(QIcon(":/images/exit-icon.png"));
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(exitGame()));

    optionsAction = new QAction("&Options...", this);
    optionsAction->setIcon(QIcon(":/images/options-icon.png"));
    optionsAction->setShortcut(tr("Ctrl+O"));
    connect(optionsAction, SIGNAL(triggered()), this, SLOT(options()));

    scoreHistoryAction = new QAction("Score &History...",this);
    scoreHistoryAction->setIcon(QIcon(":/images/history-icon.png"));
    scoreHistoryAction->setShortcut(tr("Ctrl+H"));
    connect(scoreHistoryAction, SIGNAL(triggered()), this, SLOT(scoreHistory()));

    contextHelpAction = new QAction("Context Help...", this);
    contextHelpAction->setIcon(QIcon(":/images/help-icon.png"));
    contextHelpAction->setShortcut(tr("F1"));
    connect(contextHelpAction, SIGNAL(triggered()), this, SLOT(helpContents()));

    aboutAction = new QAction("About Us...", this);
    aboutAction->setIcon(QIcon(":/images/about-icon.png"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus() {
    gameMenu = menuBar()->addMenu("&Game");
    gameMenu->addAction(newAction);
    gameMenu->addAction(loadAction);
    gameMenu->addAction(saveAction);
    gameMenu->addSeparator();
    gameMenu->addAction(exitAction);

    toolsMenu = menuBar()->addMenu("&Tools");
    toolsMenu->addAction(scoreHistoryAction);
    toolsMenu->addSeparator();
    toolsMenu->addAction(optionsAction);

    helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction(contextHelpAction);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAction);
}

void MainWindow::createToolBars() {
    gameToolBar = addToolBar(("&Game"));
    gameToolBar->addAction(newAction);
    gameToolBar->addAction(saveAction);
    gameToolBar->addAction(scoreHistoryAction);
    gameToolBar->addAction(exitAction);
    gameToolBar->setMovable(false);
}

void MainWindow::createStatusBar() {
    statusLabel = new QLabel;
    statusLabel->setIndent(3);
    statusBar()->addWidget(statusLabel,1);

    connect(boardwidget, SIGNAL(btnClicked(const QString &)), this, SLOT(updateStatusBar(const QString &)));
    connect(boardwidget, SIGNAL(gameOver()), this, SLOT(saveScore()));
}

void MainWindow::updateStatusBar(const QString &objName) {
    statusLabel->setText(objName);
}

void MainWindow::newGame() {
    p1->clearAll();
    p2->clearAll();
    boardwidget->clearBoard();
    p1->startTimers();
    isSaved = false;
}

void MainWindow::loadGame() {
    QString fileName = QFileDialog::getOpenFileName(this, "Othello FPG - Load Game", ".", "Othello FPG files (*.fpgoth)");
    if (!fileName.isEmpty()) {
        if (!boardwidget->readFile(fileName))
            statusLabel->setText("Loading canceled");
    }
}

bool MainWindow::saveGame() {
   QString fileName = QFileDialog::getSaveFileName(this, "Othello FPG - Save Game",  ".", "Othello FPG files (*.fpgoth)");
    if (fileName.isEmpty())
        return false;
    else {
        if (!fileName.endsWith(".fpgoth"))
            fileName.append(".fpgoth");
        isSaved = boardwidget->writeFileAs(fileName);
        if (!isSaved) {
            statusLabel->setText("Saving canceled");
            return false;
        } else {
            statusLabel->setText("Game saved");
            return true;
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (!isSaved) {
        int reply = QMessageBox::warning(this, "Othello FPG", "Do you want to save the game ?", QMessageBox::Yes | QMessageBox::No| QMessageBox::Cancel);
        if (reply == QMessageBox::Yes) {
            if (saveGame()) {
                writeSettings();
                event->accept();
            }
            else
                event->ignore();
        }
        else if (reply == QMessageBox::Cancel) {
           event->ignore();
           return;
       }
    }
    writeSettings();
    event->accept();

}

void MainWindow::exitGame() {

    if (!isSaved) {
        int reply = QMessageBox::warning(this, "Othello FPG", "Do you want to save the game ?", QMessageBox::Yes | QMessageBox::No| QMessageBox::Cancel);
        if (reply == QMessageBox::Yes)
            saveGame();
        else if (reply == QMessageBox::Cancel)
            return;
        else {
            isSaved = true;
            emit close();
        }
    } else {
        emit close();
    }
}

void MainWindow::options() {
   /* QString dateTime = QDateTime::currentDateTime().toString("M/d/yy hh:mm:ss");

    qDebug() << dateTime;*/
    OptionsDialog *dialog = new OptionsDialog(this,this);
    dialog->show();
}

void MainWindow::setIcons(QIcon *icon1, QIcon *icon2) {
    boardwidget->setIcons(icon1,icon2, true);
}

void MainWindow::scoreHistory() {
    HistoryDialog *dialog = new HistoryDialog(p1->getName(),this, boardwidget);
    dialog->show();
}

void MainWindow::helpContents() {

}

void MainWindow::about() {

}

void MainWindow::writeSettings() {
    QSettings settings("FPG Inc.", "Othello");

    if (list.size() != 0)
        settings.setValue("player1/" + p1->getName() + "/score", list);
    else
        qDebug() << "Empty";
}

void MainWindow::readSettings() {
    QSettings settings("FPG Inc.", "Othello");

    list = settings.value("player1/" + p1->getName() + "/score").toStringList();

}

void MainWindow::saveScore() {
    QString score = QString::number(p1->getScore()) + ":" + QString::number(p2->getScore());
    QString dateTime = QDateTime::currentDateTime().toString("M/d/yy hh:mm:ss");

    list.append(dateTime + "|" + score);

}


QStringList& MainWindow::getScoreList() {
    return list;
}
