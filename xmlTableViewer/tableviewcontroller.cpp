#include "tableviewcontroller.h"


#include <QAbstractItemModelTester>

TableViewController::TableViewController(QWidget *parent) : QWidget(parent),
    m_pModel(new TableModel),
    m_pTableView(new QTableView)
{
    new QAbstractItemModelTester(m_pModel, QAbstractItemModelTester::FailureReportingMode::Fatal, this);

    QThread* thread = new QThread();
    m_pModel->moveToThread(thread);

    //TODO safety
    //    connect(thread, SIGNAL(started()), task, SLOT(doWork()));
    //    connect(task, SIGNAL(workFinished()), thread, SLOT(quit()));

    //    // automatically delete thread and task object when work is done:
    //    connect(task, SIGNAL(workFinished()), task, SLOT(deleteLater()));
    //    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();
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
    m_pTableView->setContextMenuPolicy(Qt::CustomContextMenu);
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
    connect(m_pTableView, &QTableView::customContextMenuRequested, this, &TableViewController::customContextMenuRequested);

    m_pMapper = new DialogMapper;
    m_pMapper->setModel(m_pModel);

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
    m_pMapper->editRecord(m_pTableView->selectionModel()->currentIndex().row());
}

void TableViewController::customContextMenuRequested(QPoint position)
{
    /* Создаем объект контекстного меню */
    QMenu * menu = new QMenu(this);
    /* Создаём действия для контекстного меню */
    QAction * editDevice = new QAction("Edit", this);
    QAction * deleteDevice = new QAction("Delete", this);
    /* Подключаем СЛОТы обработчики для действий контекстного меню */
    connect(editDevice, &QAction::triggered, this, &TableViewController::editRecord);     // Обработчик вызова диалога редактирования
    connect(deleteDevice, &QAction::triggered, this, &TableViewController::clear); // Обработчик удаления записи

    menu->addAction(editDevice);
    menu->addAction(deleteDevice);

    menu->popup(m_pTableView->viewport()->mapToGlobal(position));
}
