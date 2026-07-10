#include "HomePage.h"
#include <FluUtils.h>

HomePage::HomePage(QWidget *parent)
{
    onThemeChanged();
    connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, this, [=](FluTheme theme){
        onThemeChanged();
    });
}


void HomePage::onThemeChanged()
{
    FluStyleSheetUitls::setQssByFileName("HomePage.qss", this, FluThemeUtils::getUtils()->getTheme());
}
