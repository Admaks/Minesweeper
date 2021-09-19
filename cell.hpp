#ifndef MINESWEEPER__MINE_HPP_
#define MINESWEEPER__MINE_HPP_

#include "config.hpp"
#include "tool.hpp"

class Cell
{
public:
    enum Status
    {
        openedSpace,
        openedMine,
        markedSpace,
        markedMine,
        coveredSapce,
        coveredMine,
        empty
    };

private:
    Status status;
    int x = 0;
    int y = 0;
    bool checked = false;

public:
    bool isOpened() const { return status == openedMine || status == openedSpace; };
    bool isCovered() const { return status == coveredMine || status == coveredSapce; };
    bool isMarked() const { return status == markedMine || status == markedSpace; };
    bool isMine() const { return status == openedMine || status == markedMine || status == coveredMine; };
    bool isSpace() const { return !isMine(); };
    bool isEmpty() const { return status == empty; };
    void setPostion(int x, int y) { this->x = x; y = this->y;};
    int getPostionX() { return x; };
    int getPostionY() { return y; };
    bool setMine()
    {
        if (isCovered())
        {
            status = coveredMine;
            return true;
        }
        else
        {
            return false;
        }
    };
    Status getStatus() const { return status; };
    void setChecked() { checked = true; };
    bool isChecked() const { return checked; };
    void resetChecked() { checked = false; };

    bool open();
    bool unmark();
    bool mark();
    bool automark();

    void reset();

    CONFIG::color getColor(int aroundMineNumber) const;
    char getRander(int aroundMineNumber) const;
    char getAnswer(int aroundMineNumber) const;
    CONFIG::color getAnsColor(int aroundMineNumber) const;
    Cell &copyFrom(Cell &_c)
    {
        this->x = _c.x;
        this->y = _c.y;
        this->status = _c.status;
        return *this;
    };

    Cell(int x, int y, bool _isMine = false, bool _isEmpty = false);
    ~Cell();
};

#endif