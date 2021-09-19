#include "cell.hpp"

bool Cell::open()
{
    switch (status)
    {
    case markedMine:
    case markedSpace:
        unmark();
        return true;
        break;
    case openedMine:
    case openedSpace:
        return true;
        break;
    case coveredMine:
        status = openedMine;
        return false;
        break;
    case coveredSapce:
        status = openedSpace;
        return true;
        break;
    case empty:
        return true;
        break;
    default:
        return true;
        break;
    }
};

bool Cell::unmark()
{
    switch (status)
    {
    case markedSpace:
        status = coveredSapce;
        return true;
        break;
    case markedMine:
        status = coveredMine;
        return true;
        break;
    default:
        return false;
        break;
    }
};
bool Cell::mark()
{
    switch (status)
    {
    case coveredSapce:
        status = markedSpace;
        return true;
        break;
    case coveredMine:
        status = markedMine;
        return true;
        break;
    default:
        return false;
        break;
    }
}

bool Cell::automark()
{
    if (isMarked())
    {
        unmark();
        return true;
    }
    else if (isCovered())
    {
        mark();
        return true;
    }
    else
    {
        return false;
    }
}

CONFIG::color Cell::getColor(int aroundMineNumber) const
{
    switch (status)
    {
    case openedMine:
        return CONFIG::C_Exploded;
        break;
    case openedSpace:
        if (aroundMineNumber)
        {
            return CONFIG::C_Space;
        }
        else
        {
            return CONFIG::C_NoNumberSpace;
        }
        break;
    case markedMine:
    case markedSpace:
        return CONFIG::C_Marked;
        break;
    case coveredMine:
    case coveredSapce:
        return CONFIG::C_Covered;
        break;
    case empty:
    default:
        return CONFIG::none;
        break;
    }
}

char Cell::getRander(int aroundMineNumber) const
{
    switch (status)
    {
    case openedMine:
        return CONFIG::Exploded;
        break;
    case openedSpace:
        if (aroundMineNumber)
        {
            return TOOL::toChar(aroundMineNumber);
        }
        else
        {
            return CONFIG::Space;
        }
        break;
    case markedMine:
    case markedSpace:
        return CONFIG::Marked;
        break;
    case coveredMine:
    case coveredSapce:
        return CONFIG::Covered;
        break;
    case empty:
    default:
        return '\0';
        break;
    }
}

CONFIG::color Cell::getAnsColor(int aroundMineNumber) const
{
    switch (status)
    {
    case openedMine:
        return CONFIG::C_Exploded;
        break;
    case openedSpace:
        if (aroundMineNumber)
        {
            return CONFIG::C_Space;
        }
        else
        {
            return CONFIG::C_NoNumberSpace;
        }
        break;
        break;
    case markedMine:
        return CONFIG::C_RightMark;
        break;
    case markedSpace:
        return CONFIG::C_WrongMark;
        break;
    case coveredMine:
        return CONFIG::C_Mine;
        break;
    case coveredSapce:
        return CONFIG::C_Covered;
        break;
    case empty:
    default:
        return CONFIG::none;
        break;
    }
}

char Cell::getAnswer(int aroundMineNumber) const
{
    switch (status)
    {
    case openedMine:
        return CONFIG::Exploded;
        break;
    case openedSpace:
        if (aroundMineNumber)
        {
            return TOOL::toChar(aroundMineNumber);
        }
        else
        {
            return CONFIG::Space;
        }
        break;
    case markedMine:
        return CONFIG::RightMark;
        break;
    case markedSpace:
        return CONFIG::WrongMark;
        break;
    case coveredMine:
        return CONFIG::Mine;
        break;
    case coveredSapce:
        return CONFIG::Covered;
        break;
    case empty:
    default:
        return '\0';
        break;
    }
}

void Cell::reset()
{
    if (isMine())
    {
        status = coveredMine;
    }
    else
    {
        status = coveredSapce;
    }
}

Cell::Cell(int x, int y, bool _isMine, bool _isEmpty)
{
    if (_isEmpty)
    {
        status = empty;
    }
    else if (_isMine)
    {
        status = coveredMine;
    }
    else
    {
        status = coveredSapce;
    }

    this->x = x;
    this->y = y;
}

Cell::~Cell()
{
}
