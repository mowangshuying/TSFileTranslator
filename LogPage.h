#pragma once

#include <FluWidget.h>
#include <FluTextEdit.h>

class LogPage : public FluWidget 
{
    Q_OBJECT
private:
    LogPage(QWidget* parent = nullptr);
public:
    static LogPage* getPage();

    void appendLog(QString text, bool isError = false);

    void onThemeChanged();

protected:
    FluTextEdit* m_logWindow;
    static LogPage* __page;
};