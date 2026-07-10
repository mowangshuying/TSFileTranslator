#pragma once

#include <QString>

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