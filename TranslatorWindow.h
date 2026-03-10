#pragma once

#include <FluWindowKitWindow.h>
#include <FluScintilla.h>
#include <FluMenu.h>
#include <FluMenuBar.h>
#include <FluAction.h>
#include <list>

class Location {
public:
    std::string filename;
    std::string line;
};

class Translation
{
public:
    std::string type;
    std::string translation;
};

class Message {
public:
    Location location;
    std::string source;
    Translation translation;
};

class Context {
public:
    std::string name;
    std::list<std::string> messages;
};

class __Xml {
public:
    std::list<Context> contexts;
};


class TranslatorWindow : public FluWindowKitWindow
{
    Q_OBJECT
public:
    TranslatorWindow(QWidget *parent = nullptr);

    void __initUI();
	void __initMenu();

    void __connect();

    void __translate();
public slots:
    void __onTriggerOpenFile(bool b = false);
    void __onTriggerAbout(bool b = false);
protected:
    
    ///
    FluMenu* m_fileMenu;
    FluMenu* m_helpMenu;

    FluAction* m_openFileAction;
    FluAction* m_saveFileAction;
    FluAction* m_saveAsFileAction;
    FluAction* m_exitAction;
    FluAction* m_aboutAction;


    // editor;
	FluScintilla* m_editor;
};