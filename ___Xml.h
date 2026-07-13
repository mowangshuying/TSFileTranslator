#pragma once

#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFile>
#include <QObject>

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

class __Xml : public QObject {
    Q_OBJECT
public:
    void __init(QString httpUrl, QString token);

    bool __read(QString filepath);

	void __write(QString filepath);

    QString __translate(QString sourceLang, QString targetLang, QString source);
    std::list<QString> __translate(QString sourceLang, QString targetLang, std::list<QString> sources);
    void __translate(QString sourceLang, QString targetLang);
signals:
    void __translateStarted();
    void __translateInfoChanged(int nTranslate, int nTotalTranslate, QString s, QString t);
    void __translateEnded();
public:
    QString __version;
    std::list<Context> __contexts;
};