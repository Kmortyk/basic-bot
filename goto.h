#ifndef GOTO_H
#define GOTO_H

#include<QMap>
#include "main/lexical.h"

class goto_struct{

    QMap<QString, int> label_table;
    lexical* l;

public:

    void setLex(lexical* l) {
        this->l = l;
        scan_labels();
    }

    /* Поиск всех меток */
    void scan_labels() {

        label_table.clear();

        do {
            l->nextToken();
            if (l->tokenType() == NUMBER) {
                label_table[l->token()] = l->getCursor();
            }
            /* если строка не помечена, то поиск следующей */
            if (l->tokenType() != EOL) l->findEOL();
        } while (l->tokenType() != FINISHED);

        l->reset();

    }

    int find_label(QString label) {
        return label_table[label];
    }


};

#endif // GOTO_H
