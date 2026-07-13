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
#include <FluMessageBox.h>

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
    m_homeNavItem  = new FluVNavigationIconTextItem(FluAwesomeType::Home, "Home", this);
    m_homeNavItem->setKey("HomePage");
    m_navView->addItemToMidLayout(m_homeNavItem);
    m_homePage = new HomePage(this);
    m_sLayout->addWidget("HomePage", m_homePage);
    connect(m_homeNavItem, &FluVNavigationIconTextItem::itemClicked, this, [=](){
        m_sLayout->setCurrentWidget("HomePage");
    });

    /// taskviewpage;
    m_taskViewNavItem  = new FluVNavigationIconTextItem(FluAwesomeType::TaskView, "TaskView", this);
    m_taskViewNavItem->setKey("TaskViewPage");
    m_navView->addItemToMidLayout(m_taskViewNavItem);
    
    m_taskViewPage = new TaskViewPage(this);
    m_sLayout->addWidget("TaskViewPage", m_taskViewPage);
    connect(m_taskViewNavItem, &FluVNavigationIconTextItem::itemClicked, this, [=](){
        m_sLayout->setCurrentWidget("TaskViewPage");
    });


    /// logpage;
    m_logNavItem  = new FluVNavigationIconTextItem(FluAwesomeType::Process, "Log", this);
    m_logNavItem->setKey("LogPage");
    m_navView->addItemToMidLayout(m_logNavItem);
    
    m_logPage = new LogPage(this);
    m_sLayout->addWidget("LogPage", m_logPage);
    connect(m_logNavItem, &FluVNavigationIconTextItem::itemClicked, this, [=](){
        m_sLayout->setCurrentWidget("LogPage");
    });


    /// settingspage;
    m_settingsNavItem  = new FluVNavigationIconTextItem(FluAwesomeType::Settings, "Settings", this);
    m_settingsNavItem->setKey("SettingsPage");
    m_navView->addItemToBottomLayout(m_settingsNavItem);
    
    m_settingsPage = new SettingsPage(this);
    m_sLayout->addWidget("SettingsPage", m_settingsPage);
    connect(m_settingsNavItem, &FluVNavigationIconTextItem::itemClicked, this, [=](){
        m_sLayout->setCurrentWidget("SettingsPage");
    });



    m_contentLayout->addWidget(m_navView);
    m_contentLayout->addLayout(m_sLayout);
    
    
    // m_navView->collapseView();
    m_navView->onMenuItemClicked();

    __log("TSWindow 窗口初始化完成!");

    connect(m_homePage, &HomePage::clickedAddToTaskListButton, this, &TSWindow::onClickedAddToTaskListButton);
    onThemeChanged();
    connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, this, &TSWindow::onThemeChanged);
}

LogPage *TSWindow::getLogPage()
{
    // return nullptr;
    return qobject_cast<LogPage *>(m_sLayout->getWidget("LogPage"));
}

void TSWindow::__log(QString text)
{
    if (getLogPage() == nullptr)
        return;
    getLogPage()->appendLog(text);
}

void TSWindow::onClickedAddToTaskListButton(TaskData taskData)
{
    emit m_taskViewNavItem->itemClicked();
    m_taskViewPage->addTaskCard(taskData);
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

void TSWindow::closeEvent(QCloseEvent* event)
{
    FluMessageBox messageBox("TS文件转换器(Qt)", "是否关闭窗口", this);
    int nExec = messageBox.exec();
    if (nExec == QDialog::Rejected)
    {
        event->ignore();
        return;
    }
    else if (nExec == QDialog::Accepted)
    {
        // event->accept();
        QApplication::quit();
    }
}
