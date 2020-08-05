#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <vector>


class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr);
    //棋盘尺寸
    int row;
    int col;
    int d;
    //轮到谁
    bool black_turn;
    //有人胜出
    bool b_win;
    virtual void newgame();//开启新游戏
    std::vector<std::vector<int>> _board;//0代表空格，1代表黑棋，2代表白棋
    virtual void click(int row_stone,int col_stone);//点击事件需要的处理函数
    bool hasWon(bool is_black,int row_stone,int col_stone);

private:
    void paintEvent(QPaintEvent *) override;
    void drawGrid(QPainter* painter);//画网格
    virtual void mousePressEvent(QMouseEvent *event) override;
    void drawStones(QPainter* painter);//刷新棋子布局
    void drawStone(QPainter* painter,int row,int col,bool isblack);//画一个棋子，由drawStones()调用

signals:
    void win(bool isblack);//有人胜出

public slots:
    virtual void won(bool isblack);//有人胜出处理函数

};

#endif // BOARD_H
