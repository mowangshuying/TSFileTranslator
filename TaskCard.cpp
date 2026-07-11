#include "TaskCard.h"
#include <QVBoxLayout>
#include <FluLabel.h>

TaskCard::TaskCard(QWidget *parent)
	: FluWidget(parent)
{
	auto vMainLayout = new QVBoxLayout(this);
	auto hFileLayout = new QHBoxLayout();
	vMainLayout->addLayout(hFileLayout);

	auto fileKeayLabel = new FluLabel(FluLabelStyle::BodyTextBlockStyle);
	fileKeayLabel->setText("文件名:");
    hFileLayout->addWidget(fileKeayLabel);

    auto fileValueLabel = new FluLabel(FluLabelStyle::BodyTextBlockStyle);
	fileValueLabel->setText("*://******/******/******/***.**");
    hFileLayout->addWidget(fileKeayLabel);
    hFileLayout->addWidget(fileValueLabel);

	/// 源语言
	auto hSourceLanguageLayout = new QHBoxLayout();
    vMainLayout->addLayout(hSourceLanguageLayout);

    auto sourceLanguageLabel = new FluLabel(FluLabelStyle::BodyTextBlockStyle);
    sourceLanguageLabel->setText("源语言:");
    hSourceLanguageLayout->addWidget(sourceLanguageLabel);

    auto sourceLanguageValueLabel = new FluLabel(FluLabelStyle::BodyTextBlockStyle);
    sourceLanguageValueLabel->setText("英文");
    hSourceLanguageLayout->addWidget(sourceLanguageValueLabel);

	/// 目标语言
    auto hTargetLanguageLayout = new QHBoxLayout();
    vMainLayout->addLayout(hTargetLanguageLayout);

    auto targetLanguageLabel = new FluLabel(FluLabelStyle::BodyTextBlockStyle);
    targetLanguageLabel->setText("目标语言:");
    hTargetLanguageLayout->addWidget(targetLanguageLabel);

    auto targetLanguageValueLabel = new FluLabel(FluLabelStyle::BodyTextBlockStyle);
    targetLanguageValueLabel->setText("中文");
    hTargetLanguageLayout->addWidget(targetLanguageValueLabel);

    /// 正在翻译...
    auto hTranslatingLayout = new QHBoxLayout();
    vMainLayout->addLayout(hTranslatingLayout);
    auto translatingLabel = new FluLabel(FluLabelStyle::BodyTextBlockStyle);
    translatingLabel->setText("正在翻译...");
}

TaskCard::~TaskCard()
{

}

