#ifndef CHECKERS1_H
#define CHECKERS1_H

#include <QMainWindow>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <cstdlib>
#include <ctime>
#include <QThread>

const int w=600, h=600;
const int row=8,col=8;
class Pieces
{
public:

    int l,m;
    Pieces(bool);
    bool color;// this is isRed
    bool king;//piece is a king
    void drawYourself(QPainter*,int,int);//painting of checkers
    bool canMove(int startx, int starty, int endx, int endy);//validity of moves
    bool isKing(bool);//kinging of pieces
    int middlex, middley;


};

class Checkers1 : public QMainWindow
{
    Q_OBJECT

public:
    int i,j,l,m,startx,starty,endx,endy,middlex,middley;
    Checkers1(QWidget *parent = 0);
    ~Checkers1();
    void paintEvent(QPaintEvent *);//drawing of board, passing of checker pieces
    void mousePressEvent(QMouseEvent *);//picking a piece
    void mouseReleaseEvent(QMouseEvent *);//moving of a piece

};

#endif // CHECKERS1_H
