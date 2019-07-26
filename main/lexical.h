#ifndef LEXICAL_H
#define LEXICAL_H

#include <QString>
#include <QDebug>

#define DEBUG 0

#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

#define LOOK_UP 5
#define LOOK_RIGHT 6
#define LOOK_DOWN 7
#define LOOK_LEFT 8

#define IF 9
#define THEN 10
#define ELSE 11
#define WHILE 12
#define ENDIF 13
#define WEND 14
#define ENDFOR 16
#define PRINT 17
#define INPUT 18
#define GOTO 19
#define FOR 20
#define NEXT 21
#define TO 22
#define GOSUB 23
#define RETURN 24
#define END 25
#define CLEAR 26

#define EOL 50
#define DELIMITER 51
#define STRING 52
#define VARIABLE 53
#define COMMAND 54
#define QUOTE 55
#define NUMBER 56

#define FINISHED 100

class lexical{

    QMap<QString, int> commands;
    QMap<QString, int> variables;

    QString _token;
    QChar _curChar;
    int _tokenType;

    QString prog;

    int cur = 0;

public:

    lexical() {

        commands["up"] = UP;
        commands["down"] = DOWN;
        commands["left"] = LEFT;
        commands["right"] = RIGHT;

        commands["lookUp"] = LOOK_UP;
        commands["lookDown"] = LOOK_DOWN;
        commands["lookLeft"] = LOOK_LEFT;
        commands["lookRight"] = LOOK_RIGHT;

        commands["IF"] = IF;
        commands["THEN"] = THEN;
        commands["ELSE"] = ELSE;
        commands["PRINT"] = PRINT;
        commands["INPUT"] = INPUT;
        commands["GOTO"] = GOTO;
        commands["GOSUB"] = GOSUB;
        commands["RETURN"] = RETURN;

        commands["FOR"] = FOR;
        commands["NEXT"] = NEXT;
        commands["TO"] = TO;

        commands["WHILE"] = WHILE;
        commands["WEND"] = WEND;

        commands["END"] = END;
        commands["CLEAR"] = CLEAR;

    }

    void setProg(QString prog) {
        this->prog = prog;
        reset();
    }

    // clears cursor position
    // and token
    void reset() {
        cur = 0;
        _curChar = prog.at(0);
        _tokenType = 0;
    }

    QString token() { return _token; }

    int tokenType() { return _tokenType; }

    bool tokenTypeExp() { return tokenType() == VARIABLE || tokenType() == DELIMITER || tokenType() == NUMBER; }

    void nextToken() {

        if(_curChar == '\0') {
            setToken("\n", FINISHED);
            if(DEBUG)qDebug() << "Token: FINISHED";
            return;
        }

        QString temp = "";
        _token = "";

        while(_curChar == ' ') { nextChar(); }

        if(_curChar == '\n' || _curChar == '\r') {
            setToken("\n", EOL);
            if(DEBUG)qDebug() << "Token: EOL";
            nextChar();
            return;
        }

        if(_curChar == '"') {
            nextChar();
            while (!QString("\"\r\n").contains(_curChar) && _curChar != '\0') {
                temp += _curChar;
                nextChar();
            }
            setToken(temp, STRING);
            if(DEBUG)qDebug() << "Token: " << token();
            nextChar();
            return;
        }

        if (QString("+-^/*%=;(),><").contains(_curChar)) {
            setToken(_curChar, DELIMITER);
            if(DEBUG)qDebug() << "Token: " << token();
            nextChar();
            return;
        }

        if (_curChar.isDigit()) {
            while (!is_delim(_curChar)) {
                temp += _curChar;
                nextChar();
            }
            setToken(temp, NUMBER);
            if(DEBUG)qDebug() << "Token: " << token();
            return;
        }

        if(_curChar.isLetter()) {

            while(!is_delim(_curChar)) {
                temp += _curChar;
                nextChar();
            }

            setToken(temp, STRING);
            if(DEBUG)qDebug() << "Token: " << token();
        }


        if (tokenType() == STRING) {
            _tokenType = commands[temp];
            if (!tokenType()) {
                if(token().at(0) == '"')
                    _tokenType = STRING;
                else
                    _tokenType = VARIABLE;
            }
        }

    }

    /* Поиск начала следующей строки */
    void findEOL() {
        do {
            nextToken();
        } while(tokenType() != EOL && tokenType() != FINISHED);
    }

    /* Вернуть токен назад */
    void returnBack() {
        if(DEBUG) qDebug() << "Return back";
        if(tokenType() == FINISHED) return;
        for(int i = token().length(); i > 0; --i)
            prevChar();
    }

    bool keyWord(QString* word) {
        QString wrd = word->remove('\n');
        return commands.keys().contains(wrd);
    }

    int getVariable(QString name) { return variables[name]; }

    void setVariable(QString name, int value) { variables[name] = value; }

    int getCursor() { return cur; }

    void setCursor(int cur) {
        this->cur = cur;

        if(cur > 0 && cur < prog.size()) {
            _curChar = prog.at(cur);
            _tokenType = 0;
        } else {
            _tokenType = FINISHED;
        }

    }

private:

    void nextChar() {
        ++cur;
        if(cur >= prog.size()) _curChar = '\0';
        else _curChar = prog.at(cur);
    }

    void prevChar() {
        --cur;
        if(cur < 0) _curChar = '\0';
        else _curChar = prog.at(cur);
    }

    void setToken(QString token, int tokenType) {
        this->_token = token;
        this->_tokenType = tokenType;
    }

    int is_white(char c) { return c == ' ' || c == '\t'; }

    int is_delim(QChar c) { return QString(" ;,+-<>/*%^=()\n\r").contains(c) || c == '\0'; }

};

#endif // LEXICAL_H
