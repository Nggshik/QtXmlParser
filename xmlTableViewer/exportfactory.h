#ifndef EXPORTFACTORY_H
#define EXPORTFACTORY_H

#include <QObject>
#include "IExportStrategy.h"

class ExportFactory : public QObject
{
    Q_OBJECT
public:
    explicit ExportFactory(QObject *parent = nullptr);
    IExportStrategy* createStrategy(const QString& filename);
signals:

};

#endif // EXPORTFACTORY_H
