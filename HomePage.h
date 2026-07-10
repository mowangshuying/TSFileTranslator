#pragma once

#include "FluVScrollView.h"


class HomePage : public FluVScrollView 
{
    Q_OBJECT
public:
    HomePage(QWidget* parent = nullptr);

    void onThemeChanged();
};