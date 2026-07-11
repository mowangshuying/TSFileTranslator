#include "LogPage.h"
#include <FluUtils.h>
#include <FluLabel.h>

LogPage::LogPage(QWidget *parent) : FluWidget(parent)
{
    auto vMainLayout = new QVBoxLayout(this);
    vMainLayout->setAlignment(Qt::AlignTop);
    setLayout(vMainLayout);
    vMainLayout->setContentsMargins(35, 35, 35, 35);

    m_logWindow = new FluScintilla;
    m_logWindow->setMarginLineNumbers(0, false);
    m_logWindow->setMarginWidth(0, 0);
    m_logWindow->setReadOnly(true);

    vMainLayout->addWidget(m_logWindow, 1);

    onThemeChanged();
    connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, this, [=](FluTheme theme){
        onThemeChanged();
    });
}

void LogPage::appendLog(QString text)
{
    // timestamp level text
    QString timestamp = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    text = QString("%1 - %2").arg(timestamp, text);
    m_logWindow->append(text);
}

void LogPage::onThemeChanged()
{
    FluStyleSheetUitls::setQssByFileName("LogPage.qss", this, FluThemeUtils::getUtils()->getTheme());
}