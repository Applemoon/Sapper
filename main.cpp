#include <QApplication>
#include "mainwindow.h"



int main( int argc, char *argv[] )
{
    QApplication a( argc, argv );

    MainWindow w;
    w.newGame();

    return a.exec();
}
