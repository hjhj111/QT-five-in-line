#include<QDebug>
#include <QMessageBox>
#include<QMouseEvent>

#include "tcp_board.h"

//报文结构
/*
[0]:1是黑子，2是白子,3是询问是否重开，4是确定重开，5是拒绝重开,6是对方胜利
[1][2]:row
[3][4]:col
*/
Tcp_Board::Tcp_Board()
{
    QMessageBox::StandardButton type=QMessageBox::information(this,"server or client","server?",
                                                              QMessageBox::No|QMessageBox::Yes);
    if(type==QMessageBox::Yes)
    {
        is_server=true;
        is_winner=false;
        server=new QTcpServer(this);
        socket=NULL;
        server->listen(QHostAddress::Any, 9999);
        connect(server,SIGNAL(newConnection()),this,SLOT(slot_NewConnection()));
    }
    else if(type==QMessageBox::No)
    {
        is_server=false;
        is_winner=false;
        socket=new QTcpSocket(this);
        socket->connectToHost(QHostAddress("127.0.0.1"), 9999);
        connect(socket, SIGNAL(readyRead()), this, SLOT(slot_ReadyRead()));
    }

}

void Tcp_Board::slot_NewConnection()
{
    qDebug()<<"new connection";
    if(socket)
        return;
    qDebug()<<"look";
    socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(slot_ReadyRead()));
    char towrite[1]={'0'};
    socket->write(towrite,1);
}

void Tcp_Board::slot_ReadyRead()
{
    QByteArray arry = socket->readAll();
    //test
    if(arry[0]=='0')
    {
        qDebug()<<"test the connect OK";
    }
    //new game?
    else if(arry[0]=='3')
    {
        QMessageBox::StandardButton type=QMessageBox::information(this,"new game","he want new game?",
                                                                  QMessageBox::No|QMessageBox::Yes);
        if(type==QMessageBox::Yes)
        {
            Board::newgame();
            emit turn_changed(black_turn);
            char towrite[1]={'4'};
            socket->write(towrite,1);
        }
        else
        {
            char towrite[1]={'5'};
            socket->write(towrite,1);
        }
    }
    //确定重开
    else if(arry[0]=='4')
    {
        Board::newgame();
        emit turn_changed(black_turn);
        is_winner=false;
    }
    //拒绝重开
    else if(arry[0]=='5')
    {
        //do nothing
    }
    else if(arry[0]=='6')
    {
        //对方胜利
        emit win(!is_server);
    }
    //黑子或白子
    else
    {
        qDebug()<<"jjjj";
        qDebug()<<arry.size();
        for(int i=0;i<5;i++)
        {
            qDebug()<<arry[i];
        }
        int row1=arry[1]-'0';
        int row2=arry[2]-'0';
        int col1=arry[3]-'0';
        int col2=arry[4]-'0';
        int row=row1*10+row2;
        int col=col1*10+col2;
        _board[row][col]=arry[0]-'0';
        black_turn=!black_turn;
        emit turn_changed(black_turn);
        //改变当前应该走的用户
        update();
    }
}

void Tcp_Board::click(int row_stone,int col_stone)
{
    if(is_server!=black_turn)
    {
        qDebug()<<"not you move";
        qDebug()<<is_server<<black_turn;
        return;
    }
    //如果该位置是空
    if(_board[row_stone][col_stone]==0&&!b_win)
    {
        char ch0,ch1,ch2,ch3,ch4;
        if(black_turn)
        {
            _board[row_stone][col_stone]=1;
            ch0='1';
        }
        else
        {
            _board[row_stone][col_stone]=2;
             ch0='2';
        }
        black_turn=!black_turn;
        emit turn_changed(black_turn);
        update();
        ch1='0'+row_stone/10;
        ch2='0'+row_stone%10;
        ch3='0'+col_stone/10;
        ch4='0'+col_stone%10;
        char towrite[5];
        towrite[0]=ch0;
        towrite[1]=ch1;
        towrite[2]=ch2;
        towrite[3]=ch3;
        towrite[4]=ch4;
        socket->write(towrite,5);
        if(hasWon(!black_turn,row_stone,col_stone))
        {
            is_winner=true;
            emit win(!black_turn);
        }
    }

}

void Tcp_Board::newgame()
{
     QMessageBox::StandardButton type=QMessageBox::information(this,"new game","you want new game?",
                                                               QMessageBox::No|QMessageBox::Yes);
     if(type==QMessageBox::Yes)
     {
         char towrite[1]={'3'};
         socket->write(towrite,1);
     }
}

void Tcp_Board::won(bool is_black)
{
    Board::won(is_black);
    if(is_winner)
    {
        qDebug()<<"is winner";
        char towrite[1]={'6'};
        socket->write(towrite,1);
    }
}
