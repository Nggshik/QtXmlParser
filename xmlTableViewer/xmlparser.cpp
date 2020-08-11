#include "xmlparser.h"

XmlParser::XmlParser(QObject *parent) : QObject(parent)
{

}

int XmlParser::parseXML(const QString& directoryPath)
{
//    if(m_pProgress != nullptr)
//        m_pProgress->deleteLater();

//    m_pProgress = new ProgressImport();


    QDir dir(directoryPath);
    QRegExp fileCheckRX("*.xml");
    fileCheckRX.setPatternSyntax(QRegExp::Wildcard);

    QFileInfoList filesInfo = dir.entryInfoList();
//    m_pProgress->setProgressMax(filesInfo.size()-2);

    for(auto info : filesInfo)
    {
        if(info.isFile())
        {
            if(fileCheckRX.exactMatch(info.fileName()))
            {
                auto xmlParameters = parseXMLfile(info.filePath());
                if(xmlParameters.size()){
//                    m_pProgress->okCountUp(xmlParameters.size());
                    emit fileParsed(xmlParameters);
                }
            }
            else
            {
                pushProgressError(info.fileName());
            }
//            m_pProgress->progressStepForward();
            emit fileProcessed();
        }

    }

    return 0;
}

QHash<QString, QVariant> XmlParser::parseXMLfile(const QString &filePath)
{
    QRegExp keyValueRX("<(\\w+)>(.+)<\\/(\\w+)>"); //<(\w+)>(.+)<\/(\g1)>
    keyValueRX.setPatternSyntax(QRegExp::RegExp2);

    QHash<QString, QVariant> hash;

    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly))
    {

        while(!file.atEnd() && !file.readLine().contains("<root>")); //Skip all lines befor root


        while (!file.atEnd()) {
            auto line = file.readLine();
            if(keyValueRX.indexIn(line) != -1)
            {
                auto caps = keyValueRX.capturedTexts();
                if((caps.size() == 4) && (caps[1] == caps[3]))
                {
                    hash.insert(caps[1],caps[2]);
                }
                else
                {
                    pushProgressError(file.fileName() + "\n At line -" + line);
                    hash.clear();
                    break;
                }
            }
            else
            {
                if(line.contains("</root>"))
                {
                    break;
                }
                else
                {
                    pushProgressError(file.fileName() + "\n At line -" + line);
                    hash.clear();
                    break;
                }
            }

        }
    }
    else
    {
        pushProgressError(file.fileName());
    }
    return  hash;
}

void XmlParser::pushProgressError(const QString& err)
{
    qDebug() << "MODEL::FILE::ERROR::" << err;
//    m_pProgress->pushError(err);
    emit fileError(err);
}

