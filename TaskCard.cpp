#include "TaskCard.h"
#include <QVBoxLayout>
#include <FluLabel.h>
#include <FluUtils.h>
#include "TaskButton.h"
#include <QApplication>
#include <thread>
//#include <TongYiOpenAi.hpp>

TaskCard::TaskCard(QWidget *parent)
	: FluWidget(parent)
{
	m_vMainLayout = new QVBoxLayout(this);
	m_hFileLayout = new QHBoxLayout();
	m_vMainLayout->addLayout(m_hFileLayout);

	m_fileKeyLabel = new FluLabel(FluLabelStyle::BodyTextBlockStyle);
	m_fileKeyLabel->setText("文件名:");
    m_hFileLayout->addWidget(m_fileKeyLabel);

    m_fileValueLabel = new FluLabel(FluLabelStyle::BodyTextBlockStyle);
	m_fileValueLabel->setText("*://******/******/******/***.**");
    m_hFileLayout->addWidget(m_fileValueLabel);
    m_hFileLayout->addStretch();

    m_languageLayout = new QHBoxLayout;
    m_vMainLayout->addLayout(m_languageLayout);
    m_languageKeyLabel = new FluLabel;
    m_languageKeyLabel->setLabelStyle(FluLabelStyle::BodyTextBlockStyle);
    m_languageKeyLabel->setText("语言:");
    m_languageLayout->addWidget(m_languageKeyLabel);

    m_languageValueLabel = new FluLabel;
    m_languageValueLabel->setLabelStyle(FluLabelStyle::BodyTextBlockStyle);
    m_languageValueLabel->setText("英文 => 中文");
    m_languageLayout->addWidget(m_languageValueLabel);
    m_languageLayout->addStretch();

    m_hTranslatingLayout = new QHBoxLayout();
    m_vMainLayout->addLayout(m_hTranslatingLayout);
    m_translatingLabel = new FluLabel(FluLabelStyle::BodyTextBlockStyle);
    m_translatingLabel->setText("正在翻译...");
    m_hTranslatingLayout->addWidget(m_translatingLabel);

    auto m_hToolButtonLayout = new QHBoxLayout();
    m_vMainLayout->addLayout(m_hToolButtonLayout);


    m_startButton = new TaskButton(TaskButtonType::Start);
    m_startButton->setEnabled(true);
    m_startButton->setToolTip("开始翻译...");
    m_hToolButtonLayout->addWidget(m_startButton);


    m_pauseButton = new TaskButton(TaskButtonType::pause);
    m_pauseButton->setEnabled(false);
    m_pauseButton->setToolTip("暂停");
    m_hToolButtonLayout->addWidget(m_pauseButton);

    m_continueButton  = new TaskButton(TaskButtonType::Continue);
    m_continueButton->setEnabled(false);
    m_continueButton->setToolTip("继续");
    m_hToolButtonLayout->addWidget(m_continueButton);

    m_stopButton = new TaskButton(TaskButtonType::Stop);
    m_stopButton->setEnabled(false);
    m_stopButton->setToolTip("停止");
    m_hToolButtonLayout->addWidget(m_stopButton);

    m_hToolButtonLayout->addStretch();

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

void TaskCard::setTaskData(TaskData taskData)
{
    m_taskData = taskData;
    m_fileValueLabel->setText(taskData.SourceFile);
    QString sLang = QString("%1 => %2").arg(taskData.SourceLang).arg( taskData.TargetLang.toUtf8());
    m_languageValueLabel->setText(sLang);

    QString sTranslating = "";
    m_translatingLabel->setText(sTranslating);
}

void TaskCard::onClickedStartButton()
{
    m_startButton->setEnabled(false);
    m_pauseButton->setEnabled(true);
    m_continueButton->setEnabled(false);
    m_stopButton->setEnabled(true);

    std::thread __thread([=]()
        {
            //// 开始执行翻译工作;
            m_xml.__init(m_taskData.HtttpUrl, m_taskData.Token);

            connect(&m_xml, &__Xml::__translateInfoChanged, this, [=](int nTranslate, int nTotalTranslate, QString s, QString t) {

                QFontMetrics metrics(font()); 
                if (metrics.horizontalAdvance(s) > 100) {
                    s = metrics.elidedText(s, Qt::ElideRight, 100);
                }

                if (metrics.horizontalAdvance(t) > 100) {
                    t = metrics.elidedText(t, Qt::ElideRight, 100);
                }

                QString translateInfo = QString("翻译进度: %1/%2: %3 => %4").arg(nTranslate).arg(nTotalTranslate).arg(s).arg(t);
                m_translatingLabel->setText(translateInfo);
                });

            m_xml.__read(m_taskData.SourceFile);
            if (m_xml.__contexts.empty())
            {
                /// 翻译结束，重置按钮状态
                m_startButton->setEnabled(true);
                m_pauseButton->setEnabled(false);
                m_continueButton->setEnabled(false);
                m_stopButton->setEnabled(false);
                return;
            }

            /// 翻译;
            /// 去除m_taskData.SourceLang(后的所有内容;
            QString sourceLang = m_taskData.SourceLang.mid(0, m_taskData.SourceLang.indexOf("("));
            QString targetLang = m_taskData.TargetLang.mid(0, m_taskData.TargetLang.indexOf("("));

            m_xml.__translate(sourceLang, targetLang);

            //// 结束翻译 应用程序文件夹下的out目录;
            QString outPath = QApplication::applicationDirPath() + "/out/";
            //// 提取 m_taskData.SourceFile中的文件名并且去除文件扩展名;
            QString fileName = outPath + QFileInfo(m_taskData.SourceFile).fileName() + "(" + targetLang + ")" + ".ts";
            m_xml.__write(fileName);

            ///// 翻译完成重置按钮状态
            //m_startButton->setEnabled(true);
            //m_pauseButton->setEnabled(false);
            //m_continueButton->setEnabled(false);
            //m_stopButton->setEnabled(false);
        });
    __thread.detach();

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
