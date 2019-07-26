#include "console.h"
#include <QTextEdit>
#include <QScrollBar>
#include <QDebug>

Console::Console(QTextEdit* textEdit) : textEdit(textEdit) {
    textEdit->clear();
}

void Console::printMessage(QString message) {
    qDebug() << message;
    if(cur == 0)
        textEdit->setText(message);
    else {
        textEdit->setText(textEdit->toPlainText() + '\n' + message);
        cur += 1;
    }
    cur += message.length();
    scrollToBottom();
}

void Console::scrollToBottom() {
    textEdit->verticalScrollBar()->setValue(textEdit->verticalScrollBar()->maximum());
}

void Console::clear() {
    textEdit->setText("");
    cur = 0;
}

QString Console::fromBool(bool b) {
    if(b) return "true";
    else return "false";
}
