#include "xmlexportstrategy.h"

xmlExportStrategy::xmlExportStrategy() : m_fileName("xmlExport.xml")
{

}

bool xmlExportStrategy::exportToFile(){
    //TODO;
}

bool xmlExportStrategy::setFileName(const QString &filename){

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
