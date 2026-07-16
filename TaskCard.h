#pragma once

#include "___def.h"
#include <FluWidget.h>
#include <FluLabel.h>
#include <FluProgressRing.h>

class TaskButton;
class TaskCard  : public FluWidget 
{
	Q_OBJECT

public:
	TaskCard(QWidget *parent = nullptr);
	~TaskCard();

    void setTaskId(int taskId)
    {
        m_taskId = taskId;
    }

    void setTaskData(TaskData taskData);

    void onClickedStartButton();

    void onClickedPauseButton();

    void onClickedContinueButton();

    void onClickedStopButton();

	void onThemeChanged();

protected:
    QHBoxLayout* m_hMainLayout;
    QHBoxLayout* m_hFileLayout;

    FluLabel* m_fileKeyLabel;
    FluLabel* m_fileValueLabel;
    QHBoxLayout* m_languageLayout;
    FluLabel* m_languageKeyLabel;
    FluLabel* m_languageValueLabel;
    QHBoxLayout* m_hTranslatingLayout;
    FluLabel* m_translatingLabel;


    TaskButton* m_startButton;
    TaskButton* m_pauseButton;
    TaskButton* m_continueButton;
    TaskButton* m_stopButton;

    FluProgressRing* m_progressRing;

    __Xml m_xml; 
    TaskData m_taskData;
    int m_taskId;
};

