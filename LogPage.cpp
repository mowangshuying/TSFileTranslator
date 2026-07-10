#include "LogPage.h"
#include <FluUtils.h>
#include <FluLabel.h>

LogPage::LogPage(QWidget *parent) : FluWidget(parent)
{
    auto vMainLayout = new QVBoxLayout(this);
    vMainLayout->setAlignment(Qt::AlignTop);
    setLayout(vMainLayout);
    vMainLayout->setContentsMargins(35, 35, 35, 35);

    // auto titleLabel = new FluLabel(FluLabelStyle::TitleTextBlockStyle, this);
    // titleLabel->setText("流程日志");
    // vMainLayout->addWidget(titleLabel);

    onThemeChanged();
    connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, this, [=](FluTheme theme){
        onThemeChanged();
    });
}

void LogPage::onThemeChanged()
{
    FluStyleSheetUitls::setQssByFileName("LogPage.qss", this, FluThemeUtils::getUtils()->getTheme());
}