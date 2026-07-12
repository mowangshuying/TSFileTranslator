#pragma once
#include "___def.h"
#include <FluFrameLessWidget.h>
#include <FluVNavigationView.h>
#include <FluStackedLayout.h>

class LogPage;
class TSWindow : public FluFrameLessWidget
{
    Q_OBJECT
public:
    TSWindow(QWidget* parent = nullptr);

    void __initUI();


    LogPage* getLogPage();
    void __log(QString text);

// public slots:
    void onClickedAddToTaskListButton(TaskData taskData);
    void onThemeChanged();
protected:
    FluVNavigationView* m_navView;
    FluStackedLayout* m_sLayout;
};