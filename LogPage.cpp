#include "LogPage.h"
#include <FluUtils.h>
#include <FluLabel.h>
#include <FluTextEdit.h>
#include <QScrollBar>

LogPage* LogPage::__page = nullptr;
LogPage::LogPage(QWidget *parent) : FluWidget(parent)
{
    auto vMainLayout = new QVBoxLayout(this);
    vMainLayout->setAlignment(Qt::AlignTop);
    setLayout(vMainLayout);
    vMainLayout->setContentsMargins(35, 35, 35, 35);

    m_logWindow = new FluTextEdit;
    vMainLayout->addWidget(m_logWindow, 1);

    onThemeChanged();
    connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, this, [=](FluTheme theme){
        onThemeChanged();
    });
}

LogPage* LogPage::getPage()
{
    //return nullptr;
    if (__page == nullptr)
        __page = new LogPage();
    return __page;
}

void LogPage::appendLog(QString text, bool isError)
{
    // timestamp level text
    QString timestamp = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    text = QString("%1 - %2").arg(timestamp, text);

    if (isError)
    {
        text = QString("<font color=\"red\">%1</font>").arg(text);
    }

    m_logWindow->append(text);
    QTimer::singleShot(0, this, [=](){
        m_logWindow->verticalScrollBar()->setValue(m_logWindow->verticalScrollBar()->maximum());
    });
}

void LogPage::onThemeChanged()
{
    FluStyleSheetUitls::setQssByFileName("LogPage.qss", this, FluThemeUtils::getUtils()->getTheme());
}