#include "dialogmapper.h"

DialogMapper::DialogMapper(QWidget *parent) : QWidget(parent),
    m_pMapper(new QDataWidgetMapper),
    m_pOkBtn(new QPushButton("Ok")),
    m_pCancelBtn(new QPushButton("Cancel"))
{

    connect(m_pOkBtn, &QPushButton::clicked, this, &DialogMapper::okButtonClicked);
    connect(m_pCancelBtn, &QPushButton::clicked, this, &DialogMapper::hide);

    m_pMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    this->setWindowTitle("Edit record");

}




void DialogMapper::setModel(QAbstractItemModel* model)
{
    m_pMapper->setModel(model);
    connect(model, &QAbstractItemModel::headerDataChanged, this, &DialogMapper::setLineEditorsData);
}

void DialogMapper::setLineEditorsData(Qt::Orientation orintation, int first, int last)
{
    Q_UNUSED(first)
    Q_UNUSED(last)

    if(orintation == Qt::Horizontal)
    {
        QVBoxLayout *pVLayout = new QVBoxLayout;
        auto model = m_pMapper->model();
        for(int i = 1; i < model->columnCount(); ++i){
            QHBoxLayout* pHLayout = new QHBoxLayout;
            QLabel* pNameLabel = new QLabel;
            QLineEdit* pLineEditor = new QLineEdit;

            QString text = model->headerData(i, Qt::Horizontal).toString(); //take name from header model data
            pNameLabel->setText(text);
            pHLayout->addWidget(pNameLabel);
            pHLayout->addWidget(pLineEditor);
            pVLayout->addLayout(pHLayout);
            m_pMapper->addMapping(pLineEditor, i);
        }


        QHBoxLayout* pHLayout = new QHBoxLayout;
        pHLayout->addWidget(m_pOkBtn);
        pHLayout->addWidget(m_pCancelBtn);
        pVLayout->addLayout(pHLayout);

        setLayout(pVLayout);
    }
}

void DialogMapper::editRecord(int row)
{

    if(row == -1){
        return;
    } else {
        m_pMapper->setCurrentModelIndex(m_pMapper->model()->index(row,0));
    }
    show();
}

void DialogMapper::okButtonClicked()
{
    m_pMapper->submit();
    this->hide();
}
