#ifndef XMLEXPORTSTRATEGY_H
#define XMLEXPORTSTRATEGY_H

#include <QtCore>

#include "IExportStrategy.h"

class XmlExportStrategy : public IExportStrategy
{
public:
    XmlExportStrategy();
    bool exportToFile(const QList<QPair<QString, QVariant>> &record) override;
    bool setFileName(const QString& filename) override;
private:
    QString m_fileName;
};

#endif // XMLEXPORTSTRATEGY_H
