#include "HomePage.h"
#include <FluUtils.h>
#include <FluWidget.h>
#include <FluLabel.h>
#include <FluLineEdit.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <FluComboBoxEx.h>
#include <FluIconButton.h>
#include <FluStyleButton.h>
#include <QFileDialog>
#include <FluUtils.h>
#include "___Configs.h"

HomePage* HomePage::__page = nullptr;
HomePage::HomePage(QWidget *parent) : FluWidget(parent)
{

    auto vMainLayout = new QVBoxLayout(this);
    setLayout(vMainLayout);

    vMainLayout->setContentsMargins(35, 35, 35, 35);


    auto wrapWidget = new FluWidget(this);
    wrapWidget->setFixedSize(578, 220);
    vMainLayout->addWidget(wrapWidget, 1, Qt::AlignCenter);
    
    auto vWrapWidgetLayout = new QVBoxLayout(wrapWidget);
    wrapWidget->setLayout(vWrapWidgetLayout);

    /// httpUrl;

    auto hHttpUrlLayout = new QHBoxLayout;
    vWrapWidgetLayout->addLayout(hHttpUrlLayout);

    auto httpUrlLabel = new FluLabel;
    httpUrlLabel->setFixedWidth(80);
    httpUrlLabel->setText("HttpUrl:");
    httpUrlLabel->setLabelStyle(FluLabelStyle::BodyTextBlockStyle);
    hHttpUrlLayout->addWidget(httpUrlLabel);

    auto httpUrlLineEdit = new FluLineEdit;
    httpUrlLineEdit->setFixedSize(480, 30);
    httpUrlLineEdit->setText("https://dashscope.aliyuncs.com/compatible-mode/v1/chat/completions");
    // hHttpUrlLayout->addWidget(httpUrlLabel);
    hHttpUrlLayout->addWidget(httpUrlLineEdit);


    /// tooken;
    auto hTokonLayout = new QHBoxLayout;
    vWrapWidgetLayout->addLayout(hTokonLayout);

    auto tokenLabel = new FluLabel;
    tokenLabel->setFixedWidth(80);
    tokenLabel->setText("Token:");
    tokenLabel->setLabelStyle(FluLabelStyle::BodyTextBlockStyle);
    hTokonLayout->addWidget(tokenLabel);

    auto tokenLineEdit = new FluLineEdit;
    tokenLineEdit->setFixedSize(480, 30);
    tokenLineEdit->setText("");
    tokenLineEdit->setPlaceholderText("请输入API Key");
    hTokonLayout->addWidget(tokenLineEdit);

    /// sourceFile;
    auto hSourceFileLayout = new QHBoxLayout;
    vWrapWidgetLayout->addLayout(hSourceFileLayout);
    auto sourceFileLabel = new FluLabel;
    sourceFileLabel->setText("sourceFile:");
    sourceFileLabel->setFixedWidth(80);
    sourceFileLabel->setLabelStyle(FluLabelStyle::BodyTextBlockStyle);
    hSourceFileLayout->addWidget(sourceFileLabel);

    auto sourceFileLineEidt = new FluLineEdit;
    sourceFileLineEidt->setFixedSize(442, 30);
    hSourceFileLayout->addWidget(sourceFileLineEidt);

    auto loadSourceFileButton = new FluIconButton;
    loadSourceFileButton->setType(FluAwesomeType::FolderOpen);
    loadSourceFileButton->setFixedSize(30, 30);
    hSourceFileLayout->addWidget(loadSourceFileButton);


    /// sourceLang;
    auto hSourceLangLayout = new QHBoxLayout;
    vWrapWidgetLayout->addLayout(hSourceLangLayout);
    auto sourceLangLabel = new FluLabel;
    sourceLangLabel->setText("sourceLang:");
    sourceLangLabel->setFixedWidth(80);
    sourceLangLabel->setLabelStyle(FluLabelStyle::BodyTextBlockStyle);
    hSourceLangLayout->addWidget(sourceLangLabel);

    auto sourceLangComboBox = new FluComboBoxEx;
    sourceLangComboBox->setFixedWidth(150);
    sourceLangComboBox->addItem("auto(自动)");
    sourceLangComboBox->addItem("en(英文)");
    sourceLangComboBox->addItem("zh(中文)");
    sourceLangComboBox->addItem("zh_tw(繁体中文)");
    sourceLangComboBox->addItem("ru(俄语)");
    sourceLangComboBox->addItem("ja(日语)");
    sourceLangComboBox->addItem("ko(韩语)");
    sourceLangComboBox->addItem("fr(法语)");
    sourceLangComboBox->addItem("de(德语)");
    sourceLangComboBox->addItem("it(意大利语)");
    sourceLangComboBox->addItem("th(泰语)");
    sourceLangComboBox->addItem("id(越南)");
    sourceLangComboBox->addItem("tr(土耳其语)");
    sourceLangComboBox->setMaxVisibleItems(6);
    hSourceLangLayout->addWidget(sourceLangComboBox);
    hSourceLangLayout->addStretch();

    /// targetLang;
    auto hTargetLangLayout = new QHBoxLayout;
    vWrapWidgetLayout->addLayout(hTargetLangLayout);
    auto targetLangLabel = new FluLabel;
    targetLangLabel->setText("targetLang:");
    targetLangLabel->setFixedWidth(80);
    targetLangLabel->setLabelStyle(FluLabelStyle::BodyTextBlockStyle);
    hTargetLangLayout->addWidget(targetLangLabel);

    auto targetLangComboBox = new FluComboBoxEx;
    targetLangComboBox->setFixedWidth(150);
    targetLangComboBox->addItem("auto(自动)");
    targetLangComboBox->addItem("en(英文)");
    targetLangComboBox->addItem("zh(中文)");
    targetLangComboBox->addItem("zh_tw(繁体中文)");
    targetLangComboBox->addItem("ru(俄语)");
    targetLangComboBox->addItem("ja(日语)");
    targetLangComboBox->addItem("ko(韩语)");
    targetLangComboBox->addItem("fr(法语)");
    targetLangComboBox->addItem("de(德语)");
    targetLangComboBox->addItem("it(意大利语)");
    targetLangComboBox->addItem("th(泰语)");
    targetLangComboBox->addItem("id(越南)");
    targetLangComboBox->addItem("tr(土耳其语)");
    targetLangComboBox->setMaxVisibleItems(6);
    hTargetLangLayout->addWidget(targetLangComboBox);
    hTargetLangLayout->addStretch();

    /// addToTask;
    auto hAddToTaskLayout = new QHBoxLayout;
    vWrapWidgetLayout->addLayout(hAddToTaskLayout);
    auto addToTaskListButton = new FluStyleButton;
    addToTaskListButton->setFixedSize(240, 30);
    addToTaskListButton->setText("添加至任务列表");
    hAddToTaskLayout->addWidget(addToTaskListButton);

    {
        /// load settings
        auto pSettings = FluConfigUtils::getUtils()->getSettings();
        pSettings->sync();
        pSettings->beginGroup("config");
        QString httpUrl = pSettings->value("HttpUrl", "").toString();
        QString token = pSettings->value("Token").toString();
        QString sourceFile = pSettings->value("SourceFile").toString();
        QString sourceLang = pSettings->value("SourceLang").toString();
        QString targetLang = pSettings->value("TargetLang").toString();
        pSettings->endGroup();

        httpUrlLineEdit->setText(httpUrl);
        tokenLineEdit->setText(token);
        sourceFileLineEidt->setText(sourceFile);
        sourceLangComboBox->setIndexByText(sourceLang);
        targetLangComboBox->setIndexByText(targetLang);
    }

    connect(loadSourceFileButton, &FluIconButton::clicked, this, [=]() {
        QFileDialog qfd(this);
        qfd.setFileMode(QFileDialog::ExistingFile);
        int nExec = qfd.exec();
        if (nExec != QDialog::Accepted)
        {
            qfd.close();
            return;
        }

        QStringList sFiles = qfd.selectedFiles();
        if (sFiles.isEmpty())
        {
            return;
        }

        QString filepath = sFiles[0];
        QFile file(filepath);
        if (!file.open(QIODevice::ReadOnly))
        {
            return;
        }

        sourceFileLineEidt->setText(filepath);

        });

    connect(addToTaskListButton, &FluStyleButton::clicked, this, [=](){
        // TaskData data;
        QString httpUrl = httpUrlLineEdit->text();
        QString token = tokenLineEdit->text();

        QSettings* pSettings = FluConfigUtils::getUtils()->getSettings();
        pSettings->sync();
        pSettings->beginGroup("config");
        pSettings->setValue("HttpUrl", httpUrl);
        pSettings->setValue("Token", token);
        pSettings->setValue("SourceFile", sourceFileLineEidt->text());
        pSettings->setValue("SourceLang", sourceLangComboBox->getTextBtn()->text());
        pSettings->setValue("TargetLang", targetLangComboBox->getTextBtn()->text());
        pSettings->endGroup();
        
        TaskData data;
        data.SourceFile = sourceFileLineEidt->text();
        data.SourceLang = sourceLangComboBox->getTextBtn()->text();
        data.TargetLang = targetLangComboBox->getTextBtn()->text();
        ___Configs::getConfig()->addTaskData(data);
        emit clickedAddToTaskListButton(data);
    });

    onThemeChanged();
    connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, this, [=](FluTheme theme){
        onThemeChanged();
    });
}

HomePage* HomePage::getPage()
{
    //return nullptr;
    if (__page == nullptr)
        __page = new HomePage();
    return __page;
}


void HomePage::onThemeChanged()
{
    FluStyleSheetUitls::setQssByFileName("HomePage.qss", this, FluThemeUtils::getUtils()->getTheme());
}
