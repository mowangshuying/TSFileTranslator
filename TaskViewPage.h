#pragma once
#include "___def.h"
#include <FluVScrollView.h>

class TaskViewPage : public FluVScrollView 
{
    Q_OBJECT
public:
    TaskViewPage(QWidget* parent = nullptr);
    
    void addTaskCard(TaskData taskData);

    void onThemeChanged();
protected:
};