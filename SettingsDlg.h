#pragma once
#include <FluDialog.h>
#include <FluLabel.h>
#include <FluLineEdit.h>
#include <FluComboBoxEx.h>


class SettingsDlg : public FluDialog
{
    Q_OBJECT
public:
    SettingsDlg(QWidget *parent = nullptr);

    void __initUI();
    void __connect();

    void __loadSettings();
    void __saveSettings();

// slots:
    //void __onClickedLoadBtn();
protected:
    FluLabel* m_httpurlLabel;
    FluLineEdit* m_httpurlEdit;

    FluLabel* m_tokenLabel;
    FluLineEdit* m_tokenEdit;

    FluLabel* m_sourceLangLabel;
    FluComboBoxEx* m_sourceLangCombo;
    FluLabel* m_targetLangLabel;
    FluComboBoxEx* m_targetLangCombo;

    //FluLabel* m_saveFileLabel;
    //FluLineEdit* m_saveFileEdit;
    //FluPushButton* m_loadBtn;
};