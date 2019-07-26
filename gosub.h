#ifndef GOSUB_H
#define GOSUB_H

#define SUB_NEST 255

class gosub_struct{

    int gstack[SUB_NEST]; /* стек оператора GOSUB */
    int _gtos = 0; /* индекс начала стека GOSUB */

public:

    void gpush(int cur) {
        _gtos++;
        gstack[_gtos] = cur;
    }

    int gpop() {
        return gstack[_gtos--];
    }

    int gtos() { return _gtos; }

};

#endif // GOSUB_H
