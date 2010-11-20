#include "boardwidget.h"
#include "othello.h"
#include "player.h"
#include <QButtonGroup>
#include <QApplication>
#include <QFont>
#include <QPushButton>
#include <QWidget>
#include <QList>
#include <QString>
#include <QFile>
#include <QMessageBox>

#include <QDebug>

BoardWidget::BoardWidget(Player *player1, Player *player2, QWidget *parent)
    :QWidget(parent)
{
    short i,j;
    unsigned int xloc = 0, yloc = 0;
    p1 = player1;
    p2 = player2;
    othello = new Othello(player1, player2, this,true);

    greenIcon = new QIcon(":/images/green.png");
    blackIcon = new QIcon(":/images/black.png");
    whiteIcon = new QIcon(":/images/white.png");
    validgreenIcon = new QIcon(":/images/validgreen.png");

    this->resize(B_WIDTH*8, B_HEIGHT*8);

    buttongroup = new QButtonGroup();
    QString name;

    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < 8; j++)
        {

            QPushButton *button = new QPushButton(this);

            button->setIcon(*greenIcon);
            button->setIconSize(QSize(B_WIDTH,B_WIDTH));

            name = QString::number(i) + QString::number(j);
            button->setObjectName(name+"g");
            button->setGeometry(xloc, yloc, B_WIDTH, B_WIDTH);
            buttongroup->addButton(button);

            btnArray[i][j] = button;
            QObject::connect(button, SIGNAL(clicked()), this, SLOT(clickHandler()));
            xloc += B_WIDTH;

        }
        yloc += B_WIDTH;
        xloc = 0;

    }
    othello->initializeTableStateAndAllowedMoves();
    btnArray[3][4]->setIcon(*blackIcon);
    btnArray[3][3]->setIcon(*whiteIcon);
    btnArray[4][3]->setIcon(*blackIcon);
    btnArray[4][4]->setIcon(*whiteIcon);

    btnArray[3][4]->setObjectName("34b");
    btnArray[3][3]->setObjectName("33w");
    btnArray[4][3]->setObjectName("43b");
    btnArray[4][4]->setObjectName("44w");

}

void BoardWidget::clickHandler()
{

    QString objName =  QObject::sender()->objectName();
    if (btnArray[objName.at(0).digitValue()][objName.at(1).digitValue()]->objectName().endsWith("b") || btnArray[objName.at(0).digitValue()][objName.at(1).digitValue()]->objectName().endsWith("w"))
        return;
    if (btnArray[objName.at(0).digitValue()][objName.at(1).digitValue()]->objectName().endsWith("g"))
        return;

    Player *p; //What is that doing here?

    qDebug() << "OK";

    if (othello->allowed(objName.at(0).digitValue(), objName.at(1).digitValue())) {qDebug() << "OK";
        p = othello->updateBoardTableStateAllowedMoves(objName.at(0).digitValue(), objName.at(1).digitValue()); }

    qDebug() << "OK";

    if (p == NULL) {
        p1->stopTimers();
        p2->stopTimers();
        emit gameOver();
    } else
        emit btnClicked(p->getName() + "'s turn");
}

void BoardWidget::setIconAt(int color, const int i, const int j) {
    QString objName = btnArray[i][j]->objectName();

    if (color == WHITE) {
        btnArray[i][j]->setIcon(*whiteIcon);
        objName.replace(objName.length()-1,1,"w");
        qDebug() << objName;
        btnArray[i][j]->setObjectName(objName);
    } else if (color == GREEN) {
        btnArray[i][j]->setIcon(*greenIcon);
        objName.replace(objName.length()-1,1,"g");
        btnArray[i][j]->setObjectName(objName);
    } else if (color == BLACK){
        btnArray[i][j]->setIcon(*blackIcon);
        objName.replace(objName.length()-1,1,"b");
        btnArray[i][j]->setObjectName(objName);
    } else {
        btnArray[i][j]->setIcon(*validgreenIcon);
    }
}

