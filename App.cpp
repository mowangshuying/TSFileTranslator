#include <QApplication>
#include "TSWindow.h"
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    /// 直接使用中文;
    //QTranslator translator;
    //bool bLoad = translator.load(":/i18n/zh-CN.qm");
    //if (bLoad)
    //{
    //    app.installTranslator(&translator);
    //}

    TSWindow w;
    w.show();
    
    return app.exec();
}