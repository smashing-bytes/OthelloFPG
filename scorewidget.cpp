#include "scorewidget.h"
#include <QGridLayout>
#include <QLabel>
#include <QLCDNumber>
#include <QTimer>
#include <QTime>

ScoreWidget::ScoreWidget(QWidget *parent)
    :QWidget(parent)
{
    gridLayout = new QGridLayout(this);

    totalTimeLCD = new QLCDNumber(8,this);
    timeLCD = new QLCDNumber(8,this);
    scoreLCD = new QLCDNumber(3,this);

    scoreLCD->setSegmentStyle(QLCDNumber::Flat);
    score = 2;
    scoreLCD->display(score);

    strTurnTime = strTotalTime = "00:00:00";

    totalTimer = new QTimer(totalTimeLCD);
    totalTimer->setObjectName("totalTimer");
    connect(totalTimer, SIGNAL(timeout()), this, SLOT(showTotalTime()));
    totalTimeLCD->setSegmentStyle(QLCDNumber::Flat);
    totalTimeLCD->display("00:00:00");

    turnTimer = new QTimer(timeLCD);
    turnTimer->setObjectName("turnTimer");
    connect(turnTimer, SIGNAL(timeout()), this, SLOT(showTotalTime()));
    timeLCD->setSegmentStyle(QLCDNumber::Flat);
    timeLCD->display("00:00:00");

    label1 = new QLabel(this);
    label1->setText("TIME:");
    label2 = new QLabel(this);
    label2->setText("TOTAL TIME:");
    label3 = new QLabel(this);
    label3->setText("SCORE:");


    gridLayout->addWidget(timeLCD,0,2,1,1);
    gridLayout->addWidget(totalTimeLCD,1,2,1,1);
    gridLayout->addWidget(scoreLCD,3,2,1,1 );
    gridLayout->addWidget(label2, 1,0,1,1);
    gridLayout->addWidget(label3,2,0,2,1);
    gridLayout->addWidget(label1, 0,0,1,1);
    gridLayout->setSizeConstraint(QLayout::SetNoConstraint);

};

void ScoreWidget::showTotalTime() {
    QString objS = QObject::sender()->objectName();
    char *ptimer;
    bool isTotalTimer = false;

    if (objS.compare("totalTimer") == 0) {
        ptimer = timer;
        isTotalTimer = true;
    }
    else if (objS.compare("turnTimer") == 0)
        ptimer = turnTimerArray;

    if (ptimer[0] == 59) {
        if (ptimer[1] == 59) {
            if (ptimer[2] == 59)
                ptimer[2] = 0;
            else
               ptimer[2] += 1;

            ptimer[1] = 0;
        } else
            ptimer[1] += 1;
        ptimer[0] = 0;
    } else
        ptimer[0] += 1;

    QString hours = QString::number(ptimer[2]);
    QString minutes = QString::number(ptimer[1]);
    QString seconds = QString::number(ptimer[0]);

    if (hours.length() == 1)
        hours.prepend("0");
    if (minutes.length() == 1)
        minutes.prepend("0");
    if (seconds.length() == 1)
        seconds.prepend("0");

    if (isTotalTimer) {
        strTotalTime = hours + ":" + minutes + ":" + seconds;
        totalTimeLCD->display(hours + ":" + minutes + ":" + seconds);
    }
    else {
        strTurnTime = hours + ":" + minutes + ":" + seconds;
        timeLCD->display(hours + ":" + minutes + ":" + seconds);
    }
}

void ScoreWidget::startTotalTimer(int msec) {
    totalTimer->start(msec);
}

void ScoreWidget::startTurnTimer(int msec) {
    turnTimer->start(msec);
}

void ScoreWidget::stopTotalTimer() {
    totalTimer->stop();
}

void ScoreWidget::stopTurnTimer() {
    turnTimer->stop();
    turnTimerArray[0] = turnTimerArray[1] = turnTimerArray[2] = 0;
}

void ScoreWidget::setPoints(short points) {
    score = points;
    scoreLCD->display(score);
}

void ScoreWidget::addPoints(short points) {
    score += points;
    scoreLCD->display(score);
}

void ScoreWidget::subPoints(short points) {
    score -= points;
    scoreLCD->display(score);
}

void ScoreWidget::clearAll() {
    stopTotalTimer();
    stopTurnTimer();
    setPoints(2);
    timer[0] = timer[1] = timer[2] = 0;
    strTurnTime = strTotalTime  = "00:00:00";

    totalTimeLCD->display("00:00:00");
    timeLCD->display("00:00:00");

}

QString& ScoreWidget::getTotalTime() {
    return strTotalTime;
}

QString& ScoreWidget::getTurnTime() {
    return strTurnTime;
}

void ScoreWidget::setTotalTime(QString &strTime) {
    strTotalTime = strTime;
    totalTimeLCD->display(strTime);
}

void ScoreWidget::setTurnTime(QString &strTime) {
    strTurnTime = strTime;
    timeLCD->display(strTime);
}
