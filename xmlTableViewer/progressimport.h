#ifndef PROGRESSIMPORT_H
#define PROGRESSIMPORT_H

#include <QWidget>
#include <QtWidgets>

class ProgressImport : public QDialog
{
    Q_OBJECT
public:
    explicit ProgressImport(QWidget *parent = nullptr);
    virtual ~ProgressImport(){;}

    void pushError(const QString& err);
    void setProgressMax(size_t max);
    void progressStepForward();
    void errorCountUp(size_t count = 1);
    void okCountUp();
    void clear();
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
