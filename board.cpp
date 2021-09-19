#include "board.hpp"
#include "cell.hpp"

#include <random>
#include <ctime>

Board::Board(int _xRange, int _yRange, int _mineNumber) : xRange(_xRange),
                                                          yRange(_yRange),
                                                          mineNumber(_mineNumber)
{
    getMemory();
    std::default_random_engine e(time(0));
    std::uniform_int_distribution<unsigned> xRandom(0, this->xRange - 1);
    std::uniform_int_distribution<unsigned> yRandom(0, this->yRange - 1);

    while (_mineNumber > 0)
    {
        int x = xRandom(e);
        int y = yRandom(e);

        if (getCell(x, y).isMine())
        {
            continue;
        }
        else
        {
            getCell(x, y).setMine();
            _mineNumber--;
        }
    }
    // 调试
    // getCell(0, 2).setMine();
    // getCell(0, 5).setMine();
    // getCell(1, 5).setMine();
    // getCell(2, 2).setMine();
    // getCell(4, 6).setMine();
    // getCell(5, 4).setMine();
    // getCell(6, 1).setMine();
    // getCell(6, 7).setMine();
    // getCell(7, 6).setMine();
    // getCell(7, 7).setMine();
}

Board::Board(Board &copy) : xRange(copy.xRange),
                            yRange(copy.yRange),
                            mineNumber(copy.mineNumber),
                            finished(copy.finished),
                            allOpened(copy.allOpened),
                            openedSpaceNumber(openedSpaceNumber),
                            markedNumber(markedNumber)
{
    getMemory();
    for (int x = 0; x < xRange; x++)
    {
        for (int y = 0; y < yRange; y++)
        {
            this->getCell(x, y).copyFrom(copy.getCell(x, y));
        }
    }
}

Board::~Board()
{
    for (int x = 0; x < xRange; x++)
    {
        for (int y = 0; y < yRange; y++)
        {
            delete board[x][y];
        }
        delete[] board[x];
    }
    delete[] board;
};

void Board::updata()
{
    openedSpaceNumber = 0;
    markedNumber = 0;
    for (int x = 0; x < xRange; x++)
    {
        for (int y = 0; y < yRange; y++)
        {
            Cell &c = getCell(x, y);
            c.resetChecked();
            if (c.isOpened() && c.isSpace())
            {
                openedSpaceNumber++;
            }
            else if (c.isMarked())
            {
                markedNumber++;
            }
            else if (c.isMine() && c.isOpened())
            {
                this->finish();
            }
        }
    }

    if (openedSpaceNumber == xRange * yRange - mineNumber)
    {
        this->allOpened = true;
        this->finish();
    }
}

void Board::automark(int posX, int posY)
{
    getCell(posX, posY).automark();
    updata();
}

bool Board::open(int posX, int posY)
{
    if (getCell(posX, posY).isOpened() || getCell(posX, posY).isMarked())
    {
        return true;
    }

    bool result = openResc(posX, posY);
    if (!result)
    {
        this->finish();
    }

    getCell(posX, posY).open();

    updata();
    return result;
}

bool Board::openResc(int posX, int posY)
{
    getCell(posX, posY).setChecked();
    if (getCell(posX, posY).isMine())
    {
        return false;
    }
    else
    {
        Around around = getAround(posX, posY);
        for (int i = 0; i < 8 && !around[i]->isEmpty(); i++)
        {
            if (around[i]->isChecked() || around[i]->isMine())
            {
                continue;
            }

            if (getAroundMineNumber(around[i]->getPostionX(), around[i]->getPostionY()))
            {
                if (getAroundMineNumber(around) == 0)
                {
                    around[i]->open();
                }
                continue;
            }

            if (openResc(around[i]->getPostionX(), around[i]->getPostionY()))
            {
                around[i]->open();
            }
        }
        return true;
    }
}

