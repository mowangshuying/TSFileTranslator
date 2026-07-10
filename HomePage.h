#pragma once

#include <FluWidget.h>  

class HomePage : public FluWidget 
{
    Q_OBJECT
public:
    HomePage(QWidget* parent = nullptr);

    void onThemeChanged();
};