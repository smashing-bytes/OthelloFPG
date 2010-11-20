#include "othello.h"
#include "player.h"
#include "boardwidget.h"
#include <QMessageBox>
#include <QString>

#include <QDebug>

Othello::Othello(Player *player1, Player *player2,BoardWidget *board, bool turn) {
    p1 = player1;
    p2 = player2;
    boardwidget = board;
    isBlacksTurn = turn;
}

int Othello::getNextTurn() {
    if (isBlacksTurn)
        return 1;
    else
        return 2;
}

void Othello::setNextTurn(int t) {
    if (t == 1)
        isBlacksTurn = true;
    else
        isBlacksTurn = false;
}

void Othello::setTableState(QString &state) {
    QChar *data = state.data();
    int i = 0;
    int j = 0;

    while (!data->isNull()) {

        if (data->toAscii() == '0') {
            tableState[i][j] = 0;
            boardwidget->setIconAt(GREEN,i,j);
            boardwidget->setButtonName(QString::number(i)+QString::number(j)+"g",i,j);
        }
        else if (data->toAscii() == '1') {
            tableState[i][j] = 1;
            boardwidget->setIconAt(BLACK,i,j);
            boardwidget->setButtonName(QString::number(i)+QString::number(j)+"b",i,j);
        }
        else if (data->toAscii() == '2') {
            tableState[i][j] = 2;
            boardwidget->setIconAt(WHITE,i,j);
            boardwidget->setButtonName(QString::number(i)+QString::number(j)+"w",i,j);
        }

        if (j == 7) {
            i++;
            j = 0;
        } else
            j++;
        ++data;
    }
}

void Othello::setAllowedMoves(QString &moves) {
    QChar *data = moves.data();
    int i = 0;
    int j = 0;

    while (!data->isNull()) {
        if (data->toAscii() == '1') {
            boardwidget->setIconAt(VALIDGREEN,i,j);
            boardwidget->setButtonName(QString::number(i)+QString::number(j)+"v",i,j);
            allowedMoves[i][j] = true;
        }
        else
            allowedMoves[i][j] = false;

        if (j == 7) {
            i++;
            j = 0;
        } else
            j++;
        ++data;
    }
}

QString* Othello::getAllowedMoves() {
    QString *strAMoves = new QString();

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (allowedMoves[i][j])
                strAMoves->append("1");
            else
                strAMoves->append("0");
        }
    }

    return strAMoves;
}

QString* Othello::getTableState() {
    QString *strState = new QString();

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            strState->append(QString::number(tableState[i][j]) );
        }
    }

    return strState;
}

/**
  * initialiazes the tableState array and the allowedMoves array.
  * in the tableState array, empty/green = 0, black = 1, white = 2
  * in the allowedMoves array, not legitimate move = false, legitimate move = true
  */
void Othello::initializeTableStateAndAllowedMoves()
{

    tableState = new int*[8];
    for (int i = 0; i < 8; i++)
        tableState[i] = new int[8];

    allowedMoves = new bool*[8];
       for (int i = 0; i < 8; i++)
           allowedMoves[i] = new bool[8];

    //set everything to empty and not legitimate
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            tableState[i][j] = 0;
            allowedMoves[i][j] = false;
        }
    }

    //initialize the tableState
    tableState[3][4] = 1;
    tableState[4][3] = 1;
    tableState[3][3] = 2;
    tableState[4][4] = 2;

    //initialize the allowedMoves
    allowedMoves[3][2] = true;
    allowedMoves[2][3] = true;
    allowedMoves[5][4] = true;
    allowedMoves[4][5] = true;

    boardwidget->setIconAt(VALIDGREEN, 3, 2);
    boardwidget->setIconAt(VALIDGREEN, 2, 3);
    boardwidget->setIconAt(VALIDGREEN, 5, 4);
    boardwidget->setIconAt(VALIDGREEN, 4, 5);

    boardwidget->setButtonName("32v",3,2);
    boardwidget->setButtonName("23v",2,3);
    boardwidget->setButtonName("54v",5,4);
    boardwidget->setButtonName("45v",4,5);

}

bool Othello::allowed(const int i, const int j) {
    return allowed(allowedMoves,i,j);
}

/**
  * returns true if the move is legitimate else returns false
  */
bool Othello::allowed(bool **allowed, const int i, const int j)
{
    return allowed[i][j];
}

Player* Othello::updateBoardTableStateAllowedMoves(const int i, const int j) {
    return updateBoardTableStateAllowedMoves(tableState,allowedMoves,i,j);
}

