#ifndef TCP_BOARD_H
#define TCP_BOARD_H

#include "board.h"
#include <QTcpServer>
#include <QTcpSocket>


class Tcp_Board : public Board//继承自Board
{
    Q_OBJECT
public:
    Tcp_Board();
    void click(int row_stone,int col_stone) override;
    void newgame() override;
    bool is_server;//server 是黑方
    bool is_winner;//是否胜方

private:
    QTcpServer* server;
    QTcpSocket* socket;

private slots:
    void slot_NewConnection();
    void slot_ReadyRead();
    void won(bool isblack) override;

signals:
    void turn_changed(bool black_turn);

};

#endif // TCP_BOARD_H
