#include "xmlparser.h"

XmlParser::XmlParser(QObject *parent) : QObject(parent)
{

}


/**
 * @brief XmlParser::parseXML
 *        Parse all existing file in directory
 *        Contract: XML files should have same list of tags and only one row
 *
 *        Example file:
 *        <?xml version="1.0" encoding="UTF-8"?>
 *              <root>
 *                  <texteditor>Atom</texteditor>
 *                  <fileformats>everything</fileformats>
 *                  <encoding>ANSI; UTF;</encoding>
 *                  <hasintellisense>true</hasintellisense>
 *                  <hasplugins>true</hasplugins>
 *                  <cancompile>false</cancompile>
 *              </root>
 *
 *        emit fileParsed(const QList<QPair<QString, QVariant>>& file) - if file was parsed successfuly
 *        emit fileProcessed() - if file skiped
 * @param directoryPath
 * @return
 */
int XmlParser::parseXML(const QString& directoryPath)
{

    QDir dir(directoryPath);
    QRegExp fileCheckRX("*.xml");
    fileCheckRX.setPatternSyntax(QRegExp::Wildcard);

    QFileInfoList filesInfo = dir.entryInfoList();

    for(auto info : filesInfo)
    {
        if(info.isFile())
        {
            if(fileCheckRX.exactMatch(info.fileName()))
            {
                auto xmlParameters = parseXMLfile(info.filePath());
                if(xmlParameters.size()){
                    emit fileParsed(xmlParameters);
                }
            }
            else
            {
                pushProgressError(info.fileName());
            }
            emit fileProcessed();
        }

    }

    return 0;
}


/**
 * @brief XmlParser::parseXMLfile
 *        Parse xml file from path
 * @param filePath
 * @return
 */
QList<QPair<QString, QVariant>> XmlParser::parseXMLfile(const QString &filePath)
{
    QRegExp keyValueRX("<(\\w+)>(.+)<\\/(\\w+)>"); //<(\w+)>(.+)<\/(\g1)>
    keyValueRX.setPatternSyntax(QRegExp::RegExp2);

    QList<QPair<QString, QVariant>> list;

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
                    list.append(qMakePair(caps[1],caps[2]));
                }
                else
                {
                    pushProgressError(file.fileName() + "\n At line -" + line);
                    list.clear();
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
                    list.clear();
                    break;
                }
            }

        }
    }
    else
    {
        pushProgressError(file.fileName());
    }
    return  list;
}


/**
 * @brief XmlParser::pushProgressError
 *        emit fileError(const QString& err) -user signal
 * @param err
 */
void XmlParser::pushProgressError(const QString& err)
{
    qDebug() << "MODEL::FILE::ERROR::" << err;
    emit fileError(err);
}

