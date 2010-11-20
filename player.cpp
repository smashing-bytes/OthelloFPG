#include "player.h"
#include "scorewidget.h"

Player::Player(bool isHuman, const QString &name) {
    isBlack = isHuman;
    playerName = name;
    score = 2;
}

void Player::setScoreWidget(ScoreWidget *widget) {
    scorewidget = widget;
}


void Player::startTimers() {
    scorewidget->startTotalTimer(1000);
    scorewidget->startTurnTimer(1000);
}

void Player::stopTimers() {
    scorewidget->stopTotalTimer();
    scorewidget->stopTurnTimer();
}

void Player::setName(const QString &name) {
    playerName = name;
}

QString Player::getName() const{
    return playerName;
}

void Player::setScore(short n) {
    scorewidget->setPoints(n);
    score = n;
}

void Player::incrScore(short n) {
    scorewidget->addPoints(n);
    score += n;
}

void Player::incrScore() {
    incrScore(1);
}

void Player::decrScore(short n) {
    scorewidget->subPoints(n);
    score -= n;
}

void Player::decrScore() {
    decrScore(1);
}

void Player::clearAll() {
   scorewidget->clearAll();
}

short Player::getScore() {
    return score;
}

QString& Player::getTotalTime() {
    return scorewidget->getTotalTime();
}

QString& Player::getTurnTime() {
    return scorewidget->getTurnTime();
}

void Player::setTotalTime(QString &totalTime) {
    scorewidget->setTotalTime(totalTime);
}

void Player::setTurnTime(QString &turnTime) {
    scorewidget->setTurnTime(turnTime);
}
