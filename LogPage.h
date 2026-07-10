#pragma once

#include <FluWidget.h>

class LogPage : public FluWidget 
{
    Q_OBJECT
public:
    LogPage(QWidget* parent = nullptr);
    
    void onThemeChanged();
};