#include "progressimport.h"

ProgressImport::ProgressImport(QWidget *parent) : QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    m_pLayout(new QVBoxLayout),
    m_pProgress(new QProgressBar),
    m_pLabel(new QLabel),
    m_pPlainText(new QPlainTextEdit)
{
    setFixedSize(500,400);
    setWindowTitle("Progress");

    m_pPlainText->document()->setMaximumBlockCount(100);
    QPalette p = m_pPlainText->palette();
    p.setColor(QPalette::Base, QColor(50,50,50));
    p.setColor(QPalette::Text, QColor(20,185,40));
    setPalette(p);
    QFont f( "Times New Roman", 12);
    m_pPlainText->setFont(f);
    m_pPlainText->setReadOnly(true);


    m_pProgress->setRange(0,1);
    m_pProgress->setValue(0);
    m_pProgress->setAlignment(Qt::AlignCenter);


    m_pLayout->addWidget(m_pProgress);
    m_pLayout->addWidget(m_pLabel);
    m_pLayout->addWidget(m_pPlainText);

    setLayout(m_pLayout);
}

void ProgressImport::pushError(const QString &err)
{
    errorCountUp();
    m_pPlainText->insertPlainText(err+'\n');
}

void ProgressImport::setErrorsCountString(const QString &err)
{
    m_pLabel->setText(err);
}

void ProgressImport::setProgressMax(size_t max)
{
    m_pProgress->setMaximum(max);
}

void ProgressImport::progressStepForward()
{
    m_pProgress->setValue(m_pProgress->value()+1);
}

void ProgressImport::okCountUp()
{
    m_okCount += 1;
    m_pLabel->setText(QString("Imported = %1, Errors = %2").arg(m_okCount).arg(m_errCount));
}

void ProgressImport::errorCountUp(size_t count)
{
    m_errCount += count;
    m_pLabel->setText(QString("Imported = %1, Errors = %2").arg(m_okCount).arg(m_errCount));
}

void ProgressImport::clear()
{
    m_errCount = 0;
    m_okCount = 0;
    m_pLabel->clear();
    m_pPlainText->clear();
    m_pProgress->setValue(0);
}
