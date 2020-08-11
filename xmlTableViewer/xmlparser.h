#ifndef XMLPARSER_H
#define XMLPARSER_H

//System include
#include <QObject>
#include <QtCore>

//User include
#include "progressimport.h"

class XmlParser : public QObject
{
    Q_OBJECT
public:
    explicit XmlParser(QObject *parent = nullptr);
    int parseXML(const QString& directoryPath);
signals:
    void fileParsed(const QList<QPair<QString, QVariant>>& file);
    void fileError(const QString& err);
    void fileProcessed();
private:
    QList<QPair<QString, QVariant>> parseXMLfile(const QString &filePath);
    void pushProgressError(const QString& err);
private:
    ProgressImport* m_pProgress = nullptr;
};

#endif // XMLPARSER_H
