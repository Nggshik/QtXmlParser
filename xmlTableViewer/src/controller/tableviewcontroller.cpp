#include "tableviewcontroller.h"
#include "exportfactory.h"

#ifdef QT_DEBUG
    #include <QAbstractItemModelTester>
#endif

TableViewController::TableViewController(QWidget *parent) : QWidget(parent),
    m_pModel(new TableModel(this)),
    m_pTableView(new QTableView(this)),
    m_pXmlParser(new XmlParser),
    m_pDataBase(new DataBaseLite),
    m_pProgress(new ProgressImport)
{
//Tester for model
#ifdef QT_DEBUG
   new QAbstractItemModelTester(m_pModel, QAbstractItemModelTester::FailureReportingMode::Fatal, this);
#endif


    //Move xmlParser in thread and connect signals with model by QueuedConnection
    QThread* pXmlThread = new QThread();
    m_pXmlParser->moveToThread(pXmlThread);

    connect(pXmlThread, &QThread::finished, pXmlThread, &QThread::deleteLater);

    connect(m_pXmlParser,&XmlParser::fileParsed, m_pModel, &TableModel::appendFile, Qt::QueuedConnection);

    connect(m_pXmlParser,&XmlParser::fileParsed, m_pProgress, &ProgressImport::okCountUp, Qt::QueuedConnection);
    connect(m_pXmlParser,&XmlParser::fileProcessed, m_pProgress, &ProgressImport::progressStepForward, Qt::QueuedConnection);
    connect(m_pXmlParser,&XmlParser::fileError, m_pProgress, &ProgressImport::pushError, Qt::QueuedConnection);

    connect(this,&TableViewController::importXML, m_pXmlParser, &XmlParser::parseXML, Qt::QueuedConnection);
    pXmlThread->start();

    //Move DataBase in thread and connect signals with model by QueuedConnection
    QThread* pDBThread = new QThread();
    m_pDataBase->moveToThread(pDBThread);

    /*DataBase thread connections*/
    connect(pDBThread, &QThread::finished, pDBThread, &QThread::deleteLater);
    connect(pDBThread,&QThread::started, m_pDataBase, &DataBaseLite::connectDB, Qt::QueuedConnection);

    /*DataBase and Model connections*/
    connect(m_pDataBase,&DataBaseLite::dataSelected, m_pModel, &TableModel::appendFile, Qt::QueuedConnection);
    connect(m_pModel,&TableModel::cleared, m_pDataBase, &DataBaseLite::deleteTable, Qt::QueuedConnection);
    connect(m_pModel,&TableModel::rowRemoved, m_pDataBase, &DataBaseLite::removeRow, Qt::QueuedConnection);
    connect(m_pModel,&TableModel::cellDataChanged , m_pDataBase, &DataBaseLite::updateIntoTable, Qt::QueuedConnection);

    /*DataBase and xmlParser connections*/
    connect(m_pXmlParser,&XmlParser::fileParsed, m_pDataBase, &DataBaseLite::insertIntoTable, Qt::QueuedConnection);

    pDBThread->start();


}


TableViewController::~TableViewController()
{
    if(m_pXmlParser) delete m_pXmlParser;
    if(m_pDataBase) delete m_pDataBase;
    if(m_pProgress) delete m_pProgress;
}


/**
 * @brief TableViewController::createUI
 *        Create UI of TableView with Model
 *        Set model into dialogMapper
 */
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


/**
 * @brief TableViewController::importData
 *        Call QFileDialog to choice directory to export *.xml files
 *        Set ProgressImport count of files to parse
 *        emit importXML - user signal
 */
void TableViewController::importData()
{
   auto dirPath = QFileDialog::getExistingDirectory();

   QDir dir(dirPath);
   dir.setFilter(QDir::Files);

   m_pProgress->clear();
   m_pProgress->setProgressMax(dir.count());
   m_pProgress->setModal(true);
   m_pProgress->show();

   emit importXML(dirPath);

}


/**
 * @brief TableViewController::clear
 *        Call model clear
 */
void TableViewController::clear()
{
    m_pModel->clear();
}


/**
 * @brief TableViewController::editRecord
 *        Call dialogMapper to edit selected row
 */
void TableViewController::editRecord()
{
    m_pMapper->editRecord(m_pTableView->selectionModel()->currentIndex().row());
}


/**
 * @brief TableViewController::customContextMenuRequested
 *        Create custom context menu in position
 * @param position
 */
void TableViewController::customContextMenuRequested(QPoint position)
{

    QMenu * menu = new QMenu(this);

    QAction* editDevice = new QAction("Edit", this);
    QAction* deleteDevice = new QAction("Delete", this);
    QAction* exportRecord = new QAction("Export",this);


    connect(editDevice, &QAction::triggered, this, &TableViewController::editRecord);
    connect(deleteDevice, &QAction::triggered, this, &TableViewController::removeRow);
    connect(exportRecord, &QAction::triggered, this, &TableViewController::exportRecord);

    menu->addAction(editDevice);
    menu->addAction(deleteDevice);
    menu->addAction(exportRecord);

    menu->popup(m_pTableView->viewport()->mapToGlobal(position));
}


/**
 * @brief TableViewController::exportRecord
 *        Used ExportFactory to create export strategy by filename
 *        Call export strategy to export file
 */
void TableViewController::exportRecord()
{
    ExportFactory fac;

    auto row = m_pTableView->selectionModel()->currentIndex().row();
    QList<QPair<QString, QVariant>> record;
    auto count = m_pModel->columnCount();
    for(int i = 0; i < count; ++i)
    {
        record.append(qMakePair(m_pModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString(), m_pModel->index(row,i).data()));
    }

    QString filename = QFileDialog::getSaveFileName(this, "Export record",QDir::currentPath(),"Files format (*.xml)");
    auto exporter = fac.createStrategy(filename);
    if(exporter != nullptr)
        exporter->exportToFile(record);

}


/**
 * @brief TableViewController::removeRow
 *        Remove selected row from model
 */
void TableViewController::removeRow()
{
    m_pModel->removeRow(m_pTableView->selectionModel()->currentIndex().row());
}
