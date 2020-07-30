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
    void progressStepUp();
signals:


private:
    QVBoxLayout *m_pLayout;
    QProgressBar *m_pProgress;
    QLabel* m_pLabel;
    QPlainTextEdit* m_pPlainText;
};

#endif // PROGRESSIMPORT_H
