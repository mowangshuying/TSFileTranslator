#include "OutputWindow.h"

OutputWindow::OutputWindow(QWidget *parent) : FluScintilla(parent)
{
    setMarginLineNumbers(0, false);
    setMarginWidth(0, 0);
    setReadOnly(true);
}

void OutputWindow::append(QString text)
{
    // timestamp level text
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    text = QString("[%1] %2").arg(timestamp, text);
    FluScintilla::append(text);
}