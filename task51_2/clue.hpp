#ifndef CLUE_HPP
#define CLUE_HPP

#include <string>

//Структура для зберігання підказки (числа зі стрілкою)
struct Clue {
    int row;
    int col;
    int count;
    std::string direction;

    Clue(int r, int c, int cnt, std::string dir) : row(r), col(c), count(cnt), direction(dir) {}
};

#endif // CLUE_HPP
