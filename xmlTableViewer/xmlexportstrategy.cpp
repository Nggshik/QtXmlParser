#include "xmlexportstrategy.h"

XmlExportStrategy::XmlExportStrategy() : m_fileName("xmlExport.xml")
{

}

bool XmlExportStrategy::exportToFile(const QList<QPair<QString, QVariant>> &record)
{
    QFile file(m_fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "XMLEXPORTSTRATEGY::EXPORT::ERROR::" << file.errorString();
        return false;
    }


    QTextStream out(&file);

    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    out << "<root>\n";
    for(auto pair : record)
    {

            out << QString("\t<%1>%2</%1>\n").arg(pair.first).arg(pair.second.toString());

    }
    out << "</root>\n";

    return true;
}


bool XmlExportStrategy::setFileName(const QString &filename){

    QRegExp fileCheckRX("*.xml");
    fileCheckRX.setPatternSyntax(QRegExp::Wildcard);
    if(fileCheckRX.exactMatch(filename))
    {
       m_fileName = filename;
       return true;
    }
    qDebug() << "XMLEXPORTSTRATEGY::SETFILENAME::ERROR::" << filename;
    return false;
}


