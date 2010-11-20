#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>

class Othello;
class Player;
class QButtonGroup;
class QPushButton;

#define B_WIDTH 60
#define B_HEIGHT 60
#define WHITE 1
#define GREEN 2
#define BLACK 3
#define VALIDGREEN 4
#define MAGIC_NUMBER 0x8F21CEA8

class BoardWidget : public QWidget
{
    Q_OBJECT

public:
    BoardWidget(Player *player1, Player *player2, QWidget *parent = 0);

    QButtonGroup *buttongroup;
    void setIconAt(int color, const int i, const int j);
    void clearBoard();
    bool writeFileAs(const QString &fileName);
    bool readFile(const QString &fileName);
    void setButtonName(const QString &name, const int i, const int j);
    void setIcons(QIcon *icon1, QIcon *icon2, bool isFirst);

signals:
    void btnClicked(const QString &);
    void gameOver();

private slots:
    void clickHandler();

private:

    QPushButton *btnArray[8][8];
    Othello *othello;
    QIcon *greenIcon;
    QIcon *blackIcon;
    QIcon *whiteIcon;
    QIcon *validgreenIcon;
    Player *p1,*p2;
};

#endif // BOARDWIDGET_H
