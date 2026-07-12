#pragma once

#include "___def.h"
#include <QPushButton>

// // 开始
// // 停止
// // 继续
// // 暂停
// enum class TaskButtonType {
//     Start,      // 开始
//     Stop,       // 完全停止
//     Continue,   // 继续
//     pause,      // 中途停止
// };


class TaskButton : public QPushButton 
{
    Q_OBJECT
public:
    TaskButton(QWidget* parent = nullptr);

    TaskButton(TaskButtonType type, QWidget* parent =  nullptr);

    void setTaskButtonType(TaskButtonType type, bool bEnabled = true);

    void setEnabled(bool b);

    void setDisabled(bool b);

    void onThemeChanged();

protected:
    TaskButtonType m_buttonType;
};