void BoardWidget::clearBoard() {
    int i,j;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            btnArray[i][j]->setIcon(*greenIcon);
            btnArray[i][j]->setObjectName(QString::number(i) + QString::number(j) + "g");
        }
    }
    btnArray[3][4]->setIcon(*blackIcon);
    btnArray[3][3]->setIcon(*whiteIcon);
    btnArray[4][3]->setIcon(*blackIcon);
    btnArray[4][4]->setIcon(*whiteIcon);
    btnArray[3][4]->setObjectName("34b");
    btnArray[3][3]->setObjectName("33w");
    btnArray[4][3]->setObjectName("43b");
    btnArray[4][4]->setObjectName("44w");
    othello->initializeTableStateAndAllowedMoves();
}

bool BoardWidget::writeFileAs(const QString &fileName) {
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Othello FPG", tr("Cannot write file %1:\n%2.").arg(file.fileName()).arg(file.errorString()));
        return false;
    }

    QDataStream out(&file);
   // out.setVersion(QDataStream::Qt_4_7);
    out.setVersion(QDataStream::Qt_4_6);
    out << quint32(MAGIC_NUMBER);
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QString tableState = *(othello->getTableState());
    QString allowedMoves = *( othello->getAllowedMoves());
    QString totalTime, turnTime, score;

    out << quint16(tableState.length()) << tableState;
    out << quint16(allowedMoves.length()) << allowedMoves;

    totalTime = p1->getTotalTime();
    turnTime = p1->getTurnTime();
    score = QString::number(p1->getScore());

    out << quint8(1) << totalTime << quint8(1) << turnTime << quint8(1) << score;

    totalTime = p2->getTotalTime();
    turnTime = p2->getTurnTime();
    score = QString::number(p2->getScore());

    out << quint8(2) << totalTime << quint8(2) << turnTime << quint8(2) << score;
    out << quint32(othello->getNextTurn());

    QApplication::restoreOverrideCursor();

    return true;
}

bool BoardWidget::readFile(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Othello FPG", tr("Cannot read file %1:\n%2.").arg(file.fileName()).arg(file.errorString()));
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_6);

    quint32 magic;

    in >> magic;
    if (magic != MAGIC_NUMBER) {
        QMessageBox::warning(this, "Othello FPG", "The file is not an Othello-FPG file.");
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    quint32 t;
    quint16 l1,l2;
    quint8 pt1;
    QString tableState, allowedState, totalTime, turnTime, score;

    in >> l1 >> tableState;
    in >> l2 >> allowedState;

    othello->setTableState(tableState);
    othello->setAllowedMoves(allowedState);

    in >> pt1 >> totalTime;
    in >> pt1 >> turnTime;
    in >> pt1 >> score;

    p1->setTotalTime(totalTime);
    p1->setTurnTime(turnTime);
    p1->setScore(score.toShort());

    in >> pt1 >> totalTime;
    in >> pt1 >> turnTime;
    in >> pt1 >> score;

    p2->setTotalTime(totalTime);
    p2->setTurnTime(turnTime);
    p2->setScore(score.toShort());

    in >> t;

    othello->setNextTurn(t);

    if (t == 1) {
        emit btnClicked(p1->getName() + "'s turn");
    } else
        emit btnClicked(p2->getName() + "'s turn");

    QApplication::restoreOverrideCursor();
    return true;
}

void BoardWidget::setButtonName(const QString &name, const int i, const int j) {
    btnArray[i][j]->setObjectName(name);
}

void BoardWidget::setIcons(QIcon *icon1, QIcon *icon2, bool isFirst) {
    QString objName;

    if (isFirst) {
        blackIcon = icon1;
        whiteIcon = icon2;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                objName = btnArray[i][j]->objectName();
                if (objName.endsWith("b"))
                    btnArray[i][j]->setIcon(*icon1);
                else if (objName.endsWith("w"))
                    btnArray[i][j]->setIcon(*icon2);
            }
        }
    } else {
        blackIcon = icon2;
        whiteIcon = icon1;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                objName = btnArray[i][j]->objectName();
                if (objName.endsWith("b"))
                    btnArray[i][j]->setIcon(*icon2);
                else if (objName.endsWith("w"))
                    btnArray[i][j]->setIcon(*icon1);
            }
        }
    }
}
