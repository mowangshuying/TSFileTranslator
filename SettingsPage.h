#pragma once

#include <FluWidget.h>

class SettingsPage : public FluWidget 
{
    Q_OBJECT
private:
    SettingsPage(QWidget* parent = nullptr);
public:
    static SettingsPage* getPage();

    void onThemeChanged();
protected:
    static SettingsPage* __page;
};