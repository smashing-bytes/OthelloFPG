#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "boardwidget.h"
#include "scorewidget.h"
#include "player.h"

class QMenu;
class QToolBar;
class QAction;
class QLabel;
class QDockWidget;
class QIcon;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
    QStringList& getScoreList();
    void setIcons(QIcon *icon1, QIcon *icon2);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void updateStatusBar(const QString &objName);
    void newGame();
    void loadGame();
    bool saveGame();
    void saveScore();
    void exitGame();
    void options();
    void scoreHistory();
    void helpContents();
    void about();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createGui();
    void createStatusBar();
    void readSettings();
    void writeSettings();


    BoardWidget *boardwidget;
    ScoreWidget *scoreWidgetP1;
    ScoreWidget *scoreWidgetP2;
    QDockWidget *dockWidgetP1;
    QDockWidget *dockWidgetP2;
    QMenu *gameMenu;
    QMenu *toolsMenu;
    QMenu *helpMenu;

    QLabel *statusLabel;

    QToolBar *gameToolBar;

    QAction *newAction;
    QAction *loadAction;
    QAction *saveAction;
    QAction *exitAction;
    QAction *optionsAction;
    QAction *scoreHistoryAction;
    QAction *contextHelpAction;
    QAction *aboutAction;

    bool isSaved;
    Player *p1, *p2;
    QStringList list;
};

#endif // MAINWINDOW_H
