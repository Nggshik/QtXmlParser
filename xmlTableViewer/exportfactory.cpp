#include "exportfactory.h"
#include "xmlexportstrategy.h"

ExportFactory::ExportFactory(QObject *parent) : QObject(parent)
{

}

IExportStrategy* ExportFactory::createStrategy(const QString &filename)
{
    QStringList types;
    types << "xml";

    if(filename.isEmpty())
        return nullptr;
    QFile file(filename);
    QFileInfo info(file);

    IExportStrategy* strategy = nullptr;
    switch (types.indexOf(info.suffix()))
    {
    case 0:
    {
        strategy = new XmlExportStrategy();
        strategy->setFileName(filename);
        return strategy;
        break;
    }
    default:
        qDebug() << "EXPORTFACTORY::CREATESTRATEGY::ERROR" << filename;
        return nullptr;
    }
}
