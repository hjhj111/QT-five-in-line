#include "board.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>

Board::Board(QWidget *parent) : QWidget(parent)
{
    row=15;
    col=15;
    d=40;
    black_turn=true;
    b_win=false;
    _board=std::vector<std::vector<int>>(row,std::vector<int>(100,0));
    this->resize((row+1)*d,(col+1)*d);
    //this->setWindowFlags(Qt::FramelessWindowHint);
    connect(this,SIGNAL(win(bool)),this,SLOT(won(bool)));
}

void Board::paintEvent(QPaintEvent*)
{
    QPainter* painter=new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->setPen(QPen(3));
    drawGrid(painter);
    drawStones(painter);
    painter->end();
}

void Board::drawGrid(QPainter* painter)
{

    //竖线
    for(int i=1;i<=col;i++)
    {
        painter->drawLine(i*d,d,i*d,row*d);
    }
    //横线
    for(int i=1;i<=row;i++)
    {
        painter->drawLine(d,i*d,col*d,i*d);
    }

}

void Board::drawStones(QPainter* painter)
{
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            if(_board[i][j]>0)
            {
                drawStone(painter,i,j,_board[i][j]==1);
            }
        }
    }
}

void Board::drawStone(QPainter* painter,int row, int col,bool isblack)
{
    if(isblack)
    {
        painter->setBrush(QBrush(Qt::black));
    }
    else
    {
        painter->setBrush(QBrush(Qt::blue));
    }
    double x=col*d+d/2;
    double y=row*d+d/2;
    painter->drawEllipse(x,y,d,d);
    //painter->setBrush(QBrush());

}

void Board::mousePressEvent(QMouseEvent *event)
{
    int row_stone,col_stone;
    double x_click=event->x();
    double y_click=event->y();
    int left_boundary=x_click/d;
    int up_boundary=y_click/d;
    if(left_boundary>col||up_boundary>row||left_boundary<1||up_boundary<1)
    {
        return;
    }
    double left_far=x_click-left_boundary*d;
    double up_far=y_click-up_boundary*d;
    if(left_far<d/2)
    {
        if(up_far<d/2)
        {
            row_stone=up_boundary-1;
            col_stone=left_boundary-1;
        }
        else
        {
            row_stone=up_boundary;
            col_stone=left_boundary-1;
        }
    }
    else
    {
        if(up_far<d/2)
        {
            row_stone=up_boundary-1;
            col_stone=left_boundary;
        }
        else
        {
            row_stone=up_boundary;
            col_stone=left_boundary;
        }
    }
    click(row_stone,col_stone);

}

void Board::click(int row_stone, int col_stone)
{
    //如果该位置是空
    if(_board[row_stone][col_stone]==0&&!b_win)
    {
        if(black_turn)
        {
            _board[row_stone][col_stone]=1;

        }
        else
        {
            _board[row_stone][col_stone]=2;

        }
        black_turn=!black_turn;
        update();
        if(hasWon(!black_turn,row_stone,col_stone))
        {
            emit win(!black_turn);
        }
    }

}

bool Board::hasWon(bool is_black, int row_stone, int col_stone)
{
    bool ret1=true;
    bool ret2=true;
    bool ret3=true;
    bool ret4=true;
    bool _ret1=true;
    bool _ret2=true;
    bool _ret3=true;
    bool _ret4=true;
    int flag=is_black?1:2;
    //down
    for(int i=0;i<5;i++)
    {
        if(row_stone+i>row)
        {
            ret1=false;
            break;
        }
        if(_board[row_stone+i][col_stone]!=flag)
        {
            ret1=false;
            break;
        }
    }
    //up
    for(int i=0;i<5;i++)
    {
        if(row_stone-i<0)
        {
            _ret1=false;
            break;
        }
        if(_board[row_stone-i][col_stone]!=flag)
        {
            _ret1=false;
            break;
        }
    }
    //right
    for(int i=0;i<5;i++)
    {
        if(col_stone+i>col)
        {
            ret2=false;
            break;
        }
        if(_board[row_stone][col_stone+i]!=flag)
        {
            ret2=false;
            break;
        }
    }
    //left
    for(int i=0;i<5;i++)
    {
        if(col_stone-i<0)
        {
            _ret2=false;
            break;
        }
        if(_board[row_stone][col_stone-i]!=flag)
        {
            _ret2=false;
            break;
        }
    }
    //right-down
    for(int i=0;i<5;i++)
    {
        if(col_stone+i>col||row_stone+i>row)
        {
            ret3=false;
            break;
        }
        if(_board[row_stone+i][col_stone+i]!=flag)
        {
            ret3=false;
            break;
        }
    }
    //left-up
    for(int i=0;i<5;i++)
    {
        if(col_stone-i<0||row_stone-i<0)
        {
            _ret3=false;
            break;
        }
        if(_board[row_stone-i][col_stone-i]!=flag)
        {
            _ret3=false;
            break;
        }
    }
    //right-up
    for(int i=0;i<5;i++)
    {
        if(row_stone-i<0||col_stone+i>col)
        {
            ret4=false;
            break;
        }
        if(_board[row_stone-i][col_stone+i]!=flag)
        {
            ret4=false;
            break;
        }
    }
    //left-down
    for(int i=0;i<5;i++)
    {
        if(row_stone+i>row||col_stone-i<0)
        {
            _ret4=false;
            break;
        }
        if(_board[row_stone+i][col_stone-i]!=flag)
        {
            _ret4=false;
            break;
        }
    }
    return ret1||ret2||ret3||ret4||_ret1||_ret2||_ret3||_ret4;
}

void Board::newgame()
{
    _board=std::vector<std::vector<int>>(row,std::vector<int>(col,0));
    black_turn=true;
    b_win=false;
    update();
}

void Board::won(bool isblack)
{
    b_win=true;
    QString text=isblack?"black has won":"blue has won";
    QMessageBox::information(this,"won",text);
}
