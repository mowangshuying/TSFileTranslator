#include <QApplication>
#include "TranslatorWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    TranslatorWindow w;
    w.show();
    
    return app.exec();
}