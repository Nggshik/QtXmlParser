#include <QApplication>
#include <QtWidgets>
#include "tableviewcontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TableViewController controller;
    controller.createUI();
    controller.show();

    return a.exec();
}
