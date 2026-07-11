#pragma once

#include <FluWidget.h>
#include <FluScintilla.h>

class LogPage : public FluWidget 
{
    Q_OBJECT
public:
    LogPage(QWidget* parent = nullptr);
    
    void appendLog(QString text);

    void onThemeChanged();

protected:
    FluScintilla* m_logWindow;
};