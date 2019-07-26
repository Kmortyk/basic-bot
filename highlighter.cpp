#include "highlighter.h"
#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bIF\\b" << "\\bTHEN\\b" << "\\bELSE\\b"
                    << "\\bFOR\\b" << "\\bWHILE\\b"
                    << "\\bPRINT\\b" << "\\bINPUT\\b" << "\\bGOTO\\b"
                    << "\\bFOR\\b" << "\\bNEXT\\b" << "\\bTO\\b" << "\\bGOSUB\\b"
                    << "\\bRETURN\\b"<< "\\bEND\\b" << "\\bWHILE\\b" << "\\bWEND\\b"
                    << "\\bCLEAR\\b";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    quotationFormat.setForeground(Qt::darkGreen);
    //rule.pattern = QRegularExpression("\".*\"");
    rule.pattern = QRegularExpression("\"(\\.|[^\"])*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);
}
