#pragma once

#include <FluWidget.h>

class TaskButton;
class TaskCard  : public FluWidget 
{
	Q_OBJECT

public:
	TaskCard(QWidget *parent = nullptr);
	~TaskCard();

    void onClickedStartButton();

    void onClickedPauseButton();

    void onClickedContinueButton();

    void onClickedStopButton();

	void onThemeChanged();

protected:
    TaskButton* m_startButton;
    TaskButton* m_pauseButton;
    TaskButton* m_continueButton;
    TaskButton* m_stopButton;
};

