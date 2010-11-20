#ifndef HISTORYDIALOG_H
#define HISTORYDIALOG_H

#include <QDialog>
#include <QWidget>

class QString;
class QTableWidget;
class QPushButton;
class MainWindow;

class HistoryDialog : public QDialog {
    Q_OBJECT

public :
        HistoryDialog(const QString &playerName,MainWindow *window, QWidget *parent = 0);

private:
    void loadScores();

    QTableWidget *tablewidget;
    QPushButton *closeButton;
    MainWindow *mainWindow;
};


#endif // HISTORYDIALOG_H
