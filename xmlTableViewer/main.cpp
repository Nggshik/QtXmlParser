#include "mainwindow.h"

#include <QApplication>
#include <QtWidgets>
#include "tableviewcontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    TableModel model;
//    model.parseXML(QFileDialog::getExistingDirectory());

//    QTableView view;

////    view.setColumnHidden(0, true);
//    view.setSelectionBehavior(QAbstractItemView::SelectRows);
//    view.setSelectionMode(QAbstractItemView::SingleSelection);
//    view.resizeColumnsToContents();
//    view.setEditTriggers(QAbstractItemView::NoEditTriggers);
//    view.horizontalHeader()->setStretchLastSection(true);
//    view.horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    view.verticalHeader()->hide();
//    view.show();
//    view.setModel(&model);


    TableViewController controller;
    controller.createUI();
    controller.show();

    return a.exec();
}
