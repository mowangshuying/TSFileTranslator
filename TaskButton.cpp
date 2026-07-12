#include "TaskButton.h"
#include <FluUtils.h>

TaskButton::TaskButton(QWidget *parent) : QPushButton(parent)
{
    setFixedSize(20, 20);
    setIconSize(QSize(20, 20));

    onThemeChanged();
    connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, this, [=](FluTheme theme){
        onThemeChanged();
    });
}

TaskButton::TaskButton(TaskButtonType type, QWidget *parent) : TaskButton(parent)
{
    setTaskButtonType(type);
}

void TaskButton::setTaskButtonType(TaskButtonType type, bool bEnabled)
{
    m_buttonType = type;
    /// 根据TaskButtonType及主题色设置按钮;

    if  (bEnabled)
    {
        if (type == TaskButtonType::Start)
        {
            /// 开始按钮 134,204,130
            QPixmap pixmap = FluIconUtils::getFluentIconPixmap(FluAwesomeType::Play, QColor(134, 204, 130), 20, 20);
            setIcon(QIcon(pixmap));
            return;
        }

        if (type == TaskButtonType::Stop)
        {
            /// 停止按钮 198,117,100
            QPixmap pixmap = FluIconUtils::getFluentIconPixmap(FluAwesomeType::Stop, QColor(198, 117, 100), 20, 20);
            setIcon(QIcon(pixmap));
            return;
        }

        if (type == TaskButtonType::Continue)
        {
            QPixmap pixmap = FluIconUtils::getFluentIconPixmap(FluAwesomeType::Next, QColor(113, 182, 242), 20, 20);
            setIcon(QIcon(pixmap));
            return;
        }

        if (type == TaskButtonType::pause)
        {
            QPixmap pixmap = FluIconUtils::getFluentIconPixmap(FluAwesomeType::Pause, QColor(113,182,242), 20, 20);
            setIcon(QIcon(pixmap));
            return;
        }
        return;
    }

    if  (!bEnabled)
    {
        if (type == TaskButtonType::Start)
        {
            /// 开始按钮 134,204,130
            QPixmap pixmap = FluIconUtils::getFluentIconPixmap(FluAwesomeType::Play, QColor(126, 126, 126), 20, 20);
            setIcon(QIcon(pixmap));
            return;
        }

        if (type == TaskButtonType::Stop)
        {
            /// 停止按钮 198,117,100
            QPixmap pixmap = FluIconUtils::getFluentIconPixmap(FluAwesomeType::Stop, QColor(126, 126, 126), 20, 20);
            setIcon(QIcon(pixmap));
            return;
        }

        if (type == TaskButtonType::Continue)
        {
            QPixmap pixmap = FluIconUtils::getFluentIconPixmap(FluAwesomeType::Next, QColor(126, 126, 126), 20, 20);
            setIcon(QIcon(pixmap));
            return;
        }

        if (type == TaskButtonType::pause)
        {
            QPixmap pixmap = FluIconUtils::getFluentIconPixmap(FluAwesomeType::Pause, QColor(126, 126, 126), 20, 20);
            setIcon(QIcon(pixmap));
            return;
        }
        return;
    }


}

void TaskButton::setEnabled(bool b)
{
    QPushButton::setEnabled(b);
    setTaskButtonType(m_buttonType, b);
}

void TaskButton::setDisabled(bool b)
{
    setEnabled(!b);
}

void TaskButton::onThemeChanged()
{
    FluStyleSheetUitls::setQssByFileName("TaskButton.qss", this, FluThemeUtils::getUtils()->getTheme());
}
