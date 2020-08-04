#include "tableviewcontroller.h"

#include <QAbstractItemModelTester>

TableViewController::TableViewController(QWidget *parent) : QWidget(parent),
    m_pModel(new TableModel),
    m_pTableView(new QTableView)
{
    new QAbstractItemModelTester(m_pModel, QAbstractItemModelTester::FailureReportingMode::Fatal, this);
}

void TableViewController::createUI()
{
    QGridLayout* pLayout = new QGridLayout;
    pLayout->setContentsMargins(5,5,5,5);
    pLayout->setSpacing(15);

    m_pTableView->setModel(m_pModel);
    m_pTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTableView->resizeColumnsToContents();
    m_pTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pTableView->horizontalHeader()->setStretchLastSection(true);
    m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_pTableView->verticalHeader()->hide();
    m_pTableView->show();


    pLayout->addWidget(m_pTableView,0,0,1,4);

    QPushButton* pClearBtn = new QPushButton("Clear");
    QPushButton* pImportBtn = new QPushButton("Import");
    pLayout->addWidget(pClearBtn,1,2);
    pLayout->addWidget(pImportBtn,1,3);

    //Connect Button Signals
    connect(pImportBtn, &QPushButton::clicked, this, &TableViewController::importData);
    connect(pClearBtn, &QPushButton::clicked, this, &TableViewController::clear);

    //Connect context actions
    connect(m_pTableView, &QTableView::customContextMenuRequested, this, &TableViewController::customMenuRequest);

    QDataWidgetMapper* mapper = new QDataWidgetMapper();
    mapper->setModel(m_pModel);

    QLineEdit* pTemp = new QLineEdit;
    mapper->addMapping(pTemp, 1);
    pTemp = new QLineEdit;
    mapper->addMapping(pTemp, 2);
    pTemp = new QLineEdit;
    mapper->addMapping(pTemp, 3);
    /* Ручное подтверждение изменения данных
     * через mapper
     * */
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);


    setLayout(pLayout);
}

void TableViewController::importData()
{
    m_pModel->parseXML(QFileDialog::getExistingDirectory());
}

void TableViewController::clear()
{
    m_pModel->clear();
}

void TableViewController::editRecord()
{
    ;
//    m_pModel->
}

void TableViewController::customMenuRequest()
{

}
