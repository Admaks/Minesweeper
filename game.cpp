#include "board.hpp"
#include "config.hpp"
#include <curses.h>
#include <cstdlib>
void printer(Board::outputOpt, CONFIG::color, int posX, int posY, int ch);
int getInput();
void setColor(CONFIG::color);
void initColor();

struct
{
    int x;
    int y;
} coursePotion{
    0,
    0};

struct
{
    int x;
    int y;
    int MineNumber;
} range{
    8,
    8,
    10};

int main(int argc, char *argv[])
{
    if (argc != 1 && argc != 4)
    {
        return 1;
    }
    else if (argc == 4)
    {
        range.x = atoi(argv[1]);
        range.y = atoi(argv[2]);
        range.MineNumber = atoi(argv[3]);
    }

    initscr();            // 切换到curses
    cbreak();             // 禁止行缓冲（也就是按键后立即读取字符，而不是等回车以后）
    noecho();             // 禁止输入回显（否则按功能键就会出现D之类的字符）
    keypad(stdscr, TRUE); // 允许使用功能键（包括方向键）
    refresh();

    initColor();

    Board *board = new Board(range.x, range.y, range.MineNumber);

    bool isLoop = true;
    do
    {
        while (!board->isFinished())
        {
            board->rander(0, printer);

            printw("\n\rOpened: %d, Marked: %d, Mine: %d",
                   board->getOpenedSpaceNumber(),
                   board->getMarkedNumber(),
                   board->getMineNumber() - board->getMarkedNumber());
            refresh();

            switch (getInput())
            {
            case 1:
                board->open(coursePotion.x, coursePotion.y);
                break;
            case 2:
                board->automark(coursePotion.x, coursePotion.y);
                break;
            case 3:
                board->openRange(coursePotion.x, coursePotion.y);
                break;
            }

        }

        if (board->isAllOpened())
        {
            delete board;

            printw("\n\r"
                   "Good job,\n\r"
                   "would you like play again?(Y/n)");
            char c = getch();
            if (c == 'n' || c == 'N')
            {
                isLoop = false;
            }
            else
            {
                board = new Board(range.x, range.y, range.MineNumber);
                isLoop = true;
            }
        }
        else
        {
            board->rander(1, printer);
            printw("\n\r"
                   "what a pity,\n\r"
                   "would you want play a new game?(Y/n),\n\r"
                   "or just this game?(r)");
            char c = getch();
            if (c == 'n' || c == 'N')
            {
                delete board;
                isLoop = false;
            }
            else if (c == 'R' || c == 'r')
            {
                isLoop = true;
                board->reset();
            }
            else
            {
                delete board;
                board = new Board(range.x, range.y, range.MineNumber);
                isLoop = true;
            }
        }

    } while (isLoop);

    endwin();
    return 0;
}

void printer(Board::outputOpt option, CONFIG::color color, int posX, int posY, int ch)
{
    setColor(color);
    switch (option)
    {
    case Board::start:
        clear();
        break;
    case Board::endline:
        printw("\n\r");
        break;
    case Board::put:
        if (posX == coursePotion.x && posY == coursePotion.y)
        {
            setColor(CONFIG::none);
            setColor(CONFIG::C_Hint);
            printw("%c", CONFIG::Hint);
            setColor(CONFIG::none);
            setColor(color);
            printw("%c", char(ch));

        }
        else
        {
            printw(" %c", (char)ch);
        }
        break;
    case Board::end:
        refresh();
        break;
    }
    setColor(CONFIG::none);
}

int getInput()
{
    int result = -1;
    do
    {
        int key = getch();

        switch (key)
        {
        case KEY_UP:
        case CONFIG::UP:
            if (coursePotion.x > 0)
            {
                coursePotion.x--;
            }
            else
            {
                coursePotion.x = range.x - 1;
            }
            result = 0;
            break;
        case KEY_DOWN:
        case CONFIG::DOWN:
            if (coursePotion.x < range.x - 1)
            {
                coursePotion.x++;
            }
            else
            {
                coursePotion.x = 0;
            }
            result = 0;
            break;
        case KEY_LEFT:
        case CONFIG::LEFT:
            if (coursePotion.y > 0)
            {
                coursePotion.y--;
            }
            else
            {
                coursePotion.y = range.y - 1;
            }
            result = 0;
            break;
        case KEY_RIGHT:
        case CONFIG::RIGHT:
            if (coursePotion.y < range.y - 1)
            {
                coursePotion.y++;
            }
            else
            {
                coursePotion.y = 0;
            }
            result = 0;
            break;
        case '\x0a':
        case KEY_ENTER:
        case CONFIG::OPEN:
            result = 1;
            break;
        case CONFIG::MARK:
            result = 2;
            break;
        case CONFIG::OPENRANGE:
            result = 3;
            break;
        default:
            result = -1;
            break;
        }
    } while (result < 0);
    return result;
}
void initColor()
{
    if (has_colors && CONFIG::colorful)
    {
        start_color();

        short background = 0;
        switch (CONFIG::C_Background)
        {
        case CONFIG::black:
            background = COLOR_BLACK;
            break;

        case CONFIG::red:
            background = COLOR_RED;
            break;
        case CONFIG::green:
            background = COLOR_GREEN;
            break;
        case CONFIG::yellow:
            background = COLOR_YELLOW;
            break;
        case CONFIG::blue:
            background = COLOR_BLUE;
            break;
        case CONFIG::magenta:
            background = COLOR_MAGENTA;
            break;
        case CONFIG::cyan:
            background = COLOR_CYAN;
            break;
        case CONFIG::white:
            background = COLOR_WHITE;
            break;
        default:
            break;
        }
        init_pair(CONFIG::black, COLOR_BLACK, background);
        init_pair(CONFIG::red, COLOR_RED, background);
        init_pair(CONFIG::green, COLOR_GREEN, background);
        init_pair(CONFIG::yellow, COLOR_YELLOW, background);

        init_pair(CONFIG::blue, COLOR_BLUE, background);
        init_pair(CONFIG::magenta, COLOR_MAGENTA, background);
        init_pair(CONFIG::cyan, COLOR_CYAN, background);
        init_pair(CONFIG::white, COLOR_WHITE, background);
    }
}

void setColor(CONFIG::color color)
{
    static CONFIG::color _color = CONFIG::none;
    if (!CONFIG::colorful || !has_colors())
    {
        return;
    }

    if (color == CONFIG::none)
    {
        attroff(COLOR_PAIR(_color));
    }
    else
    {
        attron(COLOR_PAIR(color));
        _color = color;
    }
}
