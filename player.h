#ifndef PLAYER_H
#define PLAYER_H

#include <QString>

class ScoreWidget;


class Player {

public:

    /* piece = (isHuman == true ? BLACK : WHITE) */
    Player(bool isHuman, const QString &name);

    void setName(const QString &name);
    QString getName() const;

    void startTimers();
    void stopTimers();
    void incrScore();
    void decrScore();
    void incrScore(short n);
    void decrScore(short n);
    void setScoreWidget(ScoreWidget *widget);
    void setScore(short n);
    void clearAll();
    short getScore();
    QString& getTotalTime();
    QString& getTurnTime();
    void setTotalTime(QString &totalTime);
    void setTurnTime(QString &turnTime);

private:
    ScoreWidget *scorewidget;
    QString playerName;
    bool isBlack;
    short score;
};

#endif // PLAYER_H
