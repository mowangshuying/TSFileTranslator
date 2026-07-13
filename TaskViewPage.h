#pragma once
#include "___def.h"
#include <FluVScrollView.h>

class TaskViewPage : public FluVScrollView 
{
    Q_OBJECT
private:
    TaskViewPage(QWidget* parent = nullptr);
   
public:
    static TaskViewPage* getPage();

    int getTaskId();

    void addTaskCard(TaskData taskData);

    void onThemeChanged();
protected:
    static TaskViewPage* __page;
};