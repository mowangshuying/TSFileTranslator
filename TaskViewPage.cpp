#include "TaskViewPage.h"

TaskViewPage::TaskViewPage(QWidget *parent) : FluVScrollView(parent)
{
    onThemeChanged();
    connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, this, [=](FluTheme theme) {
        onThemeChanged();    
    });
}

void TaskViewPage::onThemeChanged()
{
    FluStyleSheetUitls::setQssByFileName("TaskViewPage.qss", this, FluThemeUtils::getUtils()->getTheme());
}