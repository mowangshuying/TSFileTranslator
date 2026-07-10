#pragma once

#include <FluWidget.h>

class SettingsPage : public FluWidget 
{
    Q_OBJECT
public:
    SettingsPage(QWidget* parent = nullptr);
    
    void onThemeChanged();
};