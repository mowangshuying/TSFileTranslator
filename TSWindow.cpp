#include "TSWindow.h"
#include <FluUtils.h>
#include <FluThemeButton.h>
#include <FramelessHelper/Core/framelessmanager.h>
#include <FramelessHelper/Widgets/framelesswidgetshelper.h>
#include <FramelessHelper/Widgets/standardsystembutton.h>
#include <FramelessHelper/Widgets/standardtitlebar.h>
#include "HomePage.h"
#include <FluVNavigationIconTextItem.h>
#include "SettingsPage.h"
#include "TaskViewPage.h"
#include "LogPage.h"

FRAMELESSHELPER_USE_NAMESPACE
TSWindow::TSWindow(QWidget *parent/*= nullptr */) : FluFrameLessWidget(parent)
{
    __initUI();
}

void TSWindow::__initUI()
{
    setWindowTitle("TS文件转换器(Qt)");
    setWindowIcon(QIcon("./App.png"));

    m_titleBar->chromePalette()->setTitleBarActiveBackgroundColor(Qt::transparent);
    m_titleBar->chromePalette()->setTitleBarInactiveBackgroundColor(Qt::transparent);
    m_titleBar->chromePalette()->setTitleBarActiveForegroundColor(Qt::black);
    m_titleBar->chromePalette()->setTitleBarInactiveForegroundColor(Qt::black);
    m_titleBar->setFixedHeight(36);

#ifndef Q_OS_MACOS
    auto hLayout = (QHBoxLayout *)m_titleBar->layout();
    auto vLayout = (QVBoxLayout *)hLayout->itemAt(1)->layout();
    auto hButtonLayout = (QHBoxLayout *)vLayout->itemAt(0)->layout();
    auto themeButton = new FluThemeButton;
    hButtonLayout->insertWidget(0, themeButton);
    FramelessWidgetsHelper::get(this)->setHitTestVisible(themeButton);
#endif


    m_navView = new FluVNavigationView(this);
    // m_navView->collapseView();
    m_navView->hideMenuItem();
    m_navView->hideSearchItem();
    m_sLayout = new FluStackedLayout;


    /// homepage;
    auto homeNavItem  = new FluVNavigationIconTextItem(FluAwesomeType::Home, "Home", this);
    homeNavItem->setKey("HomePage");
    m_navView->addItemToMidLayout(homeNavItem);
    auto homePage = new HomePage(this);
    m_sLayout->addWidget("HomePage", homePage);
    connect(homeNavItem, &FluVNavigationIconTextItem::itemClicked, this, [=](){
        m_sLayout->setCurrentWidget("HomePage");
    });

    /// taskviewpage;
    auto taskviewNavItem  = new FluVNavigationIconTextItem(FluAwesomeType::TaskView, "TaskView", this);
    taskviewNavItem->setKey("TaskViewPage");
    m_navView->addItemToMidLayout(taskviewNavItem);
    auto taskviewPage = new TaskViewPage(this);
    m_sLayout->addWidget("TaskViewPage", taskviewPage);
    connect(taskviewNavItem, &FluVNavigationIconTextItem::itemClicked, this, [=](){
        m_sLayout->setCurrentWidget("TaskViewPage");
    });


    /// logpage;
    auto logNavItem  = new FluVNavigationIconTextItem(FluAwesomeType::Process, "Log", this);
    logNavItem->setKey("LogPage");
    m_navView->addItemToMidLayout(logNavItem);
    auto logPage = new LogPage(this);
    m_sLayout->addWidget("LogPage", logPage);
    connect(logNavItem, &FluVNavigationIconTextItem::itemClicked, this, [=](){
        m_sLayout->setCurrentWidget("LogPage");
    });


    /// settingspage;
    auto settingsNavItem  = new FluVNavigationIconTextItem(FluAwesomeType::Settings, "Settings", this);
    settingsNavItem->setKey("SettingsPage");
    m_navView->addItemToBottomLayout(settingsNavItem);
    auto settingsPage = new SettingsPage(this);
    m_sLayout->addWidget("SettingsPage", settingsPage);
    connect(settingsNavItem, &FluVNavigationIconTextItem::itemClicked, this, [=](){
        m_sLayout->setCurrentWidget("SettingsPage");
    });



    m_contentLayout->addWidget(m_navView);
    m_contentLayout->addLayout(m_sLayout);
    
    
    // m_navView->collapseView();
    m_navView->onMenuItemClicked();
    onThemeChanged();
    connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, this, &TSWindow::onThemeChanged);
}


void TSWindow::onThemeChanged()
{
    if (FluThemeUtils::isLightTheme())
    {
        m_titleBar->chromePalette()->setTitleBarActiveBackgroundColor(Qt::transparent);
        m_titleBar->chromePalette()->setTitleBarInactiveBackgroundColor(Qt::transparent);
        m_titleBar->chromePalette()->setTitleBarActiveForegroundColor(Qt::black);
        m_titleBar->chromePalette()->setTitleBarInactiveForegroundColor(Qt::black);
#ifndef Q_OS_MACOS
        m_titleBar->minimizeButton()->setActiveForegroundColor(Qt::black);
        m_titleBar->closeButton()->setActiveForegroundColor(Qt::black);
        m_titleBar->maximizeButton()->setActiveForegroundColor(Qt::black);
#endif
        m_titleBar->show();
    }
    else if (FluThemeUtils::isDarkTheme() || FluThemeUtils::isAtomOneDarkTheme())
    {
        m_titleBar->chromePalette()->setTitleBarActiveBackgroundColor(Qt::transparent);
        m_titleBar->chromePalette()->setTitleBarInactiveBackgroundColor(Qt::transparent);
        m_titleBar->chromePalette()->setTitleBarActiveForegroundColor(Qt::white);
        m_titleBar->chromePalette()->setTitleBarInactiveForegroundColor(Qt::white);
#ifndef Q_OS_MACOS
        m_titleBar->minimizeButton()->setActiveForegroundColor(Qt::white);
        m_titleBar->closeButton()->setActiveForegroundColor(Qt::white);
        m_titleBar->maximizeButton()->setActiveForegroundColor(Qt::white);
#endif
        m_titleBar->show();
    }

    FluStyleSheetUitls::setQssByFileName("TSWindow.qss", this, FluThemeUtils::getUtils()->getTheme());
}
