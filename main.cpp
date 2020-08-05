#include "mainwindow.h"
#include "board.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //在单机上模拟时建立两个实例
    MainWindow w;
    w.show();
    MainWindow e;
    e.show();
    //Board board;
    //board.show();
    return a.exec();
}
