#pragma once
#include "___def.h"
#include <FluWidget.h>

// class TaskData {
// public:
//     QString HtttpUrl;
//     QString Tooken;
//     QString SourceFile;
//     QString SourceLang;
//     QString TargetLang;
// };

class HomePage : public FluWidget 
{
    Q_OBJECT
public:
    HomePage(QWidget* parent = nullptr);

signals:
    void clicekdAddToTaskListButton(TaskData taskData);
public slots:
    void onThemeChanged();
};