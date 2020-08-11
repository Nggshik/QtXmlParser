#ifndef XMLEXPORTSTRATEGY_H
#define XMLEXPORTSTRATEGY_H

#include <QtCore>

#include "IExportStrategy.h"

class XmlExportStrategy : public IExportStrategy
{
public:
    XmlExportStrategy();
    bool exportToFile(const QHash<QString, QVariant> &record) override;
    bool setFileName(const QString& filename) override;
    void addRecord(const QHash<QString, QVariant> &record);
    bool setDirToExport(const QString& dirPath);
private:
    QString m_fileName;
    QString m_dirPath;
    QList<QHash<QString, QVariant>> m_data;
};

#endif // XMLEXPORTSTRATEGY_H
