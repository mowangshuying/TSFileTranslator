#include "SettingsDlg.h"
#include <QFileDialog>
//#include "FluComboBoxEx.h"

SettingsDlg::SettingsDlg(QWidget *parent) : FluDialog(parent)
{
    __initUI();
    __connect();
    __loadSettings();
}

void SettingsDlg::__initUI()
{
    getWidget()->setFixedSize(400, 300);
   
    auto hLayout = new QHBoxLayout;
    m_httpurlLabel = new FluLabel(FluLabelStyle::BodyTextBlockStyle);
    m_httpurlLabel->setText(tr("httpurl:"));
    m_httpurlLabel->setFixedWidth(80);
    m_httpurlEdit = new FluLineEdit;
    m_httpurlEdit->setFixedHeight(30);
    hLayout->addWidget(m_httpurlLabel);
    hLayout->addWidget(m_httpurlEdit);
    getContentWidgetLayout()->addLayout(hLayout);

    hLayout = new QHBoxLayout;
    m_tokenLabel = new FluLabel(FluLabelStyle::BodyTextBlockStyle);
    m_tokenLabel->setText(tr("token:"));
    m_tokenLabel->setFixedWidth(80);
    m_tokenEdit = new FluLineEdit;
    m_tokenEdit->setFixedHeight(30);
    hLayout->addWidget(m_tokenLabel);
    hLayout->addWidget(m_tokenEdit);
    getContentWidgetLayout()->addLayout(hLayout);

    hLayout = new QHBoxLayout;
    m_sourceLangLabel = new FluLabel(FluLabelStyle::BodyTextBlockStyle);
    m_sourceLangLabel->setText(tr("sourceLang:"));
    m_sourceLangLabel->setFixedWidth(80);
    m_sourceLangCombo = new FluComboBoxEx;
    m_sourceLangCombo->setMaxVisibleItems(5);
    hLayout->addWidget(m_sourceLangLabel);
    hLayout->addWidget(m_sourceLangCombo);
    getContentWidgetLayout()->addLayout(hLayout);

    hLayout = new QHBoxLayout;
    m_targetLangLabel = new FluLabel(FluLabelStyle::BodyTextBlockStyle);
    m_targetLangLabel->setText(tr("targetLang:"));
    m_targetLangLabel->setFixedWidth(80);
    m_targetLangCombo = new FluComboBoxEx;
    m_targetLangCombo->setMaxVisibleItems(5);
    hLayout->addWidget(m_targetLangLabel);
    hLayout->addWidget(m_targetLangCombo);
    getContentWidgetLayout()->addLayout(hLayout);

   /* hLayout = new QHBoxLayout;
    m_saveFileLabel = new FluLabel(FluLabelStyle::BodyTextBlockStyle);
    m_saveFileLabel->setText(tr("saveFile:"));
    m_saveFileLabel->setFixedWidth(80);
    m_saveFileEdit = new FluLineEdit;
    m_saveFileEdit->setFixedHeight(30);*/
    // m_saveFileEdit->setFixedHeight(30);
    
    //m_loadBtn = new FluPushButton;
    //m_loadBtn->setFixedSize(30, 30);
    //m_loadBtn->setText(tr("..."));
    
    //hLayout->addWidget(m_saveFileLabel);
    //hLayout->addWidget(m_saveFileEdit, 1);
    //hLayout->addWidget(m_loadBtn);
    //getContentWidgetLayout()->addLayout(hLayout);
}

void SettingsDlg::__connect()
{
    //connect(m_loadBtn, &FluPushButton::clicked, this, &SettingsDlg::__onClickedLoadBtn);
    connect(m_okBtn, &FluPushButton::clicked, this, &SettingsDlg::__saveSettings);
}

void SettingsDlg::__loadSettings()
{
    // load for httpurl;
    // load for token;
    // load for lang;
    // load for lang;
    // load for savefile;

    FluConfigUtils::getUtils()->getSettings()->beginGroup("config");
	QString httpurl = FluConfigUtils::getUtils()->getSettings()->value("httpurl", "https://dashscope.aliyuncs.com/compatible-mode/v1/chat/completions").toString();
	QString token = FluConfigUtils::getUtils()->getSettings()->value("token", "sk-******").toString();
	QString sourceLang = FluConfigUtils::getUtils()->getSettings()->value("sourceLang", "en").toString();
    QString targetLang = FluConfigUtils::getUtils()->getSettings()->value("targetLang", "zh").toString();
    //QString saveFile = FluConfigUtils::getUtils()->getSettings()->value("saveFile", "").toString();
	FluConfigUtils::getUtils()->getSettings()->endGroup();

    m_httpurlEdit->setText(httpurl);
    m_tokenEdit->setText(token);
    //m_sourceLangCombo->getTextBtn()->setText(sourceLang);
    //m_targetLangCombo->getTextBtn()->setText(targetLang);
    //m_saveFileEdit->setText(saveFile);

    //// data;
    QStringList langs;
    langs << "en"; // 英文
    langs << "zh"; // 中文
    langs << "zh_tw"; // 繁体中文
    langs << "ru"; // 俄语
    langs << "ja"; // 日语
    langs << "ko"; // 韩语
    langs << "fr"; // 法语
    langs << "de"; // 德语
    langs << "it"; // 意大利语
    langs << "th"; // 泰语
    langs << "id"; // 越南

    for (int i = 0; i < langs.size(); i++)
    {
        m_sourceLangCombo->addItem(langs[i]);
        m_targetLangCombo->addItem(langs[i]);

        if (langs[i] == sourceLang)
            m_sourceLangCombo->setIndex(i);

        if (langs[i] == targetLang)
            m_targetLangCombo->setIndex(i);
    }
}

void SettingsDlg::__saveSettings()
{
    QSettings settings;
    FluConfigUtils::getUtils()->getSettings()->beginGroup("config");
    
    QString httpurl = m_httpurlEdit->text();
    QString token = m_tokenEdit->text();
    QString sourceLang = m_sourceLangCombo->getTextBtn()->text();
    QString targetLang = m_targetLangCombo->getTextBtn()->text();
    //QString saveFile = m_saveFileEdit->text();
    FluConfigUtils::getUtils()->getSettings()->setValue("httpurl", httpurl);
    FluConfigUtils::getUtils()->getSettings()->setValue("token", token);
    FluConfigUtils::getUtils()->getSettings()->setValue("sourceLang", sourceLang);
    FluConfigUtils::getUtils()->getSettings()->setValue("targetLang", targetLang);
    //FluConfigUtils::getUtils()->getSettings()->setValue("saveFile", saveFile);
    FluConfigUtils::getUtils()->getSettings()->endGroup();
}

//void SettingsDlg::__onClickedLoadBtn()
//{
//    QFileDialog qfd(this);
//    qfd.setFileMode(QFileDialog::ExistingFile);
//    int nExec = qfd.exec();
//    if (nExec != QDialog::Accepted)
//    {
//        qfd.close();
//        return;
//    }
//
//    QStringList sFiles = qfd.selectedFiles();
//    if (sFiles.isEmpty())
//    {
//        return;
//    }
//
//    QString filepath = sFiles[0];
//    //m_saveFileEdit->setText(filepath);
//}
//
