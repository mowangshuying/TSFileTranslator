#pragma once

#include <FluFrameLessWidget.h>
#include <FluVNavigationView.h>
#include <FluStackedLayout.h>

class TSWindow : public FluFrameLessWidget
{
    Q_OBJECT
public:
    TSWindow(QWidget* parent = nullptr);

    void __initUI();

// public slots:
    void onThemeChanged();
protected:
    FluVNavigationView* m_navView;
    FluStackedLayout* m_sLayout;
};