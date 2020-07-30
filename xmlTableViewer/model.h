#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QtCore>

#include "progressimport.h"

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

signals:

private:
    int parseXML(const QString& directoryPath);
    QVector<QPair<QString, QString>> parseXMLfile(const QString &filePath);
    void pushProgressError(const QString& err);
private:
    QVector<QVector<QPair<QString, QString>>> m_table;
    QString m_path;
    QStringList m_errorFiles;
    ProgressImport m_progress;
};

#endif // MODEL_H
