#include "SettingsPage.h"
#include <FluUtils.h>
#include <FluLabel.h>
#include <FluSettingsSelectBox.h>
#include <FluSettingsVersionBox.h>
#include <FluHyperLinkButton.h>

SettingsPage* SettingsPage::__page = nullptr;
SettingsPage::SettingsPage(QWidget *parent) : FluWidget(parent)
{
    auto vMainLayout = new QVBoxLayout(this);
    vMainLayout->setAlignment(Qt::AlignTop);
    setLayout(vMainLayout);
    vMainLayout->setContentsMargins(35, 35, 35, 35);

    auto titleLabel = new FluLabel(FluLabelStyle::TitleTextBlockStyle, this);
    titleLabel->setText("设置");
    vMainLayout->addWidget(titleLabel);

    auto appearanceLabel = new FluLabel(FluLabelStyle::BodyStrongTextBlockStyle, this);
    appearanceLabel->setText("外观");
    vMainLayout->addWidget(appearanceLabel);

    auto appThemeSelectBox = new FluSettingsSelectBox;
    appThemeSelectBox->setTitleInfo("应用主题", "选择要显示的应用主题");
    appThemeSelectBox->setIcon(FluAwesomeType::Color);
    appThemeSelectBox->getComboBox()->addItem("浅色");
    appThemeSelectBox->getComboBox()->addItem("深色");
    appThemeSelectBox->getComboBox()->addItem("暗色");
     connect(appThemeSelectBox->getComboBox(), &FluComboBoxEx::currentIndexChanged, [=](int index) {
        if (index == (int)FluThemeUtils::getUtils()->getTheme())
            return;

        if (index == 0)
            FluThemeUtils::getUtils()->setTheme(FluTheme::Light);
        else if (index == 1)
            FluThemeUtils::getUtils()->setTheme(FluTheme::Dark);
        else
            FluThemeUtils::getUtils()->setTheme(FluTheme::AtomOneDark);
    });

    vMainLayout->addWidget(appThemeSelectBox);


    auto abboutLabel = new FluLabel(FluLabelStyle::BodyStrongTextBlockStyle, this);
    abboutLabel->setText("关于");
    vMainLayout->addWidget(abboutLabel);


    auto settingsVersionBox = new FluSettingsVersionBox;
    settingsVersionBox->getTitleLabel()->setText("TS文件转换器(Qt)");
    settingsVersionBox->getInfoLabel()->setText("@2026 TS文件转换器(Qt).  版权所有");
    settingsVersionBox->getVersionLabel()->setText("V1.0.4");

    auto repoLabel = new FluLabel;
    repoLabel->setLabelStyle(FluLabelStyle::BodyTextBlockStyle);
    repoLabel->setText("仓库地址:");
    auto repoLinkRef = new FluHyperLinkButton("https://github.com/mowangshuying/TSFileTranslator");
    repoLinkRef->setText("https://github.com/mowangshuying/TSFileTranslator");
    
    auto repoLayout = new QHBoxLayout;
    repoLayout->addWidget(repoLabel);
    repoLayout->addWidget(repoLinkRef);

    auto repoWidget = new FluWidget;
    repoWidget->setLayout(repoLayout);

    settingsVersionBox->addWidget(repoWidget);

    QIcon icon("./App.png");
    settingsVersionBox->getIconLabel()->setPixmap(icon.pixmap(20, 20));
    settingsVersionBox->getIconLabel()->setFixedSize(40, 40);
    vMainLayout->addWidget(settingsVersionBox, 0, Qt::AlignTop);


    onThemeChanged();
    connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, this, [=](FluTheme theme){
        onThemeChanged();
    });
}

SettingsPage* SettingsPage::getPage()
{
    if (__page == nullptr)
        __page = new SettingsPage();
    return __page;
}

void SettingsPage::onThemeChanged()
{
    FluStyleSheetUitls::setQssByFileName("SettingsPage.qss", this, FluThemeUtils::getUtils()->getTheme());
}
