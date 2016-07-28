#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    // creates and displays the main window
    MainWindow w;
    w.show();

    return a.exec();
}
