#pragma once

#include <FluVScrollView.h>

class TaskViewPage : public FluVScrollView 
{
    Q_OBJECT
public:
    TaskViewPage(QWidget* parent = nullptr);
    
    void onThemeChanged();
};