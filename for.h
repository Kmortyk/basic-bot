#ifndef FOR_H
#define FOR_H

#define FOR_NEST 255 // максимальная вложенность

#include <QDebug>

struct while_struct{
    int cond_cur; // условие
};


struct for_stack{
    QString var; // счетчик цикла
    int max; // конечное значение
    int cur;
};

struct for_struct {

    // for
    for_stack fstack[FOR_NEST];
    int ftos = 0; // индекс начала стека

    void fpush(for_stack i) {
       if (ftos > FOR_NEST)
            qDebug() << "For nest greater than max value.";
       fstack[ftos++]=i;
    }

    struct for_stack fpop() {
       if (--ftos < 0) qDebug() << "For nest count error.";
       return(fstack[ftos]);
    }


};

#endif // FOR_H
