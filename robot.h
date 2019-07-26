#ifndef ROBOT_H
#define ROBOT_H

#include <ctype.h>

class Robot{

public:

    size_t i, j;

    Robot(size_t i = 0, size_t j = 0) : i(i), j(j) { }

    void up()   { i -= 1; }
    void down() { i += 1; }

    void left()  { j -= 1; }
    void right() { j += 1; }

    void setMapPos(size_t i, size_t j) {
        this->i = i;
        this->j = j;
    }

};

#endif
