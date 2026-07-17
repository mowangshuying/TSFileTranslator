#include "TaskViewPage.h"
#include <QVBoxLayout>
#include "TaskCard.h"
#include <FluLabel.h>
#include "___Configs.h"

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

    {
        /// load taskdata
        std::vector<TaskData> taskDatas;
        ___Configs::getConfig()->getTaskDatas(taskDatas);
        for (auto taskData : taskDatas)
        {
            addTaskCard(taskData);
        }
    }

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

void TaskViewPage::removeTaskCard(qint64 taskId)
{
    for (int i = 0; i < getMainLayout()->count(); ++i) {
        QLayoutItem* item = getMainLayout()->itemAt(i);
        if (item && item->widget()) { // 安全检查：确保项存在且是控件
            auto taskCard = dynamic_cast<TaskCard*>(item->widget());
            if (taskCard && taskCard->getTaskId() == taskId) {
                getMainLayout()->takeAt(i);
                // taskCard->hide();
                // delete taskCard;
                taskCard->deleteLater();
                ___Configs::getConfig()->deleteTaskDataById(taskId);
                break;
            }
        }
    }

}

void TaskViewPage::onThemeChanged()
{
    FluStyleSheetUitls::setQssByFileName("TaskViewPage.qss", this, FluThemeUtils::getUtils()->getTheme());
}