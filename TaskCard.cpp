#include "TaskCard.h"
#include <QVBoxLayout>
#include <FluLabel.h>
#include <FluUtils.h>
#include "TaskButton.h"

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

    auto languageLayout = new QHBoxLayout;
    vMainLayout->addLayout(languageLayout);
    auto languageKeyLabel = new FluLabel;
    languageKeyLabel->setLabelStyle(FluLabelStyle::BodyTextBlockStyle);
    languageKeyLabel->setText("语言:");
    languageLayout->addWidget(languageKeyLabel);

    auto languageValueLabel = new FluLabel;
    languageValueLabel->setLabelStyle(FluLabelStyle::BodyTextBlockStyle);
    languageValueLabel->setText("英文 => 中文");
    languageLayout->addWidget(languageValueLabel);
    languageLayout->addStretch();



    /// 正在翻译...
    // disable color rgb(126, 126, 126)
    auto hTranslatingLayout = new QHBoxLayout();
    vMainLayout->addLayout(hTranslatingLayout);
    auto translatingLabel = new FluLabel(FluLabelStyle::BodyTextBlockStyle);
    translatingLabel->setText("正在翻译...");
    hTranslatingLayout->addWidget(translatingLabel);

    auto hToolButtonLayout = new QHBoxLayout();
    vMainLayout->addLayout(hToolButtonLayout);


    m_startButton = new TaskButton(TaskButtonType::Start);
    m_startButton->setEnabled(true);
    m_startButton->setToolTip("开始翻译...");
    hToolButtonLayout->addWidget(m_startButton);


    m_pauseButton = new TaskButton(TaskButtonType::pause);
    m_pauseButton->setEnabled(false);
    m_pauseButton->setToolTip("暂停");
    hToolButtonLayout->addWidget(m_pauseButton);

    m_continueButton  = new TaskButton(TaskButtonType::Continue);
    m_continueButton->setEnabled(false);
    m_continueButton->setToolTip("继续");
    hToolButtonLayout->addWidget(m_continueButton);

    m_stopButton = new TaskButton(TaskButtonType::Stop);
    m_stopButton->setEnabled(false);
    m_stopButton->setToolTip("停止");
    hToolButtonLayout->addWidget(m_stopButton);

    hToolButtonLayout->addStretch();

    setFixedHeight(90);


    connect(m_startButton, &TaskButton::clicked, this, &TaskCard::onClickedStartButton);
    connect(m_pauseButton, &TaskButton::clicked, this, &TaskCard::onClickedPauseButton);
    connect(m_continueButton, &TaskButton::clicked, this, &TaskCard::onClickedContinueButton);
    connect(m_stopButton, &TaskButton::clicked, this, &TaskCard::onClickedStopButton);

    onThemeChanged();
    connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, this, [=](FluTheme theme){
        onThemeChanged();
    });
}

TaskCard::~TaskCard()
{

}

void TaskCard::onClickedStartButton()
{

    /// set state;
    m_startButton->setEnabled(false);
    m_pauseButton->setEnabled(true);
    m_continueButton->setEnabled(false);
    m_stopButton->setEnabled(true);

}

void TaskCard::onClickedPauseButton()
{
    /// set state;
    m_startButton->setEnabled(false);
    m_pauseButton->setEnabled(false);
    m_continueButton->setEnabled(true);
    m_stopButton->setEnabled(true);
}

void TaskCard::onClickedContinueButton()
{
    m_startButton->setEnabled(false);
    m_pauseButton->setEnabled(true);
    m_continueButton->setEnabled(false);
    m_stopButton->setEnabled(true);
}

void TaskCard::onClickedStopButton()
{
    m_startButton->setEnabled(true);
    m_pauseButton->setEnabled(false);
    m_continueButton->setEnabled(false);
    m_stopButton->setEnabled(false);
}

void TaskCard::onThemeChanged()
{
    FluStyleSheetUitls::setQssByFileName("TaskCard.qss", this, FluThemeUtils::getUtils()->getTheme());
}
