#ifndef MINESWEEPER__CONFIG_HPP_
#define MINESWEEPER__CONFIG_HPP_

namespace CONFIG {
    const char Covered = 'X';
    const char Exploded = '*';
    const char Marked = '#';
    const char WrongMark = '\\';
    const char RightMark = '#';
    const char Mine = '@';
    const char Space = ' ';
    const char Hint = '>';

    // key
    const char UP = 'w';
    const char DOWN = 's';
    const char LEFT = 'a';
    const char RIGHT = 'd';
    const char OPEN = 'o';
    const char MARK = 'm';
    const char OPENRANGE = ' ';
    // color
    enum color
    {
        none,
        black,
        red,
        green,
        yellow,
        blue,
        magenta,
        cyan,
        white
        // grey
    };

    const bool colorful = true;

    const color C_Covered = none;
    const color C_Marked = yellow;
    const color C_Exploded = red;
    const color C_Space = green;

    const color C_WrongMark = red;
    const color C_RightMark = yellow;
    const color C_Mine = red;
    const color C_NoNumberSpace = cyan;
    const color C_Background = black;
    const color C_Hint = blue;
}

#endif