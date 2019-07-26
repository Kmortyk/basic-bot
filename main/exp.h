#ifndef EXP_H
#define EXP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <QString>
#include <QDebug>
#include "lexical.h"

class exp{

    lexical* l = nullptr;

public:

    void setLex(lexical* l) { this->l = l; }

    /* Точка входа в анализатор синтаксический анализатор алгебраических выражений*/
    void get_exp(int &result) {
        level2(result);
        l->returnBack();
    }

    /* Сложение или вычитание двух термов */
    void level2(int &result) {
        QChar op;
        int hold;

        level3(result);

        while (l->token() == '+' || l->token() == '-') {

            op = l->token().at(0);
            l->nextToken();

            level3(hold);

            arith(op, result, hold);

        }
    }

    /* Вычисление произведения или частного двух факторов */
    void level3(int &result) {
        QChar op;
        int hold;

        level4(result);

        while (l->token() == '*' || l->token() == '/' || l->token() == '%') {

            op = l->token().at(0);
            l->nextToken();

            level4(hold);

            arith(op, result, hold);

        }
    }

    /* Обработка степени числа (целочисленной) */
    void level4(int &result) {
        int hold;

        level5(result);

        if (l->token() == '^') {
            l->nextToken();
            level4(hold);
            arith('^', result, hold);
        }
    }

    /* Унарный + или - */
    void level5(int &result) {
        QChar op = '\0';

        if (l->tokenType() == DELIMITER && (l->token() == '+' || l->token() == '-')) {
            op = l->token().at(0);
            l->nextToken();
        }

        level6(result);
        if (op != '\0') { unary(op, result); }
    }

    /* Обработка выражения в круглых скобках */
    void level6(int &result) {
        if (l->token() == '(') {
            l->nextToken();
            level2(result);
            if (l->token().at(0) != ')')
                serror(1);
            l->nextToken();
        } else
            primitive(result);
    }

    /* Определение значения переменной по ее имени */
    void primitive(int &result) {
        switch (l->tokenType()) {
            case VARIABLE: // Если переменная, то определием ее значение
                result = l->getVariable(l->token());
                l->nextToken();
                return;
            case NUMBER: // Если это число, то приводим его с помощью функции atoi
                result = l->token().toInt();
                l->nextToken();
                return;
            default:
                serror(0);
        }
    }

    // Функция, которая выполняет операции в зависимости от знака, который мы в нее передаем
    void arith(QChar o, int &r, const int &h) {

        switch (o.unicode()) {
            case '-': r -= h;
                break;
            case '+': r += h;
                break;
            case '*': r *= h;
                break;
            case '/': r /= h;
                break;
            case '%': r %= h;
                break;
            case '^': {
                /* возедение в нулевую степень */
                if (h == 0) {
                    r = 1;
                    break;
                }

                int ex = r;
                for (int t = h - 1; t > 0; --t) r *= ex;
                break;
            }
        }

    }

    /* Изменение знака (используется в случае унарного минуса) */
    void unary(QChar o, int &r) {
        if (o == '-')  r = -r;
    }

    void serror(int error) {
        static const char *e[] = {
                "Syntax error",  // 0
                "Unpaired parentheses", // 1
                "It's not expression", // 2
                "The equality symbol is assumed.", // 3
                "Not an variable", // 4
                "Tag table full", // 5
                "Label duplication", // 6
                "Undefined label", // 7
                "THEN operator is need", // 8
                "TO operator is need", // 9
                "FOR loop nesting level is too large", // 10
                "NEXT does not match FOR", // 11
                "Nesting level of GOSUB too large", // 12
                "RETURN not matching GOSUB", // 13
                "Unknown expression statement" // 14
        };
        qDebug() << e[error] << " tok:" << l->token() << " tp:" << l->token();
    }

};

#endif // EXP_H
