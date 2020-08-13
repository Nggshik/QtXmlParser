#ifndef IEXPORTSTRATEGY_H
#define IEXPORTSTRATEGY_H

#include <QtCore>

class IExportStrategy{
public:
    virtual bool exportToFile(const QList<QPair<QString, QVariant>> &record) = 0;
    virtual bool setFileName(const QString& filename) = 0;
};


#endif // IEXPORTSTRATEGY_H
