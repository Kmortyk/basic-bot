#ifndef PROG_H
#define PROG_H

#include "robotmap.h"
#include "console.h"
#include "for.h"
#include "gosub.h"
#include "goto.h"
#include "exp.h"
#include <iostream>
#include <stdio.h>
#include <QMap>
#include <QMainWindow>
#include <QThread>
#include <QTimer>
#include <QStack>

class Prog{

    QMainWindow* mw;
    RobotMap* map;
    Console* console;

    goto_struct gstruct;
    gosub_struct gsbstruct;
    for_struct fstruct;

    QStack<while_struct> while_stack;

    lexical* l;
    exp exp;


public:

    Prog(QMainWindow* mw, RobotMap* map, Console* console) : mw(mw), map(map), console(console) {
        l = new lexical;
    }

    bool keyWord(QString* word) { return l->keyWord(word); }

    void interpret(QString prog) {

        l->setProg(prog);
        exp.setLex(l);
        // gstruct.setLex(l);

        while(l->tokenType() != FINISHED && l->tokenType() != END) {

            l->nextToken();
            mw->update();
            processToken(l->tokenType());

            if(!map->isRobotAlive()) {
                console->printMessage("Robot is destroyed!");
                return;
            }

        }

    }

private:

    void processToken(int tokenType) {
        switch (tokenType) {
        case VARIABLE:
            processAssignment();
            break;
        case UP:
            map->moveRobotUp();
            break;
        case RIGHT:
            map->moveRobotRight();
            break;
        case DOWN:
            map->moveRobotDown();
            break;
        case LEFT:
            map->moveRobotLeft();
            break;
        case IF:
            processIf();
            break;
        case WHILE:
            processWhile();
            break;
        case WEND:
            processWend();
            break;
        case FOR:
            processFor();
            break;
        case NEXT:
            processNext();
            break;
        case PRINT:
            processPrint();
            break;
        case INPUT:
            processInput();
            break;
        case GOTO:
            processGoto();
            break;
        case GOSUB:
            processGosub();
            break;
        case RETURN:
            processGosubReturn();
            break;
        case LOOK_UP:
            console->printMessage("Сверху свободно = " + Console::fromBool(map->topIsFree()));
            break;
        case LOOK_DOWN:
            console->printMessage("Снизу свободно = " + Console::fromBool(map->bottomIsFree()));
            break;
        case LOOK_LEFT:
            console->printMessage("Слева свободно = " + Console::fromBool(map->leftIsFree()));
            break;
        case LOOK_RIGHT:
            console->printMessage("Справа свободно = " + Console::fromBool(map->rightIsFree()));
            break;
        case CLEAR:
            console->clear();
            break;
        case END: // watch! return
            return;
        }
    }

    void processAssignment() {

        QString name = l->token();
        l->nextToken(); // =

        int val;
        l->nextToken();
        exp.get_exp(val);

        l->setVariable(name, val);

    }

    bool processCondition() {
        bool result = false;
        l->nextToken();

        if(l->tokenType() == NUMBER || l->tokenType() == VARIABLE ||
                l->tokenType() == DELIMITER) { // DELIMITER for ()

            int x = 0, y = 0;

            exp.get_exp(x); /* получить левое выражение */

            l->nextToken();
            if(l->tokenType() != DELIMITER) { // if only one sub-expression
                l->returnBack();
                if(x) result = true;
                return result;
            }
            QChar op = l->token().at(0);

            l->nextToken();
            exp.get_exp(y);  /* получить правое выражение */

            //console->printMessage("x= " + QString(x) + "y= " + QString(y));

            /* Определение результата */
            switch (op.unicode()) {
            case '=':
                if (x == y) result = true;
                break;
            case '<':
                if (x < y) result = true;
                break;
            case '>':
                if (x > y) result = true;
                break;    
            default:
                console->printMessage("Condition error: no operator.");
                break;
            }

        } else {

            switch (l->tokenType()) {
            case LOOK_UP:
                result = map->topIsFree();
                break;
            case LOOK_DOWN:
                result = map->bottomIsFree();
                break;
            case LOOK_LEFT:
                result = map->leftIsFree();
                break;
            case LOOK_RIGHT:
                result = map->rightIsFree();
                break;
            default:
                console->printMessage("Wrong operation: " + l->token());
            }

        }
        return result;
    }

