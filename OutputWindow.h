#pragma once
#include <QByteArray>
#include <FluScintilla.h>

class OutputWindow : public FluScintilla
{
    Q_OBJECT
public:
    explicit OutputWindow(QWidget *parent = nullptr);
    virtual ~OutputWindow()
    {

    }
    
    void append(QString text);
};