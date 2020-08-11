#ifndef XMLEXPORTSTRATEGY_H
#define XMLEXPORTSTRATEGY_H

#include <QtCore>

#include "IExportStrategy.h"

class xmlExportStrategy : public IExportStrategy
{
public:
    xmlExportStrategy();
    bool exportToFile() override;
    void addRecord(QHash<QString, QVariant> &record) override;
    bool setFileName(const QString& filename);
    void setDirToExport(const QString& dirPath);
private:
    QString m_fileName;
    QString m_dirPath;
    QList<QHash<QString, QVariant>> m_data;
};

#endif // XMLEXPORTSTRATEGY_H
