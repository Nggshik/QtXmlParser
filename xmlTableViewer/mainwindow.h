#ifndef MAINWINDOW_H
#define MAINWINDOW_H


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

};
#endif // MAINWINDOW_H
