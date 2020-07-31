#include "mainwindow.h"

#include <QApplication>
#include <QtWidgets>
#include "tablemodel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    TableModel model;
    model.parseXML(QFileDialog::getExistingDirectory());

    QTableView view;
    view.setModel(&model);
//    view.setColumnHidden(0, true);
    view.setSelectionBehavior(QAbstractItemView::SelectRows);
    view.setSelectionMode(QAbstractItemView::SingleSelection);
    view.resizeColumnsToContents();
    view.setEditTriggers(QAbstractItemView::NoEditTriggers);
    view.horizontalHeader()->setStretchLastSection(true);
    view.verticalHeader()->hide();
    view.show();

    return a.exec();
}
