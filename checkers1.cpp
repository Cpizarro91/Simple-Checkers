#include "checkers1.h"

Pieces* pieces[row][col];//had to declare global so both class Pieces and MainWindow could reach it
bool turn;
bool gameover;

Checkers1::Checkers1(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(w,h);//resize the window to your width and height
    srand(time(NULL));//initialize random function to use current time, cplusplus.com

    //go through the board to determine the placement of the pieces
    for(i=0;i<row;i++)//rows
    {
        for(j=0;j<col;j++)//columns
        {
          if (i%2==j%2 && i<=2)//top side of the board, black pieces start here
          {
            pieces[j][i] = new Pieces(false);//this will mark which are black pieces
          }

         else if (i%2==j%2 && i>=5)//bottom side of the board, red pieces start here
          {
            pieces[j][i] = new Pieces(true);//this will mark which are red pieces
          }
         else
          {
            pieces[j][i]=NULL;//this will mark the squares that have no piece
          }
        }

    }

}
Pieces::Pieces(bool c)
{
    color = c;//passes true or false
    king = false;

}

bool Pieces::isKing(bool q)
{

    king = q;//passes true into king

}

bool Pieces::canMove(int startx, int starty, int endx, int endy)
{
    //in this case turn is false so black will always start
    if(color==turn)//if the color matches whose turn it is, they can move
    {
    //checking if the integer is valid 0 - 7
    if(startx<0||starty<0||endx<0||endy<0)
    {
        if(startx>7||starty>7||endx>7||endy>7)
        {
            return false;//integer invalid, move invalid
        }

     }

    //checking if the space their landing on is empty
    if(pieces[endx][endy]!=NULL)
    {
        return false;//move invalid, didn't land on empty space
    }

    //making sure the appropriate color is moving to its respective paths
    //if regular checker it can only move one direction, this doesn't apply to the king
    if(!king)
    {
        if (color) //if its red
        {
            if(endy>starty) //not endy<starty,
                return false;
        }
        else //if its black
        {
            if(starty>endy)//not endy>starty starty should be bigger than endy starty>endy, starty<endy
                return false;
        }
    }


    //making sure the piece is only moving 1 or 2 spaces
    if(abs(startx-endx) !=1&&abs(startx-endx) !=2)
        return false;

    if (abs(starty-endy) != 1&&abs(starty-endy) !=2)
         return false;


    //making sure piece is moving diagonally
    if(abs(startx-endx)!=abs(starty-endy))
        return false;

    //if piece is jumping
    if(abs(startx-endx)==2)
    {
        middlex=startx+(endx-startx)/2;//this will get index middle piece x
        middley=starty+(endy-starty)/2; //this will get index middle piece y

        if (pieces[middlex][middley]==NULL)//delete the middle piece
            return false;
        if (pieces[middlex][middley] -> color == color)//if same color return false
            return false;
    }
    return true;//move is valid
    }

    return false;//move is invalid
}


