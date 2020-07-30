#ifndef PROGRESSIMPORT_H
#define PROGRESSIMPORT_H

#include <QWidget>
#include <QtWidgets>

class ProgressImport : public QWidget
{
    Q_OBJECT
public:
    explicit ProgressImport(QWidget *parent = nullptr);
    void setErrorsCountString(const QString& err);
    void pushError(const QString& err);
    void setProgressMax(size_t max);
    void progressStepForward();
    void errorCountUp(size_t count = 1);
    void okCountUp(size_t count = 1);
signals:


private:
    QVBoxLayout *m_pLayout;
    QProgressBar *m_pProgress;
    QLabel* m_pLabel;
    QPlainTextEdit* m_pPlainText;
    size_t m_okCount = 0;
    size_t m_errCount = 0;
};

#endif // PROGRESSIMPORT_H