Player* Othello::updateBoardTableStateAllowedMoves(int **state,bool **allowed,const int i, const int j)
{
    updateBoardTableState(state,i,j);
    isBlacksTurn = !isBlacksTurn;
    short totalScore = p1->getScore() + p2->getScore();
    if ( !updateAllowedMoves(allowed,state) && totalScore != 64) {
        if (isBlacksTurn)
            QMessageBox::warning(boardwidget, "Othello FPG", "It is not allowed to make a move :(", QMessageBox::Ok);
        isBlacksTurn = !isBlacksTurn;
        return updateBoardTableStateAllowedMoves(state,allowed,i,j);
    } else if (totalScore == 64) {
        if (p1->getScore() == 32) {
            QMessageBox::information(boardwidget, "Othello FPG", "Tie", QMessageBox::Ok);
            return NULL;
        }
        QString winner = p1->getScore() > p2->getScore() ? p1->getName() : p2->getName();
        QMessageBox::information(boardwidget, "Othello FPG", "The winner is " + winner, QMessageBox::Ok);
        return NULL;
    }
    qDebug() << "Oth OK3";
    if (isBlacksTurn) {
        p2->stopTimers();
        p1->startTimers();
        return p1;
    } else {
        p1->stopTimers();
        p2->startTimers();
        return p2;
    }
}

/**
  * updates the board and the tableState array, with the move at (i,j)
  */
void Othello::updateBoardTableState(int** state,const int i, const int j)
{
    int** series = getSeries(state,i, j);
    updateTableState(state,i, j, series);

    for (int i = 0; i < seriesNumber; i++) {
            delete[] series[i];
        }
    delete[] series;
}

/**
  * return a pointer to an array[seriesNumber][2], with every row being a series of pawns to be changed between the row and (i,j)
  */
int** Othello::getSeries(int **state,const int i, const int j)
{
    seriesNumber = 0;
    int upperLeft = checkUpperLeft(state,i, j);
    if (upperLeft != -1)
        seriesNumber++;
    int lowerLeft = checkLowerLeft(state,i, j);
    if (lowerLeft != -1)
        seriesNumber++;
    int left = checkLeft(state,i, j);
    if (left != -1)
        seriesNumber++;
    int upperRight = checkUpperRight(state,i, j);
    if (upperRight != -1)
        seriesNumber++;
    int lowerRight = checkLowerRight(state,i, j);
    if (lowerRight != -1)
        seriesNumber++;
    int right = checkRight(state,i, j);
    if (right != -1)
        seriesNumber++;
    int top = checkTop(state,i, j);
    if (top != -1)
        seriesNumber++;
    int bottom = checkBottom(state,i, j);
    if (bottom != -1)
        seriesNumber++;

    int** series = new int*[seriesNumber];
    for (int k = 0; k < seriesNumber; k++)
        series[k] = new int[2];

    int k = 0;
    //set the coordinates based on the results of the aforesaid methods
    if (upperLeft != -1)
    {
        series[k][0] = upperLeft;
        series[k][1] = j - (i - upperLeft);
        k++;
    }

    if (lowerLeft != -1)
    {
        series[k][0] = lowerLeft;
        series[k][1] = j - (lowerLeft - i);
        k++;
    }

    if (left != -1)
    {
        series[k][0] = i;
        series[k][1] = left;
        k++;
    }

    if (upperRight != -1)
    {
        series[k][0] = upperRight;
        series[k][1] = j + (i - upperRight);
        k++;
    }

    if (lowerRight != -1)
    {
        series[k][0] = lowerRight;
        series[k][1] = j + (lowerRight - i);
        k++;
    }

    if (right != -1)
    {
        series[k][0] = i;
        series[k][1] = right;
        k++;
    }

    if (top != -1)
    {
        series[k][0] = top;
        series[k][1] = j;
        k++;
    }

    if (bottom != -1)
    {
        series[k][0] = bottom;
        series[k][1] = j;
    }

    //return the series
    return series;
}

/**
  * uses the entries in the series array and changes the tableState and the board icons for these series
  */