Checkers1::~Checkers1()
{

}
void Checkers1::mousePressEvent(QMouseEvent* event)
{
    qDebug() << "mouse was pressed at "<< event->x()/75<<","<< event->y()/75<<endl;
    startx=event->x()/75;
    starty=event->y()/75;


    if(startx<0||startx>7)//if less than 0 or greater than 7
    {
        qDebug() << "Improper integer." << endl;
    }
    else if(starty<0||starty>7)
    {
        qDebug() << "Improper integer." << endl;
    }


    if(pieces[startx][starty]==NULL)//if empty square is picked
    {
        qDebug() << "You didn't pick a checker." << endl;
    }
    repaint();
}
void Checkers1::mouseReleaseEvent(QMouseEvent* event)
{
    qDebug() << "mouse was released at" << event->x()/75 <<","<<event->y()/75<<endl;
    endx=event->x()/75;
    endy=event->y()/75;

    if(endx<0||endx>7)//if less than 0 or greater than 7
    {
        qDebug() << "Improper integer." << endl;
    }
    else if(endy<0||endy>7)
    {
        qDebug() << "Improper integer." << endl;
    }


        //this is the human:

        //if the square contains a checker
        if(pieces[startx][starty]!=NULL)
       {
            if (pieces[startx][starty] -> canMove(startx,starty,endx,endy))//check to see if move is valid
            {//if the move is valid it will go into this loop to move the piece

                pieces[endx][endy]=pieces[startx][starty];//moves picked piece into end slot

                pieces[startx][starty]=NULL;//delete the original checker piece


                    if(abs(startx-endx)==2)//if the piece is jumping
                    {

                        middlex=startx+(endx-startx)/2;//this will get index middle piece x
                        middley=starty+(endy-starty)/2;//middle piece y

                        pieces[middlex][middley]=NULL;//delete middle piece
                    }

                    if(endy==0||endy==7)//if the piece is at the end of the board
                    {
                        pieces[endx][endy]->isKing(true);//the piece turns into a king
                    }

                repaint();//redraws piece into new spot

                turn=!turn;//switch turns


                //we must check if there are any valid moves left on the board for whoever just moved a piece
                gameover=true;
                for(i=0;i<row;i++)//x coordinate of a piece
                {
                    for(j=0;j<col;j++)//y coordinate of a piece
                    {
                        if(pieces[i][j]!=NULL)//x,y has to be a checker, not an empty square
                        {
                            if(pieces[i][j]->color==turn)//if the checker belongs to whoever just moved
                            {
                                for(endx=0;endx<8;endx++)//x coordinate of the end square
                                {
                                    for(endy=0;endy<8;endy++)//y coordinate of the end square
                                    {
                                        if(pieces[i][j]->canMove(i,j,endx,endy))//moving the checker into the end square and seeing if its a valid move
                                        {//if it is a valid move
                                             gameover=false;//then the game is not over
                                             break;
                                        }

                                    }

                                }
                            }
                        }
                    }
                }
                if (gameover==true)//if after all the moves have been checked and there are no valid moves left
                {
                    setWindowTitle("Black Wins!");//then this color has won
                }





            //this is the AI:

            if(gameover==false)//if the game is not over, there are valid moves left, the ai can move
            {
                //Sleep::msleep(250);
            while(true)
            {

                //generate a random checker and end square
                startx=rand()%8;
                starty=rand()%8;
                endx=rand()%8;
                endy=rand()%8;

                //if square contains a checker
                if(pieces[startx][starty]!=NULL)
                {


                    if(pieces[startx][starty]->canMove(startx,starty,endx,endy))//check to see if move is valid
                    {//if move is valid it will move into this loop to actually move it
                        //Sleep::msleep(250);

                        pieces[endx][endy]=pieces[startx][starty];//moves picked piece into end slot
                        //Sleep::msleep(250);
                        pieces[startx][starty]=NULL;//delete the original checker piece

                        if(abs(startx-endx)==2)//if jumping
                        {
                            //Sleep::msleep(250);
                            middlex=startx+(endx-startx)/2;//this will get index middle piece x
                            middley=starty+(endy-starty)/2;//index middle piece y
                            //Sleep::msleep(250);
                            pieces[middlex][middley]=NULL;//delete middle checker
                        }

                        if(endy==0||endy==7)//if piece reaches end of board
                        {
                         pieces[endx][endy]->isKing(true);//turn the piece into a king
                        }

                        repaint();//redraw the checker

                        turn=!turn;//change turns


                        //checking if there are any valid moves left for this color
                        for(i=0;i<row;i++)//x coordinate of a piece
                        {
                            for(j=0;j<col;j++)//y coordinate of a piece
                            {
                                if(pieces[i][j]!=NULL)//x,y has to be a checker, not an empty square
                                {
                                    if(pieces[i][j]->color==turn)//if the checker belongs to whoever just moved
                                    {
                                        for(endx=0;endx<8;endx++)//x coordinate of the end square
                                        {
                                            for(endy=0;endy<8;endy++)//y coordinate of the end square
                                            {
                                                if(pieces[i][j]->canMove(i,j,endx,endy))//check if the checker is a valid move
                                                {//if it is a valid move
                                                    gameover=false;//then the game is not over, there are still valid moves left
                                                    break;
                                                }

                                            }

                                        }
                                    }
                                }
                            }
                        }
                        if (gameover==true)//if after all the moves have been checked and there are no valid moves left
                        {

                            setWindowTitle("Red Wins!");//then this color wins
                            break;

                        }

                        break;

                    }


                    }
                }
            }
            }
            }


        }

void Checkers1::paintEvent(QPaintEvent *)//paint event gets called when the user has any interaction with the window
{
    QPainter p(this);
    p.fillRect(0,0,w,h,Qt::white);//set the background color white

    //this will generate the colored squares
    for(i=0;i<row;i++)//rows
    {
        for(j=0;j<col;j++)//column
        {
            if (i%2==0)//if the row is even
                p.fillRect(QRect(75*i,150*j,75,75),Qt::gray);//draw a gray square here

            if(i%2==1)//if the row is odd
                p.fillRect(QRect(75*i,75+150*j,75,75),Qt::gray);//draw a gray square here


        }
    }

    //this will generate the pieces
    for(i=0;i<row;i++)
    {
        for(j=0;j<col;j++)
        {
            if (pieces[j][i] != NULL)//if it's not an empty square
            {
                l = j*75;
                m = i*75;
                pieces[j][i] -> drawYourself(&p,l,m);//call to draw this checker
                j = j + 1;
            }
        }
    }



}



void Pieces::drawYourself(QPainter* p, int l,int m)//here we will actually draw the checker pieces
{
    if (color)//if true, or is red
    {
        p->setBrush(Qt::red);//set the color to red
        p->drawEllipse(l,m,75,75);//draw red checker piece
    }
    else//if false, if black
    {
        p->setBrush(Qt::black);//set the color to black
        p->drawEllipse(l,m,75,75);//draw black checker piece
    }

    if (color)
    {
        if (king)//if red and is a king
        {
        p->setBrush(Qt::red);//set the color to red
        p->drawEllipse(l,m-10,75,75);//draw red checker piece
        }
    }
    else//if false, if black
    {
        if (king) //if black and is a king
        {
        p->setBrush(Qt::black);//set the color to black
        p->drawEllipse(l,m-10,75,75);//draw black checker piece
        }
    }
}


