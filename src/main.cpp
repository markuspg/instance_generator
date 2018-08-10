#include "mainwindow.h"
#include <QApplication>

#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
