#include "TaskViewPage.h"
#include <QVBoxLayout>
#include "TaskCard.h"
#include <FluLabel.h>

TaskViewPage::TaskViewPage(QWidget *parent) : FluVScrollView(parent)
{

    getMainLayout()->setContentsMargins(35, 35, 35, 35);
    getMainLayout()->setAlignment(Qt::AlignTop);

    auto titleLabel = new FluLabel;
    titleLabel->setText("任务列表");
    titleLabel->setLabelStyle(FluLabelStyle::TitleTextBlockStyle);
    getMainLayout()->addWidget(titleLabel, 0, Qt::AlignTop);

    //auto taskCard = new TaskCard();
    // vMainLayout->addWidget(taskCard);
    //getMainLayout()->addWidget(taskCard, 0, Qt::AlignTop);

    onThemeChanged();
    connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, this, [=](FluTheme theme) {
        onThemeChanged();    
    });
}

void TaskViewPage::addTaskCard(TaskData taskData)
{
    auto taskCard = new TaskCard();
    taskCard->setTaskData(taskData);
    getMainLayout()->addWidget(taskCard, 0, Qt::AlignTop);
}

void TaskViewPage::onThemeChanged()
{
    FluStyleSheetUitls::setQssByFileName("TaskViewPage.qss", this, FluThemeUtils::getUtils()->getTheme());
}