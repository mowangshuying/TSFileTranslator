#pragma once

#include <FluWidget.h>
#include <FluScintilla.h>

class LogPage : public FluWidget 
{
    Q_OBJECT
private:
    LogPage(QWidget* parent = nullptr);
public:
    static LogPage* getPage();

    void appendLog(QString text);

    void onThemeChanged();

protected:
    FluScintilla* m_logWindow;
    static LogPage* __page;
};