    void processIf() {

        bool cond = processCondition();
        if (cond) {  /* если значение IF "истина"  */
            l->nextToken();
            if (l->tokenType() != THEN) {
                console->printMessage("Syntax error: no THEN operator");
                return;
            }

            /* иначе, программа выполняется со следующей строки */
            while(l->tokenType() != EOL && l->tokenType() != FINISHED) {
                l->nextToken();
                processToken(l->tokenType());
            }

        } else { l->findEOL(); /* поиск точки старта программы */ }

        l->nextToken();
        if(l->tokenType() == ELSE) {
            if(cond) { l->findEOL(); }
            else{
                /* nothing, process program as usually */
            }
        } else { l->returnBack(); }

    }

    void processPrint() {

        QString last_delim;
        QString res = "";

        do {

            l->nextToken(); /* получить следующий элемент списка */
            if (l->tokenType() == EOL || l->tokenType() == FINISHED) { break; }
            if (l->tokenType() == STRING) { /* это строка */
                res += l->token();
            } else { /* это выражение */
                int answer = 0;
                exp.get_exp(answer);
                res += QString::number(answer);
            }

            l->nextToken(); /* разделитель */
            last_delim = l->token();

            if      (last_delim == ';') { res += " "; }
            else if (last_delim == ',') { /* ничего не делать */ }
            else if (l->tokenType() != EOL && l->tokenType() != FINISHED) {
                l->returnBack(); // next operator
                break;
            }

        } while (l->token() == ';' || l->token() == ',');

        console->printMessage(res);

    }

    void processInput() {

        int i;

        l->nextToken(); /*просматривается если существует строка символов*/

        if (l->tokenType() == STRING) {
            console->printMessage(l->token());
            l->nextToken(); // ,
            l->nextToken();
        } else console->printMessage("? ");
\
        scanf("%d", &i);
        l->setVariable(l->token(), i);

    }

    void processGoto() {

        l->nextToken();
        QString label = l->token();

        int c = gstruct.find_label(label);
        l->setCursor(c);

    }

    void processFor() {

        struct for_stack i;
        int value;

        l->nextToken();
        i.var = l->token();

        l->nextToken(); // =

        l->nextToken();
        exp.get_exp(value);
        l->setVariable(i.var, value);

        l->nextToken(); // TO
        l->nextToken();
        exp.get_exp(i.max);

        /* Если цикл выполняется последний раз, поместить
       информацию в стек  */
        if (value >= l->getVariable(i.var)) {
            i.cur = l->getCursor();
            fstruct.fpush(i);
        } else /* пропустить весь цикл */
            while (l->tokenType() != NEXT) l->nextToken();

    }

    void processNext() {

        struct for_stack i = fstruct.fpop(); /* чтение информации о цикле */

        l->setVariable(i.var, l->getVariable(i.var)+1);
        if ( l->getVariable(i.var) > i.max) return; /* конец цикла */

        fstruct.fpush(i); /* иначе, сохранить информацию в стеке */
        l->setCursor(i.cur);

    }

    void processWhile() {
        // save condition
        struct while_struct ws;
        ws.cond_cur = l->getCursor();

        bool result = processCondition();

        if(result) {
            while_stack.push(ws);
        }else // condition is false
          while (l->tokenType() != WEND && l->tokenType() != FINISHED)
              l->nextToken();
    }

    void processWend() {

        struct while_struct ws = while_stack.pop(); /* чтение информации о цикле */

        int wend_cur = l->getCursor();  // save current cursor (after wend)
        l->setCursor(ws.cond_cur); // move to condition

        bool result = processCondition();

        if(result) {
            while_stack.push(ws);
        } else {
            l->setCursor(wend_cur);
            l->nextToken();
        }

    }

    void processGosub() {

        l->nextToken();
        /* поиск метки вызова */
        int cur = gstruct.find_label(l->token());
        gsbstruct.gpush(l->getCursor());

        l->setCursor(cur);

    }

    void processGosubReturn() {
        if(gsbstruct.gtos() == 0) return;
        l->setCursor(gsbstruct.gpop());
    }

};

#endif
