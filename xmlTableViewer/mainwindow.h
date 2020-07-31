#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "model.h"
#include "tablewidget.h"

#include <QtCore>
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void importXML();
private:
    Model* m_pModel;
    TableWidget* m_pTable;
};
#endif // MAINWINDOW_H
