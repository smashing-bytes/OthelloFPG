#ifndef SCOREWIDGET_H
#define SCOREWIDGET_H

#include <QWidget>

class QGridLayout;
class QLCDNumber;
class QLabel;
class QString;

class ScoreWidget : public QWidget {
    Q_OBJECT

public:
    ScoreWidget(QWidget *parent = 0);
    void startTotalTimer(int msec);
    void stopTotalTimer();
    void startTurnTimer(int msec);
    void stopTurnTimer();
    void clearAll();
    void setPoints(short points);
    void addPoints(short points);
    void subPoints(short points);
    QString& getTotalTime();
    QString& getTurnTime();
    void setTotalTime(QString& strTime);
    void setTurnTime(QString& strTime);

private slots:
    void showTotalTime();

private:
    QGridLayout *gridLayout;
    QLCDNumber *totalTimeLCD,*timeLCD, *scoreLCD;
    QLabel *label1,*label2,*label3;
    char timer[3];
    char turnTimerArray[3];
    short score;
    QTimer *totalTimer;
    QTimer *turnTimer;
    QString strTotalTime;
    QString strTurnTime;
};

#endif // SCOREWIDGET_H
