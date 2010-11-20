#include <QApplication>
#include "mainwindow.h"
//#include "boardwidget.h"



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainW;
    mainW.show();



    return app.exec();

}