void Othello::updateTableState(int** state,const int i, const int j, int** series)
{


    short tBlack = 0;
    short tWhite = 0;
    for (int k = 0; k < seriesNumber; k++)
    {
        int c;
        if (series[k][0] < i && series[k][1] < j) //upperLeft
        {
            c = j;
            for (int r = i; r > series[k][0]; r--)
            {
                if (isBlacksTurn) {
                    if (state[r][c] != 1)
                        tBlack++;
                    if (state[r][c] == 2)
                        tWhite--;
                    state[r][c] = 1;
                }
                else {
                    if (state[r][c] != 2)
                        tWhite++;
                    if (state[r][c] == 1)
                        tBlack--;
                    state[r][c] = 2;
                }

            changeIcon(r,c);
                c--;
            }
        }
        else if (series[k][0] > i && series[k][1] < j) //lowerLeft
        {
            c = j;
            for (int r = i; r < series[k][0]; r++)
            {
                if (isBlacksTurn) {
                    if (state[r][c] != 1)
                        tBlack++;
                    if (state[r][c] == 2)
                        tWhite--;
                    state[r][c] = 1;
                }
                else {
                    if (state[r][c] != 2)
                        tWhite++;
                     if (state[r][c] == 1)
                        tBlack--;
                   state[r][c] = 2;
                }

                changeIcon(r,c);
                c--;
            }
        }
        else if (series[k][0] == i && series[k][1] < j) //left
        {
            for (c = j; c > series[k][1]; c--)
            {
                if (isBlacksTurn) {
                    if (state[i][c] != 1)
                        tBlack++;
                    if (state[i][c] == 2)
                        tWhite--;
                    state[i][c] = 1;
                }
                else {
                    if (state[i][c] != 2)
                        tWhite++;
                   if (state[i][c] == 1)
                        tBlack--;
                    state[i][c] = 2;
                }

               changeIcon(i,c);
            }
        }
        else if (series[k][0] == i && series[k][1] > j) //right
        {
            for (int c = j; c < series[k][1]; c++)
            {
                if (isBlacksTurn) {
                    if (state[i][c] != 1)
                        tBlack++;
                    if (state[i][c] == 2)
                        tWhite--;
                    state[i][c] = 1;
                }
                else {
                    if (state[i][c] != 2)
                        tWhite++;
                     if (state[i][c] == 1)
                        tBlack--;
                    state[i][c] = 2;
                }

                changeIcon(i,c);
            }
        }
        else if (series[k][0] < i && series[k][1] > j) //upperRight
        {
            c = j;
            for (int r = i; r > series[k][0]; r--)
            {
                if (isBlacksTurn) {
                    if (state[r][c] != 1)
                        tBlack++;
                   if (state[r][c] == 2)
                        tWhite--;
                    state[r][c] = 1;
                }
                else {
                    if (state[r][c] != 2)
                        tWhite++;
                   if (state[r][c] == 1)
                        tBlack--;
                    state[r][c] = 2;
                }
                changeIcon(r,c);
                c++;
            }
        }
        else if (series[k][0] > i && series[k][1] > j) //lowerRight
        {
            c = j;
            for (int r = i; r < series[k][0]; r++)
            {
                if (isBlacksTurn) {
                    if (state[r][c] != 1)
                        tBlack++;
                   if (state[r][c] == 2)
                        tWhite--;
                    state[r][c] = 1;
                } else {
                    if (state[r][c] != 2)
                        tWhite++;
                 if (state[r][c] == 1)
                        tBlack--;
                    state[r][c] = 2;
                }
                changeIcon(r,c);
                c++;
            }
        }
        else if (series[k][0] < i && series[k][1] ==j) //top
        {
            for (int r = i; r > series[k][0]; r--)
            {
                if (isBlacksTurn) {
                    if (state[r][j] != 1)
                        tBlack++;
                     if (state[r][j] == 2)
                        tWhite--;
                   state[r][j] = 1;
                }
                else {
                    if (state[r][j] != 2)
                        tWhite++;
                    if (state[r][j] == 1)
                        tBlack--;
                    state[r][j] = 2;
                }
              changeIcon(r,j);
            }
        }
        else //bottom
        {
            for (int r = i; r < series[k][0]; r++)
            {
                if (isBlacksTurn) {
                    if (state[r][j] != 1)
                        tBlack++;
                   if (state[r][j] == 2)
                        tWhite--;
                    state[r][j] = 1;
                }
                else {
                    if (state[r][j] != 2)
                        tWhite++;
                     if (state[r][j] == 1)
                        tBlack--;
                    state[r][j] = 2;
                }
                changeIcon(r,j);
            }
        }
    }

    p1->incrScore(tBlack);
    p2->incrScore(tWhite);
}
/**
  * changes the icon at the (i, j) button
  */
