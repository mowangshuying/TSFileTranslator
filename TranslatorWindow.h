#pragma once

#include <FluWindowKitWindow.h>
#include <FluScintilla.h>
#include <FluMenu.h>
#include <FluMenuBar.h>
#include <FluAction.h>
#include <list>
#include <QString>
#include <FluDockManager.h>
#include "OutputWindow.h"
#include <FluListView.h>
#include <QListWidgetItem>
#include <FluRoundMenu.h>
#include "___Xml.h"

enum class TranslateState {
    Ready,
    Working,
};

class TranslatorWindow : public FluWindowKitWindow
{
    Q_OBJECT
public:
    TranslatorWindow(QWidget *parent = nullptr);

    void loadConfig();

    void __initUI();
	void __initMenu();

    void __connect();

    void __log(QString text);
    void __clearLog();

    //void __translate();

    __Xml __read(QString filepath);

    QString __translate(QString sourceLang, QString targetLang, QString source);
    std::list<QString> __translate(QString sourceLang, QString targetLang, std::list<QString> sources);
    void __translate(__Xml& xml);
    void __write(QString filepath, __Xml xml);

    void closeEvent(QCloseEvent* event);
signals:
    void __translateStart();
    void __translateEnd();
    void __translateValueChanged(int c, int t, QString source, QString dest);
    void __translateProgress(QString p);
    void __translateError(QString error);
public slots:
    void __onTriggerOpenFile(bool b = false);
    void __onTriggerOpenConfigFile(bool b = false);
    void __onTriggerSaveFile(bool b = false);
    void __onTriggerAbout(bool b = false);

    void __onTriggerSettings(bool b = false);

    void __onTriggerTranslate(bool b = false);
    void __onTriggerExit(bool b = false);
    void __onTranslateValueChanged(int c, int t, QString source, QString dest);

    void __onItemClicked(QListWidgetItem* item);
    // right click item
    // void __onItemRightClicked(QListWidgetItem* item);
    void __onListViewCustomContextMenuRequested(const QPoint& pos);
protected:
    FluRoundMenu* m_contextMenu;
    FluAction* m_openFileAction;
    FluAction* m_delteFileAction;
    FluAction* m_settingsAction;// settings;
    FluAction* m_translateAction;


    // editor;
	FluScintilla* m_editor;
    // outputwindow
    OutputWindow* m_outputWindow;

    // list view
    FluListView* m_listView;

    // 
    QString m_xmlFilePath;

    TranslateState m_translateState;
    QString m_curFilePath;

    ///
    FluDockManager* m_dockMgr;


    //// settings;
    QString m_httpurl;
    QString m_token;
    QString m_sourceLang;
    QString m_targetLang;
};