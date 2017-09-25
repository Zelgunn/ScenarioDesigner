#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    if(argc>1)
    {
        QString fileNameArgument(argv[1]);
        w.loadScenario(fileNameArgument);
    }
    w.show();

    return a.exec();
}