bool Board::openRange(int posX, int posY)
{
    if (!getCell(posX, posY).isOpened())
    {
        return true;
    }
    int marked = 0;
    int mine = 0;
    bool result = true;
    Around around = getAround(posX, posY);
    for (int i = 0; i < 8 && !around[i]->isEmpty(); i++)
    {
        if (around[i]->isMarked())
        {
            marked++;
        }
        if (around[i]->isMine())
        {
            mine++;
        }
        if (around[i]->isMine() && !around[i]->isMarked())
        {
            result = false;
        }
    }

    if (marked == mine)
    {
        if (result)
        {
            for (int i = 0; i < 8 && !around[i]->isEmpty(); i++)
            {
                if (around[i]->isCovered() && this->openResc(around[i]->getPostionX(),
                                                             around[i]->getPostionY()))
                {
                    around[i]->open();
                }
            }
            updata();
            return true;
        }
        else
        {
            for (int i = 0; i < 8 && !around[i]->isEmpty(); i++)
            {
                if (around[i]->isCovered())
                {
                    around[i]->open();
                }
            }
            updata();
            this->finish();
            return false;
        }
    }
    else
    {
        updata();
        return true;
    }
}

int Board::getAroundMineNumber(Around _around)
{
    int result = 0;
    for (int i = 0; i < 8 && !_around[i]->isEmpty(); i++)
    {
        if (_around[i]->isMine())
        {
            result++;
        }
    }
    return result;
};

void Board::getMemory()
{
    board = new Cell **[xRange];
    for (int x = 0; x < xRange; x++)
    {
        board[x] = new Cell *[yRange];
        for (int y = 0; y < yRange; y++)
        {
            board[x][y] = new Cell({x, y});
        }
    }
}

Around Board::getAround(int posX, int posY)
{
    int index = 0;
    Around around;
    if (posX > 0)
    {
        if (posY > 0)
        {
            around.at(index) = &getCell(posX - 1, posY - 1);
            index++;
        }

        {
            around.at(index) = &getCell(posX - 1, posY);
            index++;
        }

        if (posY < yRange - 1)
        {
            around.at(index) = &getCell(posX - 1, posY + 1);
            index++;
        }
    }

    {
        if (posY > 0)
        {
            around.at(index) = &getCell(posX, posY - 1);
            index++;
        }

        if (posY < yRange - 1)
        {
            around.at(index) = &getCell(posX, posY + 1);
            index++;
        }
    }

    if (posX < xRange - 1)
    {
        if (posY > 0)
        {
            around.at(index) = &getCell(posX + 1, posY - 1);
            index++;
        }

        {
            around.at(index) = &getCell(posX + 1, posY);
            index++;
        }

        if (posY < yRange - 1)
        {
            around.at(index) = &getCell(posX + 1, posY + 1);
            index++;
        }
    }

    while (index < 8)
    {
        around.at(index) = &emptyMine;
        index++;
    }

    return around;
}

void Board::rander(short option,
                   void (*printer)(outputOpt, CONFIG::color, int posX, int posY, int ch))
{
    printer(start, CONFIG::none, -1, -1, 0);
    for (int x = 0; x < xRange; x++)
    {
        for (int y = 0; y < yRange; y++)
        {
            Cell &c = getCell(x, y);
            int aroundMineNumber = getAroundMineNumber(c.getPostionX(), c.getPostionY());
            if (option == 0)
            {

                printer(put,
                        c.getColor(aroundMineNumber),
                        x, y,
                        c.getRander(aroundMineNumber));
            }
            else if (option = 1)
            {
                printer(put,
                        c.getAnsColor(aroundMineNumber),
                        x, y,
                        c.getAnswer(aroundMineNumber));
            }
        }
        printer(endline, CONFIG::none, -1, -1, 0);
    }
    printer(end, CONFIG::none, -1, -1, 0);
}

void Board::reset()
{
    for (int x = 0; x < xRange; x++)
    {
        for (int y = 0; y < yRange; y++)
        {
            getCell(x, y).reset();
        }
    }
    finished = false;
    allOpened = false;
    openedSpaceNumber = 0;
    markedNumber = 0;
}
