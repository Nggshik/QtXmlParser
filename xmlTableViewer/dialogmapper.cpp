#include "dialogmapper.h"

DialogMapper::DialogMapper(QWidget *parent) : QWidget(parent),
    m_pMapper(new QDataWidgetMapper)
{
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
        auto currLayout = this->layout();
        if(currLayout)
        {
            qDeleteAll(this->children());
//            delete currLayout;
        }

        QVBoxLayout *pVLayout = new QVBoxLayout;
        auto model = m_pMapper->model();
        m_pMapper->clearMapping();

        for(int i = 0; i < model->columnCount(); ++i){
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

        QPushButton* pOkBtn = new QPushButton("Ok");
        QPushButton* pCancelBtn = new QPushButton("Cancel");

        connect(pOkBtn, &QPushButton::clicked, this, &DialogMapper::okButtonClicked);
        connect(pCancelBtn, &QPushButton::clicked, this, &DialogMapper::hide);

        QHBoxLayout* pHLayout = new QHBoxLayout;
        pHLayout->addWidget(pOkBtn);
        pHLayout->addWidget(pCancelBtn);
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
