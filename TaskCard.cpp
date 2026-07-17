#include "TaskCard.h"
#include <QVBoxLayout>
#include <FluLabel.h>
#include <FluUtils.h>
#include "TaskButton.h"
#include <QApplication>
#include <thread>
#include "LogPage.h"
// #include <FluProgressRing.h>
//#include <TongYiOpenAi.hpp>
#include <QDir>
#include <FluUtils.h>
#include "TaskViewPage.h"

TaskCard::TaskCard(QWidget *parent)
	: FluWidget(parent)
{

    // m_vMainLayout = new QVBoxLayout(this);
    m_hMainLayout = new QHBoxLayout;
    m_hMainLayout->setContentsMargins(4 , 4, 4, 4);
    setLayout(m_hMainLayout);
    auto wrapInfoWidget = new QWidget;
    m_hMainLayout->addWidget(wrapInfoWidget, 1);

    auto vWrapInfoWidgetLayout = new QVBoxLayout;
    vWrapInfoWidgetLayout->setContentsMargins(4,4,4,4);
    wrapInfoWidget->setLayout(vWrapInfoWidgetLayout);


    m_progressRing = new FluProgressRing;
    m_hMainLayout->addWidget(m_progressRing);

    auto wrapToolButtonWidget = new QWidget;
    wrapToolButtonWidget->setFixedWidth(150);
    m_hMainLayout->addWidget(wrapToolButtonWidget);

    auto vWrapToolButtonLayout = new QVBoxLayout;
    vWrapToolButtonLayout->setContentsMargins(4,4,4,4);
    wrapToolButtonWidget->setLayout(vWrapToolButtonLayout);

	// m_vMainLayout = new QVBoxLayout(this);
	m_hFileLayout = new QHBoxLayout();
	vWrapInfoWidgetLayout->addLayout(m_hFileLayout);

	m_fileKeyLabel = new FluLabel(FluLabelStyle::CaptionTextBlockSylte);
	m_fileKeyLabel->setText("文件名:");
    m_hFileLayout->addWidget(m_fileKeyLabel);

    m_fileValueLabel = new FluLabel(FluLabelStyle::CaptionTextBlockSylte);
	m_fileValueLabel->setText("*://******/******/******/***.**");
    m_hFileLayout->addWidget(m_fileValueLabel);
    m_hFileLayout->addStretch();

    m_languageLayout = new QHBoxLayout;
    vWrapInfoWidgetLayout->addLayout(m_languageLayout);
    m_languageKeyLabel = new FluLabel;
    m_languageKeyLabel->setLabelStyle(FluLabelStyle::CaptionTextBlockSylte);
    m_languageKeyLabel->setText("语言:");
    m_languageLayout->addWidget(m_languageKeyLabel);

    m_languageValueLabel = new FluLabel;
    m_languageValueLabel->setLabelStyle(FluLabelStyle::CaptionTextBlockSylte);
    m_languageValueLabel->setText("英文 => 中文");
    m_languageLayout->addWidget(m_languageValueLabel);
    m_languageLayout->addStretch();

    // m_hTranslatingLayout = new QHBoxLayout();
    // vWrapInfoWidgetLayout->addLayout(m_hTranslatingLayout);
    m_translatingLabel = new FluLabel(FluLabelStyle::CaptionTextBlockSylte);
    m_translatingLabel->setText("正在翻译...");
    vWrapInfoWidgetLayout->addWidget(m_translatingLabel);

    m_outPutFileLabel = new FluLabel(FluLabelStyle::CaptionTextBlockSylte);
    m_outPutFileLabel->setText("");
    vWrapInfoWidgetLayout->addWidget(m_outPutFileLabel);


    vWrapInfoWidgetLayout->addStretch();


    m_startButton = new FluPushButton;
    m_startButton->setFixedWidth(120);
    m_startButton->setText("开始翻译");
    vWrapToolButtonLayout->addWidget(m_startButton);

    m_deleteTaskButton = new FluPushButton;
    m_deleteTaskButton->setFixedWidth(120);
    m_deleteTaskButton->setText("删除任务");
    vWrapToolButtonLayout->addWidget(m_deleteTaskButton);
    vWrapToolButtonLayout->addStretch();

    setFixedHeight(90);


    connect(m_startButton, &FluPushButton::clicked, this, &TaskCard::onClickedStartButton);
    connect(m_deleteTaskButton, &FluPushButton::clicked, this, &TaskCard::onClickedDeleteTaskButton);
    // connect(m_pauseButton, &TaskButton::clicked, this, &TaskCard::onClickedPauseButton);
    // connect(m_continueButton, &TaskButton::clicked, this, &TaskCard::onClickedContinueButton);
    // connect(m_stopButton, &TaskButton::clicked, this, &TaskCard::onClickedStopButton);

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
    std::thread __thread([=]()
        {
            //// 开始执行翻译工作;
            auto pSettings = FluConfigUtils::getUtils()->getSettings();
            pSettings->sync();
            pSettings->beginGroup("config");
            QString httpUrl = pSettings->value("HttpUrl", "").toString();
            QString token = pSettings->value("Token").toString();
            pSettings->endGroup();
            
            m_xml.__init(httpUrl, token);
            m_outPutFileLabel->setText("");
            connect(&m_xml, &__Xml::__translateInfoChanged, this, [=](int nTranslate, int nTotalTranslate, QString s, QString t) {

                m_progressRing->setMinMaxValue(0, nTotalTranslate);
                m_progressRing->setCurValue(nTranslate);
                m_progressRing->setShowText(true);

                bool isError = s == t;
                QString sLog = QString::asprintf("翻译: %d/%d: %s => %s", nTranslate, nTotalTranslate, s.toStdString().c_str(), t.toStdString().c_str());
                LogPage::getPage()->appendLog(sLog, isError);
                
                
                QFontMetrics metrics(font()); 
                if (metrics.horizontalAdvance(s) > 100) {
                    s = metrics.elidedText(s, Qt::ElideRight, 100);
                }

                if (metrics.horizontalAdvance(t) > 100) {
                    t = metrics.elidedText(t, Qt::ElideRight, 100);
                }

                QString translateInfo = QString("翻译: %1/%2  %3 => %4").arg(nTranslate).arg(nTotalTranslate).arg(s).arg(t);
                m_translatingLabel->setText(translateInfo);



                });

            m_xml.__read(m_taskData.SourceFile);
            if (m_xml.__contexts.empty())
            {
                return;
            }

            /// 翻译;
            /// 去除m_taskData.SourceLang(后的所有内容;
            QString sourceLang = m_taskData.SourceLang.mid(0, m_taskData.SourceLang.indexOf("("));
            QString targetLang = m_taskData.TargetLang.mid(0, m_taskData.TargetLang.indexOf("("));

            m_xml.__translate(sourceLang, targetLang);

            //// 结束翻译 应用程序文件夹下的out目录;
            QString outDirPath = QApplication::applicationDirPath() + "/outputTS/";
            QDir __outDir(outDirPath);
            if (!__outDir.exists())
            {
                __outDir.mkdir(outDirPath);
            }

            //// 提取 m_taskData.SourceFile中的文件名并且去除文件扩展名;
            QString fileName = outDirPath + QFileInfo(m_taskData.SourceFile).fileName() + "(" + targetLang + ")" + ".ts";
            
            QString sOutOutFileNameLog = "输出文件:"+fileName;
            m_outPutFileLabel->setText(sOutOutFileNameLog);
            LogPage::getPage()->appendLog(sOutOutFileNameLog);
            m_xml.__write(fileName);
        });
    __thread.detach();

}

void TaskCard::onClickedDeleteTaskButton()
{
    TaskViewPage::getPage()->removeTaskCard(m_taskData.id);
}

void TaskCard::onThemeChanged()
{
    FluStyleSheetUitls::setQssByFileName("TaskCard.qss", this, FluThemeUtils::getUtils()->getTheme());
}
