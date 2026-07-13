#pragma once
#include "___def.h"
#include <FluFrameLessWidget.h>
#include <FluVNavigationView.h>
#include <FluStackedLayout.h>

class HomePage;
class TaskViewPage;
class LogPage;
class SettingsPage;
class FLuVNavigationIconTextItem;

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

    HomePage* m_homePage;
    TaskViewPage* m_taskViewPage;
    LogPage* m_logPage;
    SettingsPage* m_settingsPage;

    FluVNavigationIconTextItem* m_homeNavItem;
    FluVNavigationIconTextItem* m_taskViewNavItem;
    FluVNavigationIconTextItem* m_logNavItem;
    FluVNavigationIconTextItem* m_settingsNavItem;
};