void Othello::changeIcon(const int i, const int j)
{
    if (isBlacksTurn)
    {
        boardwidget->setIconAt(BLACK,i,j);
    }
    else
    {
        boardwidget->setIconAt(WHITE,i,j);
    }

}

bool Othello::updateAllowedMoves(bool** allowed, int** state)
{
    bool allow = false;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (state[i][j] != 0) {
                allowed[i][j] = false;
            }
            else if ((checkUpperLeft(state,i,j) != -1) || (checkUpperRight(state,i,j) != -1) || (checkLowerLeft(state,i,j) != -1) ||
                     (checkLowerRight(state,i,j) != -1) || (checkLeft(state,i,j) != -1) || (checkRight(state,i,j) != -1) || (checkTop(state,i,j) != -1) || (checkBottom(state,i,j) != -1)) {
                allowed[i][j] = true;
                boardwidget->setIconAt(VALIDGREEN, i, j);
                boardwidget->setButtonName(QString::number(i) + QString::number(j) + "v", i, j);
                allow = true;
            }
            else {qDebug() << "3f true";
                allowed[i][j] = false;
                boardwidget->setIconAt(GREEN, i, j);
                boardwidget->setButtonName(QString::number(i) + QString::number(j) + "g", i, j);
            }
        }
    }
    return allow;
}

int Othello::checkUpperLeft(int **state,const int i, const int j)
{
    if (i != 0 && j !=0) //if i || j == 0 then there's no upper left spot left
       {
           if (isBlacksTurn)
           {
               if (state[i-1][j-1] != 2)//no series, since there's no different pawn
                   return -1;
               else
               {
                   int r = i-1, c = j-1;
                   while (r > 0 && c > 0)
                   {
                       r--;
                       c--;
                       if (state[r][c] == 1)
                           return r;
                       else if (state[r][c] == 0) //if empty, return -1
                           return -1;
                   }
               }
           }
           else
           {
               if (state[i-1][j-1] != 1)//no series, since there's no different pawn
                   return -1;
               else
               {
                   int r = i-1, c = j-1;
                   while (r > 0 && c > 0)
                   {
                       r--;
                       c--;
                       if (state[r][c] == 2)
                           return r;
                       else if (state[r][c] == 0) //if empty, return -1
                           return -1;
                   }
               }
           }
       }

       return -1;
}

int Othello::checkUpperRight(int **state,const int i, const int j)
{
    if (i != 0 && j !=7) //if i || j == 0 then there's no upper right spot left
        {
            if (isBlacksTurn)
            {
                if (state[i-1][j+1] != 2)//no series, since there's no different pawn
                    return -1;
                else
                {
                    int r = i-1, c = j+1;
                    while (r > 0 && c < 7)
                    {
                        r--;
                        c++;
                        if (state[r][c] == 1)
                            return r;
                        else if (state[r][c] == 0) //if empty, return -1
                            return -1;
                    }
                }
            }
            else
            {
                if (state[i-1][j+1] != 1)//no series, since there's no different pawn
                    return -1;
                else
                {
                    int r = i-1, c = j+1;
                    while (r > 0 && c < 7)
                    {
                        r--;
                        c++;
                        if (state[r][c] == 2)
                            return r;
                        else if (state[r][c] == 0) //if empty, return -1
                            return -1;
                    }
                }
            }
        }
        return -1;
}

int Othello::checkLowerLeft(int **state,const int i, const int j)
{
    if (i != 7 && j !=0) //if i || j == 0 then there's no lower left spot left
        {
            if (isBlacksTurn)
            {
                if (state[i+1][j-1] != 2)//no series, since there's no different pawn
                    return -1;
                else
                {
                    int r = i+1, c = j-1;
                    while (r < 7 && c > 0)
                    {
                        r++;
                        c--;
                        if (state[r][c] == 1)
                            return r;
                        else if (state[r][c] == 0) //if empty, return -1
                            return -1;
                    }
                }
            }
            else
            {
                if (state[i+1][j-1] != 1)//no series, since there's no different pawn
                    return -1;
                else
                {
                    int r = i+1, c = j-1;
                    while (r < 7 && c > 0)
                    {
                        r++;
                        c--;
                        if (state[r][c] == 2)
                            return r;
                        else if (state[r][c] == 0) //if empty, return -1
                            return -1;
                    }
                }
            }
        }

        return -1;
}

