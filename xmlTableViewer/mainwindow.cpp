#include "mainwindow.h"
#include "model.h"
#include <QtCore>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    Model* md = new Model;
}

MainWindow::~MainWindow()
{
}

