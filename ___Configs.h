#pragma once

#include "___def.h"
#include <CppSQLite3.h>
#include <QString>
#include <QApplication>
#include <QDir>
// #include "___def.h"

class ___Configs 
{
protected:
    ___Configs()
    {
        //// 结束翻译 应用程序文件夹下的out目录;
            QString outDirPath = "../config/";
            QDir __outDir(outDirPath);
            if (!__outDir.exists())
            {
                __outDir.mkdir(outDirPath);
            }

            QString __configsFilePath = outDirPath + "___configs.db";
            open(__configsFilePath);
    }
public:

    static ___Configs* getConfig()
    {
        if (___configs == nullptr)
            ___configs = new ___Configs();
        return ___configs;
    }

    bool open(QString dbName)
    {
        __dbName = dbName;
        try
        {
            __db.open(__dbName.toStdString().c_str());
            return true;
        }
        catch(CppSQLite3Exception& e)
        {
            return false;
        }
        return false;
    }

    bool close()
    {
        try
        {
            __db.close();
            return true;
        }
        catch(CppSQLite3Exception& e)
        {
            return false;
        }
        return false;
    }

    bool tableExists(QString table)
    {
        try
        {
            return __db.tableExists(table.toStdString().c_str());
        }
        catch(const std::exception& e)
        {
            return false;
        }
        return false;
        
    }

    qint64 lastRowId()
    {
        return static_cast<qint64>(__db.lastRowId());
    }

    int execDML(const QString sql)
    {
        try 
        {
            return __db.execDML(sql.toStdString().c_str());
        }
        catch(CppSQLite3Exception& e)
        {
            return -1;
        }
        return -1;
    }


    /// Task;
    bool createTaskTable()
    {
        QString sql = "CREATE TABLE task(id INTEGER PRIMARY KEY AUTOINCREMENT, source_file TEXT, source_lang TEXT, target_lang TEXT);";
        if (!tableExists("task"))
        {
            return execDML(sql) != -1;
        }
        return true;
    }

    bool addTaskData(TaskData& data)
    {
        QString sql = QString("INSERT INTO task (source_file, source_lang, target_lang) VALUES ('%1', '%2', '%3');")
            .arg(data.SourceFile).arg(data.SourceLang).arg(data.TargetLang);
        
        int nChanges = execDML(sql);
        if (nChanges > 0)
        {
            data.id = lastRowId();
            return true;
        }

        data.id = -1;
        return false;
    }

    bool deleteTaskDataById(qint64 id)
    {
        QString sql = QString("DELETE FROM task WHERE id = %1").arg(id);
        return execDML(sql) > 0;
    }

    bool updateTaskDataById(qint64 id, const TaskData& data)
    {
        QString sql = QString("UPDATE task SET source_file = %1, source_lang = %2, target_lang = %3 WHERE id = %4")
            .arg(data.SourceFile).arg( data.SourceLang).arg( data.TargetLang).arg(data.id);
        return execDML(sql) > 0;
    }

    int getTaskDatas(std::vector<TaskData>& datas)
    {
        QString sql = QString("SELECT id, source_file, source_lang, target_lang FROM task;");
        CppSQLite3Query query;
        try 
        {
            query = __db.execQuery(sql.toStdString().c_str());
        }
        catch(CppSQLite3Exception& e)
        {
            return -1;
        }

        int num = query.numFields();
        
        TaskData data;
        for (int i = 0; i < num; i++)
        {
            std::string fieldName = query.fieldName(i);
            if (fieldName == "id")
            {
                data.id = query.getInt64Field("id", 0);
                // continue;
            }

            if (fieldName == "source_file")
            {
                data.SourceFile = query.getStringField("source_file");
                // continue;
            }

            if (fieldName == "source_lang")
            {
                data.SourceLang = query.getStringField("source_lang");
                // continue;
            }

            if (fieldName == "target_lang")
            {
                data.SourceLang = query.getStringField("target_lang");
                // continue;
            }

            if (i == (num - 1))
            {
                datas.push_back(data);
            }
        }
    }
protected:
    CppSQLite3DB __db;
    QString __dbName;
    static ___Configs* ___configs;
};