int Othello::checkLowerRight(int **state,const int i, const int j)
{

    if (i != 7 && j !=7) //if i || j == 0 then there's no upper right spot left
        {
            if (isBlacksTurn)
            {
                if (state[i+1][j+1] != 2)//no series, since there's no different pawn
                    return -1;
                else
                {
                    int r = i+1, c = j+1;
                    while (r < 7 && c < 7)
                    {
                        r++;
                        c++;
                        if (state[r][c] == 1)
                            return r;
                        else if (state[r][c] == 0) //if empty, return -1
                            return -1;
                    }
                }
            }
            else
            {
                if (state[i+1][j+1] != 1)//no series, since there's no different pawn
                    return -1;
                else
                {
                    int r = i+1, c = j+1;
                    while (r < 7 && c < 7)
                    {
                        r++;
                        c++;
                        if (state[r][c] == 2)
                            return r;
                        else if (state[r][c] == 0) //if empty, return -1
                            return -1;
                    }
                }
            }
        }
        return -1;
}

int Othello::checkLeft(int **state,const int i, const int j)
{
    if (j !=0) //if i || j == 0 then there's no lower left spot left
        {
            if (isBlacksTurn)
            {
                if (state[i][j-1] != 2)//no series, since there's no different pawn
                    return -1;
                else
                {
                    int c = j-1;
                    while (c > 0)
                    {
                        c--;
                        if (state[i][c] == 1)
                            return c;
                        else if (state[i][c] == 0) //if empty, return -1
                            return -1;
                    }
                }
            }
            else
            {
                if (state[i][j-1] != 1)//no series, since there's no different pawn
                    return -1;
                else
                {
                    int c = j-1;
                    while (c > 0)
                    {
                        c--;
                        if (state[i][c] == 2)
                            return c;
                        else if (state[i][c] == 0) //if empty, return -1
                            return -1;
                    }
                }
            }
        }
        return -1;
}

int Othello::checkRight(int **state,const int i, const int j)
{

    if (j != 7) //if i || j == 0 then there's no lower left spot left
    {
        if (isBlacksTurn)
        {
            if (state[i][j+1] != 2)//no series, since there's no different pawn
                return -1;
            else
            {
                int c = j+1;
                while (c > 0)
                {
                    c++;
                    if (state[i][c] == 1)
                        return c;
                    else if (state[i][c] == 0) //if empty, return -1
                        return -1;
                }
            }
        }
        else
        {
            if (state[i][j+1] != 1)//no series, since there's no different pawn
                return -1;
            else
            {
                int c = j+1;
                while (c > 0)
                {
                    c++;
                    if (state[i][c] == 2)
                        return c;
                    else if (state[i][c] == 0) //if empty, return -1
                        return -1;
                }
            }
        }
    }

    return -1;
}

int Othello::checkTop(int **state,const int i, const int j)
{
    if (i !=0) //if i || j == 0 then there's no lower left spot left
        {
            if (isBlacksTurn)
            {
                if (state[i-1][j] != 2)//no series, since there's no different pawn
                    return -1;
                else
                {
                    int r = i-1;
                    while (r > 0)
                    {
                        r--;
                        if (state[r][j] == 1)
                            return r;
                        else if (state[r][j] == 0) //if empty, return -1
                            return -1;
                    }
                }
            }
            else
            {
                if (state[i-1][j] != 1)//no series, since there's no different pawn
                    return -1;
                else
                {
                    int r = i-1;
                    while (r > 0)
                    {
                        r--;
                        if (state[r][j] == 2)
                            return r;
                        else if (state[r][j] == 0) //if empty, return -1
                            return -1;
                    }
                }
            }
        }
        return -1;
}

int Othello::checkBottom(int **state,const int i, const int j)
{
    if (i != 7) //if i || j == 0 then there's no lower left spot left
        {
            if (isBlacksTurn)
            {
                if (state[i+1][j] != 2)//no series, since there's no different pawn
                    return -1;
                else
                {
                    int r = i+1;
                    while (r < 7)
                    {
                        r++;
                        if (state[r][j] == 1)
                            return r;
                        else if (state[r][j] == 0) //if empty, return -1
                            return -1;
                    }
                }
            }
            else
            {
                if (state[i+1][j] != 1)//no series, since there's no different pawn
                    return -1;
                else
                {
                    int r = i+1;
                    while (r < 7)
                    {
                        r++;
                        if (state[r][j] == 2) {
                            return r;
                        }
                        else if (state[r][j] == 0) {
                            return -1;
                        }
                    }
                    qDebug() << "checkBottom8";
                }
            }
        }
        return -1;
}

