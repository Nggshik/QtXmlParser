#include "tablewidget.h"

TableWidget::TableWidget(QWidget *parent) : QWidget(parent),
    m_pTable(new QTableWidget)
{
    QGridLayout* pLayout = new QGridLayout;
    pLayout->setContentsMargins(5,5,5,5);
    pLayout->setSpacing(15);
    pLayout->addWidget(m_pTable,0,0,1,4);

    QPushButton* pClearBtn = new QPushButton("Clear");
    QPushButton* pImportBtn = new QPushButton("Import");
    pLayout->addWidget(pClearBtn,1,2);
    pLayout->addWidget(pImportBtn,1,3);

    connect(pImportBtn, &QPushButton::clicked, this, &TableWidget::importData);

    setLayout(pLayout);
}

TableWidget::~TableWidget()
{

}

void TableWidget::fillTable(const QVector<QVector<QPair<QString, QString>>>& table)
{
    if(table.empty())
        return;
    QStringList headers;
    auto& firstLine = table[0];
    for(auto& item : firstLine)
    {
        headers << item.first;
    }
    m_pTable->setColumnCount(firstLine.size()); // Указываем число колонок
    m_pTable->setShowGrid(true); // Включаем сетку
    // Разрешаем выделение только одного элемента
    m_pTable->setSelectionMode(QAbstractItemView::SingleSelection);
    // Разрешаем выделение построчно
    m_pTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем заголовки колонок
    m_pTable->setHorizontalHeaderLabels(headers);
    // Растягиваем последнюю колонку на всё доступное пространство
    m_pTable->horizontalHeader()->setStretchLastSection(true);
//    // Скрываем колонку под номером 0
//    m_pTable->hideColumn(0);
    int lastRow = m_pTable->rowCount();
    for(auto &vec : table)
    {
        m_pTable->insertRow(lastRow);
        for(int i = 0; i < vec.size(); ++i)
        {
            m_pTable->setItem(lastRow, i, new QTableWidgetItem(vec[i].second));
        }
        ++lastRow;
    }
    m_pTable->resizeColumnsToContents();
}
