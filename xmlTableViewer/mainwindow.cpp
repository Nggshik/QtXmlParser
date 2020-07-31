#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_pModel(new Model(this)),
      m_pTable(new TableWidget)
{
    setCentralWidget(m_pTable);
    connect(m_pTable,&TableWidget::importData,this, &MainWindow::importXML);
    connect(m_pModel,&Model::xmlParsed, m_pTable, &TableWidget::fillTable);
}

MainWindow::~MainWindow()
{
}

void MainWindow::importXML()
{
    m_pModel->parseXML(QFileDialog::getExistingDirectory());
}
