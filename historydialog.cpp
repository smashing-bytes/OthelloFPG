#include "historydialog.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QString>
#include <QPushButton>
#include "mainwindow.h"

#include <QDebug>

HistoryDialog::HistoryDialog(const QString &playerName, MainWindow *window, QWidget *parent) :QDialog(parent) {
    resize(340, 229);

    mainWindow = window;

    tablewidget = new QTableWidget(this);
    tablewidget->setColumnCount(3);

    QTableWidgetItem *hItem0 = new QTableWidgetItem();
    hItem0->setText("Date");
    tablewidget->setHorizontalHeaderItem(0, hItem0);

    QTableWidgetItem *hItem1 = new QTableWidgetItem();
    hItem1->setText(playerName);
    tablewidget->setHorizontalHeaderItem(1,hItem1);

    QTableWidgetItem *hItem2 = new QTableWidgetItem();
    hItem2->setText("Computer");
    tablewidget->setHorizontalHeaderItem(2,hItem2);

    tablewidget->setGeometry(QRect(10, 10, 311, 188));

    closeButton = new QPushButton("Close", this);
    closeButton->setGeometry(QRect(130, 200, 77, 27));

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    setWindowTitle("Score History");

    loadScores();
}

void HistoryDialog::loadScores() {

    QStringList list = mainWindow->getScoreList();
    int size = list.size();

    if (size == 0)
        return;

    int i = 0, index;
    QTableWidgetItem *row;
    QTableWidgetItem *cell0,*cell1,*cell2;

    tablewidget->setRowCount(size);


    foreach (QString listItem , list) {
        row = new QTableWidgetItem();
        tablewidget->setVerticalHeaderItem(i, row);

        cell0 = new QTableWidgetItem();
        cell1 = new QTableWidgetItem();
        cell2 = new QTableWidgetItem();

        index = listItem.indexOf("|");
        cell0->setText(listItem.left(index));

        listItem = listItem.mid(index).section("|",1);
        index = listItem.indexOf(":");
        cell1->setText(listItem.left(index));
        cell2->setText(listItem.section(":",1));

        tablewidget->setItem(i,0,cell0);
        tablewidget->setItem(i,1,cell1);
        tablewidget->setItem(i,2,cell2);

        i++;
    }
}
