#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //board=new Board(this);
    tcp_board=new Tcp_Board();
    ui->horizontalLayout->addWidget(tcp_board);
    this->setFixedSize(tcp_board->d*(tcp_board->col+1)+100,tcp_board->d*(tcp_board->row+1));
    if(tcp_board->is_server)
    {
        ui->label->setText("your turn");
    }
    else
    {
        ui->label->setText("his turn");
    }
    connect(tcp_board,SIGNAL(turn_changed(bool)),this,SLOT(change_turn(bool)));
    connect(tcp_board,SIGNAL(win(bool)),this,SLOT(won(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    tcp_board->newgame();
}

void MainWindow::change_turn(bool black_turn)
{
    qDebug()<<"come to change_turn()";
    if(black_turn&&tcp_board->is_server||!black_turn&& !tcp_board->is_server)
    {
        qDebug()<<"come to your turn";
        ui->label->setText("your turn");
    }
    else
    {
        ui->label->setText("his turn");
    }
}

void MainWindow::won(bool is_black)
{
    QString text=is_black?"black won":"blue won";
    ui->label->setText(text);
}
