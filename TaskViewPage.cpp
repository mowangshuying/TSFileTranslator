#include "TaskViewPage.h"
#include <QVBoxLayout>
#include "TaskCard.h"
#include <FluLabel.h>

TaskViewPage* TaskViewPage::__page = nullptr;
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

TaskViewPage* TaskViewPage::getPage()
{
    if (__page == nullptr)
        __page = new TaskViewPage();
    return __page;
}

int TaskViewPage::getTaskId()
{
    static int taskId = 1;
    return taskId++;
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