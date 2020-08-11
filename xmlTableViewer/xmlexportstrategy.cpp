#include "xmlexportstrategy.h"

XmlExportStrategy::XmlExportStrategy() : m_fileName("xmlExport.xml")
{

}

bool XmlExportStrategy::exportToFile(const QHash<QString, QVariant> &record)
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
    for(auto key : record.keys())
    {

            out << QString("\t<%1>%2</%1>\n").arg(key).arg(record[key].toString());

    }
    out << "</root>\n";


//    file.write(buffer.data());
    return true;
}

void XmlExportStrategy::addRecord(const QHash<QString, QVariant> &record)
{
    m_data.append(record);
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

//bool XmlExportStrategy::setDirToExport(const QString &dirPath)
//{
//    QDir dir(dirPath);
//    if(dir.exists())
//    {
//        m_dirPath = dirPath;
//        return true;
//    }

//    qDebug() << "XMLEXPORTSTRATEGY::SETDIRTOEXPORT::ERROR::" << dirPath;
//    return false;

//}
