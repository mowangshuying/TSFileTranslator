#pragma once
#include <FluWindowKitWindow.h>

class TranslatorWindow : public FluWindowKitWindow
{
    Q_OBJECT
public:
    TranslatorWindow(QWidget *parent = nullptr) : FluWindowKitWindow(parent)
    {
        setWindowTitle("TsFileTranslator");
    }
};