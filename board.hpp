#ifndef MINESWEEPER__BOARD_HPP_
#define MINESWEEPER__BOARD_HPP_

#include <array>
#include "cell.hpp"

using std::array;

typedef array<Cell *, 8> Around;

class Board
{

private:
    int mineNumber = 0;
    int xRange;
    int yRange;
    int finished = false;
    bool allOpened = false;
    int openedSpaceNumber = 0;
    int markedNumber = 0;

    Cell ***board = nullptr;

    Cell emptyMine = Cell(-1, -1, false, true); // 占位格子，用于 getAround方法

    void getMemory(); // 申请内存

    bool openResc(int posX, int posY); // 自动打开格子，递归

    Around getAround(int posX, int posY);                                   // 获取周围格子
    Cell &getCell(int posX, int posY) const { return *board[posX][posY]; }; // 获取格子
    int getAroundMineNumber(Around _around);                                // 获取周围雷的数量
    int getAroundMineNumber(int posX, int posY)                             // 获取周围雷的数量
    {
        return getAroundMineNumber(getAround(posX, posY));
    };

    void updata();

public:
    bool open(int posX, int posY); // 自动打开格子
    bool openRange(int posX, int posY);
    void automark(int posX, int posY); // 标记格子（旗）
    enum outputOpt
    {
        start,
        put,
        endline,
        end
    };
    void rander(short option,
                void (*printer)(outputOpt, CONFIG::color, int posX, int posY, int ch));
    void randerAnswer(void (*printer)(outputOpt, CONFIG::color, int posX, int posY, int ch));
    void finish() { finished = true; };     // 结束游戏
    bool isFinished() { return finished; }; // 获取游戏状态
    void reset();
    int getXRange() { return xRange; };
    int getYRange() { return yRange; };
    int getOpenedSpaceNumber() { return openedSpaceNumber; };
    int getMarkedNumber() { return markedNumber; };
    int getMineNumber() { return mineNumber; };
    bool isAllOpened() { return allOpened; };
    Board(int xRange = 10, int yRange = 10, int Mine = 10);
    Board(Board &_b);
    ~Board();
};

#endif