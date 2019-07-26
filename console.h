#ifndef CONSOLE_H
#define CONSOLE_H
#include <QString>
#include <QTextEdit>

class Console
{

private:

    QTextEdit* textEdit;
    int cur = 0;

public:

    Console(QTextEdit* textEdit);
    void printMessage(QString message);
    void clear();
    void scrollToBottom();
    static QString fromBool(bool b);

};

#endif // CONSOLE_H
