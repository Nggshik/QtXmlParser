#include "tableviewcontroller.h"


#include <QAbstractItemModelTester>

TableViewController::TableViewController(QWidget *parent) : QWidget(parent),
    m_pModel(new TableModel(this)),
    m_pTableView(new QTableView(this)),
    m_pXmlParser(new XmlParser),
    m_pDataBase(new DataBaseLite),
    m_pProgress(new ProgressImport)
{
//    new QAbstractItemModelTester(m_pModel, QAbstractItemModelTester::FailureReportingMode::Fatal, this);

//    QThread* thread = new QThread();
//    m_pModel->moveToThread(thread);

//    //TODO safety
//    connect(thread, &QThread::started, m_pModel, &TableModel::connectDB);
////    connect(thread, &QThread::started, this, &TableViewController::createUI);
//    //    connect(task, SIGNAL(workFinished()), thread, SLOT(quit()));

//    //    // automatically delete thread and task object when work is done:
//    //    connect(task, SIGNAL(workFinished()), task, SLOT(deleteLater()));
//    //    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

//    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
//    thread->start();

    QThread* pXmlThread = new QThread();
    m_pXmlParser->moveToThread(pXmlThread);
    connect(pXmlThread, &QThread::finished, pXmlThread, &QThread::deleteLater);
    connect(m_pXmlParser,&XmlParser::fileParsed, m_pModel, &TableModel::appendFile, Qt::QueuedConnection);
    connect(m_pXmlParser,&XmlParser::fileParsed, m_pProgress, &ProgressImport::okCountUp, Qt::QueuedConnection);
    connect(m_pXmlParser,&XmlParser::fileProcessed, m_pProgress, &ProgressImport::progressStepForward, Qt::QueuedConnection);
    connect(m_pXmlParser,&XmlParser::fileError, m_pProgress, &ProgressImport::pushError, Qt::QueuedConnection);
    connect(this,&TableViewController::importXML, m_pXmlParser, &XmlParser::parseXML, Qt::QueuedConnection);
    pXmlThread->start();

    QThread* pDBThread = new QThread();
    m_pDataBase->moveToThread(pDBThread);
    connect(pDBThread,&QThread::started, m_pDataBase, &DataBaseLite::connectDB, Qt::QueuedConnection);
    connect(pDBThread, &QThread::finished, pDBThread, &QThread::deleteLater);
    connect(m_pDataBase,&DataBaseLite::dataSelected, m_pModel, &TableModel::appendFile, Qt::QueuedConnection);
    connect(m_pModel, &TableModel::tableCreated,m_pDataBase,&DataBaseLite::createDB,  Qt::QueuedConnection);
    connect(m_pModel,&TableModel::cleared, m_pDataBase, &DataBaseLite::clear, Qt::QueuedConnection);
    connect(m_pXmlParser,&XmlParser::fileParsed, m_pDataBase, &DataBaseLite::insertIntoTable, Qt::QueuedConnection);
    connect(m_pModel,&TableModel::cellDataChanged , m_pDataBase, &DataBaseLite::updateIntoTable, Qt::QueuedConnection);
    pDBThread->start();


}


TableViewController::~TableViewController()
{
    if(m_pXmlParser) delete m_pXmlParser;
    if(m_pDataBase) delete m_pDataBase;
    if(m_pProgress) delete m_pProgress;
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
   auto dirPath = QFileDialog::getExistingDirectory();

   QDir dir(dirPath);
//   m_pProgress = new ProgressImport();
   QFileInfoList filesInfo = dir.entryInfoList();
   m_pProgress->clear();
   m_pProgress->setProgressMax(filesInfo.size()-2);
   m_pProgress->show();

   emit importXML(dirPath);

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

