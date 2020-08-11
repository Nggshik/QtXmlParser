#ifndef IEXPORTSTRATEGY_H
#define IEXPORTSTRATEGY_H

#include <QtCore>

class IExportStrategy{
public:
    virtual bool exportToFile() = 0;
    virtual void addRecord(QHash<QString, QVariant>& record) = 0;
};


#endif // IEXPORTSTRATEGY_H
