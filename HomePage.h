#pragma once
#include "___def.h"
#include <FluWidget.h>

class HomePage : public FluWidget 
{
    Q_OBJECT
private:
    HomePage(QWidget* parent = nullptr);

public:
    static HomePage* getPage();
signals:
    void clickedAddToTaskListButton(TaskData taskData);
public slots:
    void onThemeChanged();

protected:
    static HomePage* __page;
};