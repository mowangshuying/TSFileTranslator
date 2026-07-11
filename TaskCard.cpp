#include "TaskCard.h"
#include <QVBoxLayout>
#include <FluLabel.h>
#include <FluUtils.h>

TaskCard::TaskCard(QWidget *parent)
	: FluWidget(parent)
{
	auto vMainLayout = new QVBoxLayout(this);
	auto hFileLayout = new QHBoxLayout();
	vMainLayout->addLayout(hFileLayout);

	auto fileKeayLabel = new FluLabel(FluLabelStyle::BodyTextBlockStyle);
	fileKeayLabel->setText("文件名:");
    hFileLayout->addWidget(fileKeayLabel);
    // hFileLayout->addStretch()

    auto fileValueLabel = new FluLabel(FluLabelStyle::BodyTextBlockStyle);
	fileValueLabel->setText("*://******/******/******/***.**");
    // hFileLayout->addWidget(fileKeayLabel);
    hFileLayout->addWidget(fileValueLabel);
    hFileLayout->addStretch();

	/// 源语言
	auto hSourceLanguageLayout = new QHBoxLayout();
    vMainLayout->addLayout(hSourceLanguageLayout);

    auto sourceLanguageLabel = new FluLabel(FluLabelStyle::BodyTextBlockStyle);
    sourceLanguageLabel->setText("源语言:");
    hSourceLanguageLayout->addWidget(sourceLanguageLabel);

    auto sourceLanguageValueLabel = new FluLabel(FluLabelStyle::BodyTextBlockStyle);
    sourceLanguageValueLabel->setText("英文");
    hSourceLanguageLayout->addWidget(sourceLanguageValueLabel);

    hSourceLanguageLayout->addStretch();

	/// 目标语言
    auto hTargetLanguageLayout = new QHBoxLayout();
    vMainLayout->addLayout(hTargetLanguageLayout);

    auto targetLanguageLabel = new FluLabel(FluLabelStyle::BodyTextBlockStyle);
    targetLanguageLabel->setText("目标语言:");
    hTargetLanguageLayout->addWidget(targetLanguageLabel);

    auto targetLanguageValueLabel = new FluLabel(FluLabelStyle::BodyTextBlockStyle);
    targetLanguageValueLabel->setText("中文");
    hTargetLanguageLayout->addWidget(targetLanguageValueLabel);
    hTargetLanguageLayout->addStretch();

    /// 正在翻译...
    auto hTranslatingLayout = new QHBoxLayout();
    vMainLayout->addLayout(hTranslatingLayout);
    auto translatingLabel = new FluLabel(FluLabelStyle::BodyTextBlockStyle);
    translatingLabel->setText("正在翻译...");
    hTranslatingLayout->addWidget(translatingLabel);

    setFixedHeight(90);
    onThemeChanged();
    connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, this, [=](FluTheme theme){
        onThemeChanged();
    });
}

TaskCard::~TaskCard()
{

}

void TaskCard::onThemeChanged()
{
    FluStyleSheetUitls::setQssByFileName("TaskCard.qss", this, FluThemeUtils::getUtils()->getTheme());
}
