#ifndef OTHELLO_H
#define OTHELLO_H

class Player;
class BoardWidget;
class QString;

class Othello {

public:
    Othello(Player *player1, Player *player2, BoardWidget *board, bool turn);
    bool allowed(bool**,const int, const int);
    bool allowed(const int, const int);
    Player* updateBoardTableStateAllowedMoves(const int, const int);
    Player* updateBoardTableStateAllowedMoves(int**,bool **,const int, const int);
    void initializeTableStateAndAllowedMoves();//done
    QString* getTableState();
    QString* getAllowedMoves();
    int getNextTurn();
    void setNextTurn(int t);
    void setTableState(QString& state);
    void setAllowedMoves(QString& moves);

private:

    void updateBoardTableState(int**,const int, const int);//done
    void updateTableState(int**,const int, const int, int**);//done
    bool updateAllowedMoves(bool **, int **);
    void changeIcon(const int, const int);
    int checkUpperLeft(int**,const int, const int);
    int checkLowerLeft(int**,const int, const int);
    int checkLeft(int**,const int, const int);
    int checkUpperRight(int**,const int, const int);
    int checkLowerRight(int**,const int, const int);
    int checkRight(int**,const int, const int);
    int checkTop(int**,const int, const int);
    int checkBottom(int**,const int, const int);
    int** getSeries(int**,const int, const int);//done

    BoardWidget *boardwidget;
    bool isBlacksTurn;
    //int tableState[8][8];
    //bool allowedMoves[8][8];
    int** tableState;
    bool** allowedMoves;
    int seriesNumber;
    Player *p1, *p2;
};

#endif // OTHELLO_H
