#include <QApplication>
#include "TSWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // TranslatorWindow w;
    // w.show();
    TSWindow w;
    w.show();
    
    return app.exec();
}