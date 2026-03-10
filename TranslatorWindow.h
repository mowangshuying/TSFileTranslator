#pragma once

#include <FluWindowKitWindow.h>
#include <FluScintilla.h>
#include <FluMenu.h>
#include <FluMenuBar.h>
#include <FluAction.h>
#include <list>
#include <QString>
#include <TongYiOpenAi.hpp>

class Location {
public:
    void clear()
    {
        filename.clear();
        line.clear();
    }
public:
    QString filename;
    QString line;
};

class Translation
{
public:
    void clear()
    {
        type = "";
        translation = "";
    }
public:
    QString type;
    QString translation;
};

class Message {
public:
    void clear()
    {
        locations.clear();
        source = "";
        translation.clear();
    }
public:
    std::list<Location> locations;
    QString source;
    Translation translation;
};

class Context {
public:
    void clear()
    {
        name = "";
        messages.clear();
    }
public:
    QString name;
    std::list<Message> messages;
};

class __Xml {
public:
    QString version;
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

    //void __translate();

    __Xml __read(QString filepath);

    void __translate(QString sourceLang, QString targetLang, QString source);
    void __translate(__Xml& xml);
    void __write(QString filepath, __Xml xml);
public slots:
    void __onTriggerOpenFile(bool b = false);
    void __onTriggerAbout(bool b = false);
    void __onTriggerTranslate(bool b = false);
protected:
    
    ///
    FluMenu* m_fileMenu;
    FluMenu* m_operMenu;
    FluMenu* m_helpMenu;

    FluAction* m_openFileAction;
    FluAction* m_saveFileAction;
    FluAction* m_saveAsFileAction;
    FluAction* m_exitAction;

    FluAction* m_translateAction;

    FluAction* m_aboutAction;


    // editor;
	FluScintilla* m_editor;

    // 
    QString m_xmlFilePath;
};