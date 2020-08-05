#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "board.h"
#include "tcp_board.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void change_turn(bool black_turn);//改变提示信息等
    void won(bool is_black);//连接Board的win(bool)信号

private:
    Ui::MainWindow *ui;
    Board* board;
    Tcp_Board* tcp_board;

};
#endif // MAINWINDOW_H
