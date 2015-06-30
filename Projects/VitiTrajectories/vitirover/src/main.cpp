#include <time.h>
#include <iostream>
#include <string>

#include <QApplication>

#include "Map.h"
#include "MainWindow.h"

using namespace std;

int main( int argc, char* argv[] )
{
    srand ( time(NULL) );


    QApplication app(argc, argv);
    MainWindow mainwindow;

    mainwindow.show();

    return app.exec();

}
